/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msumon <msumon@student.42vienna.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/18 12:01:49 by msumon            #+#    #+#             */
/*   Updated: 2024/03/25 12:14:23 by msumon           ###   ########.fr       */
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

	if (ac < 5 || ac > 6)
		return (error_msg("Error: Invalid arguments\n", NULL));
	if (syntax_checker(av))
		return (error_msg("Error: Invalid arguments\n", NULL));
	if (data_init(&data, ac, av))
		return (error_msg("Error: Data init failed\n", &data));
	if (data.philo_count == 1)
		return (one_philo(&data));
	if (create_threads(&data))
		return (error_msg("Error: Invalid arguments\n", &data));
	mutex_destroyer(&data);
	return (0);
}
