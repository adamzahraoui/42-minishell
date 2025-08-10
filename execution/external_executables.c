/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   external_executables.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akira <akira@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 05:10:57 by adzahrao          #+#    #+#             */
/*   Updated: 2025/08/10 02:06:40 by akira            ###   ########.fr       */
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

void    external_executables(t_cmd **cmd, char **path, char **envp, t_myenv **myenv)
{
    pid_t pid;
    void (*old_sigint)(int);
    void (*old_sigquit)(int);

    if (!(*cmd)->args[0])
        return ;
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
        {
            exec_path = (*cmd)->args[0];
            if (access(exec_path, F_OK) == -1)
            {
                ft_putstr_fd("minishell: ", 2);
                ft_putstr_fd((*cmd)->args[0], 2);
                ft_putstr_fd(": No such file or directory\n", 2);
                exit(127);
            }
            if (access(exec_path, X_OK) == -1)
            {
                ft_putstr_fd("minishell: ", 2);
                ft_putstr_fd((*cmd)->args[0], 2);
                ft_putstr_fd(": Permission denied\n", 2);
                exit(126);
            }
        }
        else
        {
            exec_path = check_cmd(path, (*cmd)->args[0]);
        }
        if (!exec_path)
        {
            ft_putstr_fd("minishell: ", 2);
            ft_putstr_fd((*cmd)->args[0], 2);
            ft_putstr_fd(": command not found\n", 2);
            exit(127);
        }
        execve(exec_path, (*cmd)->args, envp);
        if (errno == EACCES)
        {
            ft_putstr_fd("minishell: ", 2);
            ft_putstr_fd((*cmd)->args[0], 2);
            ft_putstr_fd(": Permission denied\n", 2);
            exit(126);
        }
        ft_putstr_fd("minishell: execve error\n", 2);
        exit(EXIT_FAILURE);
    }
    else if (pid > 0)
    {
        int status;
        waitpid(pid, &status, 0);
        if (WIFEXITED(status))
            set_status(myenv, NULL, WEXITSTATUS(status));
        else if (WIFSIGNALED(status))
            set_status(myenv, NULL, 128 + WTERMSIG(status));
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
