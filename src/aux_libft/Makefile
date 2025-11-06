
SHELL = /bin/bash

NAME        = libft.a

SRC_DIR     = src
INC_DIR     = include

OBJ_DIR     = ../../minishellObjects/aux_libft
CC          = cc
CFLAGS      = -Wall -Wextra -Werror -I$(INC_DIR)
AR          = ar
ARFLAGS     = rcs

SRCS = $(shell find $(SRC_DIR) -type f -name '*.c')
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

RESET           = \001\033[0m\002
TURQUOISE       = \001\033[0;36m\002
LIGHT_TURQUOISE = \001\033[1;36m\002
LIGHT_GREEN     = \001\033[1;32m\002
LIGHT_RED       = \001\033[1;91m\002

all: $(NAME)

$(NAME): $(OBJS)
	@$(AR) $(ARFLAGS) $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@
	@if [ -n "$(SHOW_PROGRESS)" ]; then $(SHOW_PROGRESS); fi

$(OBJ_DIR):
	@mkdir -p $@

clean:
	@rm -rf "$(OBJ_DIR)"

fclean: clean
	@rm -f "$(NAME)"

re: fclean all

.PHONY: all clean fclean re

# Hay que cambiar la wildcards [ *.c ]
