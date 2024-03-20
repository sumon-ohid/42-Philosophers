/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_01.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msumon <msumon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/18 12:33:56 by msumon            #+#    #+#             */
/*   Updated: 2024/03/20 14:23:32 by msumon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

long long get_time()
{
    struct timeval cur_time;
    long long msec;

    if(gettimeofday(&cur_time, NULL))
        return(error_msg("gettimeofday failed\n"));
    msec = (cur_time.tv_sec / 1000) + (cur_time.tv_usec * 1000);
    return (msec);
}

int     ft_atoi(const char *str)
{
    int     i;
    int     sign;
    int     result;

    i = 0;
    sign = 1;
    result = 0;
    while (str[i] == ' ' || str[i] == '\t' || str[i] == '\n'
        || str[i] == '\v' || str[i] == '\f' || str[i] == '\r')
        i++;
    if (str[i] == '-' || str[i] == '+')
    {
        if (str[i] == '-')
            sign = -1;
        i++;
    }
    while (str[i] >= '0' && str[i] <= '9')
    {
        result = result * 10 + (str[i] - '0');
        i++;
    }
    return (result * sign);
}

int     ft_isdigit(int c)
{
    if (c >= '0' && c <= '9')
        return (0);
    return (1);
}

void ft_putstr_fd(char *str, int fd)
{
    int i;

    i = 0;
    while (str[i])
    {
        write(fd, &str[i], 1);
        i++;
    }    
}
