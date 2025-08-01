/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pipe.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adzahrao <adzahrao@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 00:49:06 by adzahrao          #+#    #+#             */
/*   Updated: 2025/08/01 04:39:03 by adzahrao         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void ft_pipe(t_cmd **cmd, char **path, t_myenv **myenv, char **or_env)
{
    t_cmd *arg = *cmd;
    t_myenv *env = *myenv;
    int prev_fd = -1;
    int i = 0;

    // Count how many commands
    int count = 0;
    for (t_cmd *tmp = arg; tmp; tmp = tmp->next)
        count++;

    int *pids = malloc(sizeof(int) * count);
    if (!pids)
        return;

    while (arg)
    {
        if (arg->next && pipe(env->fd) == -1)
        {
            perror("pipe");
            return;
        }

        pids[i] = fork();
        if (pids[i] == -1)
        {
            perror("fork");
            return;
        }

        if (pids[i] == 0)
        {
            if (prev_fd != -1)
            {
                dup2(prev_fd, 0);
                close(prev_fd);
            }
            if (arg->next)
            {
                dup2(env->fd[1], 1);
                close(env->fd[0]);
                close(env->fd[1]);
            }

            char *exec_path;
            if (ft_strchr(arg->args[0], '/'))
                exec_path = arg->args[0];
            else
                exec_path = check_cmd(path, arg->args[0]);

            if (!exec_path)
            {
                printf("minishell: %s: command not found\n", arg->args[0]);
                exit(127);
            }

            execve(exec_path, arg->args, or_env);
            perror("execve");
            exit(EXIT_FAILURE);
        }

        if (prev_fd != -1)
            close(prev_fd);

        if (arg->next)
        {
            close(env->fd[1]);
            prev_fd = env->fd[0];
        }
        else
        {
            prev_fd = -1;
        }
        arg = arg->next;
        i++;
    }

    for (int j = 0; j < count; j++)
        waitpid(pids[j], NULL, 0);

    free(pids);
}



// void    ft_pipe(t_cmd **cmd, char **path, t_myenv **myenv, char **or_env)
// {
//     t_cmd *arg;
//     t_myenv *env;
//     int count;
//     int *arr;
//     int i = 0;
//     count = 0;
//     arg = *cmd;
//     env = *myenv;
//     int prev_fd = -1; // Save previous read-end
    
//     while(arg)
//     {
//         arg = arg->next;
//         count++;
//     }
//     arg = *cmd;
//     arr = malloc(sizeof(int) * count);
//     if(!arr)
//         return ;
//     while(arg)
//     {
//         if(arg->next && pipe(env->fd) == -1)
//             perror("error in pipe");
//         arr[i] = fork();
//         if(arr[i] == 0)
//         {
//             if(prev_fd != -1)
//             {
//                 dup2(prev_fd, 0);
//                 close(prev_fd);
//             }
//             if(i < count - 1)
//             {
//                 dup2(env->fd[1], 1);
//                 close(env->fd[1]);
//                 close(env->fd[0]);
//             }
//             char *exec_path;
//             if (ft_strchr(arg->args[0], '/'))
//                 exec_path = arg->args[0];
//             else
//                 exec_path = check_cmd(path, arg->args[0]);
//             if (!exec_path)
//             {
//                 printf("miniahell: %s: command not found\n", arg->args[0]);
//                 exit(127);
//             }
//             execve(exec_path, arg->args, or_env);
//             perror("execve");
//             exit(EXIT_FAILURE);          
//         }   
//         arg = arg->next;
//         if (prev_fd != -1)
//             close(prev_fd);
//         if (arg->next)
//         {
//             close(env->fd[1]);
//             prev_fd = env->fd[0];
//         }
//         else
//         {
//             prev_fd = -1; // nothing more to pass
//         }   

//         i++;
//     }
//     close(prev_fd);
//     i = 0;
//     while (i < count)
//         waitpid(arr[i++], NULL, 0);
// }
