/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   03_token_extraction.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlaidi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 00:06:32 by mlaidi            #+#    #+#             */
/*   Updated: 2025/08/02 00:06:33 by mlaidi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*get_next_token_part(char *line, int *i)
{
	int	start;

	if (line[*i] == '\'' || line[*i] == '"')
		return (get_quoted_token(line, i));
	else if (line[*i] == '$')
	{
		start = *i;
		(*i)++;
		while (line[*i] && (ft_isalnum(line[*i]) || line[*i] == '_'))
			(*i)++;
		return (ft_substr(line, start, *i - start));
	}
	else
		return (get_word_token(line, i));
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
		&& line[*i] != '"' && line[*i] != '$')
		(*i)++;
	length = *i - start;
	if (length == 0)
		return (NULL);
	token = ft_substr(line, start, length);
	if (!token)
		ft_putendl_fd("Error: Memory allocation failed", 2);
	return (token);
}

int	extract_var_name(const char *token, int *i, char *var_name)
{
	int	j;

	j = 0;
	(*i)++;
	if (token[*i] == '?')
	{
		var_name[j++] = '?';
		var_name[j] = '\0';
		return (j);
	}
	while (token[*i] && (ft_isalnum(token[*i]) || token[*i] == '_'
			|| token[*i] == '?') && j < 255)
		var_name[j++] = token[(*i)++];
	var_name[j] = '\0';
	(*i)--;
	return (j);
}

char	*remove_quotes(const char *str)
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
    
    // if (len >= 2 && ((str[0] == '"' && str[len - 1] == '"')))
    // {
    //     i = 1; 
    //     len--; 
    // }
    while (i < len)
    {
        result[j] = str[i];
        j++;
        i++;
    }
    result[j] = '\0';
    return (result);
}