/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_c.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akira <akira@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 18:46:21 by akira             #+#    #+#             */
/*   Updated: 2025/08/14 16:01:06 by akira            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	add_env_node_ex(t_myenv_ex **myenv, char *env)
{
	t_myenv_ex	*new_node;
	t_myenv_ex	*last;

	new_node = ft_malloc(sizeof(t_myenv_ex));
	new_node->data = ft_strdup_gc(env);
	if (!new_node->data)
		return ;
	new_node->next = NULL;
	if (*myenv == NULL)
	{
		*myenv = new_node;
		return ;
	}
	last = *myenv;
	while (last->next)
		last = last->next;
	last->next = new_node;
}

void	set_env_ex(t_myenv_ex **myenv, char **env)
{
	int	y;

	y = 0;
	while (env[y])
	{
		add_env_node_ex(myenv, env[y]);
		y++;
	}
}

void	swap_stack_b(t_myenv_ex **myenv_ex)
{
	char	*tmp;

	if (!*myenv_ex || !(*myenv_ex)->next)
		return ;
	tmp = (*myenv_ex)->data;
	(*myenv_ex)->data = (*myenv_ex)->next->data;
	(*myenv_ex)->next->data = tmp;
}

void	process_export_data(t_myenv_ex *list)
{
	int		i;
	char	*p;

	i = 0;
	while (list->data[i])
	{
		if (list->data[i] == '=' && list->data[i + 1] != '"'
			&& list->data[ft_strlen(list->data) - 1] != '"')
		{
			p = check_val(list->data);
			ft_free_one(list->data);
			if (p)
				list->data = p;
			break ;
		}
		i++;
	}
}

void	sort_export(t_myenv_ex **myenv_ex)
{
	t_myenv_ex	*list;

	list = *myenv_ex;
	while (list)
	{
		if (list->data)
			process_export_data(list);
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
