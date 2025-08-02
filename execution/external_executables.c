/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   external_executables.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adzahrao <adzahrao@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 05:10:57 by adzahrao          #+#    #+#             */
/*   Updated: 2025/08/02 17:05:19 by adzahrao         ###   ########.fr       */
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
    pid_t pid;
    void (*old_sigint)(int);
    void (*old_sigquit)(int);

    if (ft_strcmp((*cmd)->args[0], "echo") == 0)
        return ;
    old_sigint = signal(SIGINT, SIG_IGN);
    old_sigquit = signal(SIGQUIT, SIG_IGN);
    pid = fork();
    if (pid == 0)
    {
        signal(SIGINT, SIG_DFL);
        signal(SIGQUIT, SIG_DFL);
        char *exec_path;
        if (ft_strchr((*cmd)->args[0], '/'))
            exec_path = (*cmd)->args[0];
        else
            exec_path = check_cmd(path, (*cmd)->args[0]);
        if (!exec_path)
        {
            printf("miniahell: %s: command not found\n", (*cmd)->args[0]);
            exit(127);
        }
        execve(exec_path, (*cmd)->args, envp);
        perror("execve");
        exit(EXIT_FAILURE);
    }
    else if (pid > 0)
    {
        int status;
        waitpid(pid, &status, 0);
        signal(SIGINT, old_sigint);
        signal(SIGQUIT, old_sigquit);
        if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
            write(1, "\n", 1);
    }
    else
    {
        perror("fork");
        signal(SIGINT, old_sigint);
        signal(SIGQUIT, old_sigquit);
    }
}
