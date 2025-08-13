/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   12_heredoc_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akira <akira@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 02:20:10 by akira             #+#    #+#             */
/*   Updated: 2025/08/13 02:22:50 by akira            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	handle_quoted_expansion(char *line, t_token_state *st, char *result,
		t_expand_context *ctx)
{
	char	quote;
	int		len;

	len = ft_strlen(line);
	if (st->in_single)
		quote = '\'';
	else
		quote = '"';
	result[st->j++] = quote;
	while (++st->i < len - 1)
	{
		process_quoted_char(line, st, result, ctx);
	}
	result[st->j++] = quote;
}

char	*expand_heredoc_line(char *line, t_expand_context *ctx)
{
	char			*result;
	t_token_state	st;

	if (!line)
		return (NULL);
	result = malloc(MAX_TOKEN_LEN);
	if (!result)
		return (NULL);
	memset(&st, 0, sizeof(st));
	if (st.in_single || st.in_double)
		handle_quoted_expansion(line, &st, result, ctx);
	else
	{
		while (line[st.i] && st.j < MAX_TOKEN_LEN - 1)
		{
			if (line[st.i] == '$')
				st.j += expand_variable(line, &st.i, result + st.j, ctx);
			else
				result[st.j++] = line[st.i];
			st.i++;
		}
	}
	result[st.j] = '\0';
	return (result);
}

char	*remove_all_quotes(const char *str)
{
	char	*result;
	int		i;
	int		j;
	int		len;

	if (!str)
		return (NULL);
	len = ft_strlen(str);
	result = malloc(len + 1);
	if (!result)
		return (NULL);
	i = 0;
	j = 0;
	while (str[i])
	{
		if (str[i] != '\'' && str[i] != '"')
		{
			result[j] = str[i];
			j++;
		}
		i++;
	}
	result[j] = '\0';
	return (result);
}

int	is_quoted(const char *str)
{
	int	len;

	if (!str)
		return (0);
	len = strlen(str);
	return (len >= 2 && ((str[0] == '"' && str[len - 1] == '"')
			|| (str[0] == '\'' && str[len - 1] == '\'')));
}
