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

int	handle_heredoc(const char *delimiter, t_expand_context *ctx)
{
	char	*clean_delimiter;
	int		fds[2];
	pid_t	pid;
	int		quoted;

	if (heredoc_setup(delimiter, &clean_delimiter, &quoted) == -1)
		return (-1);
	pid = heredoc_pipe_and_fork(fds, clean_delimiter);
	if (pid == -1)
		return (-1);
	if (pid == 0)
		heredoc_child(fds, clean_delimiter, quoted, ctx);
	return (heredoc_parent(pid, fds, clean_delimiter));
}

int	is_redirection(t_token_type type)
{
	return (type == TOKEN_REDIR_IN || type == TOKEN_REDIR_OUT
		|| type == TOKEN_HEREDOC || type == TOKEN_APPEND);
}
