
MAKEFLAGS += --no-print-directory

NAME = libft.a

CC = cc
CFLAGS = -Wall -Wextra -Werror -I$(INC_DIR)
AR = ar
ARFLAGS = rcs

SRC_DIR = src
INC_DIR = include

RESET = \001\033[0m\002
GREEN = \001\033[1;32m\002
CYAN = \001\033[1;36m\002
COMPILE = \001\033[1;33m\002
RED = \001\033[1;91m\002

SRCS = $(SRC_DIR)/dblptr_print.c \
	   $(SRC_DIR)/dblptr_push.c \
	   $(SRC_DIR)/ft_atoi.c \
	   $(SRC_DIR)/ft_atoi_exit.c \
	   $(SRC_DIR)/ft_bzero.c \
	   $(SRC_DIR)/ft_calloc.c \
	   $(SRC_DIR)/ft_copy_dblptr.c \
	   $(SRC_DIR)/ft_isalnum.c \
	   $(SRC_DIR)/ft_isalpha.c \
	   $(SRC_DIR)/ft_isascii.c \
	   $(SRC_DIR)/ft_isdigit.c \
	   $(SRC_DIR)/ft_isprint.c \
	   $(SRC_DIR)/ft_itoa.c \
	   $(SRC_DIR)/ft_lstadd_back.c \
	   $(SRC_DIR)/ft_lstadd_front.c \
	   $(SRC_DIR)/ft_lstclear.c \
	   $(SRC_DIR)/ft_lstdelone.c \
	   $(SRC_DIR)/ft_lstiter.c \
	   $(SRC_DIR)/ft_lstlast.c \
	   $(SRC_DIR)/ft_lstmap.c \
	   $(SRC_DIR)/ft_lstnew.c \
	   $(SRC_DIR)/ft_lstsize.c \
	   $(SRC_DIR)/ft_memchr.c \
	   $(SRC_DIR)/ft_memcmp.c \
	   $(SRC_DIR)/ft_memcpy.c \
	   $(SRC_DIR)/ft_memmove.c \
	   $(SRC_DIR)/ft_memset.c \
	   $(SRC_DIR)/ft_putchar_fd.c \
	   $(SRC_DIR)/ft_putendl_fd.c \
	   $(SRC_DIR)/ft_putnbr_fd.c \
	   $(SRC_DIR)/ft_putstr_fd.c \
	   $(SRC_DIR)/ft_split.c \
	   $(SRC_DIR)/ft_strchr.c \
	   $(SRC_DIR)/ft_strcmp.c \
	   $(SRC_DIR)/ft_strdup.c \
	   $(SRC_DIR)/ft_striteri.c \
	   $(SRC_DIR)/ft_strjoin.c \
	   $(SRC_DIR)/ft_strlcat.c \
	   $(SRC_DIR)/ft_strlcpy.c \
	   $(SRC_DIR)/ft_strlen.c \
	   $(SRC_DIR)/ft_strmapi.c \
	   $(SRC_DIR)/ft_strncmp.c \
	   $(SRC_DIR)/ft_strnstr.c \
	   $(SRC_DIR)/ft_strrchr.c \
	   $(SRC_DIR)/ft_strtrim.c \
	   $(SRC_DIR)/ft_substr.c \
	   $(SRC_DIR)/ft_tolower.c \
	   $(SRC_DIR)/ft_toupper.c

OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	@echo -e "$(COMPILE)Compilando libft.a$(RESET)"
	@$(AR) $(ARFLAGS) $@ $^
	@echo -e "$(GREEN)libft.a ready!$(RESET)"

%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -f $(OBJS)

fclean: clean
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re

# If you see “-e” when compiling, it is because echo needs this flag to display colors depending on the system.
