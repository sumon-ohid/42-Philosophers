/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data_init.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msumon <msumon@student.42vienna.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/18 12:31:00 by msumon            #+#    #+#             */
/*   Updated: 2024/03/25 08:54:22 by msumon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

int	fork_mutex_init(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->philo_count)
	{
		pthread_mutex_init(&data->fork_mutex[i], NULL);
		i++;
	}
	data->philos[0].left_fork = &data->fork_mutex[0];
	data->philos[0].right_fork = &data->fork_mutex[data->philo_count - 1];
	i = 1;
	while (i < data->philo_count)
	{
		data->philos[i].left_fork = &data->fork_mutex[i];
		data->philos[i].right_fork = &data->fork_mutex[i - 1];
		i++;
	}
	return (0);
}

int	alloc_mem(t_data *data)
{
	data->fork_mutex = malloc(sizeof(pthread_mutex_t) * data->philo_count);
	if (!data->fork_mutex)
		return (1);
	data->t_id = malloc(sizeof(pthread_t) * data->philo_count);
	if (!data->fork_mutex)
		return (1);
	data->philos = malloc(sizeof(t_philo) * data->philo_count);
	if (!data->philos)
		return (1);
	return (0);
}

int	philo_init(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->philo_count)
	{
		data->philos[i].data = data;
		data->philos[i].id = i + 1;
		data->philos[i].time_to_die = data->time_to_die;
		data->philos[i].philo_eat = 0;
		data->philos[i].eating = 0;
		data->philos[i].status = 0;
		pthread_mutex_init(&data->philos[i].lock, NULL);
		i++;
	}
	return (0);
}

int	data_init(t_data *data, int ac, char **av)
{
	data->philo_count = ft_atoi(av[1]);
	data->time_to_die = ft_atoi(av[2]);
	data->time_to_eat = ft_atoi(av[3]);
	data->time_to_sleep = ft_atoi(av[4]);
	data->meal_count = -1;
	if (ac == 6)
		data->meal_count = ft_atoi(av[5]);
	data->dead = 0;
	data->finished = 0;
	if (alloc_mem(data))
		return (1);
	if (fork_mutex_init(data))
		return (1);
	if (philo_init(data))
		return (1);
	pthread_mutex_init(&data->main_mutex, NULL);
	pthread_mutex_init(&data->lock, NULL);
	return (0);
}
