/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msumon <msumon@student.42vienna.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/18 16:01:48 by msumon            #+#    #+#             */
/*   Updated: 2024/03/25 08:54:51 by msumon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>

# define FORK "has taken a fork"
# define EATING "is eating"
# define SLEEPING "is sleeping"
# define THINKING "is thinking"
# define DIED "died"

struct s_data;

typedef struct s_philo
{
	struct s_data	*data;
	pthread_t		ph_id;
	int				id;
	int				philo_eat;
	int				time_to_die;
	int				eating;
	int				status;
	pthread_mutex_t	lock;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
}					t_philo;

typedef struct s_data
{
	pthread_t		*t_id;
	long long		start_time;
	int				dead;
	int				finished;
	int				philo_count;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				meal_count;
	int				forks;
	t_philo			*philos;
	pthread_mutex_t	*fork_mutex;
	pthread_mutex_t	lock;
	pthread_mutex_t	main_mutex;
}					t_data;

// clean up
int					error_msg(char *msg, t_data *data);
int					mutex_destroyer(t_data *data);

// utils
void				ft_putstr_fd(char *str, int fd);
int					ft_atoi(const char *str);
long				ft_atol(char *str);
int					ft_isdigit(int c);
long long			get_time(void);
int					ft_usleep(useconds_t time);
int					ft_strcmp(char *s1, char *s2);

// activities
void				philo_eating(t_philo *philos);
void				ft_massages(char *msg, t_philo *philos);

// main
void				*routine(void *arg);

// init
int					data_init(t_data *data, int ac, char **av);

// threads
int                 create_threads(t_data *data);
int                 one_philo(t_data *data);

#endif