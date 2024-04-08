/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   activities.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msumon <msumon@student.42vienna.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/27 13:40:25 by msumon            #+#    #+#             */
/*   Updated: 2024/04/08 18:47:32 by msumon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

void	sleeping_action(t_philo *philo)
{
	messages(philo, SLEEPING);
	if (ft_usleep(philo->data->time_to_sleep, philo->data))
		return ;
}

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

void	putback_forks(t_philo *philo)
{
	if (philo->philo_id % 2 == 0)
	{
		pthread_mutex_unlock(philo->left_fork);
		pthread_mutex_unlock(philo->right_fork);
	}
	else
	{
		pthread_mutex_unlock(philo->right_fork);
		pthread_mutex_unlock(philo->left_fork);
	}
}

void	pick_forks(t_philo *philo)
{
	if (philo->philo_id == philo->data->philo_count && philo->data->philo_count
		% 2 != 0)
	{
		usleep(philo->data->time_to_eat * 1000 + 100);
		pthread_mutex_lock(philo->right_fork);
		messages(philo, TAKEN_FORK);
		pthread_mutex_lock(philo->left_fork);
		messages(philo, TAKEN_FORK);
	}
	else if (philo->philo_id % 2 == 0)
	{
		pthread_mutex_lock(philo->left_fork);
		messages(philo, TAKEN_FORK);
		pthread_mutex_lock(philo->right_fork);
		messages(philo, TAKEN_FORK);
	}
	else if (philo->philo_id != philo->data->philo_count && philo->philo_id
		% 2 == 1)
	{
		pthread_mutex_lock(philo->right_fork);
		messages(philo, TAKEN_FORK);
		pthread_mutex_lock(philo->left_fork);
		messages(philo, TAKEN_FORK);
	}
}

int	eating_action(t_philo *philo)
{
	pick_forks(philo);
	pthread_mutex_lock(&philo->data->monitoring_mutex);
	philo->last_meal_time = get_time(philo->data, philo);
	if (philo->data->gtod_failed)
	{
		pthread_mutex_unlock(&philo->data->monitoring_mutex);
		putback_forks(philo);
		return (1);
	}
	pthread_mutex_unlock(&philo->data->monitoring_mutex);
	messages(philo, EATING);
	if (ft_usleep(philo->data->time_to_eat, philo->data))
	{
		putback_forks(philo);
		return (1);
	}
	pthread_mutex_lock(&philo->data->monitoring_mutex);
	philo->meals_eaten += 1;
	pthread_mutex_unlock(&philo->data->monitoring_mutex);
	putback_forks(philo);
	return (0);
}
