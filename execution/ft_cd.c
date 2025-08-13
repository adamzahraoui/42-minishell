/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akira <akira@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 16:09:59 by adzahrao          #+#    #+#             */
/*   Updated: 2025/08/12 00:45:40 by akira            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_strlen_cmd(t_cmd **cmd)
{
	int		i;
	t_cmd	*str;

	str = *cmd;
	i = 0;
	while (str->args[i])
		i++;
	return (i);
}

void	update_path(char *p, t_myenv **myenv, t_myenv_ex **myenv_ex)
{
	check_double(myenv_ex, p);
	check_double_env(myenv, p);
}

int	just_cd(char *temp, t_myenv **myenv, t_myenv_ex **myenv_ex)
{
	char	*path;
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (!cwd)
		return (set_status(1), 0);
	temp = ft_strjoin_gc("OLDPWD=", cwd);
	free(cwd);
	update_path(temp, myenv, myenv_ex);
	path = my_get_path(*myenv, "HOME=");
	if (path == NULL)
	{
		ft_putstr_fd("minishell: cd: HOME not set\n", 2);
		set_status(1);
		return (0);
	}
	if (chdir(&path[0]) == -1)
	{
		ft_putstr_fd("minishell: cd: ", 2);
		ft_putstr_fd(&path[0], 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		return (set_status(1), 0);
	}
	set_status(0);
	return (1);
}

int	exist_path(t_cmd **cmd, char *temp, t_myenv **myenv, t_myenv_ex **myenv_ex)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (!cwd)
		return (set_status(1), 0);
	temp = ft_strjoin_gc("OLDPWD=", cwd);
	free(cwd);
	update_path(temp, myenv, myenv_ex);
	if (chdir((*cmd)->args[1]) == -1)
	{
		ft_putstr_fd("minishell: cd: ", 2);
		ft_putstr_fd((*cmd)->args[1], 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		set_status(1);
		return (0);
	}
	set_status(0);
	return (1);
}

void	ft_cd(t_cmd **cmd, t_myenv **myenv, t_myenv_ex **myenv_ex)
{
	char	*temp;
	char	*cwd;

	temp = NULL;
	if ((*cmd)->args[1] == NULL)
	{
		if (!just_cd(temp, myenv, myenv_ex))
			return ;
	}
	else if (ft_strlen_cmd(cmd) == 2)
	{
		if (!exist_path(cmd, temp, myenv, myenv_ex))
			return ;
	}
	else
	{
		ft_putstr_fd("minishell: cd: too many arguments\n", 2);
		return (set_status(1));
	}
	cwd = getcwd(NULL, 0);
	if (!cwd)
		return (set_status(1));
	temp = ft_strjoin_gc("PWD=", cwd);
	free(cwd);
	update_path(temp, myenv, myenv_ex);
}
