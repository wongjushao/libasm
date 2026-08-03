# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: wjun-kea <wjun-kea@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/08/03 10:15:00 by wjun-kea          #+#    #+#              #
#    Updated: 2026/08/03 10:15:00 by wjun-kea         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libasm.a

FILES = ft_strlen.s ft_write.s ft_read.s ft_strcmp.s ft_strcpy.s ft_strdup.s

OBJS = $(FILES:.s=.o)

BONUS =

BONUS_OBJS = $(BONUS:.s=.o)

NASM = nasm

NASMFLAGS = -f elf64

RM = rm -f

all: $(NAME)

$(NAME): $(OBJS)
	@ar rcs $(NAME) $(OBJS)
	@echo "Library $(NAME) has been created."

%.o: %.s
	@$(NASM) $(NASMFLAGS) $< -o $@

clean:
	@$(RM) $(OBJS) $(BONUS_OBJS)
	@$(RM) -rf build
	@echo "Object files and test build removed."

fclean: clean
	@$(RM) $(NAME)
	@echo "Library $(NAME) removed."

re: fclean all

bonus: $(OBJS) $(BONUS_OBJS)
	@ar rcs $(NAME) $(OBJS) $(BONUS_OBJS)
	@echo "Bonus library $(NAME) has been created."

test:
	@./test.sh

.PHONY: all clean fclean re bonus test
