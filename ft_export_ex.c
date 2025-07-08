/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export_ex.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adzahrao <adzahrao@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 01:32:13 by adzahrao          #+#    #+#             */
/*   Updated: 2025/07/07 05:36:41 by adzahrao         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_strcmp(char	*s1, char	*s2)
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

int check_exist(char *str, char *dest)
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

int check_double(t_myenv_ex **myenv_ex, char *str)
{
    t_myenv_ex *list;
    char *p;

    list = *myenv_ex;
    while (list)
    {
        if(ft_strcmp(list->data, str) == 0)
            return 0;
        if (check_exist(list->data, str) == 1)
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
