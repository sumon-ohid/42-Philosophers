/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msumon <msumon@student.42vienna.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 14:18:12 by msumon            #+#    #+#             */
/*   Updated: 2024/03/26 08:07:00 by msumon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

int	one_philo(t_data *data)
{
	data->start_time = get_time();
	if (pthread_create(&data->t_id[0], NULL, &routine, &data->philos[0]))
		return (1);
	if (pthread_detach(data->t_id[0]))
		return (1);
	while (data->dead == 0)
		ft_usleep(0);
	mutex_destroyer(data);
	return (0);
}

void	*manager(void *data)
{
	t_philo	*philos;

	philos = (t_philo *)data;
	while (philos->data->dead == 0)
	{
		pthread_mutex_lock(&philos->lock);
		if (get_time() >= philos->time_to_die && philos->eating == 0)
			ft_massages(DIED, philos);
		if (philos->philo_eat == philos->data->meal_count)
		{
			pthread_mutex_lock(&philos->data->lock);
			philos->data->finished++;
			philos->philo_eat++;
			pthread_mutex_unlock(&philos->data->lock);
		}
		pthread_mutex_unlock(&philos->lock);
	}
	return ((void *)0);
}

void	*monitor(void *data)
{
	t_philo	*philo;

	philo = (t_philo *)data;
	pthread_mutex_lock(&philo->data->main_mutex);
	printf("data val: %d", philo->data->dead);
	pthread_mutex_unlock(&philo->data->main_mutex);
	while (philo->data->dead == 0)
	{
		pthread_mutex_lock(&philo->lock);
		if (philo->data->finished >= philo->data->philo_count)
			philo->data->dead = 1;
		pthread_mutex_unlock(&philo->lock);
	}
	return ((void *)0);
}

void	*routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (pthread_create(&philo->ph_id, NULL, &manager, (void *)philo))
		return ((void *)1);
	while (philo->data->dead == 0)
	{
		philo->time_to_die = get_time() + philo->data->time_to_die;
		philo_eating(philo);
		ft_massages(THINKING, philo);
	}
	if (pthread_join(philo->ph_id, NULL))
		return ((void *)1);
	return ((void *)0);
}

int	create_threads(t_data *data)
{
	int			i;
	pthread_t	thread_id;

	i = 0;
	data->start_time = get_time();
	if (data->meal_count > 0)
	{
		if (pthread_create(&thread_id, NULL, &monitor, &data->philos[0]))
			return (error_msg("pthread_create failed\n", data));
	}
	while (i < data->philo_count)
	{
		if (pthread_create(&data->t_id[i], NULL, &routine, &data->philos[i]))
			return (error_msg("pthread create failed\n", data));
		ft_usleep(1);
		i++;
	}
	i = 0;
	while (i < data->philo_count)
	{
		if (pthread_join(data->t_id[i], NULL))
			return (error_msg("pthread join failed\n", data));
		i++;
	}
	return (0);
}
