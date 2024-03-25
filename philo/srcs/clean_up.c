/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_up.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msumon <msumon@student.42vienna.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/20 10:30:48 by msumon            #+#    #+#             */
/*   Updated: 2024/03/25 08:54:34 by msumon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

void	free_data(t_data *data)
{
	if (data->t_id)
		free(data->t_id);
	if (data->philos)
		free(data->philos);
	if (data->fork_mutex)
		free(data->fork_mutex);
}

int	mutex_destroyer(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->philo_count)
	{
		pthread_mutex_destroy(&data->fork_mutex[i]);
		pthread_mutex_destroy(&data->philos[i].lock);
        i++;
	}
	pthread_mutex_destroy(&data->main_mutex);
	pthread_mutex_destroy(&data->lock);
	free_data(data);
	return (0);
}

int	error_msg(char *msg, t_data *data)
{
	ft_putstr_fd(msg, 2);
	if (data)
		mutex_destroyer(data);
	return (1);
}
