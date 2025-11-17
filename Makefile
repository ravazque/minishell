
MAKEFLAGS += --no-print-directory

NAME = minishell

CC = cc
CFLAGS = -Wall -Wextra -Werror -I$(INC_DIR) -I$(LIBFT_INC)
LDFLAGS = -lreadline

SRC_DIR = src
INC_DIR = include

LIBFT_DIR = src/aux_libft
LIBFT_INC = $(LIBFT_DIR)/include
LIBFT = $(LIBFT_DIR)/libft.a

RESET = \001\033[0m\002
GREEN = \001\033[1;32m\002
CYAN = \001\033[1;36m\002
COMPILE = \001\033[1;33m\002
RED = \001\033[1;91m\002

SRCS = $(SRC_DIR)/minishell.c \
	   $(SRC_DIR)/aux_minishell/error.c \
	   $(SRC_DIR)/aux_minishell/init.c \
	   $(SRC_DIR)/aux_minishell/loop.c \
	   $(SRC_DIR)/aux_minishell/mshlvl.c \
	   $(SRC_DIR)/aux_minishell/underscore.c \
	   $(SRC_DIR)/built_ins/builtin_cd/builtin_cd.c \
	   $(SRC_DIR)/built_ins/builtin_cd/builtin_cd_path.c \
	   $(SRC_DIR)/built_ins/builtin_cd/builtin_cd_utils.c \
	   $(SRC_DIR)/built_ins/builtin_echo.c \
	   $(SRC_DIR)/built_ins/builtin_env/builtin_env.c \
	   $(SRC_DIR)/built_ins/builtin_env/builtin_env_cmd.c \
	   $(SRC_DIR)/built_ins/builtin_env/builtin_env_exec.c \
	   $(SRC_DIR)/built_ins/builtin_env/builtin_env_fork.c \
	   $(SRC_DIR)/built_ins/builtin_env/builtins_setenv.c \
	   $(SRC_DIR)/built_ins/builtin_exit.c \
	   $(SRC_DIR)/built_ins/builtin_export/builtin_export.c \
	   $(SRC_DIR)/built_ins/builtin_export/builtin_export_print.c \
	   $(SRC_DIR)/built_ins/builtin_export/builtin_export_set.c \
	   $(SRC_DIR)/built_ins/builtin_export/builtin_export_utils.c \
	   $(SRC_DIR)/built_ins/builtin_export/builtin_export_validate.c \
	   $(SRC_DIR)/built_ins/builtin_pwd.c \
	   $(SRC_DIR)/built_ins/builtins_core.c \
	   $(SRC_DIR)/built_ins/builtins_utils.c \
	   $(SRC_DIR)/built_ins/builtin_unset.c \
	   $(SRC_DIR)/cleaner/free_cmds.c \
	   $(SRC_DIR)/cleaner/free_exec.c \
	   $(SRC_DIR)/cleaner/free_mini.c \
	   $(SRC_DIR)/cleaner/free_redirs.c \
	   $(SRC_DIR)/execution/executor/executor.c \
	   $(SRC_DIR)/execution/executor/executor_empty.c \
	   $(SRC_DIR)/execution/executor/executor_env.c \
	   $(SRC_DIR)/execution/executor/executor_error.c \
	   $(SRC_DIR)/execution/executor/executor_exec.c \
	   $(SRC_DIR)/execution/executor/executor_path.c \
	   $(SRC_DIR)/execution/executor/executor_pipes.c \
	   $(SRC_DIR)/execution/executor/executor_single.c \
	   $(SRC_DIR)/execution/executor/executor_utils.c \
	   $(SRC_DIR)/execution/executor/executor_wait.c \
	   $(SRC_DIR)/execution/heredocs/heredoc.c \
	   $(SRC_DIR)/execution/heredocs/heredoc_collect.c \
	   $(SRC_DIR)/execution/heredocs/heredoc_line.c \
	   $(SRC_DIR)/execution/redirs/redirs.c \
	   $(SRC_DIR)/fork_bomb/fork_bomb.c \
	   $(SRC_DIR)/fork_bomb/fork_bomb_detector.c \
	   $(SRC_DIR)/fork_bomb/fork_bomb_messages.c \
	   $(SRC_DIR)/parse/expand/expand.c \
	   $(SRC_DIR)/parse/expand/expand_extract.c \
	   $(SRC_DIR)/parse/expand/expand_loop.c \
	   $(SRC_DIR)/parse/expand/expand_process.c \
	   $(SRC_DIR)/parse/expand/expand_redirs.c \
	   $(SRC_DIR)/parse/expand/expand_split/expand_split_add.c \
	   $(SRC_DIR)/parse/expand/expand_split/expand_split.c \
	   $(SRC_DIR)/parse/expand/expand_split/expand_split_count.c \
	   $(SRC_DIR)/parse/expand/expand_split/expand_split_helpers.c \
	   $(SRC_DIR)/parse/expand/expand_split/expand_split_utils.c \
	   $(SRC_DIR)/parse/expand/expand_split/expand_split_word.c \
	   $(SRC_DIR)/parse/expand/expand_str.c \
	   $(SRC_DIR)/parse/expand/expand_tilde.c \
	   $(SRC_DIR)/parse/expand/expand_utils.c \
	   $(SRC_DIR)/parse/expand/expand_vars.c \
	   $(SRC_DIR)/parse/lexer/lexer.c \
	   $(SRC_DIR)/parse/lexer/lexer_classify.c \
	   $(SRC_DIR)/parse/lexer/lexer_cmd_create.c \
	   $(SRC_DIR)/parse/lexer/lexer_pipe_split.c \
	   $(SRC_DIR)/parse/lexer/lexer_pipe_valid.c \
	   $(SRC_DIR)/parse/lexer/lexer_quotes.c \
	   $(SRC_DIR)/parse/lexer/lexer_redir.c \
	   $(SRC_DIR)/parse/lexer/lexer_redir_proc.c \
	   $(SRC_DIR)/parse/lexer/lexer_token_arr.c \
	   $(SRC_DIR)/parse/lexer/lexer_token.c \
	   $(SRC_DIR)/parse/lexer/lexer_utils.c \
	   $(SRC_DIR)/parse/parse_assign.c \
	   $(SRC_DIR)/parse/parse.c \
	   $(SRC_DIR)/parse/parse_utils.c \
	   $(SRC_DIR)/parse/tokenizer/tokenizer_aux.c \
	   $(SRC_DIR)/parse/tokenizer/tokenizer.c \
	   $(SRC_DIR)/parse/tokenizer/tokenizer_handlers.c \
	   $(SRC_DIR)/parse/tokenizer/tokenizer_helpers.c \
	   $(SRC_DIR)/parse/tokenizer/tokenizer_parts.c \
	   $(SRC_DIR)/parse/tokenizer/tokenizer_utils.c \
	   $(SRC_DIR)/prompt/prompt_build.c \
	   $(SRC_DIR)/prompt/prompt.c \
	   $(SRC_DIR)/prompt/prompt_get.c \
	   $(SRC_DIR)/prompt/prompt_git.c \
	   $(SRC_DIR)/prompt/prompt_parts.c \
	   $(SRC_DIR)/prompt/prompt_utils.c \
	   $(SRC_DIR)/signals/signals.c \
	   $(SRC_DIR)/signals/signals_utils.c

OBJ_DIR = minishellObjects
OBJS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))

TOTAL_SRCS = $(words $(SRCS))

all: $(NAME)

$(NAME): $(LIBFT) $(OBJS)
	@echo -e "$(COMPILE)Compilando minishell$(RESET)"
	@$(CC) $(CFLAGS) $(OBJS) $(LIBFT) $(LDFLAGS) -o $@
	@echo -e "$(GREEN)Minishell ready!$(RESET)"

$(LIBFT):
	@$(MAKE) -C $(LIBFT_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@echo -e "$(RED)Running object cleanup...$(RESET)"
	@rm -rf $(OBJ_DIR)
	@$(MAKE) -C $(LIBFT_DIR) clean
	@echo -e "$(CYAN)Cleaning of objects completed!$(RESET)"

fclean: clean
	@echo -e "$(RED)Running a full cleanup...$(RESET)"
	@rm -f $(NAME)
	@$(MAKE) -C $(LIBFT_DIR) fclean
	@echo -e "$(CYAN)Full cleaning finished!$(RESET)"

re: fclean all

.PHONY: all clean fclean re

# If you see “-e” when compiling, it is because echo needs this flag to display colors depending on the system.
