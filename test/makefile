
NAME        := minishell

# --- Rutas del proyecto ---
SRC_DIR     := src
INC_DIR     := include

# libft dentro de src/
LIBFT_DIR   := src/auxlibft
LIBFT_SRC_D := $(LIBFT_DIR)/src
LIBFT_INC_D := $(LIBFT_DIR)/include
LIBFT_OBJ_D := minishellObjects
LIBFT_A     := $(LIBFT_DIR)/libft.a

# --- Compilación ---
CC          := cc
# CFLAGS      := -I$(INC_DIR) -I$(LIBFT_INC_D)
CFLAGS      := -Wall -Wextra -Werror -I$(INC_DIR) -I$(LIBFT_INC_D)
LDFLAGS     := -lreadline   # quita esto si aún no usas readline

BUILD_DIR   := src/minishellObjects

# Todos los .c del proyecto EXCEPTO libft
SRCS        := $(shell find $(SRC_DIR) -type f -name '*.c' -not -path '$(LIBFT_DIR)/*')
OBJS        := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRCS))

# Fuentes de libft
LIBFT_SRCS  := $(shell find $(LIBFT_SRC_D) -type f -name '*.c')
LIBFT_OBJS  := $(patsubst $(LIBFT_SRC_D)/%.c,$(LIBFT_OBJ_D)/%.o,$(LIBFT_SRCS))

.PHONY: all clean fclean re run debug

all: $(NAME)

$(NAME): $(LIBFT_A) $(OBJS)
	@echo "⛏️  Enlazando $(NAME)…"
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT_A) $(LDFLAGS) -o $@
	@echo "✅ Listo: $(NAME)"

# Objetos del proyecto
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# --- libft estática ---
$(LIBFT_A): $(LIBFT_OBJS)
	@mkdir -p $(LIBFT_DIR)
	ar rcs $@ $(LIBFT_OBJS)

$(LIBFT_OBJ_D)/%.o: $(LIBFT_SRC_D)/%.c
	@mkdir -p $(dir $@)
	$(CC) -Wall -Wextra -Werror -I$(LIBFT_INC_D) -c $< -o $@

# --- utilidades ---
run: $(NAME)
	./$(NAME)

debug: CFLAGS += -g3 -fsanitize=address
debug: re

clean:
	@echo "🧹 Limpiando objetos…"
	@rm -rf $(BUILD_DIR) $(LIBFT_OBJ_D)

fclean: clean
	@echo "🧨 Borrando binarios y librería…"
	@rm -f $(NAME) $(LIBFT_A)

re: fclean all
