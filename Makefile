# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: iouardi <iouardi@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/02/14 16:41:34 by iouardi           #+#    #+#              #
#    Updated: 2022/05/18 17:29:25 by iouardi          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME= 			philo
# NAME_BONUS= 	checker
FLAGS=			-Wall -Werror -Wextra -pthread
CC=				cc
SRC_FILES=		mandatory/philo.c \
				mandatory/ft_atoi.c \
				# mandatory/calculate_min_instruc.c \
				# mandatory/check_errors.c \
				# mandatory/fill_stack_moy_big_small.c \
				# mandatory/indexing.c \
				# mandatory/instructions.c \
				# mandatory/sorting.c \
				# mandatory/sorting_five_or_less.c

# SRC_FILES_BONUS = bonus/checker.c \
# 				bonus/check_errors_bonus.c \
# 				bonus/fill_stack_moy_big_small_bonus.c \
# 				bonus/indexing_bonus.c \
# 				bonus/instructions_bonus.c \
# 				bonus/instructions_errors_bonus.c \
# 				src/gnl/get_next_line.c \
# 				src/gnl/get_next_line_utils.c 

OBJ_FILES=			$(SRC_FILES:.c=.o)
OBJ_FILES_BONUS=	$(SRC_FILES_BONUS:.c=.o)

all: $(NAME)

$(NAME): $(OBJ_FILES)
	$(CC) $(OBJ_FILES) -o $(NAME)
%.o: %.c
	$(CC) $(FLAGS) -c $< -o $@

clean:
	find . -name "*.o" -delete
	find . -name "*.a" -delete

fclean: clean
	rm -f $(NAME)

re: fclean all

bonus: $(OBJ_FILES_BONUS)
	@make -C src/libft
	@cp src/libft/libft.a ./
	$(CC) $(FLAGS) $(OBJ_FILES_BONUS) -o $(NAME_BONUS) libft.a

.PHONY:
	all clean fclean re bonus