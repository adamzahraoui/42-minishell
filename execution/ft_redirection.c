#include "../minishell.h"

void save_fd(t_cmd *cmd)
{
    cmd->saved_stdin = dup(0);
    if (cmd->saved_stdin == -1)
    {
        cmd->saved_stdout = -1;
        return;
    }
    cmd->saved_stdout = dup(1);
    if (cmd->saved_stdout == -1)
    {
        close(cmd->saved_stdin);
        cmd->saved_stdin = -1;
        return;
    }
}

void restor_fd(t_cmd *cmd)
{
    if (cmd->saved_stdin != -1)
    {
        if (dup2(cmd->saved_stdin, 0) == -1)
            return ;
        close(cmd->saved_stdin);
    }
    if (cmd->saved_stdout != -1)
    {
        if (dup2(cmd->saved_stdout, 1) == -1)
            return ;
        close(cmd->saved_stdout);
    }
}

int open_files(t_redirection *redirection)
{
    int fd;

    fd = 0;
    if (redirection->type == REDIR_IN)
        fd = open(redirection->filename_or_delim, O_RDONLY);
    // if (redirection->type == TOKEN_HEREDOC)
    //     fd = redirection.;
    if (redirection->type == REDIR_OUT)
        fd = open(redirection->filename_or_delim, O_CREAT | O_TRUNC | O_WRONLY, 0644);
    if (redirection->type == REDIR_APPEND)
        fd = open(redirection->filename_or_delim, O_CREAT | O_WRONLY | O_APPEND, 0644);
    if (fd == -1)
        perror("open");
    return (fd);
}

int append_fd(t_redirection *redirection, int fd)
{
    int ret = 0;
    if (redirection->type == REDIR_IN)
        ret = dup2(fd, 0);
    else if (redirection->type == REDIR_OUT || redirection->type == REDIR_APPEND)
        ret = dup2(fd, 1);
    close(fd);
    if (ret == -1)
        return -1;
    return 0;
}

int redirection(t_cmd *cmd)
{
    t_redirection *current;
    int fd;

    save_fd(cmd);
    current = cmd->redirections;
    while (current)
    {
        fd = open_files(current);
        if (fd == -1)
        {
            restor_fd(cmd);
            return (1);
        }
        if (append_fd(current, fd) == -1)
        {
            restor_fd(cmd);
            return (1);
        }
        current = current->next;
    }
    return (0);
}