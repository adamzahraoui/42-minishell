/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pipe.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akira <akira@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 00:49:06 by adzahrao          #+#    #+#             */
/*   Updated: 2025/08/11 19:20:52 by akira            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	error(char *arg, char *error, int exit_code)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(arg, 2);
	ft_putstr_fd(error, 2);
	exit(exit_code);
}

void	exec_extranal_cmd(t_cmd *arg, char **path, char **or_env)
{
	char		*exec_path;
	struct stat	stats;

	if (ft_strchr(arg->args[0], '/'))
	{
		exec_path = arg->args[0];
		if (access(exec_path, F_OK) == -1)
			error(arg->args[0], ": No such file or directory\n", 127);
		stat(exec_path, &stats);
		if (S_ISDIR(stats.st_mode))
			error(arg->args[0], ": Is a directory\n", 126);
		if (access(exec_path, X_OK) == -1)
			error(arg->args[0], ": Permission denied\n", 126);
	}
	else
	{
		exec_path = check_cmd(path, arg->args[0]);
		if (!exec_path)
			error(arg->args[0], ": command not found\n", 127);
	}
	execve(exec_path, arg->args, or_env);
	if (errno == EACCES)
		error(arg->args[0], ": Permission denied\n", 126);
}

void	exec_all(t_cmd *cmd, t_myenv *my_env, t_myenv_ex *env_ex, char **or_env)
{
	char	**path;

	path = my_get_path_split(&my_env, "PATH=", ':');
	if (cmd->redirections && redirection(cmd) == 1)
		exit(1);
	if (check_builtin_cmd(&cmd, my_env, env_ex))
		exit(my_env->i);
	else
		exec_extranal_cmd(cmd, path, or_env);
}

void	ft_pipe(t_cmd **cmd, t_myenv **myenv, t_myenv_ex **env_ex,
		char **or_env)
{
	t_cmd	*arg;
	t_myenv	*env;
	t_pipe	pipe_data;

	arg = *cmd;
	env = *myenv;
	pipe_data.prev_fd = -1;
	pipe_data.i = 0;
	pipe_data.status = 0;
	pipe_data.count = 0;
	pipe_data.pids = NULL;
	for (t_cmd *tmp = arg; tmp; tmp = tmp->next)
		pipe_data.count++;
	pipe_data.pids = malloc(sizeof(int) * pipe_data.count);
	if (!pipe_data.pids)
		return ;
	while (arg)
	{
		if (arg->next && pipe(env->fd) == -1)
		{
			perror("pipe");
			return ;
		}
		pipe_data.pids[pipe_data.i] = fork();
		if (pipe_data.pids[pipe_data.i] == -1)
		{
			perror("fork");
			return ;
		}
		if (pipe_data.pids[pipe_data.i] == 0)
		{
			if (pipe_data.prev_fd != -1)
			{
				dup2(pipe_data.prev_fd, 0);
				close(pipe_data.prev_fd);
			}
			if (arg->next)
			{
				dup2(env->fd[1], 1);
				close(env->fd[0]);
				close(env->fd[1]);
			}
			exec_all(arg, *myenv, *env_ex, or_env);
		}
		if (pipe_data.prev_fd != -1)
			close(pipe_data.prev_fd);
		if (arg->next)
		{
			close(env->fd[1]);
			pipe_data.prev_fd = env->fd[0];
		}
		else
			pipe_data.prev_fd = -1;
		arg = arg->next;
		pipe_data.i++;
	}
	for (int j = 0; j < pipe_data.count; j++)
	{
		waitpid(pipe_data.pids[j], &pipe_data.status, 0);
		if (WIFEXITED(pipe_data.status))
			env->i = WEXITSTATUS(pipe_data.status);
		if (j == pipe_data.count - 1)
		{
			if (WIFEXITED(pipe_data.status))
				set_status(myenv, NULL, WEXITSTATUS(pipe_data.status));
			else if (WIFSIGNALED(pipe_data.status))
				set_status(myenv, NULL, 128 + WTERMSIG(pipe_data.status));
		}
	}
	free(pipe_data.pids);
}
