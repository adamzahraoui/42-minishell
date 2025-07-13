/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   external_executables.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adzahrao <adzahrao@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 05:10:57 by adzahrao          #+#    #+#             */
/*   Updated: 2025/07/13 06:22:39 by adzahrao         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char    *check_cmd(char **path, char *cmd)
{
    int i = 0;
    char *str, *tmp;

    while (path[i])
    {
        tmp = ft_strjoin(path[i], "/");
        if(!tmp)
            return (NULL);
        str = ft_strjoin(tmp, cmd);
        if(!str)
            return(free(tmp), NULL);
        free(tmp);
        if (access(str, X_OK) == 0)
            return str; 
        free(str);
        i++;
    }
    return (NULL);
}

void    external_executables(t_cmd **cmd, char **path, char **envp)
{
    __pid_t pid;

    if(ft_strcmp((*cmd)->args[0], "echo") == 0)
        return ;
    pid = fork();
    if (pid == 0)
    {
        char *exec_path;
        if (ft_strchr((*cmd)->args[0], '/'))
            exec_path = (*cmd)->args[0];
        else
            exec_path = check_cmd(path, (*cmd)->args[0]);

        if (!exec_path)
        {
            printf("%s: command not found\n", (*cmd)->args[0]);
            exit(127);
        }
        execve(exec_path, (*cmd)->args, envp);
        perror("execve");
        exit(EXIT_FAILURE);
    }
    else if (pid > 0)
        waitpid(pid, NULL, 0);
    else
        perror("fork");
}
