/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iouardi <iouardi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/17 20:48:00 by iouardi           #+#    #+#             */
/*   Updated: 2022/08/24 11:25:10 by iouardi          ###   ########.fr       */
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
	pthread_mutex_lock(&philo->mystruct->forks[(philo->id + 1) % n]);
	temp_time = timing_function(philo->mystruct->start);
	printf("%ld philo %d has taken a fork\n", temp_time, philo->id + 1);
	temp_time = timing_function(philo->mystruct->start);
	printf("%ld philo %d has taken a fork\n", temp_time, philo->id + 1);
	temp_time = timing_function(philo->mystruct->start);
	printf("%ld philo %d is eating\n", temp_time, philo->id + 1);
	sleep_accurate(philo->mystruct, philo->mystruct->time_eat);
	temp_time = timing_function(philo->mystruct->start);
	philo->mystruct->philo[philo->id].time_since_last_meal = temp_time;
	philo->eaten_meals += 1;
	pthread_mutex_unlock(&philo->mystruct->forks[philo->id]);
	pthread_mutex_unlock(&philo->mystruct->forks[(philo->id + 1) % n]);
}

void	philo_sleeping(t_philo *philo)
{
	long	time;

	time = timing_function(philo->mystruct->start);
	pthread_mutex_lock(&philo->mystruct->msg);
	printf("%ld philo %d is sleeping\n", time, philo->id + 1);
	pthread_mutex_unlock(&philo->mystruct->msg);
	sleep_accurate(philo->mystruct, philo->mystruct->time_sleep);
}

void	philo_thinking(t_philo *philo)
{
	long	time;

	time = timing_function(philo->mystruct->start);
	pthread_mutex_lock(&philo->mystruct->msg);
	printf("%ld philo %d is thinking\n", time, philo->id + 1);
	pthread_mutex_unlock(&philo->mystruct->msg);
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
		i = 0;
		while (i < mystruct->num_of_philos)
		{
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
			err = pthread_create(&mystruct->philo[i].philo_diali, NULL, sm_function, &mystruct->philo[i]);
			if (err != 0)
			{
           		printf("can't create thread :[%s]\n", strerror(err));
				return (3);
			}
			usleep(50);
        	i += 2;
		}
		while (1)
		{
			i = 0;
			while (i < mystruct->num_of_philos)
			{
				time = timing_function(mystruct->start);
				if ((time - mystruct->philo[i].time_since_last_meal >= mystruct->time_die))
				{
					pthread_mutex_lock(&mystruct->msg);
					printf ("%ld philo %d is dead\n", time, i + 1);
					return (0);
				}
				if (mystruct->num_of_meals != -1 && mystruct->philo[i].eaten_meals >= mystruct->num_of_meals)
				{
					// usleep(50);
					mystruct->philo_chb3 += 1;
					if (mystruct->philo_chb3 >= mystruct->num_of_philos)
					{
						pthread_mutex_lock(&mystruct->msg);
						printf ("%ld philos chb3o\n", time);
						pthread_mutex_unlock(&mystruct->msg);
						return (0);
					}
				}
				i++;
			}
		}
		i = 0;
		// while (i < mystruct->num_of_philos)
		// {
		// 	pthread_join(mystruct->philo[i].philo_diali, NULL);
		// 	i++;
		// }
	}
	return (0);
}
