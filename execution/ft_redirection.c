#include "../minishell.h"

void save_fd(t_cmd *cmd)
{
    cmd->saved_stdin = dup(0);
    if (cmd->saved_stdin == -1)
        return ; 
    cmd->saved_stdout = dup(1);
    if (cmd->saved_stdout == -1)
        return ;
}

void restor_fd(t_cmd *cmd)
{
    if (cmd->saved_stdin != -1)
    {
        if (dup2(cmd->saved_stdin, 0) == -1)
            return ;
    }
    if (cmd->saved_stdout != -1)
    {
        if (dup2(cmd->saved_stdout, 0) == -1)
            return ;
    }
}

int open_files(t_cmd *cmd)
{
    int fd;

    fd = 0;
    if (cmd->input_file)
        fd = open(cmd->input_file, O_RDONLY);
    if (cmd->heredoc_file)
        fd = cmd->heredoc_file;
    if (cmd->output_file)
        fd = open(cmd->output_file, O_CREAT | O_TRUNC | O_WRONLY, 0644);
    if (cmd->append_output)
        fd = open(cmd->append_output, O_CREAT | O_APPEND, 0644);
    if (fd == -1)
        perror("open");
    return (fd);
}

int append_fd(t_cmd *cmd, int fd)
{
    if (cmd->input_file || cmd->heredoc_file)
        if (dup2(fd, 0) == -1)
            return -1;
    else if (cmd->output_file || cmd->append_output)
        if (dup2(fd, 1) == -1)
            return (-1);
}

int redirection(t_cmd *cmd)
{
    /*see you next season*/
}