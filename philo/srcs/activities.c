/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   activities.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msumon <msumon@student.42vienna.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/27 13:40:25 by msumon            #+#    #+#             */
/*   Updated: 2024/04/05 01:38:48 by msumon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

void	sleeping_action(t_philo *philo)
{
	massages(philo, SLEEPING);
	ft_usleep(philo->data->time_to_sleep, philo->data);
}

void	massages(t_philo *philo, char *msg)
{
	long unsigned int	timestamp;

	pthread_mutex_lock(&philo->data->monitoring_mutex);
	if (philo->data->simulation_end)
	{
		pthread_mutex_unlock(&philo->data->monitoring_mutex);
		return ;
	}
	timestamp = get_time() - philo->start_time;
	printf("%ld %d %s", timestamp, philo->philo_id, msg);
	pthread_mutex_unlock(&philo->data->monitoring_mutex);
}

void	putback_forks(t_philo *philo)
{
	pthread_mutex_unlock(philo->right_fork);
	pthread_mutex_unlock(philo->left_fork);
}

int	pick_forks(t_philo *philo)
{
	if (philo->philo_id % 2 == 0)
		pthread_mutex_lock(philo->right_fork);
	else
		pthread_mutex_lock(philo->left_fork);
	massages(philo, TAKEN_FORK);
	if (philo->philo_id % 2 == 0)
	{
		if (pthread_mutex_lock(philo->left_fork))
		{
			pthread_mutex_unlock(philo->right_fork);
			return (1);
		}
		massages(philo, TAKEN_FORK);
	}
	else
	{
		if (pthread_mutex_lock(philo->right_fork))
		{
			pthread_mutex_unlock(philo->left_fork);
			return (1);
		}
		massages(philo, TAKEN_FORK);
	}
	return (0);
}

void	eating_action(t_philo *philo)
{
	if (pick_forks(philo))
		return ;
	pthread_mutex_lock(&philo->data->monitoring_mutex);
	philo->last_meal_time = get_time();
	pthread_mutex_unlock(&philo->data->monitoring_mutex);
	massages(philo, EATING);
	ft_usleep(philo->data->time_to_eat, philo->data);
	pthread_mutex_lock(&philo->data->monitoring_mutex);
	philo->meals_eaten += 1;
	pthread_mutex_unlock(&philo->data->monitoring_mutex);
	putback_forks(philo);
}
