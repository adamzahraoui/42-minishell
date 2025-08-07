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
	redir->fd = -1;
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
	t_redirection *redir;
	int	fd;

	redir = create_redirection(REDIR_HEREDOC, next->value);
	if(!redir)
		return(0);
	fd = handle_heredoc(next->value, ctx);
	if(fd < 0)
	{
		free(redir-> filename_or_delim);
		free(redir);
		return (0);
	}
	redir->fd = fd;
	add_redirection(cmd, redir);
	return (1);
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