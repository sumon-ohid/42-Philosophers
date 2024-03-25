/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   activities.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msumon <msumon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 13:38:28 by msumon            #+#    #+#             */
/*   Updated: 2024/03/25 08:43:54 by msumon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

void	putback_forks(t_philo *philos)
{
	pthread_mutex_unlock(philos->left_fork);
	pthread_mutex_unlock(philos->right_fork);
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
	ft_massages(FORK, philos);
	pthread_mutex_unlock(philos->left_fork);
	ft_massages(FORK, philos);
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
