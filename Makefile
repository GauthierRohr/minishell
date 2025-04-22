NAME = minishell

CC = cc
CFLAGS = -Wall -Wextra -Werror
INCLUDES = -I ./libft
OBJ_DIR = obj

SRCS =	src/builtins.c \
		src/env.c \
		src/exec.c \
		src/main.c \
		src/parser.c \
		src/redirect.c \
		src/signals.c \
		src/utils.c

OBJS = $(SRCS:src/%.c=$(OBJ_DIR)/%.o)

# Libraries
LIBFT = ./libft/libft.a

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) -lreadline -o $(NAME)

$(LIBFT):
	$(MAKE) -C ./libft

# créer les fichiers objets dans le répertoire obj
$(OBJ_DIR)/%.o: src/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)
	$(MAKE) -C ./libft clean

fclean: clean
	rm -f $(NAME)
	$(MAKE) -C ./libft fclean

re: fclean all

.PHONY: all clean fclean re