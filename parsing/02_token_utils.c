/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   02_token_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akira <akira@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 00:05:58 by mlaidi            #+#    #+#             */
/*   Updated: 2025/08/14 16:24:06 by akira            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	add_token_to_list(t_token **head, t_token **current, char *token_value)
{
	t_token	*new;

	new = new_token(token_value);
	if (!*head)
	{
		*head = new;
		*current = new;
	}
	else
	{
		(*current)->next = new;
		*current = (*current)->next;
	}
	return (1);
}

t_token	*new_token(char *value)
{
	t_token	*token;

	token = (t_token *)ft_malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->value = value;
	token->type = determine_token_type(value);
	token->next = NULL;
	return (token);
}

t_token_type	determine_token_type(char *value)
{
	if (!value)
		return (TOKEN_ERROR);
	if (value[0] == '|' && !value[1])
		return (TOKEN_PIPE);
	if (value[0] == '<' && !value[1])
		return (TOKEN_REDIR_IN);
	if (value[0] == '>' && !value[1])
		return (TOKEN_REDIR_OUT);
	if (value[0] == '<' && value[1] == '<' && !value[2])
		return (TOKEN_HEREDOC);
	if (value[0] == '>' && value[1] == '>' && !value[2])
		return (TOKEN_APPEND);
	return (TOKEN_WORD);
}

int	is_whitespace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f'
		|| c == '\r');
}

int	is_delimiter(char c)
{
	return (c == ' ' || c == '\t' || c == '|' || c == '<' || c == '>');
}
