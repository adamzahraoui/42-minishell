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

int	handle_heredoc_redirection(t_cmd *cmd, t_token *next, t_expand_context *ctx)
{
	int	fd;
	int	saved_stdin;

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
