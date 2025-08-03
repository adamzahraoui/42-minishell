/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   06_environment_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adzahrao <adzahrao@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 00:07:01 by mlaidi            #+#    #+#             */
/*   Updated: 2025/08/03 03:45:19 by adzahrao         ###   ########.fr       */
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
	env = malloc(sizeof(char *) * (count + 1));
	if (!env)
		return (NULL);
	temp = myenv;
	while (temp && i < count)
	{
		env[i] = ft_strdup(temp->data);
		temp = temp->next;
		i++;
	}
	env[i] = NULL;
	return (env);
}

char	*get_myenv_value(t_myenv *myenv, char *name)
{
	int	name_len;

	if (!myenv || !name)
		return (NULL);
	name_len = ft_strlen(name);
	while (myenv)
	{
		if (ft_strncmp(myenv->data, name, name_len) == 0
			&& myenv->data[name_len] == '=')
			return (myenv->data + name_len + 1);
		myenv = myenv->next;
	}
	return (NULL);
}

int	is_redirection(t_token_type type)
{
	return (type == TOKEN_REDIR_IN || type == TOKEN_REDIR_OUT
		|| type == TOKEN_HEREDOC || type == TOKEN_APPEND);
}
