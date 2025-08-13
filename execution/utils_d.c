/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_d.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akira <akira@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 21:26:04 by akira             #+#    #+#             */
/*   Updated: 2025/08/13 02:23:50 by akira            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	add_back(t_myenv_ex **myenv_ex, char *str)
{
	t_myenv_ex	*list;
	t_myenv_ex	*new;

	list = *myenv_ex;
	new = malloc(sizeof(t_myenv_ex));
	if (!new)
		return ;
	new->data = ft_strdup(str);
	new->next = NULL;
	if (!list)
	{
		*myenv_ex = new;
		return ;
	}
	while (list->next)
		list = list->next;
	list->next = new;
}

void	set_variables(t_cmd **cmd, t_myenv **myenv, t_myenv_ex **myenv_ex,
		char **env)
{
	*cmd = NULL;
	*myenv = NULL;
	*myenv_ex = NULL;
	declare_env(myenv, myenv_ex, env);
	(*myenv)->i = 0;
	setup_signals();
}
