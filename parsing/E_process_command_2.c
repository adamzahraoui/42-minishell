#include "../minishell.h"

int dispatch_redirection(t_cmd *cmd, t_token *next, t_token_type type, t_var *vars, char **env)
{
    int fd;

    if (type == TOKEN_REDIR_IN || type == TOKEN_REDIR_OUT || type == TOKEN_APPEND)
    {
        if (!handle_redirections(cmd, next, type))
            return (0);
    }
    else if (type == TOKEN_HEREDOC)
    {
        int saved_stdin = dup(STDIN_FILENO);

        fd = handle_heredoc(next->value, vars, env);
        if (fd < 0)
        {
            close(saved_stdin);
            return (0);
        }

        if (dup2(fd, STDIN_FILENO) == -1)
        {
            perror("dup2 heredoc");
            close(fd);
            close(saved_stdin);
            return (0);
        }
        close(fd);
        cmd->saved_stdin = saved_stdin;
    }
    return (1);
}

int handle_redirections(t_cmd *cmd, t_token *next, t_token_type type)
{
    if (type == TOKEN_REDIR_IN)
    {
        if (cmd->input_file)
            free(cmd->input_file);
        cmd->input_file = ft_strdup(next->value);
    }
    else if (type == TOKEN_REDIR_OUT || type == TOKEN_APPEND)
    {
        if (cmd->output_file)
            free(cmd->output_file);
        cmd->output_file = ft_strdup(next->value);
        if (type == TOKEN_APPEND)
            cmd->append_output = 1;
        else
            cmd->append_output = 0;
    }
    return (1);
}

char *remove_quotes(const char *str)
{
    int len;
    char *result;

    if (!str)
        return NULL;

    len = strlen(str);
    if (len >= 2 &&
        ((str[0] == '"' && str[len - 1] == '"') ||
         (str[0] == '\'' && str[len - 1] == '\'')))
    {
        result = malloc(len - 1);
        if (!result)
            return NULL;
        strncpy(result, str + 1, len - 2);
        result[len - 2] = '\0';
        return result;
    }
    return ft_strdup(str);
}

int heredoc_setup(const char *delimiter, char **clean_delimiter, int *quoted)
{
    if (!delimiter)
        return -1;
    *clean_delimiter = remove_quotes(delimiter);
    if (!*clean_delimiter)
        return -1;
    *quoted = is_quoted(delimiter);
    *clean_delimiter = remove_quotes(delimiter);
    if (!*clean_delimiter)
        return -1;
    return 0;
}

int heredoc_pipe_and_fork(int *fds, char *clean_delimiter)
{
    if (pipe(fds) == -1)
    {
        perror("pipe");
        free(clean_delimiter);
        return -1;
    }
    pid_t pid = fork();
    if (pid == -1)
    {
        perror("fork");
        close(fds[0]);
        close(fds[1]);
        free(clean_delimiter);
        return -1;
    }
    return pid;
}

int heredoc_child_loop(const char *clean_delimiter, int quoted, int write_fd, t_var *vars, char **env, int *lineno)
{
    char *line;
    int got_delim = 0;
    while (1)
    {
        line = readline("> ");
        if (!line)
            break;
        if (strcmp(line, clean_delimiter) == 0)
        {
            got_delim = 1;
            free(line);
            break;
        }
        char *to_write = line;
        char *expanded = NULL;
        if (!quoted)
            expanded = expand_token(line, vars, env);
        if (expanded)
            to_write = expanded;
        write(write_fd, to_write, strlen(to_write));
        write(write_fd, "\n", 1);
        if (expanded)
            free(expanded);
        free(line);
        (*lineno)++;
    }
    return got_delim;
}

void heredoc_child(int *fds, char *clean_delimiter, int quoted, t_var *vars, char **env)
{
    int lineno = 1;
    int got_delim = 0;
    signal(SIGINT, SIG_DFL);
    close(fds[0]);
    got_delim = heredoc_child_loop(clean_delimiter, quoted, fds[1], vars, env, &lineno);
    if (!got_delim)
        fprintf(stderr,
                "bash: warning: here-document at line %d delimited by end-of-file (wanted `%s')\n",
                lineno, clean_delimiter);
    close(fds[1]);
    free(clean_delimiter);
    exit(0);
}

int heredoc_parent(pid_t pid, int *fds, char *clean_delimiter)
{
    int status;
    close(fds[1]);
    free(clean_delimiter);
    void (*old_handler)(int) = signal(SIGINT, SIG_IGN);
    waitpid(pid, &status, 0);
    signal(SIGINT, old_handler);
    if (WIFSIGNALED(status))
    {
        close(fds[0]);
        return -1;
    }
    return fds[0];
}

int handle_heredoc(const char *delimiter, t_var *vars, char **env)
{
    int fds[2];
    pid_t pid;
    int quoted;
    char *clean_delimiter;
    if (heredoc_setup(delimiter, &clean_delimiter, &quoted) == -1)
        return -1;
    pid = heredoc_pipe_and_fork(fds, clean_delimiter);
    if (pid == -1)
        return -1;
    if (pid == 0)
        heredoc_child(fds, clean_delimiter, quoted, vars, env);
    return heredoc_parent(pid, fds, clean_delimiter);
}

int is_quoted(const char *str)
{
    int len;

    if (!str)
        return 0;

    len = strlen(str);
    return (len >= 2 && ((str[0] == '"' && str[len - 1] == '"') ||
                         (str[0] == '\'' && str[len - 1] == '\'')));
}