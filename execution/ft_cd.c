/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adzahrao <adzahrao@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 16:09:59 by adzahrao          #+#    #+#             */
/*   Updated: 2025/07/13 10:10:56 by adzahrao         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"


int     ft_strlen_cmd(t_cmd **cmd)
{
    int i;
    t_cmd *str;

    str = *cmd;
    i = 0;
    while(str->args[i])
        i++;
    return (i);
}

void    update_path(char *p, t_myenv **myenv, t_myenv_ex **myenv_ex)
{
    check_double(myenv_ex, p);
    check_double_env(myenv, p);
}

void    ft_cd(t_cmd **cmd, t_myenv **myenv, t_myenv_ex **myenv_ex)
{
    char *path;
    
    if((*cmd)->args[1] == NULL)
    {
        update_path(ft_strjoin("OLDPWD=", getcwd(NULL, 0)) , myenv, myenv_ex);
        path = my_get_path(*myenv, "HOME=");
        if(chdir(&path[0]) == -1)
            perror("cd");
        free(path);
    }
    else if(ft_strlen_cmd(cmd) == 2)
    {
        if(chdir((*cmd)->args[1]) == -1)
            perror("cd");
    }
    else
        ft_putstr_fd("minishell: cd: too many arguments\n", 2);
    update_path(ft_strjoin("PWD=", getcwd(NULL, 0)) , myenv, myenv_ex);
}
