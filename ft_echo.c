/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adzahrao <adzahrao@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 15:47:03 by adzahrao          #+#    #+#             */
/*   Updated: 2025/06/26 16:39:40 by adzahrao         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    ft_echo(char **cmd)
{
    int i;

    i = 1;
    if(ft_strncmp(cmd[1], "-n", ft_strlen(cmd[1])) == 0)
    {
        i = 2;
        while(cmd[i])
        {
            printf("%s", cmd[i]);
            if(cmd[i + 1] != NULL)
                printf(" ");
            i++;
        }
    }
    else
    {
        while(cmd[i])
        {
            printf("%s", cmd[i]);
            if(cmd[i + 1] != NULL)
                printf(" ");
            i++;
        }
        printf("\n");
    }
}
