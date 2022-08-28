/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iouardi <iouardi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/17 20:48:17 by iouardi           #+#    #+#             */
/*   Updated: 2022/08/28 04:14:35 by iouardi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
#define PHILO_H

#include <stdio.h>
#include <string.h>
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
	int				flag;
	int				time_die;
	int				time_eat;
	int				time_sleep;
	int 			num_of_meals;
	int 			philo_chb3;
	int 			check_philos_chb3o;
	int				death_flag;
	pthread_mutex_t	*forks;
	pthread_mutex_t	msg;
	pthread_mutex_t	death;
	pthread_mutex_t	eaten;
	pthread_mutex_t	chb3;
	t_philo		 	*philo;
} t_struct;


typedef struct s_philo
{
	int				id;
	// int 			l_fork;
	// int 			r_fork;
	t_struct		*mystruct;
	int				eaten_meals;
	long			time_since_last_meal;
	pthread_t		philo_diali;
} t_philo;

void	*test_thread();
int     ft_atoi(const char *str);
long	timing_function(long start_time);
void	printing_function(char *msg, t_philo *philo, long time, int i);
int      strcmp(const char *__s1, const char *__s2);

#endif


//2792 philo 2 is eating
//2991 philo 2 is dead