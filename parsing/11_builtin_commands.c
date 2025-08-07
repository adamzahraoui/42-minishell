/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   11_builtin_commands.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adzahrao <adzahrao@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 00:09:12 by mlaidi            #+#    #+#             */
/*   Updated: 2025/08/04 15:35:15 by adzahrao         ###   ########.fr       */
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
	char	*to_write;
	char	*expanded;

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
	to_write = line;
	expanded = NULL;
	if (!params->quoted)
		expanded = expand_token(line, ctx->vars, ctx->env);
	if (expanded)
		to_write = expanded;
	write(params->write_fd, to_write, ft_strlen(to_write));
	write(params->write_fd, "\n", 1);
	if (expanded)
		free(expanded);
	free(line);
	return (0);
}