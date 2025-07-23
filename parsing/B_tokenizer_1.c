#include "../minishell.h"

t_token *tokenize(char *line)
{
	t_token *head;
	t_token *current;
	int i;
	char *token_value;

	head = NULL;
	current = NULL;
	i = 0;
	while (line[i])
	{
		while (line[i] && (line[i] == ' ' || line[i] == '\t'))
			i++;
		if (!line[i])
			break;
		token_value = get_next_token(line, &i);
		if (token_value)
		{
			if (!add_token_to_list(&head, &current, token_value))
				return (NULL);
		}
	}
	return (head);
}

char *get_next_token(char *line, int *i)
{
	char *token;
	char *part;
	char *tmp;
	int in_single = 0, in_double = 0;

	if (!line || !line[*i])
		return (NULL);
	token = ft_strdup("");
	if (!token)
		return (NULL);
	if (!in_single && !in_double)
	{
		if (line[*i] == '<' && line[*i + 1] == '<')
		{
			free(token);
			*i += 2;
			return (ft_strdup("<<"));
		}
		if (line[*i] == '>' && line[*i + 1] == '>')
		{
			free(token);
			*i += 2;
			return (ft_strdup(">>"));
		}
		if (line[*i] == '<' || line[*i] == '>' || line[*i] == '|')
		{
			free(token);
			char single_char[2] = {line[*i], '\0'};
			(*i)++;
			return (ft_strdup(single_char));
		}
	}
	while (line[*i] && !is_whitespace(line[*i]))
	{
		if (!in_single && !in_double && 
			(line[*i] == '<' || line[*i] == '>' || line[*i] == '|'))
			break;
			
		if (line[*i] == '\'' && !in_double)
			in_single = !in_single;
		else if (line[*i] == '"' && !in_single)
			in_double = !in_double;
		part = get_next_token_part(line, i);
		if (!part)
			break;
		tmp = ft_strjoin(token, part);
		free(token);
		free(part);
		token = tmp;
	}
	if (!*token)
		return (free(token), NULL);
	return (token);
}

int add_token_to_list(t_token **head, t_token **current, char *token_value)
{
	t_token *new;

	new = new_token(token_value);
	if (!new)
	{
		free(token_value);
		free_tokens(*head);
		return (0);
	}
	if (!*head)
	{
		*head = new;
		*current = new;
	}
	else
	{
		(*current)->next = new;
		*current = (*current)->next;
	}
	return (1);
}

t_token *new_token(char *value)
{
	t_token *token;

	token = (t_token *)malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->value = value;
	token->type = determine_token_type(value);
	token->next = NULL;
	return (token);
}

t_token_type determine_token_type(char *value)
{
	if (!value)
		return (TOKEN_ERROR);
	if (value[0] == '|' && !value[1])
		return (TOKEN_PIPE);
	if (value[0] == '<' && !value[1])
		return (TOKEN_REDIR_IN);
	if (value[0] == '>' && !value[1])
		return (TOKEN_REDIR_OUT);
	if (value[0] == '<' && value[1] == '<' && !value[2])
		return (TOKEN_HEREDOC);
	if (value[0] == '>' && value[1] == '>' && !value[2])
		return (TOKEN_APPEND);
	return (TOKEN_WORD);
}
