/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adzahrao <adzahrao@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 16:09:59 by adzahrao          #+#    #+#             */
/*   Updated: 2025/06/26 20:46:17 by adzahrao         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    ft_cd(char **cmd, t_myenv *myenv)
{
    char *path;

    if(cmd[1] == NULL)
    {
        path = my_get_path(myenv, "HOME=");
        if(chdir(path) == -1)
            perror("error in cd\n");
    }
    else
        if(chdir(cmd[1]) == -1)
            perror("error in cd\n");
}
