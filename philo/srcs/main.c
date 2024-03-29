/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msumon <msumon@student.42vienna.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/18 12:01:49 by msumon            #+#    #+#             */
/*   Updated: 2024/03/29 13:28:59 by msumon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

void	destroy_data(t_data *data, pthread_mutex_t *forks, t_philo *philos)
{
	int	i;

	if (data)
		pthread_mutex_destroy(&data->monitoring_mutex);
	if (forks)
	{
		i = 0;
		while (i < data->philo_count)
		{
			pthread_mutex_destroy(&forks[i]);
			philos[i].left_fork = NULL;
			philos[i].right_fork = NULL;
			i++;
		}
		free(forks);
		forks = NULL;
	}
	if (philos)
	{
		free(philos);
		philos = NULL;
	}
}

int	syntax_checker(char **av, int ac)
{
	int	i;
	int	j;
	int	num;

	num = 0;
	i = 1;
	while (av[i])
	{
		j = 0;
		num = ft_atoi(av[i]);
		if (num < 0 || num > INT_MAX)
			return (1);
		while (av[i][j])
		{
			if (ft_isdigit(av[i][j]))
				return (1);
			j++;
		}
		i++;
	}
	if (ac < 5 || ac > 6)
		return (1);
	return (0);
}

int	error(t_philo *philos, t_data *data, pthread_mutex_t *forks,
		char *error_msg)
{
	destroy_data(data, forks, philos);
	ft_putstr_fd(error_msg, 2);
	ft_putstr_fd("\n", 2);
	return (1);
}

int	main(int ac, char **av)
{
	t_data	data;
	t_philo	*philos;

	if (syntax_checker(av, ac))
		return (1);
	if (data_init(&data, av))
		return (1);
	data.forks = init_forks(&data);
	if (!data.forks)
		return (1);
	philos = init_philos(&data, data.forks);
	if (!philos)
	{
		free_forks(data.forks, data.philo_count - 1);
		return (1);
	}
	if (create_threads_and_join(&data, philos, 0))
		return (error(philos, &data, data.forks, "ctreate and join failed"));
	destroy_data(&data, data.forks, philos);
	return (0);
}
