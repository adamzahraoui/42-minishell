/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   04_expansion_core.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akira <akira@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 00:06:43 by mlaidi            #+#    #+#             */
/*   Updated: 2025/08/14 16:27:39 by akira            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	expand_all_tokens(t_token **tokens, t_expand_context *ctx)
{
	t_token_expansion_params	params;
	t_token						*tok;
	t_token						*prev;
	t_token						*next;

	tok = *tokens;
	prev = NULL;
	while (tok)
	{
		next = tok->next;
		if (tok->type == TOKEN_WORD && tok->value && !(prev
				&& prev->type == TOKEN_HEREDOC))
		{
			params.tokens = tokens;
			params.tok = &tok;
			params.prev = &prev;
			params.next = next;
			if (process_token_expansion(&params, ctx))
				continue ;
		}
		else
			prev = tok;
		tok = next;
	}
}

int	process_token_expansion(t_token_expansion_params *params,
		t_expand_context *ctx)
{
	t_failed_expansion_params	fail_params;
	char						*expanded;
	int							was_quoted;
	int							has_variables;
	t_was						was;

	expanded = NULL;
	fail_params.tokens = params->tokens;
	fail_params.tok = *params->tok;
	fail_params.prev = *params->prev;
	fail_params.next = params->next;
	was_quoted = ((*params->tok)->value[0] == '"'
			|| (*params->tok)->value[0] == '\'');
	has_variables = (ft_strchr((*params->tok)->value, '$') != NULL);
	was.has_variables = has_variables;
	was.was_quoted = was_quoted;
	expanded = expand_token((*params->tok)->value, ctx->vars, ctx->env);
	fail_params.expanded = expanded;
	return (handle_expansion_result(params, &fail_params, expanded, was));
}

int	handle_expansion_result(t_token_expansion_params *params,
		t_failed_expansion_params *fail_params, char *expanded, t_was was)
{
	if (expanded && (*expanded || was.was_quoted))
	{
		*params->tok = handle_successful_expansion(*params->tok, expanded,
				params->next, was);
		*params->prev = *params->tok;
		return (0);
	}
	else
	{
		*params->tok = handle_failed_expansion(fail_params);
		return (1);
	}
}

t_token	*handle_successful_expansion(t_token *tok, char *expanded,
		t_token *next, t_was was)
{
	ft_free_one(tok->value);
	tok->value = expanded;
	if (was.has_variables && !was.was_quoted)
		split_token_string(&tok);
	trim_token_values(tok, next, was.was_quoted);
	while (tok->next && tok->next != next)
		tok = tok->next;
	return (tok);
}

t_token	*handle_failed_expansion(t_failed_expansion_params *params)
{
	if (params->prev)
		params->prev->next = params->next;
	else
		*params->tokens = params->next;
	ft_free_one(params->tok->value);
	if (params->expanded)
		ft_free_one(params->expanded);
	ft_free_one(params->tok);
	return (params->next);
}
