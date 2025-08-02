/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   10_heredoc_processing.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlaidi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 00:08:56 by mlaidi            #+#    #+#             */
/*   Updated: 2025/08/02 00:08:58 by mlaidi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	heredoc_setup(const char *delimiter, char **clean_delimiter, int *quoted)
{
	if (!delimiter)
		return (-1);
	*clean_delimiter = remove_quotes(delimiter);
	if (!*clean_delimiter)
		return (-1);
	*quoted = is_quoted(delimiter);
	return (0);
}

int	heredoc_pipe_and_fork(int *fds, char *clean_delimiter)
{
	pid_t	pid;

	if (pipe(fds) == -1)
	{
		perror("pipe");
		free(clean_delimiter);
		return (-1);
	}
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		close(fds[0]);
		close(fds[1]);
		free(clean_delimiter);
		return (-1);
	}
	return (pid);
}

void	heredoc_child(int *fds, char *clean_delimiter,
	int quoted, t_expand_context *ctx)
{
	int	lineno;
	int	got_delim;

	lineno = 1;
	got_delim = 0;
	signal(SIGINT, SIG_DFL);
	close(fds[0]);
	got_delim = heredoc_child_loop(clean_delimiter, quoted, fds[1], ctx,
			&lineno);
	if (!got_delim)
	{
		ft_putstr_fd("bash: warning: here-document at line ", 2);
		ft_putnbr_fd(lineno, 2);
		ft_putstr_fd(" delimited by end-of-file (wanted `", 2);
		ft_putstr_fd(clean_delimiter, 2);
		ft_putstr_fd("')\n", 2);
	}
	close(fds[1]);
	free(clean_delimiter);
	exit(0);
}

int	heredoc_parent(pid_t pid, int *fds, char *clean_delimiter)
{
	int		status;
	void	(*old_handler)(int);

	close(fds[1]);
	free(clean_delimiter);
	old_handler = signal(SIGINT, SIG_IGN);
	waitpid(pid, &status, 0);
	signal(SIGINT, old_handler);
	if (status != 0)
	{
		close(fds[0]);
		return (-1);
	}
	return (fds[0]);
}

int	heredoc_child_loop(const char *clean_delimiter, int quoted, int write_fd,
		t_expand_context *ctx, int *lineno)
{
	int	got_delim;

	got_delim = 0;
	while (1)
	{
		if (process_heredoc_line(clean_delimiter, quoted, write_fd, ctx,
				&got_delim) == -1)
			break ;
		(*lineno)++;
	}
	return (got_delim);
}
