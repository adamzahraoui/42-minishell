/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akira <akira@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 15:47:12 by adzahrao          #+#    #+#             */
/*   Updated: 2025/08/10 14:28:43 by akira            ###   ########.fr       */
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

void ft_unset_env(t_myenv **myenv, t_cmd *cmd)
{
    int         i;
    t_myenv    *cur;
    t_myenv    *prev;
    t_myenv    *tmp;

    i = 0;
    while (cmd->args[++i])
    {
        cur = *myenv;
        prev = NULL;
        while (cur)
        {
            if (ft_com(cur->data, cmd->args[i]) == 1)
            {
                tmp = cur;
                if (prev)
                    prev->next = cur->next;
                else
                    *myenv = cur->next;
                cur = cur->next;
                free(tmp->data);
                free(tmp);
            }
            else
            {
                prev = cur;
                cur = cur->next;
            }
        }
    }
    set_status(myenv, NULL, 0);
}

void ft_unset(t_myenv_ex **myenv_ex, t_myenv **myenv, t_cmd *cmd)
{
    int         i;
    t_myenv_ex  *cur;
    t_myenv_ex  *prev;
    t_myenv_ex  *tmp;

    ft_unset_env(myenv, cmd);
    i = 0;
    while (cmd->args[++i])
    {
        cur = *myenv_ex;
        prev = NULL;
        while (cur)
        {
            if (ft_com(cur->data, cmd->args[i]) == 1)
            {
                tmp = cur;
                if (prev)
                    prev->next = cur->next;
                else
                    *myenv_ex = cur->next;
                cur = cur->next;
                free(tmp->data);
                free(tmp);
            }
            else
            {
                prev = cur;
                cur = cur->next;
            }
        }
    }
    set_status(myenv, NULL, 0);
}
