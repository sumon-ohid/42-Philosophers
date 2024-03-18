/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msumon <msumon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/18 12:01:49 by msumon            #+#    #+#             */
/*   Updated: 2024/03/18 16:48:05 by msumon           ###   ########.fr       */
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
    sleep(2);
    printf("inside my function\n");
    return (NULL);
}

void    create_threads(t_data *data, char *av)
{
    pthread_t thread_id;

    printf("before thread\n");
    pthread_create(&thread_id, NULL, my_function, NULL);
    pthread_join(thread_id, NULL);
    printf("%lu\n", thread_id);
    printf("after threads\n");
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
        // philosophers eating only if both forks are available
        // and no philosopher is eating from the same fork
        // philo[i] = eating, philo[i + 1] != eating 
        // philo[i - 1] != eating
    }
    else
    {
        ft_putstr_fd("Error: syntax is invalid.\n", 2);
        return (1);
    }
    return (0);
}
