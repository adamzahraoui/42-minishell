/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   11_heredoc_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlaidi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 17:02:05 by mlaidi            #+#    #+#             */
/*   Updated: 2025/08/16 17:02:06 by mlaidi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	heredoc_child_loop(t_heredoc_loop_params *params, t_expand_context *ctx)
{
	t_heredoc_params	line_params;
	int					got_delim;

	got_delim = 0;
	line_params.clean_delimiter = params->clean_delimiter;
	line_params.write_fd = params->write_fd;
	line_params.got_delim = &got_delim;
	while (1)
	{
		if (process_heredoc_line(&line_params, ctx) == -1)
			break ;
		(*params->lineno)++;
	}
	return (got_delim);
}

int	process_heredoc_line(t_heredoc_params *params, t_expand_context *ctx)
{
	char	*line;

	line = readline("> ");
	if (!line)
	{
		*params->got_delim = 0;
		return (-1);
	}
	if (ft_strcmp(line, params->clean_delimiter) == 0)
	{
		*params->got_delim = 1;
		free(line);
		return (-1);
	}
	write_heredoc_line(params, ctx, line);
	free(line);
	return (0);
}

void	write_heredoc_line(t_heredoc_params *params, t_expand_context *ctx,
		char *line)
{
	char	*to_write;
	char	*expanded;

	to_write = line;
	expanded = NULL;
	expanded = expand_heredoc_line(line, ctx);
	if (expanded)
		to_write = expanded;
	write(params->write_fd, to_write, ft_strlen(to_write));
	write(params->write_fd, "\n", 1);
	if (expanded)
		ft_free_one(expanded);
	ft_free_one(line);
}

char	*expand_heredoc_line(char *line, t_expand_context *ctx)
{
	char			*result;
	t_token_state	st;

	if (!line)
		return (NULL);
	result = ft_malloc(MAX_TOKEN_LEN);
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
