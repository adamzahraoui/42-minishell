/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   12_parsing_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlaidi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 13:02:30 by mlaidi            #+#    #+#             */
/*   Updated: 2025/08/12 13:02:38 by mlaidi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

int	create_and_link_token(t_token *token, char *str, int split_pos)
{
	t_token	*new_token;

	new_token = malloc(sizeof(t_token));
	if (!new_token)
		return (0);
	new_token->value = ft_strdup(str + split_pos);
	new_token->type = TOKEN_WORD;
	new_token->next = token->next;
	token->value[split_pos] = '\0';
	token->next = new_token;
	return (1);
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

char	*expand_token(char *token, t_var *vars, char **env)
{
	t_expand_context	ctx;
	t_token_state		st;
	char				*result;

	ctx.vars = vars;
	ctx.env = env;
	memset(&st, 0, sizeof(st));
	if (!token)
		return (NULL);
	result = malloc(MAX_TOKEN_LEN);
	if (!result)
		return (NULL);
	result[0] = '\0';
	while (token[st.i] && st.j < MAX_TOKEN_LEN - 1)
		process_token_character(token, &st, result, &ctx);
	result[st.j] = '\0';
	return (result);
}
