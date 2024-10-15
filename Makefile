# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: obouayed <obouayed@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/07/31 08:34:42 by obouayed          #+#    #+#              #
#    Updated: 2024/10/10 16:38:34 by obouayed         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Project Name
NAME = philo

# Compilation
CC = cc
CFLAGS = -Wall -Wextra -Werror
LDFLAGS = -pthread

# Source files and object files
SRC = main.c utils.c init.c cleaning.c routine.c routine_utils.c utils2.c forks.c
SRC_DIR = src
OBJ_DIR = obj
OBJS = $(addprefix $(OBJ_DIR)/, $(SRC:.c=.o))

# Includes file
HEADER = includes/philosophers.h

# Commands
RM = rm -rf

# Compilation
all: $(NAME)

$(NAME): $(OBJS) $(HEADER)
		$(CC) $(CFLAGS) $(LDFLAGS) -o $(NAME) $(OBJS)

$(OBJ_DIR)/%.o:	$(SRC_DIR)/%.c | $(OBJ_DIR)
		$(CC) -c $< -o $@

$(OBJ_DIR):
		mkdir -p $(OBJ_DIR)

# Cleaning
clean:
	$(RM) $(OBJ_DIR)

fclean: clean
	$(RM) $(NAME)

# Re-compile everything
re: fclean all

# To prevent commands from being treated as files
.PHONY: all clean fclean re
