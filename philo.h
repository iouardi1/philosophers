/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iouardi <iouardi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/17 20:48:17 by iouardi           #+#    #+#             */
/*   Updated: 2022/08/29 22:45:15 by iouardi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <string.h>
# include <unistd.h>
# include <stdlib.h>
# include <pthread.h>
# include <string.h>
# include <stdlib.h>
# include <sys/time.h>

typedef struct s_philo	t_philo;

typedef struct s_struct
{
	int				num_of_philos;
	long			start;
	int				flag;
	int				time_die;
	int				time_eat;
	int				time_sleep;
	int				num_of_meals;
	int				philo_chb3;
	int				check_philos_chb3o;
	int				death_flag;
	pthread_mutex_t	*forks;
	pthread_mutex_t	msg;
	pthread_mutex_t	death;
	pthread_mutex_t	eaten;
	pthread_mutex_t	chb3;
	t_philo			*philo;
}	t_struct;

typedef struct s_philo
{
	int				id;
	t_struct		*mystruct;
	int				eaten_meals;
	long			time_since_last_meal;
	pthread_t		philo_diali;
}	t_philo;

void	*test_thread(void);
int		ft_atoi(const char *str);
long	timing_function(long start_time);
void	printing_function(char *msg, t_philo *philo, long time, int i);
int		strcmp(const char *__s1, const char *__s2);
int		parse_args(t_struct *mystruct, char **argv);
int		parsing(t_struct *mystruct, char **argv, int argc);
void	init_struct(t_struct *mystruct);
int		init_mutexes(t_struct *mystruct);
int		create_threads(t_struct *mystruct);
int		threads_checker(t_struct *mystruct);
void	philo_eating(t_philo *philo);
void	philo_sleeping(t_philo *philo);
void	philo_thinking(t_philo *philo);
void	sleep_accurate(t_struct *mystruct, long n);
void	*sm_function(void *p);

#endif
