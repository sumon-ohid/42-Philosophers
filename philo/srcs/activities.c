/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   activities.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msumon <msumon@student.42vienna.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/27 13:40:25 by msumon            #+#    #+#             */
/*   Updated: 2024/04/02 15:41:51 by msumon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

void	sleeping_action(t_philo *philo)
{
	monitoring(philo, SLEEPING);
	ft_usleep(philo->data->time_to_sleep, philo->data);
}

void	monitoring(t_philo *philo, char *msg)
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
	if (philo->philo_id % 2 == 0 && philo->philo_id != philo->data->philo_count)
	{
		usleep(philo->data->time_to_eat * 1000 + 100);
		pthread_mutex_lock(philo->left_fork);
		monitoring(philo, TAKEN_FORK);
		pthread_mutex_lock(philo->right_fork);
		monitoring(philo, TAKEN_FORK);
	}
	else if (philo->philo_id % 2 != 0 && philo->philo_id != philo->data->philo_count)
	{

		usleep(philo->data->time_to_eat / 2 * 1000);
		pthread_mutex_lock(philo->right_fork);
		monitoring(philo, TAKEN_FORK);
		pthread_mutex_lock(philo->left_fork);
		monitoring(philo, TAKEN_FORK);
	}
	else if (philo->philo_id == philo->data->philo_count)
	{
		usleep(philo->data->time_to_eat * 1000 + 100);
		pthread_mutex_lock(philo->left_fork);
		monitoring(philo, TAKEN_FORK);
		pthread_mutex_lock(philo->right_fork);
		monitoring(philo, TAKEN_FORK);
	}
}

void	eating_action(t_philo *philo)
{
	pick_forks(philo);
	pthread_mutex_lock(&philo->data->monitoring_mutex);
	philo->last_meal_time = get_time(philo->data, philo);
	pthread_mutex_unlock(&philo->data->monitoring_mutex);
	monitoring(philo, EATING);
	ft_usleep(philo->data->time_to_eat, philo->data);
	pthread_mutex_lock(&philo->data->monitoring_mutex);
	philo->meals_eaten += 1;
	pthread_mutex_unlock(&philo->data->monitoring_mutex);
	putback_forks(philo);
}
