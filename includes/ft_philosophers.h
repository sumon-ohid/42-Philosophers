#ifndef FT_PHILOSOPHERS_H
# define FT_PHILOSOPHERS_H

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
int		ft_atoi(const char *str);
int     ft_isdigit(int c);


//init
void    data_init(t_data *data, int ac, char **av);


#endif