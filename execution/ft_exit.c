/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adzahrao <adzahrao@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 15:47:07 by adzahrao          #+#    #+#             */
/*   Updated: 2025/07/11 04:03:01 by adzahrao         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void    ft_exit(t_myenv_ex **myenv_ex, t_myenv **myenv)
{
    t_myenv *list_env;
    t_myenv *temp_env;
    t_myenv_ex *list_ex;
    t_myenv_ex *temp_ex;

    list_env = *myenv;
    while(list_env)
    {
        temp_env = list_env->next;
        free(list_env->data);
        free(list_env);
        list_env = temp_env;
    }
    list_ex = *myenv_ex;
    while(list_ex)
    {
        temp_ex = list_ex->next;
        free(list_ex->data);
        free(list_ex);
        list_ex = temp_ex;
    }    
    printf("exit\n");
    exit(0);
}
