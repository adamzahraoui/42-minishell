/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_e.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akira <akira@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 19:26:17 by akira             #+#    #+#             */
/*   Updated: 2025/08/11 21:23:30 by akira            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_pipe_one(t_pipe *pipe_data, t_cmd *arg, t_myenv *env)
{
	if (pipe_data->prev_fd != -1)
	{
		dup2(pipe_data->prev_fd, STDIN_FILENO);
		close(pipe_data->prev_fd);
	}
	if (arg->next)
	{
		dup2(env->fd[1], STDOUT_FILENO);
		close(env->fd[0]);
		close(env->fd[1]);
	}
}

void	ft_pipe_two(t_pipe *pipe_data, t_cmd **arg_ptr, t_myenv *env)
{
	if (pipe_data->prev_fd != -1)
		close(pipe_data->prev_fd);
	if ((*arg_ptr)->next)
	{
		close(env->fd[1]);
		pipe_data->prev_fd = env->fd[0];
	}
	else
		pipe_data->prev_fd = -1;
	*arg_ptr = (*arg_ptr)->next;
	pipe_data->i++;
}

void	wait_pid(t_pipe *pipe_data, t_myenv **myenv)
{
	int	j;

	j = 0;
	signal(SIGINT, SIG_IGN);
	while (j < pipe_data->count)
	{
		waitpid(pipe_data->pids[j], &pipe_data->status, 0);
		if (WIFEXITED(pipe_data->status))
			(*myenv)->i = WEXITSTATUS(pipe_data->status);
		if (j == pipe_data->count - 1)
		{
			if (WIFEXITED(pipe_data->status))
				set_status(WEXITSTATUS(pipe_data->status));
			else if (WIFSIGNALED(pipe_data->status))
			{
				if (WTERMSIG(pipe_data->status) == SIGINT)
					write(2, "\n", 1);
				else if (WTERMSIG(pipe_data->status) == SIGQUIT)
					write(2, "Quit (core dumped)\n", 20);
				setup_signals();
				set_status(128 + WTERMSIG(pipe_data->status));
				return ;
			}
		}
		j++;
	}
	setup_signals();
}

void	init_pipe_data(t_cmd *arg, t_myenv *env, t_pipe *pipe_data)
{
	(void)env;
	pipe_data->prev_fd = -1;
	pipe_data->i = 0;
	pipe_data->status = 0;
	pipe_data->count = 0;
	pipe_data->pids = NULL;
	while (arg)
	{
		pipe_data->count++;
		arg = arg->next;
	}
}

int	create_pipe_and_fork(t_cmd *arg, t_myenv *env, t_pipe *pipe_data)
{
	if (arg->next && pipe(env->fd) == -1)
	{
		perror("pipe");
		return (1);
	}
	pipe_data->pids[pipe_data->i] = fork();
	if (pipe_data->pids[pipe_data->i] == -1)
	{
		perror("fork");
		return (1);
	}
	return (0);
}
