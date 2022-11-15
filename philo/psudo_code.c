/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   psudo_code.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kaizhang <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/14 17:56:37 by kaizhang          #+#    #+#             */
/*   Updated: 2022/11/14 22:18:03 by kaizhang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<sys/time.h>
#include<signal.h>
#include<stdlib.h>	
#include<pthread.h>	
/*
// rule only allow 4 philo to pick up chopstick at beginning so let must be a philo can eat first
//
//
// After little while, if philo cannot not get second chopstick, they must do something else first. 
//
//
//they must pick up left chopsticks and then pick up the right chopstick
//
//
//0 1 2 3 4
//0A1B2C3D4E0
semaphor(chopstick[5]);
semaphore count = 4;

void philo(int i)
{
	int count;

	count = 0;
	while(true)
	{
		think();
		p(count); // not allow other philo to eat
		request_grap(chopstick[i]) // pick up left
		request_grap(chopstick[i+1]); // pick up right
		eat();
		release_chop(i);
		release_chop(chopstick[i+1] % 5); // release left and right chopstick;
		release_signal(count); 
		sleep();
		think();
	}
}
*/


pthread_mutex_t chopstick[5];

void *myfunc(void *args)
{
	int	i;
	int left;
	int right;

	i = (int)args;
	if (i == 4)
	{
		left = i;
		right = 0;
	}
	else
	{
		left = i;
		right = i + 1;
	}
	while (1)
	{
		sleep(1);
		printf("\nphilo %d is thinking\n", i);
		pthread_mutex_lock(&chopstick[left]); //get the left chopstick
		printf("\nphilo %d grabs left chopstick %d\n", i, left);
		if (pthread_mutex_trylock(&chopstick[right]) != 0) // failed to grab the right hand chopstick
		{
			pthread_mutex_unlock(&chopstick[left]); //release left chopstick for others to use)
			continue ;
		}
		printf("\nphilo %d grabs right chopstick %d\n", i, right);
		printf("\n#philo %d is eating#\n", i);
		sleep(1);
		pthread_mutex_unlock(&chopstick[right]); //release the right chopstick
		printf("\nphilo %d drops right chopstick %d\n", i, right);
		pthread_mutex_unlock(&chopstick[left]); //release the left chopstick
		printf("\nphilo %d drops left chopstick %d\n", i, left);
	}
	return(0);
}


int main(void)
{
	pthread_t tid[5]; // 5 philo
	size_t args;
	
	args = 0;
	while(args < 5)
	{
		pthread_mutex_init(&chopstick[args], NULL);
		pthread_create(&tid[args], NULL, myfunc, (void *)args);
		++args;
	}
	args = 0;
	while(args < 5)
	{
		pthread_join(tid[args], NULL);
		++args;
	}
	return (0);
}
