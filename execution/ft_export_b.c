/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export_b.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akira <akira@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 21:30:32 by akira             #+#    #+#             */
/*   Updated: 2025/08/12 00:36:01 by akira            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*create_cupy(char *dest)
{
	int		i;
	int		a;
	char	*cupy;

	i = 1;
	a = 1;
	if (!dest)
		return (NULL);
	cupy = malloc(ft_strlen(dest) + 3);
	if (!dest || !cupy)
		return (NULL);
	cupy[0] = '=';
	while (dest[i])
		cupy[a++] = dest[i++];
	cupy[a] = '\0';
	return (cupy);
}

char	*check_val(char *str)
{
	int		i;
	char	*dest;
	char	*cupy;
	char	*first;
	char	*temp;

	(1) && (dest = ft_strchr(str, '='), i = 0);
	if (dest && dest[1] == '"')
		return (ft_strdup(str));
	cupy = create_cupy(dest);
	if (!dest || !cupy)
		return (NULL);
	first = malloc((dest - str) + 2);
	if (!first)
		return (NULL);
	while (str[i] != '=')
	{
		first[i] = str[i];
		i++;
	}
	(1) && (first[i] = '\0', temp = ft_strjoin(first, cupy));
	free(first);
	free(cupy);
	return (temp);
}

int	find_equals_index(char *arg)
{
	int	j;

	j = 0;
	while (arg[j])
	{
		if (arg[j] == '=')
			return (j);
		j++;
	}
	return (-1);
}

int	is_valid_first_char(char c)
{
	return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c == '_'));
}

int	validate_chars_before_equals(char *arg, int equals_index, t_myenv **myenv)
{
	int	j;

	j = 0;
	while (arg[j] && (equals_index == -1 || j < equals_index))
	{
		if (!ft_isalnum(arg[j]) && arg[j] != '_')
		{
			print_error_ex(myenv, arg);
			return (0);
		}
		j++;
	}
	return (1);
}
