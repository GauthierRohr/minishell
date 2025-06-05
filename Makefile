NAME = minishell

CC = gcc
CFLAGS = -Wall -Wextra -Werror
INCLUDES = -I ./libft -I ./inc
OBJ_DIR = obj

# Libraries
LIBFT = ./libft/libft.a

# Sources
SRCS =	src/builtins/export/export_utils.c \
		src/builtins/export/export_vars.c \
		src/builtins/export/export.c \
		src/builtins/builtins_main.c \
        src/builtins/cd.c \
		src/builtins/echo.c \
        src/builtins/env.c \
		src/builtins/exit.c \
		src/builtins/pwd.c \
		src/builtins/unset.c \
		src/exec/exec_utils.c \
        src/exec/exec.c \
        src/piping/piping.c \
        src/redirect/redirect_utils.c \
        src/redirect/redirect.c \
        src/tokenizer/tokenizer.c \
        src/tokenizer/tokenizer_utils.c \
        src/tokenizer/tokenizer_handle.c \
		src/utils/ft_strtol.c \
		src/utils/get_env_value.c \
		src/utils/remove_partial_quotes.c \
        src/env.c \
        src/free_utils.c \
        src/get_cmd_path.c \
        src/main.c \
        src/signals.c

#		src/piping/piping.c
#		src/piping/split_pipeline.c
#		src/piping/exec_pipeline.c
#		src/piping/child_pipeline.c

OBJS = $(SRCS:src/%.c=$(OBJ_DIR)/%.o)

all: $(NAME)

# Compilation of object files
$(OBJ_DIR)/%.o: src/%.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(LIBFT):
	@$(MAKE) -C ./libft

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) -lreadline -o $(NAME)

clean:
	rm -rf $(OBJ_DIR)
	$(MAKE) -C ./libft clean

fclean: clean
	rm -f $(NAME)
	$(MAKE) -C ./libft fclean

re: fclean all

# ---------------------------------------------------
# Autisme 2.0
RED=\033[0;31m
GREEN=\033[0;32m
YELLOW=\033[0;33m
BLUE=\033[0;34m
PURPLE=\033[0;35m
CYAN=\033[0;36m
NC=\033[0m # No Color

flex:
	@echo "🎉 Flexing those muscles! 📸"
	@sleep 0.5
	@echo "\n    ${RED}✨ ᕙ(⇀‸↼‶)ᕗ ✨${NC}\n"
	@sleep 0.5
	@echo "\n    ${GREEN}   ᕙ(✧ڡ✧)ᕗ  ${NC}\n"
	@sleep 0.5
	@echo "\n    ${YELLOW}⚡️ ᕙ(^▽^)ᕗ ⚡️${NC}\n"
	@sleep 0.5
	@echo "\n    ${BLUE}   ᕦ(ò_óˇ)ᕤ  ${NC}\n"
	@sleep 0.5
	@echo "\n    ${PURPLE}💥 ᕙ(▀̿̿Ĺ̯̿̿▀̿ ̿)ᕗ 💥${NC}\n"
	@sleep 0.5
	@echo "\n🏆 Flex session complete! I'm a fcking boss"!

seum:
	@echo "\n ${RED}   ༼;´༎ຶ ۝ ༎ຶ༽ ${NC}\n"
	@sleep 0.5
	@echo "\n ${GREEN}   (˚ ˃̣̣̥⌓˂̣̣̥ ) ${NC}\n"
	@sleep 0.5
	@echo "\n ${CYAN}╭∩╮( •̀_•́ )╭∩╮${NC}\n"
	@sleep 0.5
	@echo "\n ${PURPLE}   (ﾉಥ益ಥ) ${NC}\n"
	@sleep 0.5
	@echo "\n ${YELLOW}   ᶠᶸᶜᵏᵧₒᵤ!${NC}\n"
	@sleep 0.5

.PHONY: all clean fclean re flex seum