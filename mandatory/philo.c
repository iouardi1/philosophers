/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iouardi <iouardi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/17 20:48:00 by iouardi           #+#    #+#             */
/*   Updated: 2022/08/21 04:11:57 by iouardi          ###   ########.fr       */
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
	// long	current_time;

	i = 0;
	// current_time = timing_function(mystruct->start);
	printf ("%ld \n", timing_function(mystruct->start) - current_time);
	printf ("n === %ld \n", n);
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
		mystruct->philo_chb3 = 0;
		i++;
	}
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
	printf("%ld philo %d has taken a fork\n", temp_time, philo->id);
	pthread_mutex_lock(&philo->mystruct->forks[(philo->id + 1) % n]);
	temp_time = timing_function(philo->mystruct->start);
	printf("%ld philo %d has taken a fork\n", temp_time, philo->id);
	// pthread_mutex_lock(&philo->mystruct->philo[philo->id].eaten_meals);
	temp_time = timing_function(philo->mystruct->start);
	printf("%ld philo %d is eating\n", temp_time, philo->id);
	temp_time = timing_function(philo->mystruct->start);
	philo->time_since_last_meal = temp_time;
	philo->eaten_meals += 1;
	sleep_accurate(philo->mystruct, philo->mystruct->time_eat);
	pthread_mutex_unlock(&philo->mystruct->forks[philo->id]);
	pthread_mutex_unlock(&philo->mystruct->forks[(philo->id + 1) % n]);
	// pthread_mutex_unlock(&philo->mystruct->philo[philo->id].eaten_meals);
}

void	philo_sleeping(t_philo *philo)
{
	long	time;

	time = timing_function(philo->mystruct->start);
	sleep_accurate(philo->mystruct, philo->mystruct->time_sleep);
	printf("%ld philo %d is sleeping\n", time, philo->id);
}

void	philo_thinking(t_philo *philo)
{
	long	time;

	time = timing_function(philo->mystruct->start);
	printf("%ld philo %d is thinking\n", time, philo->id);
}

void 	*sm_function(void *p)
{
	int		temp;
	t_philo *philo;

	temp = 0;
	philo = (t_philo *)p;
	while (1)
	{
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

int	parse_args(t_struct *mystruct, char **argv)
{
	mystruct->num_of_philos = ft_atoi(argv[1]);
	mystruct->time_die = ft_atoi(argv[2]);
	mystruct->time_eat = ft_atoi(argv[3]);
	mystruct->time_sleep = ft_atoi(argv[4]);
	if (argv[4])
		mystruct->num_of_meals = ft_atoi(argv[4]);
	if (mystruct->num_of_philos > 200 || mystruct->num_of_philos <= 0)
		return (1);
	return (0);
}

int main(int argc, char **argv)
{
	t_struct			*mystruct;
	t_philo				*philo;
	int	 				i;
	int	 				err;
	long				time;
	// struct	 timeval	current_time;

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
		philo = malloc(sizeof(t_philo));
		init_struct(philo, mystruct);
		while (i < mystruct->num_of_philos)
		{
			if (pthread_mutex_init(&mystruct->forks[i], NULL))
			{
				printf("mutex init failed\n");
				return (2);
			}
			i++;
		}
		i = 0;
		// gettimeofday(&current_time, NULL);
		// mystruct->start = (current_time.tv_sec * 1000) + (current_time.tv_usec / 1000);
		// mystruct->start = timing_function((current_time.tv_sec * 1000) + (current_time.tv_usec / 1000));
		while (i < mystruct->num_of_philos)
		{
			err = pthread_create(&mystruct->philo[i].philo_diali, NULL, sm_function, &mystruct->philo[i]);
			if (err != 0)
			{
           		printf("can't create thread :[%s]\n", strerror(err));
				return (3);
			}
			usleep(100);
        	i++;
		}
		i = 0;
		// mystruct->philo_chb3 = 0;
		while (1)
		{
			while (i < mystruct->num_of_philos)
			{
				time = timing_function(mystruct->start);
				// printf("%ld ===\n", time - mystruct->philo[i].time_since_last_meal);   
				// printf("%ld ===\n", mystruct->philo[i].time_since_last_meal);
				if (time - mystruct->philo[i].time_since_last_meal > mystruct->time_die)
				{
					printf ("%ld philo %d has died\n", time, i);
					return (0);
				}
				// printf("%d = num of meals\n", mystruct->num_of_meals);
				if (mystruct->philo[i].eaten_meals >= mystruct->num_of_meals)
				{
					mystruct->philo_chb3 += 1;
					// printf("%d = num of philos\n", mystruct->philo_chb3);
					if (mystruct->philo_chb3 == mystruct->num_of_philos)
					{
						printf ("philo chb3\n");
						return (0);
					}
				}
				// usleep(100);
				i++;
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
