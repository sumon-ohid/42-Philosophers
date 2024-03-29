/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data_init.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msumon <msumon@student.42vienna.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/18 12:31:00 by msumon            #+#    #+#             */
/*   Updated: 2024/03/29 10:16:00 by msumon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

void	free_forks(pthread_mutex_t *forks, int i)
{
	int	j;

	j = 0;
	while (j < i)
	{
		pthread_mutex_destroy(&forks[j]);
		j++;
	}
	free(forks);
}

pthread_mutex_t	*init_forks(t_data *data)
{
	pthread_mutex_t	*forks;
	int				i;

	forks = malloc(sizeof(pthread_mutex_t) * data->philo_count);
	if (!forks)
		return (NULL);
	i = 0;
	while (i < data->philo_count)
	{
		if (pthread_mutex_init(&forks[i], NULL) != 0)
		{
			free_forks(forks, i);
			return (NULL);
		}
		i++;
	}
	return (forks);
}

t_philo	*init_philos(t_data *data, pthread_mutex_t *forks)
{
	t_philo	*philos;
	int		i;

	philos = (t_philo *)malloc(sizeof(t_philo) * data->philo_count);
	if (!philos)
		return (NULL);
	i = 0;
	while (i < data->philo_count)
	{
		philos[i].philo_id = i + 1;
		philos[i].meals_eaten = 0;
		philos[i].left_fork = &forks[i];
		philos[i].right_fork = &forks[(i + 1) % data->philo_count];
		philos[i].last_meal_time = get_time(data, philos);
		philos[i].data = data;
		i++;
	}
	return (philos);
}

int	data_init(t_data *data, char **av)
{
	data->philo_count = ft_atoi(av[1]);
	data->time_to_die = ft_atoi(av[2]);
	data->time_to_eat = ft_atoi(av[3]);
	data->time_to_sleep = ft_atoi(av[4]);
	data->must_eat_times = -1;
	if (av[5])
		data->must_eat_times = ft_atoi(av[5]);
	if (data->philo_count == 0 || data->must_eat_times == 0)
	{
		ft_putstr_fd("Error: Must be bigger than 0\n", 2);
		return (1);
	}
	if (pthread_mutex_init(&data->monitoring_mutex, NULL))
	{
		ft_putstr_fd("Error: Mutex init failed\n", 2);
		return (1);
	}
	data->simulation_end = false;
	return (0);
}
