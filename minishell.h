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

typedef struct s_pipe
{
	int		prev_fd;
	int		i;
	int		status;
	int		count;
	int		*pids;
} t_pipe;

typedef	struct s_free
{
	void	*ptr;
	struct s_free *next;
} t_free;


typedef struct s_myenv
{
	char *data;
	struct s_myenv *next;
	int i;
	int fd[2];
} t_myenv;

typedef struct s_myenv_ex
{
	char *data;
	struct s_myenv_ex *next;
} t_myenv_ex;

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

typedef enum e_redir_type
{
	REDIR_IN,
	REDIR_OUT,
	REDIR_APPEND,
	REDIR_HEREDOC
}	t_redir_type;

typedef struct s_redirection
{
	int fd;
	t_redir_type type;
	char *filename_or_delim;
	struct s_redirection *next;
} t_redirection;

typedef struct s_cmd
{
	char **args;
	int arg_count;
	int arg_capacity;
	t_redirection *redirections; 
	char *input_file;      
	char *output_file;     
	int append_output;     
	char *heredoc_delim;   
	char *heredoc_file;   
	int saved_stdin;
	int saved_stdout;
	struct s_cmd *next;
} t_cmd;



#define MAX_TOKEN_LEN 4096


void ft_pipe_one(t_pipe *pipe_data, t_cmd *arg, t_myenv *env);
void ft_pipe_two(t_pipe *pipe_data, t_cmd **arg, t_myenv *env);
void wait_pid(t_pipe *pipe_data, t_myenv **myenv);
void	init_pipe_data(t_cmd *arg, t_myenv *env, t_pipe *pipe_data);
int	create_pipe_and_fork(t_cmd *arg, t_myenv *env, t_pipe *pipe_data);

void setup_signals(void);
void handle_sigint(int sig);
int validate_syntax(t_token *tokens);
void handle_command(char *input, t_expand_context *ctx, t_cmd **cmd, t_token **tokens, t_myenv **myenv);
void ft_free_all();

t_token *tokenize(char *line);
char *get_next_token(char *line, int *i);
char *process_token_content(char *line, int *i, char *token, t_token_state *st);
char *handle_special_tokens(char *line, int *i, char *token);
char *process_token_part(char *line, int *i, char *token, t_token_state *st);

int add_token_to_list(t_token **head, t_token **current, char *token_value);
t_token *new_token(char *value);
t_token_type determine_token_type(char *value);
int is_whitespace(char c);
int is_delimiter(char c);

char *get_next_token_part(char *line, int *i);
char *get_quoted_token(char *line, int *i);
char *get_word_token(char *line, int *i);
int extract_var_name(const char *token, int *i, char *var_name);


void expand_all_tokens(t_token **tokens, t_expand_context *ctx);

typedef struct s_was
{
	int was_quoted;
	int has_variables;
} t_was;

t_token *handle_successful_expansion(t_token *tok, char *expanded, t_token *next, t_was was);
char *expand_token(char *token, t_var *vars, char **env);


void process_token_character(char *token, t_token_state *st, char *result, t_expand_context *ctx);
int expand_variable(char *token, int *i, char *result, t_expand_context *ctx);
void trim_token_values(t_token *tok, t_token *next, int was_quoted);
int split_token_string(t_token **token_ptr);

void	print_error_ex(t_myenv **myenv, char *str);

char *get_shell_var(t_var *vars, char *name);
char *get_env_value(char **env, char *name);
char **convert_myenv_to_env(t_myenv *myenv);
char	**env_copy_loop(t_myenv *temp, char **env, int count, int i);
char *get_myenv_value(t_myenv *myenv, char *name);
int is_redirection(t_token_type type);
void free_env_array(char **env);



void process_commands(t_token **tokens, t_var *vars, char **env, t_cmd **cmd);
t_cmd *parse_commands(t_token **tokens, t_var *vars, char **env);
t_cmd *parse_command(t_token **tokens, t_var *vars, char **env);
int validate_command(t_cmd *cur, t_cmd **cmd);
int handle_redirection(t_cmd *cmd, t_token **token_ptr, t_token **tokens, t_expand_context *ctx);


t_cmd *init_command(void);
int initialize_command_fields(t_cmd *cmd);
void add_argument(t_cmd *cmd, char *arg);
void free_commands(t_cmd *cmds);
void free_cmd_files(t_cmd *tmp);
void free_tokens(t_token *tokens);


int dispatch_redirection(t_cmd *cmd, t_token *next, t_token_type type, t_expand_context *ctx);
int handle_redirections(t_cmd *cmd, t_token *next, t_token_type type);
int handle_heredoc_redirection(t_cmd *cmd, t_token *next, t_expand_context *ctx);
int handle_heredoc(const char *delimiter, t_expand_context *ctx);
int is_quoted(const char *str);


int heredoc_setup(const char *delimiter, char **clean_delimiter, int *quoted);
int heredoc_pipe_and_fork(int *fds, char *clean_delimiter);
void heredoc_child(int *fds, char *clean_delimiter, int quoted, t_expand_context *ctx);
int heredoc_parent(pid_t pid, int *fds, char *clean_delimiter);




void builtin_echo(t_cmd *cmd);
int is_valid_n_flag(const char *str);
int create_and_link_token(t_token *token, char *str, int split_pos);


void ft_cd(t_cmd **cmd, t_myenv **myenv, t_myenv_ex **myenv_ex);
void ft_pwd(t_myenv **myenv);
void ft_export(t_myenv_ex **myenv_ex, t_myenv **myenv, t_cmd **cmd);
void ft_unset(t_myenv_ex **myenv_ex, t_myenv **myenv, t_cmd *str);
void ft_exit(t_myenv_ex **myenv_ex, t_myenv **myenv, t_cmd *cmd);
void ft_echo(t_cmd *cmd, t_myenv **myenv);

void ft_pipe(t_cmd **cmd, t_myenv **myenv, t_myenv_ex **env_ex, char **or_env);
void    external_executables(t_cmd **cmd, char **path, char **envp, t_myenv **myenv);


void set_env_ex(t_myenv_ex **myenv, char **env);
void print_env(t_myenv *myenv);
void add_back_env(t_myenv **myenv, char *str);
void set_env_doubl(t_myenv **myenv, char *str);
void declare_env(t_myenv **myenv, t_myenv_ex **myenv_ex, char **env);


char **my_get_path_split(t_myenv **myenv, char *path, char c);
char *my_get_path(t_myenv *myenv, char *path);
char *check_cmd(char **path, char *cmd);
int check_builtin_cmd(t_cmd **cmd, t_myenv *myenv, t_myenv_ex *myenv_ex);


int check_double(t_myenv_ex **myenv_ex, char *str);
int check_double_env(t_myenv **myenv, char *str);
int check_exist(char *str, char *dest);
char *check_val(char *str);
void print_export(t_myenv_ex **myenv_ex);

void ft_free_error(char *str, t_myenv **myenv, t_myenv_ex **myenv_ex, int i);
void free_error(char *str, t_myenv **myenv, t_myenv_ex **myenv_ex);
void set_status(t_myenv **myenv, char *str, int status);


void cmd_ex(t_cmd **args, char **env, t_myenv **myenv, t_myenv_ex **myenv_ex);


int ft_strcmp(const char *s1, const char *s2);
int	ft_strncmp_nv(const char *s1, const char *s2, size_t n);

char    *check_cmd(char **path, char *cmd);

void    add_back(t_myenv_ex **myenv_ex, char *str);
void	swap_stack_b(t_myenv_ex **myenv_ex);
void	process_export_data(t_myenv_ex *list);
void	sort_export(t_myenv_ex **myenv_ex);
int	validate_chars_before_equals(char *arg, int equals_index, t_myenv **myenv);
int	is_valid_first_char(char c);
int	find_equals_index(char *arg);
char	*check_val(char *str);
char	*create_cupy(char *dest);
void	process_export_arg(t_myenv_ex **myenv_ex, t_myenv **myenv, t_cmd *cmd, int i);
char	*create_cupy(char *dest);
char	*check_val(char *str);
int	find_equals_index(char *arg);
int	validate_chars_before_equals(char *arg, int equals_index, t_myenv **myenv);
int	is_valid_first_char(char c);


typedef struct s_heredoc_params
{
	const char	*clean_delimiter;
	int			quoted;
	int			write_fd;
	int			*got_delim;
}	t_heredoc_params;

typedef struct s_heredoc_loop_params
{
	const char	*clean_delimiter;
	int			quoted;
	int			write_fd;
	int			*lineno;
}	t_heredoc_loop_params;

typedef struct s_var_extraction_params
{
	char	*token;
	int		*i;
	char	*result;
	char	*var_name;
	int		*len;
}	t_var_extraction_params;

typedef struct s_token_expansion_params
{
	t_token	**tokens;
	t_token	**tok;
	t_token	**prev;
	t_token	*next;
}	t_token_expansion_params;

typedef struct s_failed_expansion_params
{
	t_token	**tokens;
	t_token	*tok;
	t_token	*prev;
	t_token	*next;
	char	*expanded;
}	t_failed_expansion_params;

int	process_heredoc_line(t_heredoc_params *params, t_expand_context *ctx);
void	write_heredoc_line(t_heredoc_params *params, t_expand_context *ctx, char *line);
int	process_variable_extraction(t_var_extraction_params *params, t_expand_context *ctx);
void variable_lookup_and_assign(t_var_extraction_params *params, t_expand_context *ctx);
int	process_token_expansion(t_token_expansion_params *params, t_expand_context *ctx);
int	handle_expansion_result(t_token_expansion_params *params, t_failed_expansion_params *fail_params, char *expanded, t_was was);
t_token	*handle_failed_expansion(t_failed_expansion_params *params);
int	heredoc_child_loop(t_heredoc_loop_params *params, t_expand_context *ctx);

int dispatch_redirection(t_cmd *cmd, t_token *next, t_token_type type, t_expand_context *ctx);
int handle_redirections(t_cmd *cmd, t_token *next, t_token_type type);
t_redirection *create_redirection(t_redir_type type, char *filename_or_delim);
void add_redirection(t_cmd *cmd, t_redirection *redir);

int redirection(t_cmd *cmd);
void restor_fd(t_cmd *cmd);


char *remove_all_quotes(const char *str);
char	*ft_strjoin_gc(char const *s1, char const *s2);
char	*ft_strdup_gc(const char *s);
void	ft_free(void *ptr);
void	ft_free_all(void);

#endif