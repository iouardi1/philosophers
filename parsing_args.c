/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_args.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iouardi <iouardi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/29 17:14:09 by iouardi           #+#    #+#             */
/*   Updated: 2022/08/29 19:23:20 by iouardi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	is_digit(char c)
{
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
}

int	check_invalid_args(char **argv)
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
	return (0);
}

int	init_args(t_struct *mystruct, char **argv)
{
	mystruct->num_of_philos = ft_atoi(argv[1]);
	mystruct->time_die = ft_atoi(argv[2]);
	mystruct->time_eat = ft_atoi(argv[3]);
	mystruct->time_sleep = ft_atoi(argv[4]);
	if (argv[5])
	{
		mystruct->num_of_meals = ft_atoi(argv[5]);
		if (mystruct->num_of_meals == 0)
			return (3);
	}
	else
		mystruct->num_of_meals = -1;
	return (0);
}

int	parse_args(t_struct *mystruct, char **argv)
{
	int		i;

	if (check_invalid_args(argv))
		return (2);
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
	if (init_args(mystruct, argv))
		return (3);
	if (mystruct->num_of_philos > 200 || mystruct->num_of_philos <= 0)
		return (1);
	return (0);
}

int	parsing(t_struct *mystruct, char **argv, int argc)
{
	int		i;

	if (argc < 5 || argc > 6)
	{
		printf("bad number of arguments\n");
		free (mystruct);
		return (1);
	}
	i = parse_args(mystruct, argv);
	if (i == 3)
		return (3);
	else if (i)
	{
		free(mystruct);
		printf ("bad arguments\n");
		return (1);
	}
	return (0);
}
