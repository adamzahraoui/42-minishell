/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_malloc_free.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adzahrao <adzahrao@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 16:51:22 by adzahrao          #+#    #+#             */
/*   Updated: 2025/08/16 16:51:23 by adzahrao         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_free	**gb_variable(void)
{
	static t_free	*g_free_list = NULL;

	return (&g_free_list);
}

void	*ft_malloc(size_t size)
{
	void	*p;
	t_free	*node;
	t_free	**g_free_list;

	g_free_list = gb_variable();
	p = malloc(size);
	if (!p)
	{
		perror("malloc");
		ft_free_all(EXIT_FAILURE);
	}
	node = malloc(sizeof(*node));
	if (!node)
	{
		perror("malloc");
		free(p);
		ft_free_all(EXIT_FAILURE);
	}
	node->ptr = p;
	node->next = *g_free_list;
	*g_free_list = node;
	return (p);
}

void	ft_free_one(void *ptr)
{
	t_free	**g_free_list;
	t_free	*temp;
	t_free	*prev;

	g_free_list = gb_variable();
	temp = *g_free_list;
	prev = NULL;
	while (temp)
	{
		if (temp->ptr == ptr)
		{
			if (prev)
				prev->next = temp->next;
			else
				*g_free_list = temp->next;
			free(temp->ptr);
			free(temp);
			return ;
		}
		prev = temp;
		temp = temp->next;
	}
}

void	ft_free_all(int exit_code)
{
	t_free	**g_free_list;
	t_free	*temp;
	t_free	*next;

	g_free_list = gb_variable();
	temp = *g_free_list;
	while (temp)
	{
		next = temp->next;
		free(temp->ptr);
		free(temp);
		temp = next;
	}
	*g_free_list = NULL;
	exit(exit_code);
}

char	*ft_strdup_gc(const char *s)
{
	size_t	len;
	char	*dup;
	size_t	i;

	len = 0;
	i = 0;
	if (!s)
		return (NULL);
	while (s[len])
		len++;
	dup = ft_malloc(len + 1);
	while (i <= len)
	{
		dup[i] = s[i];
		i++;
	}
	return (dup);
}
