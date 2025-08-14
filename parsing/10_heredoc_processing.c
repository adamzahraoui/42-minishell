/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   10_heredoc_processing.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akira <akira@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 00:08:56 by mlaidi            #+#    #+#             */
/*   Updated: 2025/08/14 17:37:10 by akira            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	handle_heredoc(const char *delimiter, t_expand_context *ctx)
{
	char	*clean_delimiter;
	int		fds[2];
	pid_t	pid;

	if (heredoc_setup(delimiter, &clean_delimiter) == -1)
		return (-1);
	pid = heredoc_pipe_and_fork(fds, clean_delimiter);
	if (pid == -1)
		return (-1);
	if (pid == 0)
		heredoc_child(fds, clean_delimiter, ctx);
	return (heredoc_parent(pid, fds, clean_delimiter));
}

int	heredoc_setup(const char *delimiter, char **clean_delimiter)
{
	if (!delimiter)
		return (-1);
	*clean_delimiter = remove_all_quotes(delimiter);
	if (!*clean_delimiter)
		return (-1);
	return (0);
}

int	heredoc_pipe_and_fork(int *fds, char *clean_delimiter)
{
	pid_t	pid;

	if (pipe(fds) == -1)
	{
		ft_free_one(clean_delimiter);
		return (-1);
	}
	pid = fork();
	if (pid == -1)
	{
		close(fds[0]);
		close(fds[1]);
		ft_free_one(clean_delimiter);
		return (-1);
	}
	return (pid);
}

void	heredoc_child(int *fds, char *clean_delimiter, t_expand_context *ctx)
{
	t_heredoc_loop_params	loop_params;
	int						lineno;
	int						got_delim;

	lineno = 1;
	got_delim = 0;
	signal(SIGINT, handle_heredoc_sigint);
	close(fds[0]);
	loop_params.clean_delimiter = clean_delimiter;
	loop_params.write_fd = fds[1];
	loop_params.lineno = &lineno;
	got_delim = heredoc_child_loop(&loop_params, ctx);
	if (!got_delim)
	{
		ft_putstr_fd("bash: warning: here-document at line ", 2);
		ft_putnbr_fd(lineno, 2);
		ft_putstr_fd(" delimited by end-of-file (wanted `", 2);
		ft_putstr_fd(clean_delimiter, 2);
		ft_putstr_fd("')\n", 2);
	}
	close(fds[1]);
	ft_free_all(0);
}

int	heredoc_parent(pid_t pid, int *fds, char *clean_delimiter)
{
	int		status;
	void	(*old_handler)(int);

	close(fds[1]);
	ft_free_one(clean_delimiter);
	old_handler = signal(SIGINT, SIG_IGN);
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		set_status(WEXITSTATUS(status));
	signal(SIGINT, old_handler);
	if (status != 0)
	{
		close(fds[0]);
		return (-1);
	}
	return (fds[0]);
}
