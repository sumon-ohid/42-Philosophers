/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msumon <msumon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/18 12:01:49 by msumon            #+#    #+#             */
/*   Updated: 2024/03/20 14:40:09 by msumon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

int syntax_checker(char **av)
{
    int i;
    int j;
    int num;

    num = 0;
    i = 1;
    while (av[i])
    {
        j = 0;
        num = ft_atoi(av[i]);
        if (num < 0)
            return (1);
        while (av[i][j])
        {
            if (ft_isdigit(av[i][j]))
                return (1);
            j++;
        }
        i++;
    }
    return (0);
}

void    *my_function(void *arg)
{
    t_data *data;
    long long time;

    time = get_time();
    data = (void*)arg;
    pthread_mutex_lock(data->fork_mutex);
    printf("%lld ", time);
    printf("%d ", data->philo_count);
    printf ("is eating\n");
    pthread_mutex_unlock(data->fork_mutex);
    return (NULL);
}

void    create_threads(t_data *data, char *av)
{
    (void)av;
    int i;
    
    i = 0;
    while (i < data->philo_count)
    {
        pthread_create(&data->t_id[i], NULL, my_function, data);
        i++;
    }
    i = 0;
    while (i < data->philo_count)
    {
        pthread_join(data->t_id[i], NULL);
        i++;
    }
    free(data->t_id);
    free(data->fork_mutex);
    pthread_mutex_destroy(data->fork_mutex);
}

int main(int ac, char **av)
{
    t_data *data;

    if (ac != 5 && ac != 6)
    {
        printf("Error: Invalid number of arguments.\n");
        printf("Usage: ./philo number_of_philosophers\n");
        printf("time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]\n");
        return (1);
    }
    data = (t_data *)malloc(sizeof(t_data));
    if (!data)
        return (1);
    if (!syntax_checker(av))
    {
        data_init(data, ac, av);
        create_threads(data, av[1]);
    }
    else
    {
        ft_putstr_fd("Error: syntax is invalid.\n", 2);
        return (1);
    }
    return (0);
}
