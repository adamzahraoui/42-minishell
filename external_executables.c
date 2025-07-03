/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   external_executables.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adzahrao <adzahrao@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 05:10:57 by adzahrao          #+#    #+#             */
/*   Updated: 2025/07/03 05:32:32 by adzahrao         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char     *check_cmd(char **path, char *cmd)
{
    int i;
    char *str;

    i = 0;
    while(path[i])
    {
        str = ft_strjoin(path[i], "/");
        str = ft_strjoin(str, cmd);
        if(access(str, X_OK) == 0)
            return (free(str),path[i]);
        free(str);
        i++;
    }
    return (NULL);
}

void    external_executables(char **cmd, char **path, char **envp)
 {
    __pid_t pid = fork();
    if (pid == 0) {
        char *exec_path;
        if (ft_strchr(cmd[0], '/'))  // if cmd has '/' → abs or rel path
            exec_path = cmd[0];
        else
            exec_path = check_cmd(path, cmd[0]);

        if (!exec_path) {
            printf(": command not found\n");
            exit(127);
        }
        execve(exec_path, cmd, envp);
        perror("execve");
        exit(EXIT_FAILURE);
    }
    else
        perror("fork");
}