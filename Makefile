CC= cc
CFLAGS= -Wall -Wextra -Werror -Ilibft -I. -g
RM= rm -f
LDFLAGS = -Llibft -lreadline

# Only the libft functions you actually use
SRC= libft/ft_isalpha.c libft/ft_isdigit.c libft/ft_isalnum.c libft/ft_isascii.c libft/ft_isprint.c \
    libft/ft_strlen.c libft/ft_memset.c libft/ft_bzero.c libft/ft_memcpy.c libft/ft_memmove.c libft/ft_strlcpy.c \
    libft/ft_strlcat.c libft/ft_toupper.c libft/ft_tolower.c libft/ft_strchr.c libft/ft_strrchr.c libft/ft_strncmp.c \
    libft/ft_memchr.c libft/ft_memcmp.c libft/ft_strnstr.c libft/ft_atoi.c libft/ft_calloc.c libft/ft_strdup.c \
    libft/ft_substr.c libft/ft_strjoin.c libft/ft_strtrim.c libft/ft_split.c \
    libft/ft_putchar_fd.c libft/ft_putstr_fd.c libft/ft_putendl_fd.c \
    libft/ft_lstnew_bonus.c libft/ft_lstadd_front_bonus.c libft/ft_lstsize_bonus.c \
    libft/ft_lstlast_bonus.c libft/ft_lstadd_back_bonus.c libft/ft_lstdelone_bonus.c \
    libft/ft_lstclear_bonus.c libft/ft_lstiter_bonus.c libft/ft_lstmap_bonus.c \
    execution/mini.c execution/utils_a.c execution/ft_env.c execution/ft_cd.c execution/ft_echo.c execution/ft_export.c \
    execution/ft_export_ex.c execution/ft_exit.c execution/ft_pwd.c execution/ft_unset.c \
    execution/external_executables.c execution/exit_error.c execution/utils_b.c \
    parsing/A_assignment_or_empty.c parsing/B_tokenizer_1.c parsing/B_tokenizer_2.c \
    parsing/C_expand_all_tokens_1.c parsing/C_expand_all_tokens_2.c \
    parsing/D_print_tokens.c parsing/E_process_command_1.c parsing/E_process_command_2.c parsing/E_process_command_3.c \
    minishell.c

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