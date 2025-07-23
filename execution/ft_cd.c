/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adzahrao <adzahrao@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 16:09:59 by adzahrao          #+#    #+#             */
/*   Updated: 2025/07/19 13:58:38 by adzahrao         ###   ########.fr       */
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

void    ft_cd(t_builtin_context *ctx)
{
    char *path;
    char *temp;

    if((*(ctx->cmd))->args[1] == NULL)
    {
        temp = ft_strjoin("OLDPWD=", getcwd(NULL, 0));
        update_path(temp , ctx->myenv, ctx->myenv_ex);
        path = my_get_path(*(ctx->myenv), "HOME=");
        if(chdir(&path[0]) == -1)
            set_status(ctx->myenv, "cd", 1);
        free(path);
        free(temp);
    }
    else if(ft_strlen_cmd(ctx->cmd) == 2)
    {
        if(chdir((*(ctx->cmd))->args[1]) == -1)
            set_status(ctx->myenv, "cd", 1);
    }
    else
        ft_putstr_fd("minishell: cd: too many arguments\n", 2);
    temp = ft_strjoin("PWD=", getcwd(NULL, 0));
    update_path(temp , ctx->myenv, ctx->myenv_ex);
    free(temp);
}
