/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_join.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msumon < msumon@student.42vienna.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/05 17:38:42 by msumon            #+#    #+#             */
/*   Updated: 2024/04/05 17:39:13 by msumon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

int	create_odd_threads(t_data *data, t_philo *philos, int k, int j)
{
	int	i;

	i = -1;
	while (++i < data->philo_count)
	{
		if (i % 2 == 1)
		{
			if (pthread_create(&philos[i].thread_id, NULL, routine,
					&philos[i]) != 0)
			{
				pthread_mutex_lock(&data->monitoring_mutex);
				data->simulation_end = true;
				pthread_mutex_unlock(&data->monitoring_mutex);
				while (j < i + k)
					if (pthread_join(philos[j++].thread_id, NULL))
						return (1);
				return (1);
			}
		}
	}
	return (0);
}

int	create_even_threads(t_data *data, t_philo *philos, int j, int *num)
{
	int	i;

	i = -1;
	while (++i < data->philo_count)
	{
		if (i % 2 == 0)
		{
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
	}
	*num = i;
	return (0);
}

int	create_threads_and_join(t_data *data, t_philo *philos, int j)
{
	int	num;
	int	i;
	int	k;

	k = 0;
	i = 0;
	philos->start_time = get_time();
	if (create_even_threads(data, philos, j, &num))
		return (1);
	k = num;
	usleep(200);
	if (create_odd_threads(data, philos, k, 0))
		return (1);
	watch_tower(data, philos);
	while (++i < data->philo_count)
	{
		if (pthread_join(philos[i].thread_id, NULL) != 0)
			return (1);
	}
	return (0);
}
