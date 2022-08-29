/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads_checker.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iouardi <iouardi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/29 17:19:26 by iouardi           #+#    #+#             */
/*   Updated: 2022/08/29 17:19:52 by iouardi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	death_checker(int i, t_struct *mystruct, long time)
{
	int		j;

	if ((time - mystruct->philo[i].time_since_last_meal >= mystruct->time_die))
	{
		pthread_mutex_lock(&mystruct->death);
		mystruct->death_flag = 1;
		pthread_mutex_unlock(&mystruct->death);
		pthread_mutex_lock(&mystruct->msg);
		printf ("%ld philo %d died\n", time, i + 1);
		j = 0;
		while (j < mystruct->num_of_philos)
		{
			pthread_detach(mystruct->philo[j].philo_diali);
			j++;
		}
		return (0);
	}
	return (1);
}

int	philos_chb3o_check(int i, t_struct *mystruct)
{
	int		j;

	if (mystruct->num_of_meals != -1 && mystruct->philo[i].eaten_meals \
		== mystruct->num_of_meals)
	{
		if (mystruct->philo_chb3 == mystruct->num_of_philos)
		{
			pthread_mutex_lock(&mystruct->chb3);
			mystruct->check_philos_chb3o = 1;
			pthread_mutex_lock(&mystruct->chb3);
			j = 0;
			while (j < mystruct->num_of_philos)
			{
				pthread_detach(mystruct->philo[j].philo_diali);
				j++;
			}
			return (0);
		}
	}
	return (1);
}

int	threads_checker(t_struct *mystruct)
{
	int		i;
	long	time;

	while (1)
	{
		usleep(100);
		i = 0;
		while (i < mystruct->num_of_philos)
		{
			time = timing_function(mystruct->start);
			if (!death_checker(i, mystruct, time))
				return (0);
			if (!philos_chb3o_check(i, mystruct))
				return (0);
			i++;
		}
	}
	return (1);
}
