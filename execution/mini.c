/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akira <akira@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 18:52:49 by adzahrao          #+#    #+#             */
/*   Updated: 2025/08/10 23:55:39 by akira            ###   ########.fr       */
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

void	ft_ft_free(char **str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		free(str[i]);
		i++;
	}
	free(str);
}

void	declare_env(t_myenv **myenv, t_myenv_ex **myenv_ex, char **env)
{
	set_env(myenv, env);
	set_env_ex(myenv_ex, env);
}

void	cmd_ex(t_cmd **args, t_token **tokens, char **env, t_myenv **myenv,
		t_myenv_ex **myenv_ex)
{
	t_cmd	*cmd;
	char	**path;

	(void)tokens;
	path = my_get_path_split(myenv, "PATH=", ':');
	cmd = *args;
	cmd->saved_stdin = -1;
	cmd->saved_stdout = -1;
	
	if (cmd)
	{
			if (!cmd->next && check_builtin_cmd(args, *myenv, *myenv_ex) == 1)
			{
				*args = NULL;
				return ;
			}
			else
				ft_pipe(args, path, myenv, myenv_ex, env);
	}
	else if (cmd->redirections)
	{
		if (redirection(cmd) == 0)
		{
			restor_fd(cmd);
		}
		else
		{
			set_status(myenv, NULL, 1);
		}
	}
	*args = NULL;
}
