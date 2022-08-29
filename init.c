/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iouardi <iouardi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/29 17:15:46 by iouardi           #+#    #+#             */
/*   Updated: 2022/08/29 17:17:37 by iouardi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	init_struct(t_philo *philo, t_struct *mystruct)
{
	int			i;

	i = 0;
	(void)philo;
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

void	*sm_function(void *p)
{
	int		temp;
	t_philo	*philo;
	int		philos_chb3o;
	int		death_flag;

	temp = 0;
	philo = (t_philo *)p;
		philos_chb3o = philo->mystruct->check_philos_chb3o;
		death_flag = philo->mystruct->death_flag;
	while (!death_flag && !philos_chb3o)
	{
		if (philo->mystruct->num_of_meals != -1 && \
			philo->eaten_meals == philo->mystruct->num_of_meals)
			continue ;
		philos_chb3o = philo->mystruct->check_philos_chb3o;
		death_flag = philo->mystruct->death_flag;
		philo_eating(philo);
		philo_sleeping(philo);
		philo_thinking(philo);
	}
	return (philo);
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
	if (pthread_mutex_init(&mystruct->msg, NULL))
	{
		printf("mutex init failed\n");
		return (2);
	}
	if (pthread_mutex_init(&mystruct->death, NULL))
	{
		printf("mutex init failed\n");
		return (2);
	}
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
