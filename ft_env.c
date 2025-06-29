/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adzahrao <adzahrao@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 21:56:09 by adzahrao          #+#    #+#             */
/*   Updated: 2025/06/29 16:27:22 by adzahrao         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    add_back_env(t_myenv **myenv, char *str)
{
    t_myenv *list;
    t_myenv *new;

    list = *myenv;
    new = malloc(sizeof(t_myenv));
    if(!new)
        return ;
    while(list->next)
        list = list->next;
    new->data = ft_strdup(str);
    new->next = NULL;
    list->next = new;
}

void print_env(t_myenv *myenv)
{
    while(myenv->next)
    {
        if(ft_strchr(myenv->data, '=') != NULL)
            printf("%s\n", myenv->data);
        myenv = myenv->next;
    }
}