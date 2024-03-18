/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msumon <msumon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/18 16:01:48 by msumon            #+#    #+#             */
/*   Updated: 2024/03/18 16:11:33 by msumon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <pthread.h>

typedef struct s_data
{
    int				philo_count;
    int				time_to_die;
    int				time_to_eat;
    int				time_to_sleep;
    int				meal_count;
    int				forks;
    pthread_mutex_t	*fork_mutex;
    pthread_mutex_t	*print_mutex;
}				t_data;

//utils
void    ft_putstr_fd(char *str, int fd);
int		ft_atoi(const char *str);
int     ft_isdigit(int c);


//init
void    data_init(t_data *data, int ac, char **av);


#endif