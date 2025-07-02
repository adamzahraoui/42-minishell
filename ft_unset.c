/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adzahrao <adzahrao@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 15:47:12 by adzahrao          #+#    #+#             */
/*   Updated: 2025/07/02 03:49:09 by adzahrao         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int     ft_com(char *str, char *dest)
{
    int i;

    i = 0;
    while(str[i] == dest[i] && str[i] != '=' && dest[i] != '=')
    {
        if(str[i] != dest[i])
            return (0);
        i++;
    }
    return (1);
}

// void    ft_unset_env(t_myenv **myenv, char *str)
// {
    
// }

void    ft_unset(t_myenv_ex **myenv_ex, char *str)
{
    t_myenv_ex      *list_ex;
    t_myenv_ex      *crr;
    t_myenv_ex      *p;

    list_ex = *myenv_ex;
    p = list_ex;
    if(ft_com(list_ex->data, str) == 1)
    {
        *myenv_ex = list_ex->next;
        crr = list_ex;
        list_ex = list_ex->next;
        free(crr->data);
        free(crr);
    }
    while(list_ex)
    {
        if(ft_com(list_ex->data, str) == 1)
        {
            p->next = list_ex->next;
            crr = list_ex;
            list_ex = list_ex->next;
            free(crr->data);
            free(crr);
        }
        else
        {
            p = list_ex;
            list_ex = list_ex->next;
        }
    }
}
