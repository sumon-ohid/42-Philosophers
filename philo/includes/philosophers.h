/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msumon <msumon@student.42vienna.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/18 16:01:48 by msumon            #+#    #+#             */
/*   Updated: 2024/04/04 23:40:03 by msumon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <limits.h>
# include <pthread.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>

# define TAKEN_FORK "has taken a fork\n"
# define DROPPED_FORK "has dropped a fork\n"
# define EATING "is eating\n"
# define SLEEPING "is sleeping\n"
# define THINKING "is thinking\n"
# define DIED "died\n"

struct	s_philo;

typedef struct s_data
{
	struct s_philo	*philos;
	int				philo_count;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				must_eat_times;
	bool			simulation_end;
	pthread_mutex_t	*forks;
	pthread_mutex_t	monitoring_mutex;
}					t_data;

typedef struct s_philo
{
	int				philo_id;
	int				meals_eaten;
	long unsigned	int	last_meal_time;
	long unsigned	int	start_time;
	pthread_t		thread_id;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	t_data			*data;
}					t_philo;

// utils.c
void				ft_putstr_fd(char *str, int fd);
int					ft_atoi(const char *str);
int					ft_isdigit(int c);
long unsigned int	get_time();
void				ft_usleep(long unsigned int time, t_data *data);

// initialization
int					data_init(t_data *data, char **av);
pthread_mutex_t		*init_forks(t_data *data);
t_philo				*init_philos(t_data *data, pthread_mutex_t *forks);

// simulation
void				massages(t_philo *philo, char *msg);
void				eating_action(t_philo *philo);
void				sleeping_action(t_philo *philo);
int					create_threads_and_join(t_data *data, t_philo *philos,
						int j);
int  				check_status(t_philo *philo);

// free and error handling
void				free_forks(pthread_mutex_t *forks, int i);
int					error(t_philo *philos, t_data *data, pthread_mutex_t *forks,
						char *error_msg);

#endif