/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_d.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akira <akira@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 21:26:04 by akira             #+#    #+#             */
/*   Updated: 2025/08/10 21:34:08 by akira            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
