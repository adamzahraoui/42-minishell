/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   11_builtin_commands.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlaidi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 00:09:12 by mlaidi            #+#    #+#             */
/*   Updated: 2025/08/02 00:09:14 by mlaidi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	builtin_echo(t_cmd *cmd)
{
	int	i;
	int	newline;

	i = 1;
	newline = 1;
	while (cmd->args[i] && is_valid_n_flag(cmd->args[i]))
	{
		newline = 0;
		i++;
	}
	while (cmd->args[i])
	{
		printf("%s", cmd->args[i]);
		if (cmd->args[i + 1])
			printf(" ");
		i++;
	}
	if (newline)
		printf("\n");
}

int	is_valid_n_flag(const char *str)
{
	int	i;

	i = 1;
	if (str[0] != '-' || str[1] != 'n')
		return (0);
	while (str[i])
	{
		if (str[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

int	process_heredoc_line(const char *clean_delimiter, int quoted, int write_fd,
		t_expand_context *ctx, int *got_delim)
{
	char	*line;
	char	*to_write;
	char	*expanded;

	line = readline("> ");
	if (!line)
		return (-1);
	if (ft_strcmp(line, clean_delimiter) == 0)
	{
		*got_delim = 1;
		free(line);
		return (-1);
	}
	to_write = line;
	expanded = NULL;
	if (!quoted)
		expanded = expand_token(line, ctx->vars, ctx->env);
	if (expanded)
		to_write = expanded;
	write(write_fd, to_write, ft_strlen(to_write));
	write(write_fd, "\n", 1);
	if (expanded)
		free(expanded);
	free(line);
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
