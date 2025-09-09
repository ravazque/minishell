
NAME = minishell

SRC_DIR = src
INC_DIR = include
LIB_DIR = src/auxlibft
OBJ_DIR = src/minishellObjects

LIB_NAME = ft
LIBFILE = $(LIB_DIR)/lib$(LIB_NAME).a

SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

CC = cc
CFLAGS = -Wall -Wextra -Werror -I$(INC_DIR) -I$(LIB_DIR)/include

all: $(LIBFILE) $(NAME)

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

$(LIBFILE):
	@$(MAKE) -C $(LIB_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFILE) -lreadline -o $(NAME)

clean:
	rm -rf $(OBJ_DIR)
	@$(MAKE) -C $(LIB_DIR) clean

fclean: clean
	rm -f $(NAME)
	@$(MAKE) -C $(LIB_DIR) fclean

re: fclean all

.PHONY: all clean fclean re
