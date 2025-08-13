/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pipe.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akira <akira@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 00:49:06 by adzahrao          #+#    #+#             */
/*   Updated: 2025/08/13 00:57:30 by akira            ###   ########.fr       */
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
		ft_free_error(NULL, &my_env, &env_ex, 1);
	if (check_builtin_cmd(&cmd, &my_env, &env_ex))
		ft_free_error(NULL, &my_env, &env_ex, 0);
	else
		exec_extranal_cmd(cmd, path, or_env);
}

void	ft_pipe(t_cmd **cmd, t_myenv **myenv, t_myenv_ex **env_ex,
		char **or_env)
{
	t_cmd	*arg;
	t_myenv	*env;
	t_pipe	pipe_data;

	(1) && (arg = *cmd, env = *myenv);
	init_pipe_data(*cmd, *myenv, &pipe_data);
	pipe_data.pids = malloc(sizeof(int) * pipe_data.count);
	if (!pipe_data.pids)
		return ;
	while (arg)
	{
		if (create_pipe_and_fork(arg, env, &pipe_data))
		{
			free(pipe_data.pids);
			return ;
		}
		if (pipe_data.pids[pipe_data.i] == 0)
		{
			signal(SIGINT, SIG_DFL);
			signal(SIGQUIT, SIG_DFL);
			ft_pipe_one(&pipe_data, arg, env);
			exec_all(arg, *myenv, *env_ex, or_env);
		}
		ft_pipe_two(&pipe_data, &arg, env);
	}
	wait_pid(&pipe_data, myenv);
	free(pipe_data.pids);
}