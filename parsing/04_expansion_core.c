/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   04_expansion_core.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlaidi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 00:06:43 by mlaidi            #+#    #+#             */
/*   Updated: 2025/08/02 00:18:25 by mlaidi           ###   ########.fr       */
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
		if (tok->type == TOKEN_WORD && tok->value
			&& !(prev && prev->type == TOKEN_HEREDOC))
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

	expanded = NULL;
	fail_params.tokens = params->tokens;
	fail_params.tok = *params->tok;
	fail_params.prev = *params->prev;
	fail_params.next = params->next;
	fail_params.expanded = expanded;
	was_quoted = ((*params->tok)->value[0] == '"'
			|| (*params->tok)->value[0] == '\'');
	expanded = expand_token((*params->tok)->value, ctx->vars, ctx->env);
	if (expanded && (*expanded || was_quoted))
	{
		*params->tok = handle_successful_expansion(*params->tok,
				expanded, params->next, was_quoted);
		*params->prev = *params->tok;
		return (0);
	}
	else
	{
		*params->tok = handle_failed_expansion(&fail_params);
		return (1);
	}
}

t_token	*handle_successful_expansion(t_token *tok, char *expanded,
		t_token *next, int was_quoted)
{
	free(tok->value);
	tok->value = expanded;
	split_token_string(&tok);
	trim_token_values(tok, next, was_quoted);
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
	free(params->tok->value);
	free(params->expanded);
	free(params->tok);
	return (params->next);
}

char	*expand_token(char *token, t_var *vars, char **env)
{
	t_expand_context	ctx;
	t_token_state		st;
	char				*final_result;
	char				*result;

	ctx.vars = vars;
	ctx.env = env;
	memset(&st, 0, sizeof(st));
	if (!token)
		return (NULL);
	result = malloc(MAX_TOKEN_LEN);
	if (!result)
		return (NULL);
	while (token[st.i] && st.j < MAX_TOKEN_LEN - 1)
	{
		process_token_character(token, &st, result, &ctx);
	}
	result[st.j] = '\0';
	final_result = remove_quotes(result);
	free(result);
	return (final_result);
}
