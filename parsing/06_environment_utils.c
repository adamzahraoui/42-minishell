/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   06_environment_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akira <akira@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 00:07:01 by mlaidi            #+#    #+#             */
/*   Updated: 2025/08/14 16:30:58 by akira            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*get_shell_var(t_var *vars, char *name)
{
	if (!vars || !name)
		return (NULL);
	while (vars)
	{
		if (ft_strcmp(vars->name, name) == 0)
			return (vars->value);
		vars = vars->next;
	}
	return (NULL);
}

char	*get_env_value(char **env, char *name)
{
	int	i;
	int	name_len;

	if (!env || !name)
		return (NULL);
	name_len = ft_strlen(name);
	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], name, name_len) == 0 && env[i][name_len] == '=')
			return (env[i] + name_len + 1);
		i++;
	}
	return (NULL);
}

char	**convert_myenv_to_env(t_myenv *myenv)
{
	t_myenv	*temp;
	char	**env;
	int		count;
	int		i;

	count = 0;
	i = 0;
	temp = myenv;
	while (temp)
	{
		count++;
		temp = temp->next;
	}
	env = ft_malloc(sizeof(char *) * (count + 1));
	temp = myenv;
	env = env_copy_loop(temp, env, count, i);
	if (!env)
		return (NULL);
	env[count] = NULL;
	return (env);
}

char	**env_copy_loop(t_myenv *temp, char **env, int count, int i)
{
	while (temp && i < count)
	{
		env[i] = ft_strdup_gc(temp->data);
		temp = temp->next;
		i++;
	}
	return (env);
}
