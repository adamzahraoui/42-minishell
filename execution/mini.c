/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akira <akira@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 18:52:49 by adzahrao          #+#    #+#             */
/*   Updated: 2025/08/11 23:49:39 by akira            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	add_env_node(t_myenv **myenv, char *env)
{
	t_myenv	*new_node;
	t_myenv	*last;

	new_node = malloc(sizeof(t_myenv));
	if (!new_node)
		return ;
	new_node->data = ft_strdup(env);
	if (!new_node->data)
		return ;
	new_node->next = NULL;
	if (*myenv == NULL)
	{
		*myenv = new_node;
		return ;
	}
	last = *myenv;
	while (last->next)
		last = last->next;
	last->next = new_node;
}

void	set_env(t_myenv **myenv, char **env)
{
	int	y;

	y = 0;
	while (env[y])
	{
		add_env_node(myenv, env[y]);
		y++;
	}
}

void	declare_env(t_myenv **myenv, t_myenv_ex **myenv_ex, char **env)
{
	set_env(myenv, env);
	set_env_ex(myenv_ex, env);
}

void	cmd_ex(t_cmd **args, char **env, t_myenv **myenv, t_myenv_ex **myenv_ex)
{
	t_cmd	*cmd;

	cmd = *args;
	cmd->saved_stdin = -1;
	cmd->saved_stdout = -1;
	if (cmd)
	{
		if (!cmd->next && check_builtin_cmd(args, *myenv, *myenv_ex) == 1)
		{
			free_commands(*args);
			*args = NULL;
			return ;
		}
		else
		{
			ft_pipe(args, myenv, myenv_ex, env);
			free_commands(*args);
		}
	}
	else if (cmd->redirections)
	{
		if (redirection(cmd) == 0)
			restor_fd(cmd);
		else
			set_status(myenv, NULL, 1);
	}
	ft_free_all();
	*args = NULL;
}
