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
                fd = handle_heredoc(next->value);
                if (fd < 0)
                        return (0);
                if (dup2(fd, STDIN_FILENO) == -1)
                {
                        perror("dup2 heredoc");
                        close(fd);
                        return (0);
                }
                close(fd);
        }
        return (1);
}

int        handle_redirections(t_cmd *cmd, t_token *next, t_token_type type)
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

int handle_heredoc(const char *delimiter)
{
        char *line;
        int fds[2];

        if (!delimiter)
                return (-1);
        
        if (pipe(fds) == -1)
        {
                perror("pipe");
                return (-1);
        }
        while (1)
        {
                line = readline("> ");
                if (!line)
                {
                        printf("\n");
                        break;
                }
                if (ft_strcmp(line, delimiter) == 0)
                {
                        free(line);
                        break;
                }
                write(fds[1], line, ft_strlen(line));
                write(fds[1], "\n", 1);
                free(line);
        }
        
        close(fds[1]);
        return (fds[0]);
}