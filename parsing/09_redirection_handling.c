/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   09_redirection_handling.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlaidi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 00:08:19 by mlaidi            #+#    #+#             */
/*   Updated: 2025/08/02 00:08:20 by mlaidi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	dispatch_redirection(t_cmd *cmd, t_token *next, t_token_type type,
		t_expand_context *ctx)
{
	if (type == TOKEN_REDIR_IN || type == TOKEN_REDIR_OUT
		|| type == TOKEN_APPEND)
	{
		if (!handle_redirections(cmd, next, type))
			return (0);
	}
	else if (type == TOKEN_HEREDOC)
	{
		if (!handle_heredoc_redirection(cmd, next, ctx))
			return (0);
	}
	return (1);
}

t_redirection	*create_redirection(t_redir_type type, char *filename_or_delim)
{
	t_redirection	*redir;

	redir = (t_redirection *)malloc(sizeof(t_redirection));
	if (!redir)
		return (NULL);
	redir->type = type;
	redir->filename_or_delim = ft_strdup(filename_or_delim);
	redir->next = NULL;
	return (redir);
}

void	add_redirection(t_cmd *cmd, t_redirection *redir)
{
	t_redirection	*current;

	if (!cmd || !redir)
		return ;
	if (!cmd->redirections)
	{
		cmd->redirections = redir;
		return ;
	}
	current = cmd->redirections;
	while (current->next)
		current = current->next;
	current->next = redir;
}

int	handle_redirections(t_cmd *cmd, t_token *next, t_token_type type)
{
	t_redirection	*redir;
	t_redir_type	redir_type;

	if (type == TOKEN_REDIR_IN)
		redir_type = REDIR_IN;
	else if (type == TOKEN_REDIR_OUT)
		redir_type = REDIR_OUT;
	else if (type == TOKEN_APPEND)
		redir_type = REDIR_APPEND;
	else
		return (0);

	redir = create_redirection(redir_type, next->value);
	if (!redir)
		return (0);
	add_redirection(cmd, redir);
	return (1);
}


int	handle_heredoc_redirection(t_cmd *cmd, t_token *next, t_expand_context *ctx)
{
	t_redirection	*redir;
	int				fd;
	int				saved_stdin;

	// Create redirection entry for heredoc
	redir = create_redirection(REDIR_HEREDOC, next->value);
	if (!redir)
		return (0);
	add_redirection(cmd, redir);

	// Handle the heredoc immediately (your existing logic)
	saved_stdin = dup(STDIN_FILENO);
	fd = handle_heredoc(next->value, ctx);
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
	return (1);
}

void	convert_redirections_for_execution(t_cmd *cmd)
{
	t_redirection	*redir;
	t_redirection	*last_input;
	t_redirection	*last_output;

	last_input = NULL;
	last_output = NULL;
	
	// Find the last input and output redirections
	redir = cmd->redirections;
	while (redir)
	{
		if (redir->type == REDIR_IN || redir->type == REDIR_HEREDOC)
			last_input = redir;
		else if (redir->type == REDIR_OUT || redir->type == REDIR_APPEND)
			last_output = redir;
		redir = redir->next;
	}
	
	// Convert to old format for execution
	if (last_input)
	{
		if (last_input->type == REDIR_IN)
		{
			cmd->input_file = ft_strdup(last_input->filename_or_delim);
			cmd->heredoc_file = NULL;
		}
		else if (last_input->type == REDIR_HEREDOC)
		{
			cmd->input_file = NULL;
			cmd->heredoc_file = ft_strdup(last_input->filename_or_delim);
		}
	}
	
	if (last_output)
	{
		cmd->output_file = ft_strdup(last_output->filename_or_delim);
		cmd->append_output = (last_output->type == REDIR_APPEND);
	}
}

int	handle_heredoc(const char *delimiter, t_expand_context *ctx)
{
	char				*clean_delimiter;
	int					fds[2];
	pid_t				pid;
	int					quoted;

	if (heredoc_setup(delimiter, &clean_delimiter, &quoted) == -1)
		return (-1);
	pid = heredoc_pipe_and_fork(fds, clean_delimiter);
	if (pid == -1)
		return (-1);
	if (pid == 0)
		heredoc_child(fds, clean_delimiter, quoted, ctx);
	return (heredoc_parent(pid, fds, clean_delimiter));
}

int	is_quoted(const char *str)
{
	int	len;

	if (!str)
		return (0);
	len = strlen(str);
	return (len >= 2 && ((str[0] == '"' && str[len - 1] == '"')
			|| (str[0] == '\'' && str[len - 1] == '\'')));
}