/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adzahrao <adzahrao@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 11:28:36 by mlaidi            #+#    #+#             */
/*   Updated: 2025/07/11 18:00:55 by adzahrao         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft/libft.h"
# include <fcntl.h>
# include <stdio.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdlib.h>
# include <string.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <unistd.h>
# include <sys/types.h>


# define MAX_TOKEN_LEN 4096

extern int			g_exit_status;

typedef struct s_var
{
	char			*name;
	char			*value;
	struct s_var	*next;
}					t_var;

typedef struct s_expand_context
{
	t_var			*vars;
	char			**env;
}					t_expand_context;

typedef struct s_token_state
{
	int				i;
	int				j;
	int				in_single;
	int				in_double;
}					t_token_state;

typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIR_IN,
	TOKEN_REDIR_OUT,
	TOKEN_HEREDOC,
	TOKEN_APPEND,
	TOKEN_ERROR
}					t_token_type;

typedef struct s_token
{
	char			*value;
	t_token_type	type;
	struct s_token	*next;
}					t_token;

typedef struct s_cmd
{
    char			**args;
    int				arg_count;
    int				arg_capacity;
    char			*input_file;
    char			*output_file;
    int				append_output;
    char			*heredoc_delim;
    char			*heredoc_file;
    struct s_cmd	*next;
}					t_cmd;

t_token				*tokenize(char *line);
t_token				*new_token(char *value);
int					add_token_to_list(t_token **head, t_token **current,
						char *token_value);
t_token_type		determine_token_type(char *value);
void				free_tokens(t_token *tokens);
char				*get_next_token(char *line, int *i);
char				*get_operator_token(char *line, int *i);
char				*get_quoted_token(char *line, int *i);
char				*get_word_token(char *line, int *i);
int					is_delimiter(char c);
int					is_whitespace(char c);
char				*get_next_token_part(char *line, int *i);

t_cmd				*parse_commands(t_token *tokens, t_var *vars, char **env);
t_cmd				*parse_command(t_token **tokens, t_var *vars, char **env);
t_cmd				*init_command(void);
int					is_redirection(t_token_type type);

// int					handle_input_redirection(t_cmd *cmd, t_token *next);
// int					handle_output_redirection(t_cmd *cmd, t_token *next,
// 						t_token_type type);
int					handle_heredoc_redirection(t_cmd *cmd, t_token *next);
int					fork_and_read_heredoc(const char *delim, const char *temp_file);
int					validate_and_prepare_heredoc(t_cmd *cmd, t_token *next, char **temp_file);



int					dispatch_redirection(t_cmd *cmd, t_token *next, t_token_type type);

int					handle_redirections(t_cmd *cmd, t_token *next, t_token_type type);


int					handle_redirection(t_cmd *cmd, t_token **token_ptr, t_token **tokens);


void				add_argument(t_cmd *cmd, char *arg);
void				free_commands(t_cmd *cmds);
void				print_tokens(t_token *tokens, t_var *vars, char **env);
char				*get_env_value(char **env, char *name);

char				*expand_token(char *token, t_var *vars, char **env);
int					handle_exit_status(char *result);
int					expand_variable(char *token, int *i, char *result,
						t_expand_context *ctx);

int					builtin_echo(t_cmd *cmd);

void				setup_signals(void);
void				handle_sigint(int sig);
void				process_commands(t_token *tokens, t_var *vars, char **env, t_cmd **cmd);
void				set_shell_var(t_var **vars, char *name, char *value);
char				*get_shell_var(t_var *vars, char *name);
void				expand_all_tokens(t_token *tokens, t_var *vars, char **env);

int					handle_assignment_or_empty(char *input, t_var **vars);
void				handle_command(char *input, char **env, t_var **vars, t_cmd **cmd);
int					extract_var_name(const char *token, int *i, char *var_name);

char				*get_next_line(int fd);
int					read_heredoc_content(const char *delimiter, const char *temp_file);

void				process_commands_s(t_token *tokens, t_var *vars, char **env);
void				print_cmds(t_cmd *cmd);
int					extract_var_name(const char *token, int *i, char *var_name);


#endif