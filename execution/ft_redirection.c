/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_redirection.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adzahrao <adzahrao@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 16:51:38 by adzahrao          #+#    #+#             */
/*   Updated: 2025/08/16 16:51:39 by adzahrao         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	save_fd(t_cmd *cmd)
{
	cmd->saved_stdin = dup(0);
	if (cmd->saved_stdin == -1)
	{
		cmd->saved_stdout = -1;
		return ;
	}
	cmd->saved_stdout = dup(1);
	if (cmd->saved_stdout == -1)
	{
		close(cmd->saved_stdin);
		cmd->saved_stdin = -1;
		return ;
	}
}

void	restor_fd(t_cmd *cmd)
{
	t_redirection	*current;

	current = cmd->redirections;
	while (current)
	{
		if (current->fd != -1)
		{
			close(current->fd);
			current->fd = -1;
		}
		current = current->next;
	}
	if (cmd->saved_stdin != -1)
	{
		if (check_dup_red(cmd, 1) == -1)
			return ;
	}
	if (cmd->saved_stdout != -1)
	{
		if (check_dup_red(cmd, 2) == -1)
			return ;
	}
}

int	open_files(t_redirection *redirection)
{
	int		fd;
	char	*msg;

	fd = 0;
	if (redirection->type == REDIR_IN)
		fd = open(redirection->filename_or_delim, O_RDONLY);
	if (redirection->type == REDIR_HEREDOC)
		fd = redirection->fd;
	if (redirection->type == REDIR_OUT)
		fd = open(redirection->filename_or_delim, O_CREAT | O_TRUNC | O_WRONLY,
				0644);
	if (redirection->type == REDIR_APPEND)
		fd = open(redirection->filename_or_delim, O_CREAT | O_WRONLY | O_APPEND,
				0644);
	if (fd == -1)
	{
		msg = ft_strjoin(": ", strerror(errno));
		msg = ft_strjoin(msg, "\n");
		error(redirection->filename_or_delim, msg, 1);
	}
	return (fd);
}

int	append_fd(t_redirection *redirection, int fd)
{
	int	ret;

	ret = 0;
	if (redirection->type == REDIR_IN)
		ret = dup2(fd, 0);
	else if (redirection->type == REDIR_HEREDOC)
		ret = dup2(fd, 0);
	else if (redirection->type == REDIR_OUT
		|| redirection->type == REDIR_APPEND)
		ret = dup2(fd, 1);
	if (ret == -1)
		return (-1);
	return (0);
}

int	redirection(t_cmd *cmd)
{
	t_redirection	*current;
	int				fd;

	save_fd(cmd);
	current = cmd->redirections;
	while (current)
	{
		fd = open_files(current);
		if (fd == -1)
		{
			close(fd);
			restor_fd(cmd);
			return (1);
		}
		if (append_fd(current, fd) == -1)
		{
			close(fd);
			restor_fd(cmd);
			return (1);
		}
		current = current->next;
	}
	close(fd);
	return (0);
}
