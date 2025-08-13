/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_d.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akira <akira@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 21:26:04 by akira             #+#    #+#             */
/*   Updated: 2025/08/13 00:10:14 by akira            ###   ########.fr       */
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
