/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iouardi <iouardi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/17 20:48:00 by iouardi           #+#    #+#             */
/*   Updated: 2022/08/26 01:19:57 by iouardi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*test_thread()
{
	char	*h = "hello";
	printf("hello\n");
	usleep(2);
	printf("hello again\n");
	return (h);
}

void	sleep_accurate(t_struct *mystruct, long n)
{
	int		i;
	long	current_time;

	i = 0;
	current_time = timing_function(mystruct->start);
	while (timing_function(mystruct->start) - current_time < n)
		usleep(100);
}

void	init_struct(t_philo *philo, t_struct *mystruct)
{
	int			i;

	i = 0;
	(void)philo;
	mystruct->philo = malloc(sizeof(t_philo) * mystruct->num_of_philos);
	mystruct->forks = malloc (sizeof(pthread_mutex_t) * mystruct->num_of_philos);
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

void	philo_eating(t_philo *philo)
{
	int		n;
	long	temp_time;
	struct	 timeval	current_time;

	n = philo->mystruct->num_of_philos;
	if (!philo->mystruct->flag)
	{
		gettimeofday(&current_time, NULL);
		philo->mystruct->start = (current_time.tv_sec * 1000) + (current_time.tv_usec / 1000);
		philo->mystruct->flag = 1;
	}
	pthread_mutex_lock(&philo->mystruct->forks[philo->id]);
	temp_time = timing_function(philo->mystruct->start);
	printing_function("has taking a fork", philo, temp_time, philo->id);
	pthread_mutex_lock(&philo->mystruct->forks[(philo->id + 1) % n]);
	temp_time = timing_function(philo->mystruct->start);
	printing_function("has taking a fork", philo, temp_time, philo->id);
	temp_time = timing_function(philo->mystruct->start);
	printing_function("is eating", philo, temp_time, philo->id);
	// pthread_mutex_lock(&philo->mystruct->msg);
	// printf("%ld philo %d has taken a fork\n", temp_time, philo->id + 1);
	// pthread_mutex_unlock(&philo->mystruct->msg);
	// pthread_mutex_lock(&philo->mystruct->msg);
	// printf("%ld philo %d has taken a fork\n", temp_time, philo->id + 1);
	// pthread_mutex_unlock(&philo->mystruct->msg);
	// pthread_mutex_lock(&philo->mystruct->msg);
	// printf("%ld philo %d is eating\n", temp_time, philo->id + 1);
	// pthread_mutex_unlock(&philo->mystruct->msg);
	sleep_accurate(philo->mystruct, philo->mystruct->time_eat);
	temp_time = timing_function(philo->mystruct->start);
	philo->mystruct->philo[philo->id].time_since_last_meal = temp_time;
	pthread_mutex_lock(&philo->mystruct->eaten);
	philo->eaten_meals += 1;
	pthread_mutex_unlock(&philo->mystruct->eaten);
	if (philo->mystruct->num_of_meals != -1 && philo->eaten_meals >= philo->mystruct->num_of_meals)
		philo->mystruct->philo_chb3 += 1;
	pthread_mutex_unlock(&philo->mystruct->forks[philo->id]);
	pthread_mutex_unlock(&philo->mystruct->forks[(philo->id + 1) % n]);
}

void	philo_sleeping(t_philo *philo)
{
	long	time;

	time = timing_function(philo->mystruct->start);
	// pthread_mutex_lock(&philo->mystruct->msg);
	// printf("%ld philo %d is sleeping\n", time, philo->id + 1);
	// pthread_mutex_unlock(&philo->mystruct->msg);
	printing_function("is sleeping", philo, time, philo->id);
	sleep_accurate(philo->mystruct, philo->mystruct->time_sleep);
}

void	philo_thinking(t_philo *philo)
{
	long	time;

	time = timing_function(philo->mystruct->start);
	// pthread_mutex_lock(&philo->mystruct->msg);
	// printf("%ld philo %d is thinking\n", time, philo->id + 1);
	// pthread_mutex_unlock(&philo->mystruct->msg);
	printing_function("is thinking", philo, time, philo->id);
}

void 	*sm_function(void *p)
{
	int		temp;
	t_philo *philo;
	int		 philos_chb3o;
	int		 death_flag;

	temp = 0;
	philo = (t_philo *)p;
		philos_chb3o = philo->mystruct->check_philos_chb3o;
		death_flag = philo->mystruct->death_flag;
	while (!death_flag && !philos_chb3o)
	{
		philos_chb3o = philo->mystruct->check_philos_chb3o;
		death_flag = philo->mystruct->death_flag;
		philo_eating(philo);
		philo_sleeping(philo);
		philo_thinking(philo);
	}
	return (philo);
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
	time_adjust = (current_time.tv_sec * 1000) + (current_time.tv_usec / 1000) - start_time;
	return (time_adjust);
}

int	is_digit(char c)
{
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
}

int	parse_args(t_struct *mystruct, char **argv)
{
	int		i;
	int		j;

	i = 1;
	while (argv[i])
	{
		j = 0;
		while (argv[i][j])
		{
			if (!is_digit(argv[i][j]))
			{
				printf("invalid argument %s\n", argv[i]);
				return (2);
			}
			j++;
		}
		i++;
	}
	i = 1;
	while (argv[i])
	{
		if (ft_atoi(argv[i]) == -1)
		{
			printf("invalid argument %s\n", argv[i]);
			return (2);
		}
		i++;
	}
	mystruct->num_of_philos = ft_atoi(argv[1]);
	mystruct->time_die = ft_atoi(argv[2]);
	mystruct->time_eat = ft_atoi(argv[3]);
	mystruct->time_sleep = ft_atoi(argv[4]);
	if (argv[5])
		mystruct->num_of_meals = ft_atoi(argv[5]);
	else
		mystruct->num_of_meals = -1;
	if (mystruct->num_of_philos > 200 || mystruct->num_of_philos <= 0)
		return (1);
	return (0);
}

void	printing_function(char *msg, t_philo *philo, long time, int i)
{
	time = timing_function(philo->mystruct->start);
	if (!philo->mystruct->death_flag && time - philo->time_since_last_meal < philo->mystruct->time_die)
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
		pthread_mutex_lock(&philo->mystruct->msg);
		printf ("%ld philo %d %s\n", time, i + 1, "is dead");
		// pthread_mutex_unlock(&philo->mystruct->msg);
	}
		
}

int main(int argc, char **argv)
{
	t_struct			*mystruct;
	int	 				i;
	int	 				err;
	long				time;

	i = 0;
	mystruct = malloc (sizeof(t_struct));
	if (argc < 5 || argc > 6)
	{
		printf("bad number of arguments\n");
		free (mystruct);
		return (1);
	}
	else if (parse_args(mystruct, argv))
	{
		free(mystruct);
		printf ("bad arguments\n");
		return (1);
	}
	else
	{
		mystruct->philo = malloc(sizeof(t_philo));
		init_struct(mystruct->philo, mystruct);
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
		i = 0;
		while (i < mystruct->num_of_philos)
		{
			mystruct->philo[i].time_since_last_meal = 0;
			err = pthread_create(&mystruct->philo[i].philo_diali, NULL, sm_function, &mystruct->philo[i]);
			if (err != 0)
			{
           		printf("can't create thread :[%s]\n", strerror(err));
				return (3);
			}
			usleep(50);
        	i += 2;
		}
		i = 1;
		while (i < mystruct->num_of_philos)
		{
			mystruct->philo[i].time_since_last_meal = 0;
			err = pthread_create(&mystruct->philo[i].philo_diali, NULL, sm_function, &mystruct->philo[i]);
			if (err != 0)
			{
           		printf("can't create thread :[%s]\n", strerror(err)); // remove this if it is not allowed
				return (3);
			}
			usleep(50);
        	i += 2;
		}
		while (1)
		{
			usleep(100);
			i = 0;
			while (i < mystruct->num_of_philos)
			{
				time = timing_function(mystruct->start);
				if ((time - mystruct->philo[i].time_since_last_meal >= mystruct->time_die))
				{
					pthread_mutex_lock(&mystruct->death);
					mystruct->death_flag = 1;
					pthread_mutex_unlock(&mystruct->death);
					// printing_function("DEAD", mystruct->philo, time, i);
					// pthread_mutex_lock(&mystruct->msg);
					// printf ("%ld philo %d is dead\n", time, i + 1);
					return (0);
				}
				if (mystruct->num_of_meals != -1 && mystruct->philo[i].eaten_meals >= mystruct->num_of_meals)
				{
					// usleep(50);
					// mystruct->philo_chb3 += 1;
					if (mystruct->philo_chb3 >= mystruct->num_of_philos)
					{
						mystruct->check_philos_chb3o = 1;
						// printing_function("chb3", mystruct, time, i);
						// pthread_mutex_lock(&mystruct->msg);
						// printf ("%ld philos chb3o\n", time);
						// pthread_mutex_unlock(&mystruct->msg);
						return (0);
					}
				}
				i++;
				// pthread_mutex_unlock(&mystruct->msg);
			}
		}
		i = 0;
		while (i < mystruct->num_of_philos)
		{
			pthread_join(mystruct->philo[i].philo_diali, NULL);
			i++;
		}
	}
	return (0);
}
