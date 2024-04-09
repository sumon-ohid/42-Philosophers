/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   activities.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msumon <msumon@student.42vienna.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/27 13:40:25 by msumon            #+#    #+#             */
/*   Updated: 2024/04/09 16:34:10 by msumon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

void	sleeping_action(t_philo *philo)
{
	messages(philo, SLEEPING);
	if (ft_usleep(philo->data->time_to_sleep, philo->data))
		return ;
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

void	pick_forks_last_philo(t_philo *philo)
{
	if (philo->data->time_to_eat > philo->data->time_to_die)
		usleep(100);
	else
		usleep(philo->data->time_to_eat * 1000 + 100);
	pthread_mutex_lock(philo->right_fork);
	messages(philo, TAKEN_FORK);
	pthread_mutex_lock(philo->left_fork);
	messages(philo, TAKEN_FORK);
}

void	pick_forks(t_philo *philo)
{
	if (philo->philo_id == philo->data->philo_count && philo->data->philo_count
		% 2 != 0)
	{
		pick_forks_last_philo(philo);
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
		pthread_mutex_lock(&philo->data->monitoring_mutex);
		philo->data->simulation_end = true;
		pthread_mutex_unlock(&philo->data->monitoring_mutex);
		return (1);
	}
	pthread_mutex_lock(&philo->data->monitoring_mutex);
	philo->meals_eaten += 1;
	pthread_mutex_unlock(&philo->data->monitoring_mutex);
	putback_forks(philo);
	return (0);
}
