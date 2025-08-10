/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   external_executables.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akira <akira@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 05:10:57 by adzahrao          #+#    #+#             */
/*   Updated: 2025/08/10 14:52:54 by akira            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	printf_error(char *msg, int exit_status)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(msg, 2);
	ft_putstr_fd(": command not found\n", 2);
	exit(exit_status);
}

void	pid_zero(t_cmd **cmd)
{
	char	*exec_path;

	exec_path = (*cmd)->args[0];
	if (access(exec_path, F_OK) == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd((*cmd)->args[0], 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		exit(127);
	}
	if (access(exec_path, X_OK) == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd((*cmd)->args[0], 2);
		ft_putstr_fd(": Permission denied\n", 2);
		exit(126);
	}
}

void	pid_plus_zero(pid_t pid, void (*old_sigint)(int),
		void (*old_sigquit)(int), t_myenv **myenv)
{
	int	status;

	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		set_status(myenv, NULL, WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
		set_status(myenv, NULL, 128 + WTERMSIG(status));
	signal(SIGINT, old_sigint);
	signal(SIGQUIT, old_sigquit);
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
		write(1, "\n", 1);
}

void	pid_is_zero(t_cmd **cmd, char **path, char **envp)
{
	char	*exec_path;

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (ft_strchr((*cmd)->args[0], '/'))
		pid_zero(cmd);
	else
	{
		exec_path = check_cmd(path, (*cmd)->args[0]);
		if (!exec_path)
			printf_error((*cmd)->args[0], 127);
	}
	execve(exec_path, (*cmd)->args, envp);
	if (errno == EACCES)
		printf_error((*cmd)->args[0], 126);
	ft_putstr_fd("minishell: execve error\n", 2);
	exit(EXIT_FAILURE);
}

void	external_executables(t_cmd **cmd, char **path, char **envp,
		t_myenv **myenv)
{
	pid_t	pid;
	void	(*old_sigint)(int);
	void	(*old_sigquit)(int);

	old_sigint = signal(SIGINT, SIG_IGN);
	old_sigquit = signal(SIGQUIT, SIG_IGN);
	if (!(*cmd)->args[0] || ft_strcmp((*cmd)->args[0], "echo") == 0)
		return ;
	pid = fork();
	if (pid == 0)
		pid_is_zero(cmd, path, envp);
	else if (pid > 0)
		pid_plus_zero(pid, old_sigint, old_sigquit, myenv);
	else
	{
		perror("fork");
		signal(SIGINT, old_sigint);
		signal(SIGQUIT, old_sigquit);
	}
}
