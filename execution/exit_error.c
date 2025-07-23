/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_error.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adzahrao <adzahrao@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 03:59:44 by adzahrao          #+#    #+#             */
/*   Updated: 2025/07/19 12:51:41 by adzahrao         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void    ft_free_error(char *str, t_env_context *env_ctx, int i)
{
    t_myenv *list;
    t_myenv *p;
    t_myenv_ex *list_ex;
    t_myenv_ex *e;

    list = *env_ctx->myenv;
    while(list)
    {
        p = list->next;
        free(list->data);
        free(list);
        list = p;
    }
    list_ex = *env_ctx->myenv_ex;
    while(list_ex)
    {
        e = list_ex->next;
        free(list_ex->data);
        free(list_ex);
        list_ex = e;
    }
    if(str != NULL)
        printf("%s", str);
    exit(i);
}
