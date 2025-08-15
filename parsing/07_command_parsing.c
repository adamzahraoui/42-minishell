/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   07_command_parsing.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akira <akira@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 00:07:59 by mlaidi            #+#    #+#             */
/*   Updated: 2025/08/14 16:54:43 by akira            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	process_commands(t_token **tokens, t_var *vars, char **env, t_cmd **cmd)
{
	t_cmd	*cur;
	
	*cmd = parse_commands(tokens, vars, env);
	if (*cmd)
	{
		cur = *cmd;
		while (cur)
		{
			if ((cur->arg_count == 0 && !cur->redirections) || (cur->args && cur->args[0]
					&& cur->args[0][0] == '\0' && !cur->redirections))
			{
				printf("minishell: : command not found\n");
				free_commands(*cmd);
				*cmd = NULL;
				return ;
			}
			cur = cur->next;
		}
	}
}

t_cmd	*parse_commands(t_token **tokens, t_var *vars, char **env)
{
	t_cmd	*head;
	t_cmd	*current;
	t_token	*token_ptr;

	head = NULL;
	current = NULL;
	token_ptr = *tokens;
	while (token_ptr)
	{
		if (token_ptr && token_ptr->type == TOKEN_PIPE)
			token_ptr = token_ptr->next;
		if (!head)
		{
			head = parse_command(&token_ptr, vars, env);
			current = head;
		}
		else
		{
			current->next = parse_command(&token_ptr, vars, env);
			current = current->next;
		}
	}
	return (head);
}

t_cmd	*parse_command(t_token **tokens, t_var *vars, char **env)
{
	t_expand_context	ctx;
	t_token				*token_ptr;
	t_cmd				*cmd;

	ctx.vars = vars;
	ctx.env = env;
	(void)env;
	cmd = init_command();
	if (!cmd)
		return (NULL);
	token_ptr = *tokens;
	while (token_ptr && token_ptr->type != TOKEN_PIPE)
	{
		if (is_redirection(token_ptr->type))
		{
			if (!handle_redirection(cmd, &token_ptr, tokens, &ctx))
				return (NULL);
			continue ;
		}
		add_argument(cmd, ft_strdup_gc(token_ptr->value));
		token_ptr = token_ptr->next;
	}
	*tokens = token_ptr;
	return (cmd);
}

int	handle_redirection(t_cmd *cmd, t_token **token_ptr, t_token **tokens,
		t_expand_context *ctx)
{
	t_token_type	type;
	t_token			*next;

	type = (*token_ptr)->type;
	next = (*token_ptr)->next;
	if (!next || next->type != TOKEN_WORD)
	{
		ft_putendl_fd("Error: Invalid redirection", 2);
		free_commands(cmd);
		*tokens = NULL;
		return (0);
	}
	if (!dispatch_redirection(cmd, next, type, ctx))
	{
		free_commands(cmd);
		*tokens = NULL;
		return (0);
	}
	*token_ptr = next->next;
	return (1);
}

void	free_env_array(char **env)
{
	int	i;

	i = 0;
	if (!env)
		return ;
	while (env[i])
	{
		ft_free_one(env[i]);
		i++;
	}
	ft_free_one(env);
}
