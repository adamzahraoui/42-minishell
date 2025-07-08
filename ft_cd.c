/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adzahrao <adzahrao@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 16:09:59 by adzahrao          #+#    #+#             */
/*   Updated: 2025/07/08 02:54:24 by adzahrao         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


// int     ft_strc

void    update_path(char *p, t_myenv **myenv, t_myenv_ex **myenv_ex)
{
    check_double(myenv_ex, p);
    check_double_env(myenv, p);
}

void    ft_cd(char **cmd, t_myenv **myenv, t_myenv_ex **myenv_ex)
{
    char *path;
    char **pa;
    char *pwd;
    char *p;

    if(cmd[1] == NULL)
    {
        update_path(ft_strjoin("OLDPWD=", getcwd(NULL, 0)) , myenv, myenv_ex);
        path = my_get_path(*myenv, "HOME=");
        if(chdir(&path[0]) == -1)
            perror("error in cd\n");
    }
    else if(ft_strcmp(cmd[1], "-") == 0)
    {
        update_path(ft_strjoin("OLDPWD=", getcwd(NULL, 0)) , myenv, myenv_ex);
        pwd = getcwd(NULL, 0);
        pa = my_get_path_split(*myenv, "OLDPWD=", ' ');
        if(chdir(pa[0]) == -1)
            perror("error in cd\n");
        p = ft_strjoin("OLDPWD=", pwd);
        update_path(p, myenv, myenv_ex);
        printf("%s\n", getcwd(NULL, 0));
    }
    else
        if(chdir(cmd[1]) == -1)
            perror("error in cd\n");
    update_path(ft_strjoin("PWD=", getcwd(NULL, 0)) , myenv, myenv_ex);
    
}
