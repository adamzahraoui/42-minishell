/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export_2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adzahrao <adzahrao@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 01:32:13 by adzahrao          #+#    #+#             */
/*   Updated: 2025/06/30 04:29:45 by adzahrao         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int check_exist(char *str, char *dest)
{
    int i;
    
    i = 0;
    // printf("ok\n");
    // printf("%s\n", str);
    // printf("%s\n", dest);
    // printf("ok\n");
    if(!str || !dest)
        return (0);
    while(str[i] == dest[i])
    {
        if(str[i] == '=' && dest[i] == '=')
            return (1);
        i++;
    }
    return (0);
}

char    *check_val_exist(char *str)
{
    int i;
    int a;
    char *dest;
    char *cupy;
    char *first;

    (1) && (i = 2, a = 2);
    dest = ft_strchr(str, '=');
    cupy = malloc(sizeof(dest) + 2);
    first  = malloc(sizeof(str - dest) + 1);
    if(!dest || !cupy || !first)
    {
        (1) && (cupy[0] = '=', cupy[1] = '"');
        while(dest[i] && dest[i] != '"')
            cupy[a++] = dest[i++];
        (1) && (cupy[a++] = '"', cupy[a] = '\0');
    }
    else
        return NULL;
    i = 0;
    while(str[i] !='=')
    {
        first[i] = str[i];
        i++;
    }
    return(ft_strjoin(first, cupy));
}

int check_double(t_myenv_ex **myenv_ex, char *str)
{
    t_myenv_ex *list;

    list = *myenv_ex;
    while(list->next)
    {
        if(check_exist(list->data, str) == 1)
        {
            list->data = check_val_exist(str);
            return (0);
        }
        list = list->next;
    }
    return (1);
}