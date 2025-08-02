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
	t_token	*tok;
	t_token	*prev;
	t_token	*next;

	tok = *tokens;
	prev = NULL;
	while (tok)
	{
		next = tok->next;
		if (tok->type == TOKEN_WORD && tok->value
			&& !(prev && prev->type == TOKEN_HEREDOC))
		{
			if (process_token_expansion(tokens, &tok, &prev, next, ctx))
				continue ;
		}
		else
			prev = tok;
		tok = next;
	}
}

int	process_token_expansion(t_token **tokens, t_token **tok, t_token **prev,
		t_token *next, t_expand_context *ctx)
{
	char	*expanded;
	int		was_quoted;

	was_quoted = ((*tok)->value[0] == '"' || (*tok)->value[0] == '\'');
	expanded = expand_token((*tok)->value, ctx->vars, ctx->env);
	if (expanded && (*expanded || was_quoted))
	{
		*tok = handle_successful_expansion(*tok, expanded, next, was_quoted);
		*prev = *tok;
		return (0);
	}
	else
	{
		*tok = handle_failed_expansion(tokens, *tok, *prev, next, expanded);
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

t_token	*handle_failed_expansion(t_token **tokens, t_token *tok, t_token *prev,
		t_token *next, char *expanded)
{
	if (prev)
		prev->next = next;
	else
		*tokens = next;
	free(tok->value);
	free(expanded);
	free(tok);
	return (next);
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
