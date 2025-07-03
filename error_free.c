/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_free.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adzahrao <adzahrao@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 21:52:05 by adzahrao          #+#    #+#             */
/*   Updated: 2025/07/02 21:55:53 by adzahrao         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    ft_free_error(char *str, t_myenv **myenv, t_myenv_ex **myenv_ex)
{
    t_myenv *list;
    t_myenv *p;
    t_myenv_ex *list_ex;
    t_myenv_ex *e;

    while(list)
    {
        p = list->next;
        free(list->data);
        free(list);
        list = p;
    }
    while(list_ex)
    {
        e = list_ex->next;
        free(list_ex->data);
        free(list_ex);
        list_ex = e;
    }
    perror(str);
    exit(0);
}
