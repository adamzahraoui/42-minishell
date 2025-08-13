/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   11_heredoc_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akira <akira@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 00:09:12 by mlaidi            #+#    #+#             */
/*   Updated: 2025/08/13 02:20:29 by akira            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
	return (0);
}

void	write_heredoc_line(t_heredoc_params *params, t_expand_context *ctx,
		char *line)
{
	char	*to_write;
	char	*expanded;

	to_write = line;
	expanded = NULL;
	if (!params->quoted)
		expanded = expand_heredoc_line(line, ctx);
	if (expanded)
		to_write = expanded;
	write(params->write_fd, to_write, ft_strlen(to_write));
	write(params->write_fd, "\n", 1);
	if (expanded)
		free(expanded);
	free(line);
}

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
