/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adzahrao <adzahrao@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 15:47:09 by adzahrao          #+#    #+#             */
/*   Updated: 2025/07/07 04:33:28 by adzahrao         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    ft_pwd()
{
    char *pwd;

    pwd = getcwd(NULL, 0);
    if(!pwd)
    {
        perror("error in pwd\n");
        return ;
    }
    printf("%s\n", pwd);
    free(pwd);
}