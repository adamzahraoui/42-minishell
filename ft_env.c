/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adzahrao <adzahrao@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 21:56:09 by adzahrao          #+#    #+#             */
/*   Updated: 2025/06/25 22:06:24 by adzahrao         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void print_env(t_myenv *myenv)
{
    while(myenv->next)
    {
        if(ft_strchr(myenv->data, '=') != NULL)
            printf("%s\n", myenv->data);
        myenv = myenv->next;
    }
}