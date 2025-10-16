NAME = minishell
CC = clang
CFLAGS = -Wall -Wextra -Werror
LIBS = -lreadline -lhistory
HEADERS = minishell.h libft/libft.h\
		parsing/parsing.h commands/commands.h ids/ids.h execution/execution.h

SRC = main.c btree_apply.c print.c \
		libft/ft_append_arr_str.c libft/ft_arrlen.c libft/ft_atoi.c libft/ft_calloc.c libft/ft_copyarr.c \
		libft/ft_free_arr.c libft/ft_free_split.c libft/ft_get_env_value.c libft/ft_isalnum.c libft/ft_isalpha.c libft/ft_isdigit.c \
		libft/ft_itoa.c libft/ft_memmove.c libft/ft_printf.c libft/ft_printf_utils.c libft/ft_putstr_fd.c libft/ft_putstrerr.c libft/ft_split_dels.c \
		libft/ft_split.c libft/ft_strchr.c libft/ft_strdup.c libft/ft_strjoin.c libft/ft_strjoinf1.c libft/ft_strlcpy.c \
		libft/ft_strlen.c libft/ft_strncmp.c libft/ft_strnstr.c libft/ft_strsjoin.c libft/ft_substr.c \
		libft/ft_write_types.c libft/ft_write_types2.c libft/palloc_err.c\
		libft/add_last_id.c libft/free_pids.c

COMMANDS = commands/cd.c\
			commands/com_main.c\
			commands/echo.c\
			commands/env.c\
			commands/export.c\
			commands/pwd.c\
			commands/unset.c\
			commands/ft_exit.c\
			commands/set_rt.c

PARSING = parsing/execution_tree.c\
		parsing/fragment.c\
		parsing/parsing.c\
		parsing/tokenize.c\
		parsing/validate_tokens.c\
		parsing/validate_tokens_2.c\
		parsing/ft_lst.c\
		parsing/parsing_utils.c\
		parsing/tokenize_utils.c\
		parsing/tokenize_utils_2.c\
		parsing/filename_expansion.c

EXECUTE = execution/binsearch.c\
		  execution/entry.c\
		  execution/execution_utils.c\
		  execution/ft_and.c\
		  execution/ft_command.c\
		  execution/ft_or.c\
		  execution/ft_pipe.c\
		  execution/ft_subshell.c

OBJ_DIR = objects/
TEST_OBJ_DIR = test_objects/

ALL_SRC = $(SRC) $(COMMANDS) $(PARSING) $(EXECUTE)
OBJ = $(patsubst %.c,$(OBJ_DIR)%.o,$(ALL_SRC))
TEST_OBJ = $(patsubst %.c,$(TEST_OBJ_DIR)%.o,$(ALL_SRC))

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(LIBS) $^ -o $@

$(OBJ_DIR)%.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# ---- TEST BUILD ----
test: $(TEST_OBJ)
	$(CC) $(CFLAGS) $(LIBS) $^ -o test

testj :
	make runt -j `nproc`

$(TEST_OBJ_DIR)%.o: %.c
	mkdir -p $(dir $@)
	$(CC) -Wall -Wextra -g -c $< -o $@

runt: test
	valgrind --leak-check=full --suppressions=readline.supp ./test

ctest: all
	./minishell

clean:
	rm -rf $(OBJ_DIR) $(TEST_OBJ_DIR)

fclean: clean
	rm -f $(NAME) test

re: fclean all

.PHONY: all clean fclean re test runt ctest testj

