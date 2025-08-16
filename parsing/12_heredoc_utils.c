/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   12_heredoc_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlaidi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 17:02:10 by mlaidi            #+#    #+#             */
/*   Updated: 2025/08/16 17:02:11 by mlaidi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	process_quoted_char(char *line, t_token_state *st, char *result,
		t_expand_context *ctx)
{
	if (line[st->i] == '$')
	{
		if (st->in_double)
			st->j += expand_variable(line, &st->i, result + st->j, ctx);
		else
			result[st->j++] = line[st->i];
	}
	else
		result[st->j++] = line[st->i];
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
	result = ft_malloc(len + 1);
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

void	handle_heredoc_sigint(int sig)
{
	(void)sig;
	write(2, "\n", 1);
	ft_free_all(130);
}

int	is_redirection(t_token_type type)
{
	return (type == TOKEN_REDIR_IN || type == TOKEN_REDIR_OUT
		|| type == TOKEN_HEREDOC || type == TOKEN_APPEND);
}
