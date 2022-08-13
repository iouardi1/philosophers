/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iouardi <iouardi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/17 20:48:00 by iouardi           #+#    #+#             */
/*   Updated: 2022/08/13 19:50:44 by iouardi          ###   ########.fr       */
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

void	init_struct(t_philo *philo, t_struct *my_struct)
{
	my_struct->philo_diali = malloc (sizeof(pthread_t) * my_struct->num_of_philos);
	my_struct->forks = malloc (sizeof(pthread_mutex_t) * my_struct->num_of_philos);
	philo->mystruct = my_struct;
	philo->eaten_meals = 0;
}

void	philo_eating(t_philo *philo, int i, int current_time)
{
	int		n;
	int		temp_time;

	n = philo->mystruct->num_of_philos;
	pthread_mutex_lock(&philo->mystruct->forks[i]);
	// current_time = timing_function();
	temp_time = timing_function();
	printf("%d philo %d has taken a fork\n", temp_time - current_time, i);
	pthread_mutex_lock(&philo->mystruct->forks[(i + 1) % n]);
	temp_time = timing_function();
	printf("%d philo %d has taken a fork\n", temp_time - current_time, i);
	temp_time = timing_function();
	printf("%d philo %d is eating\n", temp_time - current_time, i);
	philo->eaten_meals += 1;
	usleep(philo->mystruct->time_eat * 1000);
	pthread_mutex_unlock(&philo->mystruct->forks[i]);
	pthread_mutex_unlock(&philo->mystruct->forks[(i + 1) % n]);
}

void	philo_sleeping(t_philo *philo, int i, int time)
{
	usleep (philo->mystruct->time_sleep);
	printf("%d philo %d is sleeping\n", time, i);
}

void	philo_thinking(int i, int time)
{
	printf("%d philo %d is thinking\n", time, i);
}

void 	*sm_function(t_philo *philo, int i, int current_time)
{
	int		temp;
	int		temp_time;
	int		eating_time;

	temp = 0;
	while (1)
	{
		eating_time = timing_function();
		if (philo->time_since_last_meal >= philo->mystruct->time_die)
		{
			printf("%d philo %d died\n", current_time, i);
			break ;
		}
		philo_eating(philo, i, current_time);
		eating_time = timing_function();
		temp = eating_time;
		temp_time = timing_function();
		philo_sleeping(philo, i, temp_time - current_time);
		temp_time = timing_function();
		philo_thinking(i, temp_time - current_time);
		i++;
	}
	return (philo);
}

int	timing_function()
{
	int				err;
	struct timeval	current_time;
	int				time_adjust;

	err = gettimeofday(&current_time, NULL);
	if (err)
	{
		printf("gettimeofday function has failed\n");
		return (1);
	}
	time_adjust = (current_time.tv_sec * 1000) + (current_time.tv_usec / 1000);
	// printf("the current time since the EPOCH is : %ld,%d\n", current_time.tv_sec, current_time.tv_usec);
	return (time_adjust);
}

int	parse_args(t_struct *mystruct, char **argv)
{
	mystruct->num_of_philos = ft_atoi(argv[1]);
	mystruct->time_die = ft_atoi(argv[2]);
	mystruct->time_eat = ft_atoi(argv[3]);
	mystruct->time_sleep = ft_atoi(argv[4]);
	if (argv[5])
		mystruct->num_of_meals = ft_atoi(argv[5]);
	if (mystruct->num_of_philos > 200 || mystruct->num_of_philos <= 0)
		return (1);
	return (0);
}

int main(int argc, char **argv)
{
	t_struct			*my_struct;
	t_philo				*philo;
	int	 				i;
	int	 				err;
	int					current_time;

	i = 0;
	my_struct = malloc (sizeof(t_struct));
	
	if (argc < 5 || argc > 6)
	{
		printf("bad number of arguments\n");
		free (my_struct);
		return (1);
	}
	else if (parse_args(my_struct, argv))
	{
		free(my_struct);
		printf ("bad arguments\n");
		return (1);
	}
	else
	{
		philo = malloc(sizeof(t_philo));
		init_struct(philo, my_struct);
		while (i < my_struct->num_of_philos)
		{
			if (pthread_mutex_init(&my_struct->forks[i], NULL))
			{
				printf("mutex init failed\n");
				return (2);
			}
			i++;
		}
		i = 0;
		current_time = timing_function();
		while (i < my_struct->num_of_philos)
		{
			err = pthread_create(&my_struct->philo_diali[i], NULL, sm_function(philo, i, current_time), &my_struct->philo_diali[i]);
			if (err != 0)
			{
           		printf("can't create thread :[%s]\n", strerror(err));
				return (3);
			}
			usleep(10000);
        	i++;
		}
		i = 0;
		// err = gettimeofday(&current_time, NULL);
		// if (!i)
		// 	printf ("the current time since the EPOCH is : %ld,%d\n", current_time.tv_sec, current_time.tv_usec);
		// while (i < my_struct->num_of_philos)
		// {
		// 	pthread_join(my_struct->philo_diali[i], NULL);
		// 	i++;
		// }
	}
	return (0);
}
