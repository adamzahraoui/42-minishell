/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_error.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlaidi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 17:02:48 by mlaidi            #+#    #+#             */
/*   Updated: 2025/08/16 17:02:51 by mlaidi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	validate_syntax(t_token *tokens)
{
	t_token	*current;

	current = tokens;
	if (current && current->type == TOKEN_PIPE)
	{
		ft_putendl_fd("bash: syntax error near unexpected token `|'", 2);
		return (0);
	}
	while (current)
	{
		if (!validate_token_specific(current))
			return (0);
		current = current->next;
	}
	return (1);
}

int	validate_token_specific(t_token *current)
{
	t_token	*next;

	if (is_redirection(current->type))
	{
		next = current->next;
		if (!validate_redirection_next_token(next))
			return (0);
	}
	else if (current->type == TOKEN_PIPE)
	{
		if (!validate_pipe_token(current))
			return (0);
	}
	return (1);
}

int	validate_redirection_next_token(t_token *next)
{
	if (!next)
	{
		ft_putendl_fd("bash: syntax error near unexpected token `newline'", 2);
		return (0);
	}
	if (next->type == TOKEN_PIPE)
	{
		ft_putendl_fd("bash: syntax error near unexpected token `|'", 2);
		return (0);
	}
	if (is_redirection(next->type))
	{
		ft_putstr_fd("bash: syntax error near unexpected token `", 2);
		ft_putstr_fd(next->value, 2);
		ft_putendl_fd("'", 2);
		return (0);
	}
	return (1);
}

int	validate_pipe_token(t_token *current)
{
	t_token	*next;

	next = current->next;
	if (!next)
	{
		ft_putendl_fd("bash: syntax error near unexpected token `newline'", 2);
		return (0);
	}
	if (next->type == TOKEN_PIPE)
	{
		ft_putendl_fd("bash: syntax error near unexpected token `|'", 2);
		return (0);
	}
	return (1);
}
