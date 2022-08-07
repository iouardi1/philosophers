/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iouardi <iouardi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/17 20:48:00 by iouardi           #+#    #+#             */
/*   Updated: 2022/08/07 14:35:24 by iouardi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*test_thread()
{
	char	*h = "hello";
	printf("hello\n");
	sleep(2);
	printf("hello again\n");
	return (h);
}

void	init_struct(t_struct *my_struct)
{
	my_struct->philo_diali = malloc (sizeof(pthread_t) * my_struct->num_of_philos);
	my_struct->forks = malloc (sizeof(pthread_mutex_t) * my_struct->num_of_philos);
}

void 	*sm_function(t_struct	 *my_struct)
{
	// pthread_mutex_t	*forks;

	// forks = malloc (sizeof(pthread_mutex_t) * my_struct->num_of_philos);
	// pthread_mutex_init(my_struct->forks, NULL);
	// printf("forks %d\n", my_struct->num_of_meals);
	// pthread_mutex_lock();
	// a = philo->num_of_meals;
	// pthread_mutex_unlock(lock);
	philo_eating();
	philo_sleeping();
	philo_thinking();
	return ((void*)forks);
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
	// pthread_t   		*t1;
	// pthread_mutex_t		*lock = NULL;
	// t_philo				philo;
	t_struct			*my_struct;
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
		init_struct(my_struct);
		
		// if (pthread_mutex_init(lock, NULL))
		// {
		// 	printf("mutex init failed\n");
		// 	return (2);
		// }

		while (i < my_struct->num_of_philos)
		{
			err = pthread_create(&my_struct->philo_diali[i], NULL, sm_function(my_struct), my_struct);
			if (err != 0)
           		printf("can't create thread :[%s]\n", strerror(err));
        	i++;
		}
	}	
	return (0);









	
}

