/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pipe.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adzahrao <adzahrao@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 00:49:06 by adzahrao          #+#    #+#             */
/*   Updated: 2025/08/16 16:51:27 by adzahrao         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	error(char *arg, char *error, int exit_code)
{
	char	*msg;
	char	*tmp;
	size_t	len;

	tmp = ft_strjoin("minishell: ", arg);
	msg = ft_strjoin(tmp, error);
	len = ft_strlen(msg);
	write(2, msg, len);
	return (exit_code);
}

int	exec_extranal_cmd(t_cmd *arg, char **path, char **or_env)
{
	char		*exec_path;
	struct stat	stats;

	if (ft_strchr(arg->args[0], '/'))
	{
		exec_path = arg->args[0];
		if (access(exec_path, F_OK) == -1)
			return (error(arg->args[0], ": No such file or directory\n", 127));
		stat(exec_path, &stats);
		if (S_ISDIR(stats.st_mode))
			return (error(arg->args[0], ": Is a directory\n", 126));
		if (access(exec_path, X_OK) == -1)
			return (error(arg->args[0], ": Permission denied\n", 126));
	}
	else
	{
		exec_path = check_cmd(path, arg->args[0]);
		if (!exec_path)
			return (error(arg->args[0], ": command not found\n", 127));
	}
	execve(exec_path, arg->args, or_env);
	if (errno == EACCES)
		return (error(arg->args[0], ": Permission denied\n", 126));
	return (0);
}

void	exec_all(t_cmd *cmd, t_myenv *my_env, char **or_env,
		t_myenv_ex **env_ex)
{
	char	**path;
	int		exit_code;

	path = my_get_path_split(&my_env, "PATH=", ':');
	if (cmd->redirections && redirection(cmd) == 1)
		ft_free_all(1);
	if (check_builtin_cmd(&cmd, &my_env, env_ex))
		ft_free_all(0);
	else if (!cmd->args[0])
		ft_free_all(0);
	else
	{
		exit_code = exec_extranal_cmd(cmd, path, or_env);
		if (exit_code != 0)
			ft_free_all(exit_code);
	}
}

void	ft_pipe(t_cmd **cmd, char **or_env, t_myenv **myenv,
		t_myenv_ex **myenv_ex)
{
	t_cmd	*arg;
	t_myenv	*env;
	t_pipe	pipe_data;

	(1) && (arg = *cmd, env = *myenv);
	init_pipe_data(*cmd, *myenv, &pipe_data);
	pipe_data.pids = ft_malloc(sizeof(int) * pipe_data.count);
	while (arg)
	{
		if (create_pipe_and_fork(arg, env, &pipe_data))
		{
			ft_free_one(pipe_data.pids);
			return ;
		}
		if (pipe_data.pids[pipe_data.i] == 0)
		{
			signal(SIGINT, SIG_DFL);
			signal(SIGQUIT, SIG_DFL);
			ft_pipe_one(&pipe_data, arg, env);
			exec_all(arg, *myenv, or_env, myenv_ex);
		}
		ft_pipe_two(&pipe_data, &arg, env);
	}
	wait_pid(&pipe_data, myenv);
}
