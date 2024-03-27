/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   activities.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msumon <msumon@student.42vienna.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/27 13:40:25 by msumon            #+#    #+#             */
/*   Updated: 2024/03/27 15:40:38 by msumon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

void	monitoring(t_philo *philo, char *msg)
{
	useconds_t	timestamp;

	pthread_mutex_lock(&philo->data->monitoring_mutex);
	if (philo->data->simulation_end)
	{
		pthread_mutex_unlock(&philo->data->monitoring_mutex);
		return ;
	}
	timestamp = get_time() - philo->start_time;
	printf("%d %d %s", timestamp, philo->philo_id, msg);
	pthread_mutex_unlock(&philo->data->monitoring_mutex);
}

void	putback_forks(t_philo *philo)
{
	if (philo->philo_id % 2 == 0)
	{
		pthread_mutex_unlock(philo->left_fork);
		monitoring(philo, DROPPED_FORK);
		pthread_mutex_unlock(philo->right_fork);
		monitoring(philo, DROPPED_FORK);
	}
	else
	{
		pthread_mutex_unlock(philo->right_fork);
		monitoring(philo, DROPPED_FORK);
		pthread_mutex_unlock(philo->left_fork);
		monitoring(philo, DROPPED_FORK);
	}
}

void	pick_forks(t_philo *philo)
{
	if (philo->philo_id % 2 == 0)
	{
		pthread_mutex_lock(philo->left_fork);
		monitoring(philo, TAKEN_FORK);
		pthread_mutex_lock(philo->right_fork);
		monitoring(philo, TAKEN_FORK);
	}
	else
	{
		pthread_mutex_lock(philo->right_fork);
		monitoring(philo, TAKEN_FORK);
		pthread_mutex_lock(philo->left_fork);
		monitoring(philo, TAKEN_FORK);
	}
}

void	eating_action(t_philo *philo)
{
	pick_forks(philo);
	pthread_mutex_lock(&philo->data->monitoring_mutex);
	philo->last_meal_time = get_time();
	philo->meals_eaten += 1;
	pthread_mutex_unlock(&philo->data->monitoring_mutex);
	monitoring(philo, EATING);
	usleep(philo->data->time_to_eat * 1000);
	putback_forks(philo);
}
