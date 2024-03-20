/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data_init.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msumon <msumon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/18 12:31:00 by msumon            #+#    #+#             */
/*   Updated: 2024/03/20 13:50:37 by msumon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

int alloc_mem(t_data *data)
{
	data->fork_mutex = malloc (sizeof(t_data) * data->philo_count);
	if(!data->fork_mutex)
		return (1);
	data->t_id = malloc (sizeof(t_data) * data->philo_count);
	if (!data->fork_mutex)
		return (1);
	return (1);
}

int fork_mutex_init(t_data *data)
{
	(void)data;
	printf ("Hey!!!\n");
	return (0);
}

int mutex_init(t_data *data)
{
	int i;

	i = 0;
	while (i < data->philo_count)
	{
		if(pthread_mutex_init(data->fork_mutex, NULL))
			error_msg("mutex init failed in mutex_init\n");
		i++;
	}
	// i = 0;
	// if (pthread_mutex_init(data->main_mutex, NULL))
	// 	return (1);
	return (0);
}

int philo_init(t_data *data)
{
	(void) data;
	t_philo *philos;

	philos = malloc (sizeof (t_philo) * data->philo_count);
	if (!philos)
		return (1);
	philos->philo_eat = 0;
	philos->philo_sleep = 0;
	philos->philo_think = 0;
	philos->philo_died = 0;
	return (0);
}

int	data_init(t_data *data, int ac, char **av)
{
	data->philo_count = ft_atoi(av[1]);
	data->forks = ft_atoi(av[1]);
	data->time_to_die = ft_atoi(av[2]);
	data->time_to_eat = ft_atoi(av[3]);
	data->time_to_sleep = ft_atoi(av[4]);
	data->forks = data->philo_count;
	if (ac == 6)
		data->meal_count = ft_atoi(av[5]);
	else
		data->meal_count = -1;
	if (alloc_mem(data))
		return (1);
	if (philo_init(data))
		return (1);
	if (mutex_init(data))
		return (1);
	// if (fork_mutex_init(data))
	// 	return (1);
	return (0);
}
