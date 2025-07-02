/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adzahrao <adzahrao@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 15:47:07 by adzahrao          #+#    #+#             */
/*   Updated: 2025/07/02 02:06:37 by adzahrao         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    ft_exit(t_myenv_ex **myenv_ex, t_myenv **myenv)
{
    t_myenv *list_env;
    t_myenv_ex *list_ex;

    list_env = *myenv;
    list_ex = *myenv_ex;
    while(list_env)
    {
        free(list_env->data);
        list_env = list_env->next;
    }
    while(list_ex)
    {
        free(list_ex->data);
        list_ex = list_ex->next;
    }
    printf("exit\n");
    exit(0);
}