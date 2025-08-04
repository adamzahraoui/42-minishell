/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adzahrao <adzahrao@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 16:09:59 by adzahrao          #+#    #+#             */
/*   Updated: 2025/08/04 16:20:04 by adzahrao         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int ft_strlen_cmd(t_cmd **cmd)
{
    int i;
    t_cmd *str;

    str = *cmd;
    i = 0;
    while (str->args[i])
        i++;
    return (i);
}

void update_path(char *p, t_myenv **myenv, t_myenv_ex **myenv_ex)
{
    check_double(myenv_ex, p);
    check_double_env(myenv, p);
}

// Ensure all error paths set status

void ft_cd(t_cmd **cmd, t_myenv **myenv, t_myenv_ex **myenv_ex)
{
    char *path;
    char *temp;

    if ((*cmd)->args[1] == NULL)
    {
        temp = ft_strjoin("OLDPWD=", getcwd(NULL, 0));
        update_path(temp, myenv, myenv_ex);
        path = my_get_path(*myenv, "HOME=");
        if (path == NULL)
        {
            ft_putstr_fd("minishell: cd: HOME not set\n", 2);
            set_status(myenv, NULL, 1);
            free(temp);
            return;
        }
        if (chdir(&path[0]) == -1)
        {
            set_status(myenv, "cd", 1);
            free(temp);
            return;
        }
        set_status(myenv, NULL, 0);
        free(temp);
    }
    else if (ft_strlen_cmd(cmd) == 2)
    {
        temp = ft_strjoin("OLDPWD=", getcwd(NULL, 0));
        update_path(temp, myenv, myenv_ex);
        if (chdir((*cmd)->args[1]) == -1)
        {
            ft_putstr_fd("minishell: cd: ", 2);
            ft_putstr_fd((*cmd)->args[1], 2);
            ft_putstr_fd(": No such file or directory\n", 2);
            set_status(myenv, NULL, 1);
            free(temp);
            return;
        }
        set_status(myenv, NULL, 0);
        free(temp);
    }
    else
    {
        ft_putstr_fd("minishell: cd: too many arguments\n", 2);
        set_status(myenv, NULL, 1);
        return;
    }
    
    temp = ft_strjoin("PWD=", getcwd(NULL, 0));
    update_path(temp, myenv, myenv_ex);
    free(temp);
}
