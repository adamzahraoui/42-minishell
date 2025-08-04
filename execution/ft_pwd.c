/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adzahrao <adzahrao@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 15:47:09 by adzahrao          #+#    #+#             */
/*   Updated: 2025/08/04 16:21:27 by adzahrao         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void    ft_pwd(t_myenv **myenv)
{
    char *pwd;

    pwd = getcwd(NULL, 0);
    if(!pwd)
    {
        set_status(myenv, "pwd", 1);
        perror("error in pwd\n");
        return ;
    }
    printf("%s\n", pwd);
    free(pwd);
    set_status(myenv, NULL, 0);
}