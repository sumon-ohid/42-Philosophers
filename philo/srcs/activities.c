/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   activities.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msumon <msumon@student.42vienna.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 13:38:28 by msumon            #+#    #+#             */
/*   Updated: 2024/03/26 14:59:49 by msumon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

int	ft_usleep(useconds_t time)
{
	int	start;

	start = get_time();
	while (get_time() - start < (int)time)
		usleep(time);
	return (0);
}

void	putback_forks(t_philo *philos)
{
	pthread_mutex_unlock(philos->right_fork);
	pthread_mutex_unlock(philos->left_fork);
	ft_massages(SLEEPING, philos);
	ft_usleep(philos->data->time_to_sleep);
}

void	ft_massages(char *msg, t_philo *philos)
{
	int	time;

	pthread_mutex_lock(&philos->data->main_mutex);
	time = get_time() - philos->data->start_time;
	if (ft_strcmp(DIED, msg) == 0 && philos->data->dead == 0)
	{
		printf("%d %d %s\n", time, philos->id, msg);
		philos->data->dead = 1;
	}
	if (!philos->data->dead)
		printf("%d %d %s\n", time, philos->id, msg);
	pthread_mutex_unlock(&philos->data->main_mutex);
}

void	take_forks(t_philo *philos)
{
	pthread_mutex_lock(philos->right_fork);
	ft_massages(TAKEN_FORK, philos);
	pthread_mutex_lock(philos->left_fork);
	ft_massages(TAKEN_FORK, philos);
}

void	philo_eating(t_philo *philos)
{
	take_forks(philos);
	pthread_mutex_lock(&philos->lock);
	philos->eating = 1;
	philos->time_to_die = get_time() + philos->data->time_to_die;
	ft_massages(EATING, philos);
	philos->philo_eat++;
	ft_usleep(philos->data->time_to_eat);
	philos->eating = 0;
	pthread_mutex_unlock(&philos->lock);
	putback_forks(philos);
}
