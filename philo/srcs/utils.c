/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msumon <msumon@student.42vienna.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/18 12:33:56 by msumon            #+#    #+#             */
/*   Updated: 2024/04/02 15:29:34 by msumon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

int	ft_usleep(int time, t_data *data)
{
	long int	start;

	start = get_time(data, data->philos);
	while (get_time(data, data->philos) - start < time)
	{
		pthread_mutex_lock(&data->monitoring_mutex);
		if (data->simulation_end)
		{
			pthread_mutex_unlock(&data->monitoring_mutex);
			return (1);
		}
		pthread_mutex_unlock(&data->monitoring_mutex);
		if (usleep(100))
		{
			ft_putstr_fd("usleep failed.\n", 2);
			return (1);
		}
	}
	return (0);
}

long long	get_time(t_data *data, t_philo *philos)
{
	struct timeval	cur_time;

	if (gettimeofday(&cur_time, NULL))
	{
		error(philos, data, data->forks, "gettimeofday failed.");
		exit(EXIT_FAILURE);
	}
	return ((cur_time.tv_sec * 1000) + (cur_time.tv_usec / 1000));
}

int	ft_atoi(const char *str)
{
	int	i;
	int	sign;
	int	result;

	i = 0;
	sign = 1;
	result = 0;
	while (str[i] == ' ' || str[i] == '\t' || str[i] == '\n' || str[i] == '\v'
		|| str[i] == '\f' || str[i] == '\r')
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		result = result * 10 + (str[i] - '0');
		i++;
	}
	return (result * sign);
}

int	ft_isdigit(int c)
{
	if (c >= '0' && c <= '9')
		return (0);
	return (1);
}

void	ft_putstr_fd(char *str, int fd)
{
	int	i;

	i = 0;
	while (str[i])
	{
		write(fd, &str[i], 1);
		i++;
	}
}
