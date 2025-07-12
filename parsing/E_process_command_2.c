#include "../minishell.h"

int dispatch_redirection(t_cmd *cmd, t_token *next, t_token_type type)
{
	if (type == TOKEN_REDIR_IN || type == TOKEN_REDIR_OUT || type == TOKEN_APPEND)
	{
		if (!handle_redirections(cmd, next, type))
			return (0);
	}
	else if (type == TOKEN_HEREDOC)
	{
		if (!handle_heredoc_redirection(cmd, next))
			return (0);
	}
	return (1);
}


int	handle_redirections(t_cmd *cmd, t_token *next, t_token_type type)
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



int	read_heredoc_content(const char *delimiter, const char *temp_file)
{
    int		fd;
    char	*line;
    int		delim_len;

    fd = open(temp_file, O_CREAT | O_WRONLY | O_TRUNC, 0600);
    if (fd < 0)
        return (0);
    delim_len = ft_strlen(delimiter);
    while (1)
    {
        line = readline("> ");
        if (!line || (ft_strncmp(line, delimiter, delim_len) == 0 
            && line[delim_len] == '\0'))
        {
            free(line);
            break ;
        }
        write(fd, line, ft_strlen(line));
        write(fd, "\n", 1);
        free(line);
    }
    close(fd);
    return (1);
}

int	validate_and_prepare_heredoc(t_cmd *cmd, t_token *next, char **temp_file)
{
	static int	counter = 0;
	char		*num_str;
	char		*temp;

	if (cmd->heredoc_delim)
	{
		ft_putendl_fd("Error: multiple heredocs", 2);
		return (0);
	}
	cmd->heredoc_delim = ft_strdup(next->value);
	num_str = ft_itoa(getpid() * 1000 + counter++);
	temp = ft_strjoin("/tmp/heredoc_", num_str);
	*temp_file = ft_strjoin(temp, ".tmp");
	free(num_str);
	free(temp);
	if (!(*temp_file))
		return (0);
	return (1);
}

int	fork_and_read_heredoc(const char *delim, const char *temp_file)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		if (!read_heredoc_content(delim, temp_file))
			exit(1);
		exit(0);
	}
	else
	{
		waitpid(pid, &status, 0);
		if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
		{
			ft_putendl_fd("", 1);
			return (0);
		}
	}
	return (1);
}

int	handle_heredoc_redirection(t_cmd *cmd, t_token *next)
{
	char	*temp_file;

	if (!validate_and_prepare_heredoc(cmd, next, &temp_file))
		return (0);

	if (!fork_and_read_heredoc(cmd->heredoc_delim, temp_file))
	{
		free(temp_file);
		return (0);
	}
	cmd->heredoc_file = temp_file;
	if (cmd->input_file)
		free(cmd->input_file);
	cmd->input_file = ft_strdup(temp_file);
	return (1);
}
