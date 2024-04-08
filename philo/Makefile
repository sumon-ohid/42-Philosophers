# Makefile for Philosophers project
CC 		= cc
CFLAGS 	= -Wall -Wextra -Werror #-MMD -MP -g #-fsanitize=thread
NAME 	= philo
HEADER	= philosophers.h

SRCS 	= 	srcs/main.c \
			srcs/threads.c \
			srcs/utils.c \
			srcs/activities.c \
			srcs/data_init.c \

OBJS 	= $(SRCS:.c=.o)
DEPS	= $(OBJS:.o=.d)

GREEN 	= \033[0;32m
BLUE 	= \033[0;34m
RED 	= \033[0;31m
RESET 	= \033[0m

all: $(NAME)

$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) -lpthread -o $(NAME)
	@echo "$(GREEN)--> philo is created <-- $(RESET)"
	@echo "$(BLUE)(hint :) ./philo [number_of_philosophers] [time_to_die] [time_to_eat] [time_to_sleep]$(RESET)"

clean:
	@rm -f $(OBJS) $(DEPS)
	@echo "$(RED)--> Object files are removed $(RESET)"

fclean: clean
	@rm -f $(NAME)
	@echo "$(RED)--> philo is removed $(RESET)"

re: fclean all

.PHONY: all clean fclean re

-include $(DEPS)