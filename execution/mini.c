/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adzahrao <adzahrao@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 18:52:49 by adzahrao          #+#    #+#             */
/*   Updated: 2025/07/13 10:03:41 by adzahrao         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void add_env_node(t_myenv **myenv, char *env)
{
    t_myenv *new_node;
    t_myenv *last;

    new_node = malloc(sizeof(t_myenv));
    if (!new_node)
        return;
    new_node->data = ft_strdup(env);
    if (!new_node->data)
        return;
    new_node->next = NULL;
    if (*myenv == NULL)
    {
        *myenv = new_node;
        return;
    }
    last = *myenv;
    while (last->next)
        last = last->next;
    last->next = new_node;
}

void set_env(t_myenv **myenv, char **env)
{
    int y;

    y = 0;
    while (env[y])
    {
        add_env_node(myenv, env[y]);
        y++;
    }
}

void ft_ft_free(char **str)
{
    int i;

    i = 0;
    while (str[i])
    {
        free(str[i]);
        i++;
    }
    free(str);
}

void declare_env(t_myenv **myenv, t_myenv_ex **myenv_ex, char **env)
{
    set_env(myenv, env);
    set_env_ex(myenv_ex, env);
}

void cmd_ex(t_exec_context *ctx)
{
    char **path;

    (void)ctx->tokens;
    path = my_get_path_split(ctx->myenv, "PATH=", ':');

    if (check_builtin_cmd(ctx->args, *(ctx->myenv), *(ctx->myenv_ex)) == 1)
    {
        ft_ft_free(path);
    }
    else
    {
        external_executables(ctx->args, path, ctx->env);
        ft_ft_free(path);
    }
    if ((*(ctx->args))->saved_stdin != -1)
    {
        dup2((*(ctx->args))->saved_stdin, STDIN_FILENO);
        close((*(ctx->args))->saved_stdin);
        (*(ctx->args))->saved_stdin = -1;
    }
    free_commands(*(ctx->args));
    *(ctx->args) = NULL;
}

