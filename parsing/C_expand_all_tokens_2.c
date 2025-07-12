#include "../minishell.h"

char	*get_shell_var(t_var *vars, char *name)
{
	while (vars)
	{
		if (strcmp(vars->name, name) == 0)
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