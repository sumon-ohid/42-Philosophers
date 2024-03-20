/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msumon <msumon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/20 12:40:36 by msumon            #+#    #+#             */
/*   Updated: 2024/03/20 13:22:10 by msumon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <pthread.h> 
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <string.h>


pthread_t tid[2];
pthread_mutex_t lock;
int counter;

void *my_func(void *arg)
{
    (void)arg;
    pthread_mutex_lock(&lock);
    unsigned long i = 0;
    counter +=1;
    printf("Job %d has started\n", counter);
    for (i = 0; i < 10; i++)
        printf("--------> [%lu]\n", i);
    printf("Job %d finished\n", counter);
    pthread_mutex_unlock(&lock);
    return (NULL);
}

int main(void)
{
    int i = 0;
    int error;

    if(pthread_mutex_init(&lock, NULL))
        perror("pthread init failed");
    while (i < 2)
    {
        error = pthread_create(&(tid[i]), NULL, my_func, NULL);
        if (error != 0)
        {
            perror("Thread can not be created");
        }
        i++;
    }
    i = 0;
    while (i < 2)
    {
        pthread_join(tid[i], NULL);
        i++;
    }
    pthread_mutex_destroy(&lock);
    return (0);
}