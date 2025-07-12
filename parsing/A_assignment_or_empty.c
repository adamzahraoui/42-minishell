#include "../minishell.h"

int	handle_assignment_or_empty(char *input, t_var **vars)
{
	char	*eq;

	if (*input == '\0')
	{
		free(input);
		return (1);
	}
	eq = ft_strchr(input, '=');
	if (eq && eq != input && ft_strchr(input, ' ') == NULL)
	{
		*eq = '\0';
		set_shell_var(vars, input, eq + 1);
		free(input);
		return (1);
	}
	return (0);
}

void	set_shell_var(t_var **vars, char *name, char *value)
{
	t_var	*cur;
	t_var	*new;

	cur = *vars;
	while (cur)
	{
		if (ft_strncmp(cur->name, name, ft_strlen(name)) == 0)
		{
			free(cur->value);
			cur->value = ft_strdup(value);
			return ;
		}
		cur = cur->next;
	}
	new = malloc(sizeof(t_var));
	new->name = ft_strdup(name);
	new->value = ft_strdup(value);
	new->next = *vars;
	*vars = new;
}