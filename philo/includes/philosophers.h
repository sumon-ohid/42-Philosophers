/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msumon <msumon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/18 16:01:48 by msumon            #+#    #+#             */
/*   Updated: 2024/03/20 14:30:30 by msumon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <pthread.h>
# include <sys/time.h>

# define FORK "has taken a fork"
# define EATING "is eating"
# define SLEEPING "is sleeping"
# define THINKING "is thinking"
# define DIED "died"

struct  s_data;

typedef struct s_philo
{
    struct s_data   *data;
    int             philo_eat;
    int             philo_sleep;
    int             philo_think;
    int             philo_died;
    pthread_mutex_t *left_fork;
    pthread_mutex_t *right_fork;
}              t_philo;

typedef struct s_data
{
    pthread_t       *t_id;
    int				philo_count;
    int				time_to_die;
    int				time_to_eat;
    int				time_to_sleep;
    int				meal_count;
    int				forks;
    pthread_mutex_t	*fork_mutex;
    pthread_mutex_t	*main_mutex;
}				t_data;

// clean up
int error_msg(char *msg);

//utils
void        ft_putstr_fd(char *str, int fd);
int		    ft_atoi(const char *str);
long	    ft_atol(char *str);
int         ft_isdigit(int c);
long long   get_time();


//init
int         data_init(t_data *data, int ac, char **av);


#endif