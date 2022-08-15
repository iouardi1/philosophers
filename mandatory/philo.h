/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iouardi <iouardi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/17 20:48:17 by iouardi           #+#    #+#             */
/*   Updated: 2022/08/15 23:15:41 by iouardi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
#define PHILO_H

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>

typedef struct s_philo t_philo;

typedef struct s_struct
{
	int				num_of_philos;
	long			start;
	int				time_die;
	int				time_eat;
	int				time_sleep;
	int 			num_of_meals;
	pthread_mutex_t	*forks;
	t_philo		 	*philo;
} t_struct;


typedef struct s_philo
{
	int			id;
	// int 		l_fork;
	// int 		r_fork;
	t_struct	*mystruct;
	int			eaten_meals;
	int			time_since_last_meal;
	pthread_t	philo_diali;
} t_philo;

void	*test_thread();
int     ft_atoi(const char *str);
long	timing_function(long start_time);

#endif
