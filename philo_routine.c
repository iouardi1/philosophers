/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iouardi <iouardi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/29 17:21:16 by iouardi           #+#    #+#             */
/*   Updated: 2022/08/29 23:04:38 by iouardi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	initialize_time(t_philo *philo)
{
	struct timeval	current_time;

	if (!philo->mystruct->flag)
	{
		gettimeofday(&current_time, NULL);
		philo->mystruct->start = (current_time.tv_sec * 1000) + \
			(current_time.tv_usec / 1000);
		philo->mystruct->flag = 1;
	}
}

void	philo_eating(t_philo *philo)
{
	int				n;
	long			temp_time;

	n = philo->mystruct->num_of_philos;
	initialize_time(philo);
	pthread_mutex_lock(&philo->mystruct->forks[philo->id]);
	temp_time = timing_function(philo->mystruct->start);
	printing_function("has taking a fork", philo, temp_time, philo->id);
	pthread_mutex_lock(&philo->mystruct->forks[(philo->id + 1) % n]);
	temp_time = timing_function(philo->mystruct->start);
	printing_function("has taking a fork", philo, temp_time, philo->id);
	temp_time = timing_function(philo->mystruct->start);
	printing_function("is eating", philo, temp_time, philo->id);
	temp_time = timing_function(philo->mystruct->start);
	philo->mystruct->philo[philo->id].time_since_last_meal = temp_time;
	pthread_mutex_lock(&philo->mystruct->eaten);
	philo->eaten_meals += 1;
	pthread_mutex_unlock(&philo->mystruct->eaten);
	sleep_accurate(philo->mystruct, philo->mystruct->time_eat);
	if (philo->mystruct->num_of_meals != -1 && philo->eaten_meals >= \
		philo->mystruct->num_of_meals)
		philo->mystruct->philo_chb3 += 1;
	pthread_mutex_unlock(&philo->mystruct->forks[philo->id]);
	pthread_mutex_unlock(&philo->mystruct->forks[(philo->id + 1) % n]);
}

void	philo_sleeping(t_philo *philo)
{
	long	time;

	time = timing_function(philo->mystruct->start);
	printing_function("is sleeping", philo, time, philo->id);
	sleep_accurate(philo->mystruct, philo->mystruct->time_sleep);
}

void	philo_thinking(t_philo *philo)
{
	long	time;

	time = timing_function(philo->mystruct->start);
	printing_function("is thinking", philo, time, philo->id);
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
		if (philo->mystruct->death_flag)
			break ;
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
