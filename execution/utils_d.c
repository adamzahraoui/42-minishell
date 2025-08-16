/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_d.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adzahrao <adzahrao@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 21:26:04 by akira             #+#    #+#             */
/*   Updated: 2025/08/16 14:00:10 by adzahrao         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	add_back(t_myenv_ex **myenv_ex, char *str)
{
	t_myenv_ex	*list;
	t_myenv_ex	*new;

	list = *myenv_ex;
	new = ft_malloc(sizeof(t_myenv_ex));
	new->data = ft_strdup_gc(str);
	new->next = NULL;
	if (!list)
	{
		*myenv_ex = new;
		return ;
	}
	while (list->next)
		list = list->next;
	list->next = new;
}

void	set_variables(t_cmd **cmd, t_myenv **myenv, t_myenv_ex **myenv_ex,
		char **env)
{
	*cmd = NULL;
	*myenv = NULL;
	*myenv_ex = NULL;
	declare_env(myenv, myenv_ex, env);
	(*myenv)->i = 0;
	setup_signals();
}

char	*ft_strjoin_gc(char const *s1, char const *s2)
{
	size_t	i;
	size_t	j;
	char	*p;

	if (!s1 || !s2)
		return (NULL);
	i = 0;
	j = 0;
	while (s1[i] != '\0')
		i++;
	while (s2[j] != '\0')
		j++;
	p = ft_malloc((i + j + 1) * sizeof(char));
	ft_memmove(p, s1, i);
	ft_memmove(p + i, s2, j);
	p[i + j] = '\0';
	return (p);
}

int	check_dup_red(t_cmd *cmd, int flag)
{
	if (flag == 1)
	{
		if (dup2(cmd->saved_stdin, 0) == -1)
			return (-1);
		close(cmd->saved_stdin);
		cmd->saved_stdin = -1;
	}
	if (flag == 2)
	{
		if (dup2(cmd->saved_stdout, 1) == -1)
			return (-1);
		close(cmd->saved_stdout);
		cmd->saved_stdout = -1;
	}
	return (1);
}
