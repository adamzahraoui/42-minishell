/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_b.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akira <akira@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 12:51:55 by adzahrao          #+#    #+#             */
/*   Updated: 2025/08/12 00:13:27 by akira            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	set_status(t_myenv **myenv, char *str, int status)
{
	t_myenv	*env;
	char	*p;
	char	*num;

	env = *myenv;
	if (str != NULL)
		perror(str);
	env->i = status;
	num = ft_itoa(status);
	if (!num)
		return ;
	p = ft_strjoin("?=", num);
	free(num);
	if (p)
	{
		check_double_env(myenv, p);
		free(p);
	}
}

int	ft_strncmp_nv(const char *s1, const char *s2, size_t n)
{
	size_t			i;
	unsigned char	*s;
	unsigned char	*d;

	i = 0;
	s = (unsigned char *)s1;
	d = (unsigned char *)s2;
	while (i < n)
	{
		if (s[i] != d[i])
			return (s[i] - d[i]);
		i++;
	}
	return (0);
}

void	print_error_ex(t_myenv **myenv, char *str)
{
	set_status(myenv, NULL, 1);
	ft_putstr_fd("minishell: export: ", 2);
	ft_putstr_fd(str, 2);
	ft_putstr_fd(": not a valid identifier\n", 2);
}

char	*check_cmd(char **path, char *cmd)
{
	int		i;
	char	*str;
	char	*tmp;

	i = 0;
	if (!path || !path[0])
		return (NULL);
	while (path[i])
	{
		tmp = ft_strjoin_gc(path[i], "/");
		str = ft_strjoin_gc(tmp, cmd);
		if (access(str, X_OK) == 0)
			return (str);
		i++;
	}
	return (NULL);
}
