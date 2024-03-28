/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msumon <msumon@student.42vienna.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/27 13:32:11 by msumon            #+#    #+#             */
/*   Updated: 2024/03/28 09:37:52 by msumon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

void	sleeping_action(t_philo *philo)
{
	monitoring(philo, SLEEPING);
	usleep(philo->data->time_to_sleep * 1000);
}

void	*routine(void *args)
{
	t_philo	*philo;

	philo = (t_philo *)args;
	if (philo->data->philo_count == 1)
	{
		monitoring(philo, TAKEN_FORK);
		return (NULL);
	}
	while (1)
	{
		pthread_mutex_lock(&philo->data->monitoring_mutex);
		if (philo->data->simulation_end)
		{
			pthread_mutex_unlock(&philo->data->monitoring_mutex);
			break ;
		}
		pthread_mutex_unlock(&philo->data->monitoring_mutex);
		eating_action(philo);
		sleeping_action(philo);
		monitoring(philo, THINKING);
	}
	return (NULL);
}

bool	check_if_dead(t_data *data, t_philo *philo, int *philo_is_full)
{
	if (data->must_eat_times > 0 && philo->meals_eaten > data->must_eat_times)
		*philo_is_full += 1;
	if (get_time()
		- philo->last_meal_time >= (useconds_t)philo->data->time_to_die)
	{
		pthread_mutex_unlock(&data->monitoring_mutex);
		monitoring(philo, DIED);
		pthread_mutex_lock(&data->monitoring_mutex);
		data->simulation_end = true;
		pthread_mutex_unlock(&data->monitoring_mutex);
		return (true);
	}
	return (false);
}

void	watch_tower(t_data *data, t_philo *philos)
{
	int	i;
	int	philo_is_full;

	philo_is_full = 0;
	while (1)
	{
		i = 0;
		pthread_mutex_lock(&data->monitoring_mutex);
		while (i < data->philo_count)
		{
			if (check_if_dead(data, &philos[i], &philo_is_full))
				return ;
			i++;
		}
		if (philo_is_full == data->philo_count)
		{
			data->simulation_end = true;
			pthread_mutex_unlock(&data->monitoring_mutex);
			return ;
		}
		pthread_mutex_unlock(&data->monitoring_mutex);
	}
}

int	create_threads_and_join(t_data *data, t_philo *philos,
		pthread_mutex_t *forks)
{
	int	i;

	(void)forks;
	i = 0;
	while (i < data->philo_count)
	{
		philos[i].start_time = get_time();
		if (pthread_create(&philos[i].thread_id, NULL, routine,
				(void *)&philos[i]) != 0)
			return (error(philos, forks, "Error: Thread creation failed\n"));
		i++;
	}
	watch_tower(data, philos);
	i = 0;
	while (i < data->philo_count)
	{
		if (pthread_join(philos[i].thread_id, NULL) != 0)
			return (error(philos, forks, "Error: Thread join failed\n"));
		i++;
	}
	return (0);
}
