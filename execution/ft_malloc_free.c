/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_malloc_free.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akira <akira@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 00:47:52 by akira             #+#    #+#             */
/*   Updated: 2025/08/14 17:01:29 by akira            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_free **gb_variable(void)
{
    static t_free *g_free_list = NULL;

    return &g_free_list;
}

void *ft_malloc(size_t size)
{
    void    *p;
    t_free  *node;
    t_free  **g_free_list = gb_variable();

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
    return p;
}

void ft_free_one(void *ptr)
{
    t_free **g_free_list = gb_variable();
    t_free *temp = *g_free_list;
    t_free *prev = NULL;

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
            return;
        }
        prev = temp;
        temp = temp->next;
    }
}

void ft_free_all(int exit_code)
{
    t_free **g_free_list = gb_variable();
    t_free *temp = *g_free_list;
    t_free *next;

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

char	*ft_strjoin_gc(char const *s1, char const *s2)
{
	size_t	i;
	size_t	j;
	char	*p;

	if (!s1 || !s2)
		return (NULL);
	i = 0;
	j = 0;
	while (s1[i] != '\0')
		i++;
	while (s2[j] != '\0')
		j++;
	p = ft_malloc((i + j + 1) * sizeof(char));
	ft_memmove(p, s1, i);
	ft_memmove(p + i, s2, j);
	p[i + j] = '\0';
	return (p);
}
