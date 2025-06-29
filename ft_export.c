/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adzahrao <adzahrao@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 15:47:16 by adzahrao          #+#    #+#             */
/*   Updated: 2025/06/29 16:27:38 by adzahrao         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	swap_stack_b(t_myenv_ex **myenv_ex)
{
	t_myenv_ex	*tmp;

    tmp = malloc(sizeof(t_myenv_ex));
	if (!*myenv_ex || !(*myenv_ex)->next || !tmp)
		return ;
	tmp->data = (*myenv_ex)->data;
	(*myenv_ex)->data = (*myenv_ex)->next->data;
    (*myenv_ex)->next->data = tmp->data;
}

void    sort_export(t_myenv_ex **myenv_ex)
{
    t_myenv_ex *list;
    
    list = *myenv_ex;
    while(list->next)
    {
        if(list->next != NULL && list->data[0] > list->next->data[0])
        {
            swap_stack_b(&list);
            list = *myenv_ex;
        }
        else
            list = list->next;
    }
}

void    add_back(t_myenv_ex **myenv_ex, char *str)
{
    t_myenv_ex *list;
    t_myenv_ex *new;

    list = *myenv_ex;
    new = malloc(sizeof(t_myenv_ex));
    if(!new)
        return ;
    while(list->next)
        list = list->next;
    new->data = ft_strdup(str);
    new->next = NULL;
    list->next = new;
}

char    *check_val(char *str)
{
    int i;
    int a;
    char *dest;
    char *cupy;
    char *first;
    
    i = 1;
    a = 2;
    dest = ft_strchr(str, '=');
    cupy = malloc(sizeof(dest) + 2);
    if(dest != NULL || !cupy)
    {
        cupy[0] = '=';
        cupy[1] = '"';
        while(dest[i])
        {
            cupy[a++] = dest[i++];
        }
        cupy[a++] = '"';
        cupy[a] = '\0';
    }
    else
        return NULL;
    i = 0;
    first  = malloc(sizeof(str - dest) + 1);
    while(str[i] !='=')
    {
        first[i] = str[i];
        i++;
    }
    return(ft_strjoin(first, cupy));
}

void    ft_export(t_myenv_ex **myenv_ex, t_myenv **myenv, char **cmd)
{
    t_myenv_ex *pr;
    int i;
    char *egual;

    pr = *myenv_ex;
    i = 1;
    sort_export(myenv_ex);
    if(cmd[1] != NULL)
    {
        while(cmd[i])
        {
            add_back_env(myenv, cmd[i]);
            egual = check_val(cmd[i]);
            if(egual != NULL)
                add_back(myenv_ex, egual);
            else
                add_back(myenv_ex, cmd[i]);
            i++;
        }
    }
    else
        while(pr->next)
        {
            printf("declare -x %s\n", pr->data);
            pr = pr->next;   
        }
}
