#include "../minishell.h"

int	is_whitespace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f'
		|| c == '\r');
}

char	*get_next_token_part(char *line, int *i)
{
	if (line[*i] == '|' || line[*i] == '<' || line[*i] == '>')
		return (get_operator_token(line, i));
	else if (line[*i] == '\'' || line[*i] == '\"')
		return (get_quoted_token(line, i));
	else
		return (get_word_token(line, i));
}

char	*get_operator_token(char *line, int *i)
{
	char	*token;

	if ((line[*i] == '<' && line[*i + 1] == '<') || (line[*i] == '>' && line[*i
				+ 1] == '>'))
	{
		token = ft_substr(line, *i, 2);
		*i += 2;
	}
	else
	{
		token = ft_substr(line, *i, 1);
		*i += 1;
	}
	if (!token)
		ft_putendl_fd("Error: Memory allocation failed", 2);
	return (token);
}

char	*get_quoted_token(char *line, int *i)
{
	int		start;
	int		length;
	char	quote;
	char	*token;

	start = *i;
	quote = line[*i];
	(*i)++;
	while (line[*i] && line[*i] != quote)
		(*i)++;
	if (!line[*i])
	{
		ft_putendl_fd("Error: Unclosed quote", 2);
		return (NULL);
	}
	(*i)++;
	length = *i - start;
	token = ft_substr(line, start, length);
	if (!token)
		ft_putendl_fd("Error: Memory allocation failed", 2);
	return (token);
}

char	*get_word_token(char *line, int *i)
{
	int		start;
	int		length;
	char	*token;

	start = *i;
	while (line[*i] && !is_delimiter(line[*i]) && line[*i] != '\''
		&& line[*i] != '\"')
		(*i)++;
	length = *i - start;
	if (length == 0)
		return (NULL);
	token = ft_substr(line, start, length);
	if (!token)
		ft_putendl_fd("Error: Memory allocation failed", 2);
	return (token);
}
