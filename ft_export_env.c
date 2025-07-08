/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export_env.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adzahrao <adzahrao@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 05:25:10 by adzahrao          #+#    #+#             */
/*   Updated: 2025/07/07 05:34:55 by adzahrao         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ft_strcmp_env(char	*s1, char	*s2)
{
	int	i;

	i = 0;
	while (s1[i] != '\0' && s2[i] != '\0' && s1[i] == s2[i])
		i++;
    if (s1[i] == '=' && s2[i] == '\0')
        return (0);
    else
	    return (s1[i] - s2[i]);
}

static int check_exist_env(char *str, char *dest)
{
    int i;

    i = 0;
    if(!str || !dest)
        return (0);
    while (str[i] && dest[i] && str[i] != '=' && dest[i] != '=')
    {
        if (str[i] != dest[i])
            return 0;
        i++;
    }

    if ((str[i] == '=' && dest[i] == '=') ||
        (str[i] == '=' && dest[i] == '\0') ||
        (str[i] == '\0' && dest[i] == '=') ||
        (str[i] == '\0' && dest[i] == '\0'))
        return 1;
    return (0);
}

int check_double_env(t_myenv **myenv, char *str)
{
    t_myenv *list;
    char *p;

    list = *myenv;
    while (list)
    {
        if(ft_strcmp_env(list->data, str) == 0)
            return 0;
        if (check_exist_env(list->data, str) == 1)
        {
            p = check_val(str);
            if (p)
            {
                free(list->data);
                list->data = p;
                return (0);
            }
        }
        list = list->next;
    }
    return (1);
}