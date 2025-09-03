# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: dimendon <dimendon@student.hive.fi>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/06/04 13:42:42 by dimendon          #+#    #+#              #
#    Updated: 2025/08/26 by dimendon         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

.MAKEFLAGS: --no-print-directory

CC      = cc
CFLAGS  = -Wall -Wextra -Werror -I./includes -I./src/libft

VALGRIND = valgrind --leak-check=full --track-origins=yes --trace-children-skip='*/bin/*,*/sbin/*,/usr/bin/*' \
--trace-children=yes --track-fds=yes --suppressions=readline.supp

LDFLAGS = -lreadline -lncurses

TARGET      = minishell
OBJ_PATH    = obj/
LIBFT_DIR   = src/libft
LIBFT_LIB   = $(LIBFT_DIR)/libft.a

SRCS = \
    src/builtins/custom_export/export_check.c \
    src/builtins/custom_export/export_env.c \
    src/builtins/custom_export/export_print.c \
    src/builtins/custom_export/export_utils.c \
    src/builtins/custom_cd.c \
    src/builtins/custom_echo.c \
    src/builtins/custom_echo_utils.c \
    src/builtins/custom_env.c \
    src/builtins/custom_exit.c \
    src/builtins/custom_pwd.c \
    src/builtins/custom_unset.c \
    src/env/env_lookup.c \
    src/env/env_utils.c \
    src/utils/array_utils.c \
    src/utils/cleanup.c \
    src/utils/error.c \
    src/parsing/expansion/expander_utils.c \
    src/parsing/expansion/quote_utils.c \
    src/parsing/expansion/expander_build.c \
    src/parsing/expansion/expander_dollar.c \
    src/parsing/expansion/expander_var.c \
    src/parsing/expansion/word_split.c \
    src/parsing/expansion/word_split_utils.c \
    src/parsing/expansion/whitespace_split.c \
    src/parsing/redirection/redir_split.c \
    src/parsing/redirection/redir_split_utils.c \
    src/parsing/tokenization/split_pipes.c \
    src/parsing/tokenization/token_build.c \
    src/parsing/tokenization/token_build_utils.c \
    src/parsing/tokenization/token_post.c \
    src/parsing/tokenization/tokenize.c \
    src/parsing/tokenization/token_utils.c \
    src/parsing/tokenization/quote_utils.c \
    src/piping/pipeline/pipeline.c \
    src/piping/pipeline/pipeline_io.c \
    src/piping/pipeline/pipeline_child.c \
    src/piping/pipeline/pipeline_setup.c \
    src/piping/pipeline/pipeline_build.c \
    src/piping/pipeline/cmd_utils.c \
    src/piping/pipeline/execute_cmd.c \
    src/piping/pipeline/pipeline_helpers.c \
    src/piping/redirection/redir_token_utils.c \
    src/piping/redirection/redir_apply.c \
    src/piping/redirection/redir_process.c \
    src/piping/redirection/redir_clean.c \
    src/piping/redirection/redir_utils.c \
    src/piping/redirection/redirections_utils.c \
    src/piping/redirection/redirection_files.c \
    src/execution/command_processor.c \
    src/execution/builtin_processor.c \
    src/execution/command_prepare.c \
    src/execution/redirection_state.c \
    src/execution/command_execution.c \
    src/main.c

OBJS = $(SRCS:src/%.c=$(OBJ_PATH)%.o)
HEADERS = includes/minishell.h

GREEN  = \033[0;32m
YELLOW = \033[1;33m
CYAN   = \033[0;36m
RESET  = \033[0m

all: $(LIBFT_LIB) $(TARGET)

$(OBJ_PATH)%.o: src/%.c $(HEADERS)
	@mkdir -p $(dir $@)
	@printf "$(CYAN)Compiling minishell source: $<$(RESET)\n"
	@$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJS)
	@printf "$(GREEN)Linking minishell executable...$(RESET)\n"
	@$(CC) $(CFLAGS) $(OBJS) $(LIBFT_LIB) $(LDFLAGS) -o $(TARGET)
	@printf "$(GREEN)minishell built successfully!$(RESET)\n"

$(LIBFT_LIB):
	@$(MAKE) --no-print-directory -C $(LIBFT_DIR) build

run: $(TARGET)
	@echo "$(YELLOW)Running with Valgrind...$(RESET)"
	$(VALGRIND) ./$(TARGET)

clean:
	@printf "$(YELLOW)Cleaning object files (minishell)...$(RESET)\n"
	@rm -f $(OBJS)
	@rm -rf $(OBJ_PATH)
	@$(MAKE) --no-print-directory -C $(LIBFT_DIR) clean

fclean: clean
	@printf "$(YELLOW)Removing executable (minishell)...$(RESET)\n"
	@rm -f $(TARGET)
	@$(MAKE) --no-print-directory -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re run


