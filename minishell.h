/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adzahrao <adzahrao@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 18:45:07 by adzahrao          #+#    #+#             */
/*   Updated: 2025/07/19 12:55:04 by adzahrao         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
#define MINISHELL_H

#include "libft/libft.h"
#include <fcntl.h>
#include <stdio.h>
#include <readline/history.h>
#include <readline/readline.h>
#include <signal.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/types.h>

typedef struct s_myenv
{
	char *data;
	struct s_myenv *next;
	int i;
} t_myenv;

typedef struct s_myenv_ex
{
	char *data;
	struct s_myenv_ex *next;
} t_myenv_ex;

#define MAX_TOKEN_LEN 4096

typedef struct s_var
{
	char *name;
	char *value;
	struct s_var *next;
} t_var;

typedef struct s_expand_context
{
	t_var *vars;
	char **env;
} t_expand_context;

typedef struct s_token_state
{
	int i;
	int j;
	int in_single;
	int in_double;
} t_token_state;

typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIR_IN,
	TOKEN_REDIR_OUT,
	TOKEN_HEREDOC,
	TOKEN_APPEND,
	TOKEN_ERROR
} t_token_type;

typedef struct s_token
{
	char *value;
	t_token_type type;
	struct s_token *next;
} t_token;

typedef struct s_cmd
{
	char **args;
	int arg_count;
	int arg_capacity;
	char *input_file;
	char *output_file;
	int append_output;
	char *heredoc_delim;
	char *heredoc_file;
	int saved_stdin;
	struct s_cmd *next;
} t_cmd;

t_token *tokenize(char *line);
t_token *new_token(char *value);
int add_token_to_list(t_token **head, t_token **current,
					  char *token_value);
t_token_type determine_token_type(char *value);
void free_tokens(t_token *tokens);
char *get_next_token(char *line, int *i);
char *get_operator_token(char *line, int *i);
char *get_quoted_token(char *line, int *i);
char *get_word_token(char *line, int *i);
int is_delimiter(char c);
int is_whitespace(char c);
char *get_next_token_part(char *line, int *i);



t_cmd *parse_commands(t_token **tokens, t_var *vars, char **env);
t_cmd *parse_command(t_token **tokens, t_var *vars, char **env);
t_cmd *init_command(void);
int is_redirection(t_token_type type);

int handle_heredoc(const char *delimiter, t_var *vars, char **env);
int is_quoted(const char *str);

int dispatch_redirection(t_cmd *cmd, t_token *next, t_token_type type, t_var *vars, char **env);
int handle_redirections(t_cmd *cmd, t_token *next, t_token_type type);

int handle_redirection(t_cmd *cmd, t_token **token_ptr, t_token **tokens, t_var *vars, char **env);
int is_quoted(const char *str);

int heredoc_setup(const char *delimiter, char **clean_delimiter, int *quoted);
int heredoc_pipe_and_fork(int *fds, char *clean_delimiter);
int heredoc_child_loop(const char *clean_delimiter, int quoted, int write_fd, t_var *vars, char **env, int *lineno);
void heredoc_child(int *fds, char *clean_delimiter, int quoted, t_var *vars, char **env);
int heredoc_parent(pid_t pid, int *fds, char *clean_delimiter);
int handle_heredoc(const char *delimiter, t_var *vars, char **env);

char **convert_myenv_to_env(t_myenv *myenv);
char *get_myenv_value(t_myenv *myenv, char *name);

void add_argument(t_cmd *cmd, char *arg);
void free_commands(t_cmd *cmds);
void print_tokens(t_token **tokens, t_var *vars, char **env);
char *get_env_value(char **env, char *name);

char *expand_token(char *token, t_var *vars, char **env);
int expand_variable(char *token, int *i, char *result,
					t_expand_context *ctx);

void builtin_echo(t_cmd *cmd);

void setup_signals(void);
void handle_sigint(int sig);
void process_commands(t_token **tokens, t_var *vars, char **env, t_cmd **cmd);
void set_shell_var(t_var **vars, char *name, char *value);
char *get_shell_var(t_var *vars, char *name);
void expand_all_tokens(t_token **tokens, t_var *vars, char **env);
int validate_syntax(t_token *tokens);

int handle_assignment_or_empty(char *input, t_var **vars, char **env);
void handle_command(char *input, char **env, t_var **vars, t_cmd **cmd, t_token **tokens, t_myenv **myenv);
int extract_var_name(const char *token, int *i, char *var_name);

int read_heredoc_content(const char *delimiter, const char *temp_file);

void process_commands_s(t_token **tokens, t_var *vars, char **env);
void print_cmds(t_cmd *cmd);
int extract_var_name(const char *token, int *i, char *var_name);
void builtin_echo(t_cmd *cmd);
int is_valid_n_flag(const char *str);

// built-in commands :

void ft_echo(char **cmd);
void ft_cd(t_cmd **cmd, t_myenv **myenv, t_myenv_ex **myenv_ex);
void ft_pwd();
void ft_export(t_myenv_ex **myenv_ex, t_myenv **myenv, t_cmd **cmd);
void set_env_ex(t_myenv_ex **myenv, char **env);
void ft_unset(t_myenv_ex **myenv_ex, t_myenv **myenv, char *str);
void print_env(t_myenv *myenv);
void ft_exit(t_myenv_ex **myenv_ex, t_myenv **myenv);

char *remove_quotes(const char *str);

// external executables

void external_executables(t_cmd **cmd, char **path, char **env);

// help funcion
void    ft_free_error(char *str, t_myenv **myenv, t_myenv_ex **myenv_ex, int i);
char **my_get_path_split(t_myenv **myenv, char *path, char c);
char *my_get_path(t_myenv *myenv, char *path);
void add_back_env(t_myenv **myenv, char *str);
void set_env_doubl(t_myenv **myenv, char *str);
int check_double(t_myenv_ex **myenv_ex, char *str);
char *check_val(char *str);
int ft_strcmp(const char *s1, const char *s2);
int check_exist(char *str, char *dest);
int check_builtin_cmd(t_cmd **cmd, t_myenv *myenv, t_myenv_ex *myenv_ex);
void free_error(char *str, t_myenv **myenv, t_myenv_ex **myenv_ex);
int check_double_env(t_myenv **myenv, char *str);
void cmd_ex(t_cmd **args, t_token **tokens, char **env, t_myenv **myenv, t_myenv_ex **myenv_ex);
void declare_env(t_myenv **myenv, t_myenv_ex **myenv_ex, char **env);
void    set_status(t_myenv **myenv, char *str, int status);

#endif