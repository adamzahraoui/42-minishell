/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_b.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adzahrao <adzahrao@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 12:51:55 by adzahrao          #+#    #+#             */
/*   Updated: 2025/07/19 12:53:46 by adzahrao         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    set_status(t_myenv **myenv, char *str, int status)
{
    t_myenv *env;
   
    env = *myenv;
    perror(str);
    env->i = status;
}