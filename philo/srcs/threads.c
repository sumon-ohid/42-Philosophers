/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msumon <msumon@student.42vienna.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/27 13:32:11 by msumon            #+#    #+#             */
/*   Updated: 2024/04/08 19:42:24 by msumon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

void	*routine(void *args)
{
	t_philo	*philo;

	philo = (t_philo *)args;
	if (philo->data->philo_count == 1)
		return (messages(philo, TAKEN_FORK), NULL);
	if (philo->philo_id % 2 == 0)
		usleep(1000);
	while (1)
	{
		if (check_status(philo))
			break ;
		if (eating_action(philo))
			break ;
		if (check_status(philo))
			break ;
		sleeping_action(philo);
		if (check_status(philo))
			break ;
		messages(philo, THINKING);
		if (philo->data->philo_count % 2 == 1
			&& philo->philo_id != philo->data->philo_count)
			usleep(philo->data->time_to_eat * 1000 + 100);
	}
	return (NULL);
}

bool	check_if_dead(t_data *data, t_philo *philo, int *philo_is_full)
{
	if (data->must_eat_times > 0 && philo->meals_eaten >= data->must_eat_times)
		*philo_is_full += 1;
	if (get_time(data, philo)
		- philo->last_meal_time >= philo->data->time_to_die)
	{
		if (philo->data->gtod_failed)
			return (false);
		pthread_mutex_unlock(&data->monitoring_mutex);
		messages(philo, DIED);
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

	while (1)
	{
		i = 0;
		philo_is_full = 0;
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
		usleep(100);
	}
}

int	join_thread_and_monitor(t_data *data, t_philo *philos)
{
	int	i;

	i = -1;
	if (check_status(philos))
		return (0);
	watch_tower(data, philos);
	if (check_status(philos))
		return (0);
	while (++i < data->philo_count)
	{
		if (pthread_join(philos[i].thread_id, NULL) != 0)
			return (1);
	}
	return (0);
}

int	create_threads_and_join(t_data *data, t_philo *philos, int j)
{
	int			i;
	long long	time;

	i = -1;
	time = get_time(data, philos);
	if (philos->data->gtod_failed)
		return (1);
	while (++i < data->philo_count)
	{
		philos[i].start_time = time;
		if (pthread_create(&philos[i].thread_id, NULL, routine,
				&philos[i]) != 0)
		{
			pthread_mutex_lock(&data->monitoring_mutex);
			data->simulation_end = true;
			pthread_mutex_unlock(&data->monitoring_mutex);
			while (j < i)
				if (pthread_join(philos[j++].thread_id, NULL))
					return (1);
			return (1);
		}
	}
	if (join_thread_and_monitor(data, philos))
		return (1);
	return (0);
}
