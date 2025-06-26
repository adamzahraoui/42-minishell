/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adzahrao <adzahrao@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 16:43:17 by adzahrao          #+#    #+#             */
/*   Updated: 2025/06/26 16:37:48 by adzahrao         ###   ########.fr       */
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

void    check_builtin_cmd(char **cmd, t_myenv *myenv)
{
    if(ft_strncmp(cmd[0], "echo", ft_strlen(cmd[0])) == 0)
        ft_echo(cmd);
    if(ft_strncmp(cmd[0], "cd", ft_strlen(cmd[0])) == 0)
        ft_cd(cmd, myenv);
    // else if(ft_strncmp(cmd[0], "pwd", ft_strlen(cmd[0])) == 0)
    // {
        
    // }
    // else if(ft_strncmp(cmd[0], "export", ft_strlen(cmd[0])) == 0)
    // {
        
    // }
    // else if(ft_strncmp(cmd[0], "unset", ft_strlen(cmd[0])) == 0)
    // {
        
    // }
    // else if(ft_strncmp(cmd[0], "env", ft_strlen(cmd[0])) == 0)
    // {
        
    // }
    // else if(ft_strncmp(cmd[0], "exit", ft_strlen(cmd[0])) == 0)
    // {
        
    // }    
}
