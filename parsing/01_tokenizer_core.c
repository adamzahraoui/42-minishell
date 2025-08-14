/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   01_tokenizer_core.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akira <akira@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 00:05:42 by mlaidi            #+#    #+#             */
/*   Updated: 2025/08/14 16:25:35 by akira            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_token	*tokenize(char *line)
{
	t_token	*head;
	t_token	*current;
	int		i;
	char	*token_value;

	(1) && (head = NULL, current = NULL, i = 0);
	while (line[i])
	{
		while (line[i] && (line[i] == ' ' || line[i] == '\t'))
			i++;
		if (!line[i])
			break ;
		token_value = get_next_token(line, &i);
		if (token_value)
		{
			if (!add_token_to_list(&head, &current, token_value))
				return (NULL);
		}
	}
	return (head);
}

char	*get_next_token(char *line, int *i)
{
	char			*token;
	t_token_state	st;

	st.in_single = 0;
	st.in_double = 0;
	if (!line || !line[*i])
		return (NULL);
	token = ft_strdup_gc("");
	token = process_token_content(line, i, token, &st);
	return (token);
}

char	*process_token_content(char *line, int *i, char *token,
		t_token_state *st)
{
	char	*tmp;
	char	*special_token;

	if (!st->in_single && !st->in_double)
	{
		special_token = handle_special_tokens(line, i, token);
		if (special_token)
			return (special_token);
	}
	while (line[*i] && !is_whitespace(line[*i]))
	{
		tmp = token;
		token = process_token_part(line, i, token, st);
		if (token == tmp)
			break ;
	}
	return (token);
}

char	*handle_special_tokens(char *line, int *i, char *token)
{
	char	single_char[2];

	if (line[*i] == '<' && line[*i + 1] == '<')
	{
		ft_free_one(token);
		*i += 2;
		return (ft_strdup_gc("<<"));
	}
	if (line[*i] == '>' && line[*i + 1] == '>')
	{
		ft_free_one(token);
		*i += 2;
		return (ft_strdup_gc(">>"));
	}
	if (line[*i] == '<' || line[*i] == '>' || line[*i] == '|')
	{
		ft_free_one(token);
		single_char[0] = line[*i];
		single_char[1] = '\0';
		(*i)++;
		return (ft_strdup_gc(single_char));
	}
	return (NULL);
}

char	*process_token_part(char *line, int *i, char *token, t_token_state *st)
{
	char	*part;
	char	*tmp;

	if (line[*i] == '\'' && !st->in_double)
		st->in_single = !st->in_single;
	else if (line[*i] == '"' && !st->in_single)
		st->in_double = !st->in_double;
	if (!st->in_single && !st->in_double && (line[*i] == '<' || line[*i] == '>'
			|| line[*i] == '|'))
		return (token);
	part = get_next_token_part(line, i);
	if (!part)
	{
		ft_free_one(token);
		return (NULL);
	}
	tmp = ft_strjoin(token, part);
	if (tmp == NULL)
		return (ft_free_one(token), ft_free_one(part), NULL);
	ft_free_one(token);
	ft_free_one(part);
	token = tmp;
	return (token);
}
