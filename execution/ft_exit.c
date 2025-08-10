/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akira <akira@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 15:47:07 by adzahrao          #+#    #+#             */
/*   Updated: 2025/08/10 01:31:22 by akira            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <limits.h>
#include <errno.h>

void	print_error_status(t_myenv **myenv, char *str, int status)
{
    set_status(myenv, NULL, status);
    ft_putstr_fd("minishell: exit: ", 2);
    ft_putstr_fd(str, 2);
    ft_putstr_fd(": numeric argument required\n", 2);
}

// Checks if str is a valid signed 64-bit integer
int is_longlong(const char *str)
{
    char *endptr;
    errno = 0;
    // skip leading spaces
    while (*str == ' ' || *str == '\t')
        str++;
    long long val = strtoll(str, &endptr, 10);
    if (errno == ERANGE || *endptr != '\0')
        return 0;
    (void)val;
    return 1;
}

int     return_status(t_cmd *cmd, t_myenv **myenv)
{
    int i;
    int j;

    i = 0;
    j = 0;
    if (cmd->args[1])
    {
        j = 0;
        while(cmd->args[1][j])
        {
            if (ft_isdigit(cmd->args[1][j]) == 0)
            {
                print_error_status(myenv, cmd->args[i], 1);
                exit(1);
            }
            j++;
        }
    }
    else if (cmd->args[2])
    {
        print_error_status(myenv, cmd->args[i], 2);
        return (2);
    }
    return (0);
}


static int is_numeric(const char *str)
{
    int i = 0;
    if (str[0] == '-' || str[0] == '+')
        i++;
    if (!str[i])
        return 0;
    while (str[i]) {
        if (!ft_isdigit(str[i]))
            return 0;
        i++;
    }
    return 1;
}

void    ft_exit(t_myenv_ex **myenv_ex, t_myenv **myenv, t_cmd *cmd)
{
    int exit_status;
    
    exit_status = 0;
    (void) myenv_ex;
    printf("exit\n");
    if (!cmd->args[1])
        exit(0);
    if (!is_numeric(cmd->args[1]) || !is_longlong(cmd->args[1])) {
        print_error_status(myenv, cmd->args[1], 2);
        exit(2);
    }
    if (cmd->args[2]) {
        ft_putstr_fd("minishell: exit: too many arguments\n", 2);
        set_status(myenv, NULL, 1);
        return;
    }
    exit_status = ft_atoi(cmd->args[1]);
    exit((unsigned char)exit_status);
}

