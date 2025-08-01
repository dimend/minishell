# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: kievaughn <kievaughn@student.42.fr>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/11/12 19:39:40 by dimendon          #+#    #+#              #
#    Updated: 2025/07/01 21:11:20 by kievaughn        ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = cc
CFLAGS = -Wall -Wextra -Werror
TARGET = libft.a

SRCS = \
	ft_atoi.c \
	ft_isascii.c \
	ft_strchr.c \
	ft_strrchr.c \
	ft_isalnum.c \
	ft_isdigit.c \
	ft_strlen.c \
	ft_tolower.c \
	ft_isalpha.c \
	ft_isprint.c \
	ft_strncmp.c \
	ft_toupper.c \
	ft_memset.c \
	ft_bzero.c \
	ft_memcpy.c \
	ft_memmove.c \
	ft_memchr.c \
	ft_memcmp.c \
	ft_strlcpy.c \
	ft_strlcat.c \
	ft_strcatrealloc.c \
	ft_substr.c \
	ft_strnstr.c \
	ft_calloc.c \
	ft_strdup.c \
	ft_strjoin.c \
	ft_strtrim.c \
	ft_split.c \
	ft_itoa.c \
	ft_strmapi.c \
	ft_striteri.c \
	ft_putchar_fd.c \
	ft_putstr_fd.c \
	ft_putendl_fd.c \
	ft_putnbr_fd.c \
	ft_lstnew_bonus.c \
	ft_lstadd_front_bonus.c \
	ft_lstsize_bonus.c \
	ft_lstlast_bonus.c \
	ft_lstadd_back_bonus.c \
	ft_lstdelone_bonus.c \
	ft_lstclear_bonus.c \
	ft_lstiter_bonus.c \
	ft_lstmap_bonus.c \
	ft_strtok.c \
	ft_atoany.c \
	ft_is_str_num.c \
	ft_strcmp.c

OBJS = $(SRCS:.c=.o)
HEADERS = libft.h

# Color codes
YELLOW = \033[1;33m
GREEN  = \033[0;32m
RESET  = \033[0m

all: build

build:
	@printf "$(YELLOW)Compiling libft library...$(RESET)\n"
	@$(MAKE) lib

lib: $(TARGET)

$(TARGET): $(OBJS)
	@printf "$(YELLOW)Archiving...$(RESET)\n"
	@ar -crs $(TARGET) $(OBJS)
	@printf "$(GREEN)$(TARGET) built successfully!$(RESET)\n"

%.o: %.c $(HEADERS)
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@printf "$(YELLOW)Cleaning object files (libft)...$(RESET)\n"
	@rm -f $(OBJS)

fclean: clean
	@printf "$(YELLOW)Removing libft.a...$(RESET)\n"
	@rm -f $(TARGET)

re: fclean all

.PHONY: all build lib clean fclean re

