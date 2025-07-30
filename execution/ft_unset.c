/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adzahrao <adzahrao@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 15:47:12 by adzahrao          #+#    #+#             */
/*   Updated: 2025/07/11 04:03:09 by adzahrao         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int     ft_com(char *str, char *dest)
{
    int i;

    if (!str || !dest)
		return (0);
    i = 0;
    while(str[i] == dest[i] && str[i] && str[i] != '\0')
        i++;
    if(str[i] == '\0' || str[i] == '=')
        return (1);
    return (0);
}

void    ft_unset_env(t_myenv **myenv, char *str)
{
    t_myenv      *list;
    t_myenv      *crr;
    t_myenv      *p;

    list = *myenv;
    p = list;
    if(ft_com(list->data, str) == 1)
    {
        *myenv = list->next;
        crr = list;
        list = list->next;
        free(crr->data);
        free(crr);
    }
    while(list)
    {
        if(ft_com(list->data, str) == 1)
        {
            p->next = list->next;
            crr = list;
            list = list->next;
            free(crr->data);
            free(crr);
        }
        else
        {
            p = list;
            list = list->next;
        }
    }
}

void    ft_unset(t_myenv_ex **myenv_ex, t_myenv **myenv, char *str)
{
    t_myenv_ex      *list_ex;
    t_myenv_ex      *crr;
    t_myenv_ex      *p;

    list_ex = *myenv_ex;
    p = list_ex;
    ft_unset_env(myenv, str);
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
