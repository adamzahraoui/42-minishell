/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_a.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adzahrao <adzahrao@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 16:43:17 by adzahrao          #+#    #+#             */
/*   Updated: 2025/08/16 16:51:52 by adzahrao         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	**my_get_path_split(t_myenv **str, char *path, char c)
{
	char	**args;
	char	*av;
	int		i;
	t_myenv	*myenv;

	myenv = *str;
	while (myenv)
	{
		i = 0;
		while (myenv->data[i] == path[i])
			i++;
		if (path[i] == '\0')
		{
			av = &myenv->data[i];
			break ;
		}
		myenv = myenv->next;
	}
	if (path[i] != '\0')
		return (NULL);
	args = ft_split(av, c);
	return (args);
}

char	*my_get_path(t_myenv *myenv, char *path)
{
	char	*av;
	int		i;

	while (myenv->next)
	{
		i = 0;
		while (myenv->data[i] == path[i])
			i++;
		if (path[i] == '\0')
		{
			av = &myenv->data[i];
			break ;
		}
		myenv = myenv->next;
	}
	if (path[i] != '\0')
		return (NULL);
	return (av);
}

int	is_builtin_cmd(char *cmd)
{
	if (!cmd)
		return (0);
	if ((ft_strncmp_nv(cmd, "cd", 2) == 0 && ft_strlen(cmd) == 2)
		|| (ft_strncmp_nv(cmd, "pwd", 3) == 0 && ft_strlen(cmd) == 3)
		|| (ft_strncmp_nv(cmd, "echo", 4) == 0 && ft_strlen(cmd) == 4)
		|| (ft_strncmp_nv(cmd, "export", 6) == 0 && ft_strlen(cmd) == 6)
		|| (ft_strncmp_nv(cmd, "unset", 5) == 0 && ft_strlen(cmd) == 5)
		|| (ft_strncmp_nv(cmd, "env", 3) == 0 && ft_strlen(cmd) == 3)
		|| (ft_strncmp_nv(cmd, "exit", 4) == 0 && ft_strlen(cmd) == 4))
		return (1);
	return (0);
}

void	execute_builtin(t_cmd **str, t_myenv **myenv, t_myenv_ex **myenv_ex)
{
	t_cmd	*cmd;

	cmd = *str;
	if (ft_strncmp_nv(cmd->args[0], "cd", 2) == 0
		&& ft_strlen(cmd->args[0]) == 2)
		ft_cd(str, myenv, myenv_ex);
	else if (ft_strncmp_nv(cmd->args[0], "pwd", 3) == 0
		&& ft_strlen(cmd->args[0]) == 3)
		ft_pwd(myenv);
	else if (ft_strncmp_nv(cmd->args[0], "echo", 4) == 0
		&& ft_strlen(cmd->args[0]) == 4)
		ft_echo(cmd, myenv);
	else if (ft_strncmp_nv(cmd->args[0], "export", 6) == 0
		&& ft_strlen(cmd->args[0]) == 6)
		ft_export(myenv_ex, myenv, str);
	else if (ft_strncmp_nv(cmd->args[0], "unset", 5) == 0
		&& ft_strlen(cmd->args[0]) == 5)
		ft_unset(myenv_ex, myenv, cmd);
	else if (ft_strncmp_nv(cmd->args[0], "env", 3) == 0
		&& ft_strlen(cmd->args[0]) == 3)
		print_env(*myenv);
	else if (ft_strncmp_nv(cmd->args[0], "exit", 4) == 0
		&& ft_strlen(cmd->args[0]) == 4)
		ft_exit(myenv_ex, myenv, cmd);
}

int	check_builtin_cmd(t_cmd **str, t_myenv **myenv, t_myenv_ex **myenv_ex)
{
	t_cmd	*cmd;

	cmd = *str;
	if (!cmd->args[0])
		return (0);
	if (is_builtin_cmd(cmd->args[0]))
	{
		if (cmd->redirections)
		{
			if (redirection(cmd))
			{
				set_status(1);
				return (1);
			}
		}
		execute_builtin(str, myenv, myenv_ex);
		if (cmd->redirections)
			restor_fd(cmd);
		return (1);
	}
	return (0);
}
