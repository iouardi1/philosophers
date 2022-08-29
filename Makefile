# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: iouardi <iouardi@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/02/14 16:41:34 by iouardi           #+#    #+#              #
#    Updated: 2022/08/29 17:26:56 by iouardi          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME= 			philo

FLAGS=			-Wall -Werror -Wextra -pthread
CC=				cc
SRC_FILES=		philo.c \
				ft_atoi.c \
				init.c \
				parsing_args.c \
				philo_routine.c \
				threads_checker.c
				

OBJ_FILES=			$(SRC_FILES:.c=.o)
OBJ_FILES_BONUS=	$(SRC_FILES_BONUS:.c=.o)

all: $(NAME)

$(NAME): $(OBJ_FILES)
	$(CC) $(FLAGS) $(OBJ_FILES) -o $(NAME) #-fsanitize=thread -g
%.o: %.c
	$(CC) $(FLAGS) -c $< -o $@

clean:
	find . -name "*.o" -delete
	find . -name "*.a" -delete

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY:
	all clean fclean re bonus