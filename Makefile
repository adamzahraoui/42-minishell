CC= cc
RM= rm -f
CFLAGS= -Wall -Wextra -Werror -Ilibft -I. -I/usr/local/Cellar/readline/8.3/include -g
LDFLAGS = -Llibft -L/usr/local/Cellar/readline/8.3/lib -lreadline -lncurses

SRC= libft/ft_isalpha.c libft/ft_isdigit.c libft/ft_isalnum.c libft/ft_isascii.c libft/ft_isprint.c libft/ft_itoa.c \
    libft/ft_strlen.c libft/ft_memset.c libft/ft_bzero.c libft/ft_memcpy.c libft/ft_memmove.c libft/ft_strlcpy.c \
    libft/ft_strlcat.c libft/ft_toupper.c libft/ft_tolower.c libft/ft_strchr.c libft/ft_strrchr.c libft/ft_strncmp.c \
    libft/ft_memchr.c libft/ft_memcmp.c libft/ft_strnstr.c libft/ft_atoi.c libft/ft_calloc.c libft/ft_strdup.c \
    libft/ft_substr.c libft/ft_strjoin.c libft/ft_strtrim.c libft/ft_split.c libft/ft_strncpy.c \
    libft/ft_putchar_fd.c libft/ft_putstr_fd.c libft/ft_putendl_fd.c libft/ft_putnbr_fd.c \
    libft/ft_lstnew_bonus.c libft/ft_lstadd_front_bonus.c libft/ft_lstsize_bonus.c \
    libft/ft_lstlast_bonus.c libft/ft_lstadd_back_bonus.c libft/ft_lstdelone_bonus.c \
    libft/ft_lstclear_bonus.c libft/ft_lstiter_bonus.c libft/ft_lstmap_bonus.c \
    execution/mini.c execution/utils_a.c execution/ft_env.c execution/ft_cd.c execution/ft_export_a.c \
    execution/ft_export_ex.c execution/ft_exit.c execution/ft_pwd.c execution/ft_unset.c \
    execution/external_executables.c execution/exit_error.c execution/utils_b.c \
    parsing/01_tokenizer_core.c parsing/02_token_utils.c parsing/03_token_extraction.c \
    parsing/04_expansion_core.c parsing/05_expansion_processing.c parsing/06_environment_utils.c \
    parsing/07_command_parsing.c parsing/08_command_management.c parsing/09_redirection_handling.c \
    parsing/10_heredoc_processing.c parsing/11_builtin_commands.c \
    minishell.c execution/ft_pipe.c execution/ft_echo.c execution/ft_redirection.c execution/utils_c.c execution/ft_export_b.c \
    execution/utils_d.c

OBJ= $(SRC:.c=.o)
NAME= minishell

all: $(NAME)

%.o:%.c libft/libft.h minishell.h
	@$(CC) -c $(CFLAGS) $<  -o  $@

$(NAME): $(OBJ)
	@$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

clean :
	@$(RM) $(OBJ) $(BONUS_OBJ)
fclean : clean
	@$(RM) $(NAME)

re :  fclean all

.PHONY : all clean re bonus