/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msumon <msumon@student.42vienna.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 14:18:12 by msumon            #+#    #+#             */
/*   Updated: 2024/03/26 19:14:16 by msumon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

void	*handle_one_philo(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	philo->time_to_die = get_time() + philo->data->time_to_die;
	if (pthread_create(&philo->ph_id, NULL, &manager, (void *)philo))
		return ((void *)1);
	pthread_mutex_lock(philo->right_fork);
	ft_massages(TAKEN_FORK, philo);
	pthread_mutex_unlock(philo->right_fork);
	ft_usleep(1);
	pthread_join(philo->ph_id, NULL);
	return ((void *)0);
}

int	one_philo(t_data *data)
{
	data->start_time = get_time();
	if (pthread_create(&data->t_id[0], NULL, &handle_one_philo,
			&data->philos[0]))
		return (error_msg("one philo pthread_create failed", data));
	if (pthread_join(data->t_id[0], NULL))
		return (error_msg("one philo pthread_detach failed", data));
	while (data->dead == 0)
		ft_usleep(0);
	free_data(data);
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
			philos->data->philo_finished_eating++;
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
	while (philo->data->dead == 0)
	{
		pthread_mutex_lock(&philo->data->lock);
		if (philo->data->philo_finished_eating >= philo->data->philo_count)
			philo->data->dead = 1;
		pthread_mutex_unlock(&philo->data->lock);
	}
	return ((void *)0);
}

void	*routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	pthread_mutex_lock(&philo->data->lock);
	philo->time_to_die = get_time() + philo->data->time_to_die;
	pthread_mutex_unlock(&philo->data->lock);
	if (pthread_create(&philo->ph_id, NULL, &manager, (void *)philo))
		return ((void *)1);
	pthread_detach(philo->ph_id);
	while (philo->data->dead == 0)
	{
		philo_eating(philo);
		ft_massages(THINKING, philo);
	}
	ft_usleep(1);
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
		pthread_detach(thread_id);
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
