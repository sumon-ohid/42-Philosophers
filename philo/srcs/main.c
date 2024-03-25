/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msumon <msumon@student.42vienna.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/18 12:01:49 by msumon            #+#    #+#             */
/*   Updated: 2024/03/25 08:54:14 by msumon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

int	syntax_checker(char **av)
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
		if (num < 0)
			return (1);
		while (av[i][j])
		{
			if (ft_isdigit(av[i][j]))
				return (1);
			j++;
		}
		i++;
	}
	return (0);
}

int	main(int ac, char **av)
{
	t_data	data;

	if (ac != 5 && ac != 6)
		return (1);
	if (!syntax_checker(av))
	{
		data_init(&data, ac, av);
		if (data.philo_count == 1)
			return (one_philo(&data));
		create_threads(&data);
        mutex_destroyer(&data);
	}
	else
	{
		ft_putstr_fd("Error: syntax is invalid.\n", 2);
		return (1);
	}
	return (0);
}
