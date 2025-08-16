/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   08_command_management.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlaidi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 17:01:47 by mlaidi            #+#    #+#             */
/*   Updated: 2025/08/16 17:01:49 by mlaidi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_cmd	*init_command(void)
{
	t_cmd	*cmd;
	int		i;

	cmd = (t_cmd *)ft_malloc(sizeof(t_cmd));
	if (!initialize_command_fields(cmd))
		return (NULL);
	i = 0;
	while (i < cmd->arg_capacity)
	{
		cmd->args[i] = NULL;
		i++;
	}
	return (cmd);
}

int	initialize_command_fields(t_cmd *cmd)
{
	cmd->args = NULL;
	cmd->arg_count = 0;
	cmd->arg_capacity = 10;
	cmd->redirections = NULL;
	cmd->input_file = NULL;
	cmd->output_file = NULL;
	cmd->append_output = 0;
	cmd->heredoc_delim = NULL;
	cmd->heredoc_file = NULL;
	cmd->saved_stdin = -1;
	cmd->saved_stdout = -1;
	cmd->next = NULL;
	cmd->args = (char **)ft_malloc(sizeof(char *) * cmd->arg_capacity);
	return (1);
}

void	add_argument(t_cmd *cmd, char *arg)
{
	char	**new_args;
	int		i;

	if (!arg)
		return ;
	if (cmd->arg_count >= cmd->arg_capacity)
	{
		cmd->arg_capacity *= 2;
		new_args = (char **)ft_malloc(sizeof(char *) * cmd->arg_capacity);
		i = 0;
		while (i < cmd->arg_count)
		{
			new_args[i] = cmd->args[i];
			i++;
		}
		while (i < cmd->arg_capacity)
			new_args[i++] = NULL;
		ft_free_one(cmd->args);
		cmd->args = new_args;
	}
	cmd->args[cmd->arg_count++] = arg;
}

void	free_commands(t_cmd *cmds)
{
	t_cmd	*tmp;
	int		i;

	while (cmds)
	{
		tmp = cmds;
		cmds = cmds->next;
		i = 0;
		while (i < tmp->arg_count)
			ft_free_one(tmp->args[i++]);
		ft_free_one(tmp->args);
		free_cmd_files(tmp);
		ft_free_one(tmp);
	}
}

void	free_cmd_files(t_cmd *tmp)
{
	t_redirection	*redir_tmp;

	while (tmp->redirections)
	{
		redir_tmp = tmp->redirections;
		tmp->redirections = tmp->redirections->next;
		ft_free_one(redir_tmp->filename_or_delim);
		ft_free_one(redir_tmp);
	}
	if (tmp->input_file)
		ft_free_one(tmp->input_file);
	if (tmp->output_file)
		ft_free_one(tmp->output_file);
	if (tmp->heredoc_delim)
		ft_free_one(tmp->heredoc_delim);
	if (tmp->heredoc_file)
	{
		unlink(tmp->heredoc_file);
		ft_free_one(tmp->heredoc_file);
	}
}
