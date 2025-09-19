
SHELL = /bin/bash
MAKEFLAGS += --no-print-directory

NAME        = minishell

SRC_DIR     = src
INC_DIR     = include

LIBFT_DIR   = src/aux_libft
LIBFT_SRC_D = $(LIBFT_DIR)/src
LIBFT_INC_D = $(LIBFT_DIR)/include
LIBFT_A     = $(LIBFT_DIR)/libft.a

OBJ_ROOT        = minishellObjects
APP_OBJ_DIR     = $(OBJ_ROOT)/minishell
LIBFT_OBJ_DIR   = $(OBJ_ROOT)/aux_libft

CC       = cc
CFLAGS   = -Wall -Wextra -Werror -I$(INC_DIR) -I$(LIBFT_INC_D)
LDFLAGS  = -lreadline
AR       = ar
ARFLAGS  = rcs

SRCS = $(shell find $(SRC_DIR) -type f -name '*.c' -not -path '$(LIBFT_DIR)/*')
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(APP_OBJ_DIR)/%.o)

LIBFT_SRCS = $(shell find $(LIBFT_SRC_D) -type f -name '*.c')
LIBFT_OBJS = $(LIBFT_SRCS:$(LIBFT_SRC_D)/%.c=$(LIBFT_OBJ_DIR)/%.o)

RESET           = \033[0m
TURQUOISE       = \033[0;36m
LIGHT_TURQUOISE = \033[1;36m
LIGHT_GREEN     = \033[1;32m
LIGHT_RED       = \033[1;91m

TOTAL_STEPS = $(words $(SRCS) $(LIBFT_SRCS))

define show_progress
	@total=$(TOTAL_STEPS); \
	[ "$$total" -gt 0 ] || total=1; \
	curr=$$(find "$(OBJ_ROOT)" -type f -name "*.o" 2>/dev/null | wc -l); \
	width=60; \
	hashes=$$(( curr * width / total )); \
	[ "$$hashes" -ge 0 ] || hashes=0; \
	dots=$$(( width - hashes )); \
	[ "$$dots" -ge 0 ] || dots=0; \
	green=$$(printf "\033[1;32m"); \
	reset=$$(printf "\033[0m"); \
	printf "\rCompiling: ["; \
	bar=$$(printf "%*s" "$$hashes" ""); bar=$${bar// /#}; \
	printf "%s" "$$green$$bar$$reset"; \
	dot=$$(printf "%*s" "$$dots" ""); dot=$${dot// /.}; \
	printf "%s" "$$dot"; \
	printf "] %d/%d" "$$curr" "$$total"; \
	if [ "$$curr" -ge "$$total" ]; then printf " ✓\n"; fi;
endef

all: $(NAME)

$(NAME): $(LIBFT_A) $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) $(LIBFT_A) $(LDFLAGS) -o $@
	@echo -e "$(LIGHT_TURQUOISE)Minishell ready!$(RESET)"

$(LIBFT_A): $(LIBFT_OBJS)
	@$(AR) $(ARFLAGS) $@ $^

$(LIBFT_OBJ_DIR)/%.o: $(LIBFT_SRC_D)/%.c | $(LIBFT_OBJ_DIR)
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@
	$(call show_progress)

$(APP_OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(APP_OBJ_DIR)
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@
	$(call show_progress)

$(OBJ_ROOT) $(APP_OBJ_DIR) $(LIBFT_OBJ_DIR):
	@mkdir -p $@

clean:
	@echo -e "$(LIGHT_RED)Running object cleanup...$(RESET)"
	@rm -rf "$(OBJ_ROOT)"
	@echo -e "$(TURQUOISE)Cleaning of objects completed!$(RESET)"

fclean:
	@echo -e "$(LIGHT_RED)Running a full cleanup...$(RESET)"
	@rm -rf "$(OBJ_ROOT)"
	@rm -f "$(NAME)" "$(LIBFT_A)"
	@echo -e "$(TURQUOISE)Full cleaning finished!$(RESET)"

re:
	@$(MAKE) fclean
	@$(MAKE) -s all

.PHONY: all clean fclean re
