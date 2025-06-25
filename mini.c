/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adzahrao <adzahrao@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 18:52:49 by adzahrao          #+#    #+#             */
/*   Updated: 2025/06/24 22:40:30 by adzahrao         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


void    add_env_node(t_myenv **myenv, char *env)
{
    t_myenv *new_node;
    t_myenv *last;

    new_node = malloc(sizeof(t_myenv));
    new_node->data = ft_strdup(env);
    if(!new_node->data)
        return ;
    new_node->next = NULL;
    if(*myenv == NULL)
    {
        *myenv = new_node;
        return ;
    }
    last = *myenv;
    while(last->next)
        last = last->next;
    last->next = new_node;
}

void    set_env(t_myenv **myenv, char **env)
{
    int y;

    y = 0;
    while(env[y])
    {
        add_env_node(myenv, env[y]);
        y++;
    }
}

int main(int ac, char **av, char **env)
{
    char *mini;
    t_myenv *myenv;

    myenv = NULL;
    set_env(&myenv ,env);
    // while(myenv->next)
    // {
    //     printf("%s\n", myenv->data);
    //     myenv = myenv->next;
    // }
    while (1)
    {
        mini = readline("shell> ");
        add_history(mini);
    }
}