/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adzahrao <adzahrao@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 15:47:16 by adzahrao          #+#    #+#             */
/*   Updated: 2025/08/02 18:55:22 by adzahrao         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	swap_stack_b(t_myenv_ex **myenv_ex)
{
	char	*tmp;

	if (!*myenv_ex || !(*myenv_ex)->next)
		return ;
	tmp = (*myenv_ex)->data;
	(*myenv_ex)->data = (*myenv_ex)->next->data;
	(*myenv_ex)->next->data = tmp;
}

void	sort_export(t_myenv_ex **myenv_ex)
{
	t_myenv_ex	*list;
	char		*p;
	int			i;

	list = *myenv_ex;
	while (list)
	{
		if (list->data)
		{
			i = 0;
			while (list->data[i])
			{
				if (list->data[i] == '=' && list->data[i + 1] != '"'
					&& list->data[ft_strlen(list->data) - 1] != '"')
				{
					p = check_val(list->data);
					free(list->data);
					if (p)
						list->data = p;
					break ;
				}
				i++;
			}
		}
		if (list->next && list->data && list->next->data
			&& list->data[0] > list->next->data[0])
		{
			swap_stack_b(&list);
			list = *myenv_ex;
		}
		else
			list = list->next;
	}
}

void	add_back(t_myenv_ex **myenv_ex, char *str)
{
	t_myenv_ex	*list;
	t_myenv_ex	*new;

	list = *myenv_ex;
	new = malloc(sizeof(t_myenv_ex));
	if (!new || !(*myenv_ex) || !str)
		return ;
	while (list->next)
		list = list->next;
	new->data = ft_strdup(str);
	new->next = NULL;
	list->next = new;
}

char	*check_val(char *str)
{
	int		i;
	int		a;
	char	*dest;
	char	*cupy;
	char	*first;

	(1) && (i = 1, a = 1, dest = ft_strchr(str, '='));
	if (dest != NULL && dest[1] == '"')
		return (ft_strdup(str));
	if (!dest)
		return (NULL);
	cupy = malloc(ft_strlen(dest) + 3);
	if (dest != NULL && cupy != NULL)
	{
		cupy[0] = '=';
		while (dest[i])
			cupy[a++] = dest[i++];
		cupy[a] = '\0';
	}
	else
		return (NULL);
	i = 0;
	first = malloc((dest - str) + 2);
	while (str[i] != '=')
	{
		first[i] = str[i];
		i++;
	}
	first[i] = '\0';
	return (ft_strjoin(first, cupy));
}

int	check_arg_export(t_myenv **myenv, t_cmd *str, int i)
{
	t_cmd	*cmd;
	int		j;
	int		equals_index;

	j = 0;
	cmd = str;
	equals_index = -1;
	while (cmd->args[i][j])
	{
		if (cmd->args[i][j] == '=')
		{
			equals_index = j;
			break ;
		}
		j++;
	}
	j = 0;
	if ((cmd->args[i][0] >= 'a' && cmd->args[i][0] <= 'z')
		|| (cmd->args[i][0] >= 'A' && cmd->args[i][0] <= 'Z')
		|| (cmd->args[i][0] == '_'))
	{
		while (cmd->args[i][j] && (equals_index == -1 || j < equals_index))
		{
			if (!ft_isalnum(cmd->args[i][j]) && cmd->args[i][j] != '_')
			{
				print_error_ex(myenv, cmd->args[i]);
				return (0);
			}
			j++;
		}
	}
	else
	{
		print_error_ex(myenv, cmd->args[i]);
		return (0);
	}
	return (1);
}

void	ft_export(t_myenv_ex **myenv_ex, t_myenv **myenv, t_cmd **str)
{
	char	*egual;
	t_cmd	*cmd;
	int		i;

	(1) && (i = 1, cmd = *str);
	sort_export(myenv_ex);
	if (cmd->args[1] != NULL)
	{
		while (cmd->args[i])
		{
			if (check_arg_export(myenv, *str, i) == 1)
			{
				set_env_doubl(myenv, cmd->args[i]);
				egual = check_val(cmd->args[i]);
				if (check_double(myenv_ex, cmd->args[i]) == 1)
				{
					if (egual != NULL)
						add_back(myenv_ex, egual);
					else
						add_back(myenv_ex, cmd->args[i]);
				}
				free(egual);
			}
			i++;
		}
	}
	else
		print_export(myenv_ex);
}

void	print_export(t_myenv_ex **myenv_ex)
{
	t_myenv_ex	*pr;
	int			i;
	int			f;

	pr = *myenv_ex;
	while (pr)
	{
		(1) && (i = 0, f = 0);
		printf("declare -x ");
		if (pr->data != NULL)
		{
			while (pr->data[i])
			{
				printf("%c", pr->data[i]);
				if (pr->data[i] == '=' && f == 0)
				{
					printf("\"");
					f = 1;
				}
				else if (pr->data[i + 1] == '\0' && f == 1)
					printf("\"");
				i++;
			}
			printf("\n");
		}
		pr = pr->next;
	}
}
