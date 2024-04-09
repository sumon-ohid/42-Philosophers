/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   messages.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msumon <msumon@student.42vienna.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/09 16:33:25 by msumon            #+#    #+#             */
/*   Updated: 2024/04/09 16:34:20 by msumon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

void	messages(t_philo *philo, char *msg)
{
	long long	timestamp;

	pthread_mutex_lock(&philo->data->monitoring_mutex);
	if (philo->data->simulation_end)
	{
		pthread_mutex_unlock(&philo->data->monitoring_mutex);
		return ;
	}
	timestamp = get_time(philo->data, philo) - philo->start_time;
	printf("%lld %d %s", timestamp, philo->philo_id, msg);
	pthread_mutex_unlock(&philo->data->monitoring_mutex);
}
