/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iouardi <iouardi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/29 17:15:46 by iouardi           #+#    #+#             */
/*   Updated: 2022/08/29 22:46:04 by iouardi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	init_struct(t_struct *mystruct)
{
	int			i;

	i = 0;
	mystruct->philo = malloc(sizeof(t_philo) * mystruct->num_of_philos);
	mystruct->forks = malloc (sizeof(pthread_mutex_t) * \
		mystruct->num_of_philos);
	while (i < mystruct->num_of_philos)
	{
		mystruct->philo[i].mystruct = mystruct;
		mystruct->philo[i].id = i;
		mystruct->philo[i].eaten_meals = 0;
		mystruct->philo[i].time_since_last_meal = 0;
		i++;
	}
	mystruct->philo_chb3 = 0;
	mystruct->check_philos_chb3o = 0;
	mystruct->death_flag = 0;
	mystruct->flag = 0;
}

int	init_mutexes_supp(pthread_mutex_t *mutex)
{
	if (pthread_mutex_init(mutex, NULL))
	{
		printf("mutex init failed\n");
		return (2);
	}
	return (0);
}

int	init_mutexes(t_struct *mystruct)
{
	int		i;

	i = 0;
	while (i < mystruct->num_of_philos)
	{
		if (pthread_mutex_init(&mystruct->forks[i], NULL))
		{
			printf("mutex init failed\n");
			return (2);
		}
		i++;
	}
	if (init_mutexes_supp(&mystruct->msg) || \
		init_mutexes_supp(&mystruct->death) \
		|| init_mutexes_supp(&mystruct->eaten) \
		|| init_mutexes_supp(&mystruct->chb3))
		return (2);
	return (0);
}

int	create_odd_threads(t_struct *mystruct)
{
	int		i;
	int		err;

	i = 1;
	while (i < mystruct->num_of_philos)
	{
		err = pthread_create(&mystruct->philo[i].philo_diali, \
			NULL, sm_function, &mystruct->philo[i]);
		if (err)
		{
			printf("creating thread failed\n");
			return (3);
		}
		usleep(50);
		i += 2;
	}
	return (0);
}

int	create_threads(t_struct *mystruct)
{
	int		i;
	int		err;

	i = 0;
	while (i < mystruct->num_of_philos)
	{
		err = pthread_create(&mystruct->philo[i].philo_diali, \
			NULL, sm_function, &mystruct->philo[i]);
		if (err)
		{
			printf("creating thread failed\n");
			return (3);
		}
		usleep(50);
		i += 2;
	}
	if (create_odd_threads(mystruct))
		return (3);
	return (0);
}
