/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   05_expansion_processing.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlaidi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 00:06:53 by mlaidi            #+#    #+#             */
/*   Updated: 2025/08/02 00:06:54 by mlaidi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	process_token_character(char *token, t_token_state *st, char *result,
		t_expand_context *ctx)
{
	if (token[st->i] == '\'' && !st->in_double)
		st->in_single = !st->in_single;
	else if (token[st->i] == '"' && !st->in_single)
		st->in_double = !st->in_double;
	else if (token[st->i] == '$' && !st->in_single)
		st->j += expand_variable(token, &(st->i), result + st->j, ctx);
	else
		result[st->j++] = token[st->i];
	st->i++;
}

int	expand_variable(char *token, int *i, char *result, t_expand_context *ctx)
{
	t_var_extraction_params	params;
	char					var_name[256];
	int						len;
	int						process_result;

	len = 0;
	ft_memset(var_name, 0, sizeof(var_name));
	params.token = token;
	params.i = i;
	params.result = result;
	params.var_name = var_name;
	params.len = &len;
	process_result = process_variable_extraction(&params, ctx);
	if (process_result != 0)
		return (process_result);
	if (!result[0])
		result[0] = '\0';
	return (len);
}

int	process_variable_extraction(t_var_extraction_params *params,
		t_expand_context *ctx)
{
	char	*val;
	int		var_name_len;

	val = NULL;
	*params->len = 0;
	var_name_len = extract_var_name(params->token, params->i, params->var_name);
	if (var_name_len == 0)
	{
		params->result[0] = '$';
		*params->len = 1;
		return (1);
	}
	val = get_shell_var(ctx->vars, params->var_name);
	if (!val)
		val = get_env_value(ctx->env, params->var_name);
	if (val)
	{
		*params->len = ft_strlen(val);
		ft_strlcpy(params->result, val, MAX_TOKEN_LEN);
	}
	else
	{
		*params->len = 0;
		params->result[0] = '\0';
	}
	return (0);
}

void	trim_token_values(t_token *tok, t_token *next, int was_quoted)
{
	t_token	*current;
	char	*trimmed;

	current = tok;
	while (current && current != next)
	{
		if (current->value && !was_quoted)
		{
			trimmed = ft_strtrim(current->value, " \t\n\v\f\r");
			if (trimmed)
			{
				free(current->value);
				current->value = trimmed;
			}
		}
		if (current->next == next)
			break ;
		current = current->next;
	}
}

int	split_token_string(t_token **token_ptr)
{
	t_token	*token;
	int		split_pos;
	char	*str;

	token = *token_ptr;
	str = token->value;
	split_pos = 0;
	while (str[split_pos] && (ft_isalpha(str[split_pos])
			|| str[split_pos] == '_'))
		split_pos++;
	if (split_pos > 0 && str[split_pos] && (str[split_pos] == ' '
			|| str[split_pos] == '\t'))
	{
		return (create_and_link_token(token, str, split_pos));
	}
	return (0);
}
