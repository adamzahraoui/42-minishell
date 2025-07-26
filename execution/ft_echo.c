/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adzahrao <adzahrao@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 15:47:03 by adzahrao          #+#    #+#             */
/*   Updated: 2025/07/26 18:36:27 by adzahrao         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void    ft_echo(t_cmd **str)
{
    t_cmd *cmd;
    int i;

    i = 1;
    cmd = *str;
    if(ft_strncmp(cmd->args[1], "-n", ft_strlen(cmd->args[1])) == 0)
    {
        i = 2;
        while(cmd->args[i])
        {
            printf("%s", cmd->args[i]);
            if(cmd->args[i + 1] != NULL)
                printf(" ");
            i++;
        }
    }
    else
    {
        while(cmd->args[i])
        {
            printf("%s", cmd->args[i]);
            if(cmd->args[i + 1] != NULL)
                printf(" ");
            i++;
        }
         printf("\n");
    }
}
