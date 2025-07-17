#include "../minishell.h"

int dispatch_redirection(t_cmd *cmd, t_token *next, t_token_type type)
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

        fd = handle_heredoc(next->value);
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

int handle_heredoc(const char *delimiter)
{
    int fds[2];
    pid_t pid;
    int status;
    char *clean_delimiter;

    if (!delimiter)
        return -1;

    clean_delimiter = remove_quotes(delimiter);
    if (!clean_delimiter)
        return -1;

    if (pipe(fds) == -1)
    {
        perror("pipe");
        free(clean_delimiter);
        return -1;
    }

    pid = fork();
    if (pid == -1)
    {
        perror("fork");
        close(fds[0]);
        close(fds[1]);
        free(clean_delimiter);
        return -1;
    }

    if (pid == 0)
    {
        char *line;
        int lineno = 1;
        int got_delim = 0;

        signal(SIGINT, SIG_DFL);
        close(fds[0]);

        while (1)
        {
            line = readline("> ");
            if (!line)
            {
                break;
            }
            if (strcmp(line, clean_delimiter) == 0)
            {
                got_delim = 1;
                free(line);
                break;
            }
            write(fds[1], line, strlen(line));
            write(fds[1], "\n", 1);
            free(line);
            lineno++;
        }
        if (!got_delim)
            fprintf(stderr,
                    "bash: warning: here-document at line %d delimited by end-of-file (wanted `%s')\n",
                    lineno, clean_delimiter);
        close(fds[1]);
        free(clean_delimiter);
        exit(0);
    }
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
