/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iouardi <iouardi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/17 20:48:00 by iouardi           #+#    #+#             */
/*   Updated: 2022/08/11 23:10:41 by iouardi          ###   ########.fr       */
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

void	philo_eating(t_philo *philo, int i)
{
	int		n;

	n = philo->mystruct->num_of_philos;
	pthread_mutex_lock(&philo->mystruct->forks[i]);
	printf("philo %d has taken a fork\n", i);
	pthread_mutex_lock(&philo->mystruct->forks[(i + 1) % n]);
	printf("philo %d has taken a fork\n", i);
	printf("philo %d is eating\n", i);
	philo->eaten_meals += 1;
	usleep(philo->mystruct->time_eat * 1000);
	pthread_mutex_unlock(&philo->mystruct->forks[i]);
	pthread_mutex_unlock(&philo->mystruct->forks[(i + 1) % n]);
}

void	philo_sleeping(t_philo *philo, int i)
{
	usleep (philo->mystruct->time_sleep);
	printf("philo %d is sleeping\n", i);
}

void	philo_thinking(int i)
{
	printf("philo %d is thinking\n", i);
}

void 	*sm_function(t_philo *philo, int i)
{
	while (1)
	{
		philo_eating(philo, i);
		philo_sleeping(philo, i);
		philo_thinking(i);
	}
	return (philo);
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
		while (i < my_struct->num_of_philos)
		{
			err = pthread_create(&my_struct->philo_diali[i], NULL, sm_function(philo, i), &my_struct->philo_diali[i]);
			if (err != 0)
			{
           		printf("can't create thread :[%s]\n", strerror(err));
				return (3);
			}
			usleep(10000);
        	i++;
		}
		i = 0;
		// while (i < my_struct->num_of_philos)
		// {
		// 	pthread_join(my_struct->philo_diali[i], NULL);
		// 	i++;
		// }
	}
	return (0);
}

