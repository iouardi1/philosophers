/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iouardi <iouardi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/17 20:48:00 by iouardi           #+#    #+#             */
/*   Updated: 2022/08/29 17:22:43 by iouardi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	sleep_accurate(t_struct *mystruct, long n)
{
	int		i;
	long	current_time;

	i = 0;
	current_time = timing_function(mystruct->start);
	while (timing_function(mystruct->start) - current_time < n)
		usleep(500);
}

long	timing_function(long start_time)
{
	int				err;
	struct timeval	current_time;
	long			time_adjust;

	err = gettimeofday(&current_time, NULL);
	if (err)
	{
		printf("gettimeofday function has failed\n");
		return (1);
	}
	time_adjust = (current_time.tv_sec * 1000) + \
		(current_time.tv_usec / 1000) - start_time;
	return (time_adjust);
}

void	printing_function(char *msg, t_philo *philo, long time, int i)
{
	time = timing_function(philo->mystruct->start);
	if (!philo->mystruct->death_flag && time - \
		philo->time_since_last_meal < philo->mystruct->time_die)
	{
		pthread_mutex_lock(&philo->mystruct->msg);
		printf ("%ld philo %d %s\n", time, i + 1, msg);
		pthread_mutex_unlock(&philo->mystruct->msg);
	}
	else
	{
		pthread_mutex_lock(&philo->mystruct->death);
		philo->mystruct->death_flag = 1;
		pthread_mutex_unlock(&philo->mystruct->death);
	}
}

int	main(int argc, char **argv)
{
	t_struct			*mystruct;

	mystruct = malloc (sizeof(t_struct));
	if (parsing(mystruct, argv, argc))
		return (1);
	else
	{
		mystruct->philo = malloc(sizeof(t_philo));
		init_struct(mystruct->philo, mystruct);
		if (init_mutexes(mystruct))
			return (2);
		if (create_threads(mystruct))
			return (3);
		if (!threads_checker(mystruct))
			return (0);
	}
	return (0);
}
