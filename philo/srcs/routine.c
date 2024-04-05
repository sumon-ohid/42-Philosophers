/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msumon < msumon@student.42vienna.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/27 13:32:11 by msumon            #+#    #+#             */
/*   Updated: 2024/04/05 17:44:23 by msumon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

int	check_status(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->monitoring_mutex);
	if (philo->data->simulation_end == true)
	{
		pthread_mutex_unlock(&philo->data->monitoring_mutex);
		return (1);
	}
	pthread_mutex_unlock(&philo->data->monitoring_mutex);
	return (0);
}

void	*routine(void *args)
{
	t_philo	*philo;

	philo = (t_philo *)args;
	if (philo->data->philo_count == 1)
	{
		massages(philo, TAKEN_FORK);
		return (NULL);
	}
	while (1)
	{
		if (check_status(philo))
			break ;
		eating_action(philo);
		if (check_status(philo))
			break ;
		sleeping_action(philo);
		if (check_status(philo))
			break ;
		massages(philo, THINKING);
		if (check_status(philo))
			break ;
	}
	return (NULL);
}

bool	check_if_dead(t_data *data, t_philo *philo, int *philo_is_full)
{
	if (data->must_eat_times > 0 && philo->meals_eaten >= data->must_eat_times)
		*philo_is_full += 1;
	if (get_time()
		- philo->last_meal_time >= (long unsigned int)philo->data->time_to_die)
	{
		pthread_mutex_unlock(&data->monitoring_mutex);
		massages(philo, DIED);
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
			break ;
		}
		pthread_mutex_unlock(&data->monitoring_mutex);
	}
}
