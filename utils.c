/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adzahrao <adzahrao@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 16:43:17 by adzahrao          #+#    #+#             */
/*   Updated: 2025/07/02 02:06:03 by adzahrao         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char    **my_get_path_split(t_myenv *myenv, char *path, char c)
{
    char **args;
    char *av;
    int i;

    while(myenv->next)
    {
        i = 0;
        while(myenv->data[i] == path[i])
            i++;
        if(path[i] == '\0')
        {
            av = &myenv->data[i];
            break;
        }
        myenv = myenv->next;
    }
    if(path[i] != '\0')
        return (NULL);
    args = ft_split(av, c);
    return (args);
}

char    *my_get_path(t_myenv *myenv, char *path)
{
    char *av;
    int i;

    while(myenv->next)
    {
        i = 0;
        while(myenv->data[i] == path[i])
            i++;
        if(path[i] == '\0')
        {
            av = &myenv->data[i];
            break;
        }
        myenv = myenv->next;
    }
    if(path[i] != '\0')
        return (NULL);
    return (av);
}

void    check_builtin_cmd(char **cmd, t_myenv *myenv, t_myenv_ex *myenv_ex)
{
    if(ft_strncmp(cmd[0], "echo", ft_strlen(cmd[0])) == 0)
        ft_echo(cmd);
    if(ft_strncmp(cmd[0], "cd", ft_strlen(cmd[0])) == 0)
        ft_cd(cmd, myenv);
    // else if(ft_strncmp(cmd[0], "pwd", ft_strlen(cmd[0])) == 0)
    // {
        
    // }
    else if(ft_strncmp(cmd[0], "export", ft_strlen(cmd[0])) == 0)
        ft_export(&myenv_ex, &myenv, cmd);
    // else if(ft_strncmp(cmd[0], "unset", ft_strlen(cmd[0])) == 0)
    // {
        
    // }
    else if(ft_strncmp(cmd[0], "env", ft_strlen(cmd[0])) == 0)
        print_env(myenv);
    else if(ft_strncmp(cmd[0], "exit", ft_strlen(cmd[0])) == 0)
        ft_exit(&myenv_ex, &myenv);
    else
        return ;  
}

void    add_env_node_ex(t_myenv_ex **myenv, char *env)
{
    t_myenv_ex *new_node;
    t_myenv_ex *last;

    new_node = malloc(sizeof(t_myenv_ex));
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

void    set_env_ex(t_myenv_ex **myenv, char **env)
{
    int y;

    y = 0;
    while(env[y])
    {
        add_env_node_ex(myenv, env[y]);
        y++;
    }
}