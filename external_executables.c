/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   external_executables.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adzahrao <adzahrao@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 05:10:57 by adzahrao          #+#    #+#             */
/*   Updated: 2025/07/04 03:09:33 by adzahrao         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char *check_cmd(char **path, char *cmd)
{
    int i = 0;
    char *str, *tmp;

    while (path[i])
    {
        tmp = ft_strjoin(path[i], "/");
        str = ft_strjoin(tmp, cmd);
        free(tmp);
        if (access(str, X_OK) == 0)
            return str; 
        free(str);
        i++;
    }
    return NULL;
}

void external_executables(char **cmd, char **path, char **envp)
{
    __pid_t pid = fork();
    if (pid == 0)
    {
        char *exec_path;
        if (ft_strchr(cmd[0], '/'))
            exec_path = cmd[0];
        else
            exec_path = check_cmd(path, cmd[0]);

        if (!exec_path)
        {
            printf("%s: command not found\n", cmd[0]);
            exit(127);
        }
        execve(exec_path, cmd, envp);
        perror("execve");
        exit(EXIT_FAILURE);
    }
    else if (pid > 0)
    {
        waitpid(pid, NULL, 0);
    }
    else
    {
        perror("fork");
    }
}
