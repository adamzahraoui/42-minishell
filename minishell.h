/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adzahrao <adzahrao@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 18:45:07 by adzahrao          #+#    #+#             */
/*   Updated: 2025/07/07 05:38:06 by adzahrao         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
#define MINISHELL_H

#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "libft/libft.h"
#include <readline/readline.h>
#include <readline/history.h>

typedef struct s_myenv
{
    char *data;
    struct s_myenv *next;
}t_myenv;

typedef struct s_myenv_ex
{
    char *data;
    struct s_myenv_ex *next;
}t_myenv_ex;


// built-in commands :

void    ft_echo(char **cmd);
void    ft_cd(char **cmd, t_myenv **myenv, t_myenv_ex **myenv_ex);
void    ft_pwd();
void    ft_export(t_myenv_ex **myenv_ex, t_myenv **myenv, char **cmd);
void    set_env_ex(t_myenv_ex **myenv, char **env);
void    ft_unset(t_myenv_ex **myenv_ex, t_myenv **myenv, char *str);
void    print_env(t_myenv *myenv);
void    ft_exit(t_myenv_ex **myenv_ex, t_myenv **myenv);

// external executables

void    external_executables(char **cmd, char **path, char **env);


// help funcion

char    **my_get_path_split(t_myenv *myenv, char *path, char c);
char    *my_get_path(t_myenv *myenv, char *path);
void    add_back_env(t_myenv **myenv, char *str);
void    set_env_doubl(t_myenv **myenv, char *str);
int     check_double(t_myenv_ex **myenv_ex, char *str);
char    *check_val(char *str);
int	    ft_strcmp(char	*s1, char	*s2);
int     check_exist(char *str, char *dest);
int     check_builtin_cmd(char **cmd, t_myenv *myenv, t_myenv_ex *myenv_ex);
void    free_error(char *str, t_myenv **myenv, t_myenv_ex **myenv_ex);
int     check_double_env(t_myenv **myenv, char *str);
#endif