/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   08_command_management.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlaidi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 00:08:10 by mlaidi            #+#    #+#             */
/*   Updated: 2025/08/02 00:08:11 by mlaidi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_cmd	*init_command(void)
{
	t_cmd	*cmd;
	int		i;

	cmd = (t_cmd *)malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	if (!initialize_command_fields(cmd))
		return (free(cmd), NULL);
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
	cmd->input_file = NULL;
	cmd->output_file = NULL;
	cmd->append_output = 0;
	cmd->heredoc_delim = NULL;
	cmd->heredoc_file = NULL;
	cmd->saved_stdin = -1;
	cmd->next = NULL;
	cmd->args = (char **)malloc(sizeof(char *) * cmd->arg_capacity);
	if (!cmd->args)
		return (0);
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
		new_args = (char **)malloc(sizeof(char *) * cmd->arg_capacity);
		if (!new_args)
			return ;
		i = 0;
		while (i < cmd->arg_count)
		{
			new_args[i] = cmd->args[i];
			i++;
		}
		while (i < cmd->arg_capacity)
			new_args[i++] = NULL;
		free(cmd->args);
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
			free(tmp->args[i++]);
		free(tmp->args);
		if (tmp->input_file)
			free(tmp->input_file);
		if (tmp->output_file)
			free(tmp->output_file);
		if (tmp->heredoc_delim)
			free(tmp->heredoc_delim);
		if (tmp->heredoc_file)
		{
			unlink(tmp->heredoc_file);
			free(tmp->heredoc_file);
		}
		free(tmp);
	}
}

void	free_tokens(t_token *tokens)
{
	t_token	*tmp;

	while (tokens)
	{
		tmp = tokens;
		tokens = tokens->next;
		free(tmp->value);
		free(tmp);
	}
}
