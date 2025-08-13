/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akira <akira@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 21:56:09 by adzahrao          #+#    #+#             */
/*   Updated: 2025/08/13 00:09:43 by akira            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	add_back_env(t_myenv **myenv, char *str)
{
	t_myenv	*list;
	t_myenv	*new;

	list = *myenv;
	new = malloc(sizeof(t_myenv));
	if (!new)
		return ;
	new->data = ft_strdup(str);
	new->next = NULL;
	if (!list)
	{
		*myenv = new;
		return ;
	}
	while (list->next)
		list = list->next;
	list->next = new;
}

int	check_exist_env(char *str, char *dest)
{
	int	i;

	i = 0;
	if (!str || !dest)
		return (0);
	while (str[i] && dest[i] && str[i] != '=' && dest[i] != '=')
	{
		if (str[i] != dest[i])
			return (0);
		i++;
	}
	if ((str[i] == '=' && dest[i] == '=') || (str[i] == '=' && dest[i] == '\0')
		|| (str[i] == '\0' && dest[i] == '=') || (str[i] == '\0'
			&& dest[i] == '\0'))
		return (1);
	return (0);
}

int	check_double_env(t_myenv **myenv, char *str)
{
	t_myenv	*list;
	char	*p;

	list = *myenv;
	while (list)
	{
		if (ft_strcmp(list->data, str) == 0)
			return (0);
		if (check_exist(list->data, str) == 1)
		{
			p = check_val(str);
			if (p)
			{
				free(list->data);
				list->data = p;
				return (0);
			}
			free(p);
		}
		list = list->next;
	}
	return (1);
}

void	print_env(t_myenv *myenv)
{
	int	i;

	while (myenv)
	{
		if (ft_strchr(myenv->data, '=') != NULL)
		{
			i = 0;
			while (myenv->data[i] != '\0')
			{
				if (myenv->data[i] == '"' && myenv->data[i - 1] == '=')
					i++;
				else if (myenv->data[i] == '"' && myenv->data[i + 1] == '\0')
					i++;
				else
				{
					write(1, &myenv->data[i], 1);
					i++;
				}
			}
			printf("\n");
		}
		myenv = myenv->next;
	}
}

void	set_env_doubl(t_myenv **myenv, char *str)
{
	char	*egual;

	egual = check_val(str);
	if (egual != NULL && check_double_env(myenv, str) == 1)
		add_back_env(myenv, egual);
	else if (check_double_env(myenv, str) == 1)
		add_back_env(myenv, str);
	free(egual);
}
