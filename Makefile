
SHELL = /bin/bash

MAKEFLAGS += --no-print-directory

NAME = minishell

SRC_DIR = src
INC_DIR = include
LIB_DIR = src/auxlibft
OBJ_DIR = src/minishellObjects

LIB_NAME = ft
LIBFILE = $(LIB_DIR)/lib$(LIB_NAME).a

SRCS := $(shell find $(SRC_DIR) -type d -path $(LIB_DIR) -prune -o -type f -name '*.c' -print)
OBJS := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))

CC = cc
CFLAGS = -Wall -Wextra -Werror -I$(INC_DIR) -I$(LIB_DIR)/include

# Colores
RESET       = \033[0m
TURQUOISE   = \033[0;36m
LIGHT_TURQUOISE   = \033[1;36m
LIGHT_GREEN = \033[1;32m
LIGHT_RED	= \033[1;91m

# Progreso
TOTAL_STEPS := $(words $(SRCS))

define show_progress
	@bash -c '\
		total=$(TOTAL_STEPS); \
		[ "$$total" -gt 0 ] || total=1; \
		curr=$$(find $(OBJ_DIR) -type f -name "*.o" 2>/dev/null | wc -l); \
		width=60; \
		hashes=$$(( curr * width / total )); \
		[ "$$hashes" -ge 0 ] || hashes=0; \
		dots=$$(( width - hashes )); \
		[ "$$dots" -ge 0 ] || dots=0; \
		green=$$(printf "\033[1;32m"); \
		reset=$$(printf "\033[0m"); \
		printf "\rCompilando: ["; \
		bar=$$(printf "%*s" "$$hashes" ""); bar=$${bar// /#}; \
		printf "%s" "$$green$$bar$$reset"; \
		dot=$$(printf "%*s" "$$dots" ""); dot=$${dot// /.}; \
		printf "%s" "$$dot"; \
		printf "] %d/%d" "$$curr" "$$total"; \
		if [ "$$curr" -eq "$$total" ]; then printf " ✓\n"; fi; \
	'
endef

# --- Reglas ---

all: $(LIBFILE) $(NAME)

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

$(LIBFILE):
	@$(MAKE) -sC $(LIB_DIR) --no-print-directory

# Regla genérica para objetos (con subdirectorios)
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@ 2>/dev/null
	$(call show_progress)

$(NAME): $(OBJS) $(LIBFILE)
	@$(CC) $(CFLAGS) $(OBJS) $(LIBFILE) -lreadline -o $(NAME) 2>/dev/null
	@echo -e "$(LIGHT_TURQUOISE)¡Minishell lista!$(RESET)"

clean:
	@echo -e "$(LIGHT_RED)Ejecutando limpieza de objetos...$(RESET)"
	@rm -rf $(OBJ_DIR)
	@$(MAKE) -C $(LIB_DIR) clean -s
	@echo -e "$(TURQUOISE)¡Limpieza de objetos finalizada!$(RESET)"

fclean:
	@echo -e "$(LIGHT_RED)Ejecutando limpieza completa...$(RESET)"
	@rm -rf $(OBJ_DIR)
	@rm -f $(NAME)
	@$(MAKE) -C $(LIB_DIR) fclean -s
	@echo -e "$(TURQUOISE)¡Limpieza completa finalizada!$(RESET)"

re:
	@$(MAKE) fclean
	@$(MAKE) -s all

.PHONY: all clean fclean re
