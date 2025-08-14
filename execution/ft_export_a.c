/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export_a.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akira <akira@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 15:47:16 by adzahrao          #+#    #+#             */
/*   Updated: 2025/08/14 15:52:32 by akira            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	check_arg_export(t_myenv **myenv, t_cmd *str, int i)
{
	int		equals_index;
	char	*arg;

	arg = str->args[i];
	equals_index = find_equals_index(arg);
	if (!is_valid_first_char(arg[0]))
	{
		print_error_ex(myenv, arg);
		return (0);
	}
	if (!validate_chars_before_equals(arg, equals_index, myenv))
		return (0);
	return (1);
}

void	process_export_arg(t_myenv_ex **myenv_ex, t_myenv **myenv, t_cmd *cmd,
		int i)
{
	char	*egual;

	if (check_arg_export(myenv, cmd, i) == 1)
	{
		set_env_doubl(myenv, cmd->args[i]);
		egual = check_val(cmd->args[i]);
		if (check_double(myenv_ex, cmd->args[i]) == 1)
		{
			if (egual != NULL)
				add_back(myenv_ex, egual);
			else
				add_back(myenv_ex, cmd->args[i]);
		}
		ft_free_one(egual);
	}
}

static void	print_export_data(char *data)
{
	int	i;
	int	f;

	i = 0;
	f = 0;
	while (data[i])
	{
		printf("%c", data[i]);
		if (data[i] == '=' && f == 0)
		{
			printf("\"");
			f = 1;
		}
		else if (data[i + 1] == '\0' && f == 1)
			printf("\"");
		i++;
	}
	printf("\n");
}

void	print_export(t_myenv_ex **myenv_ex)
{
	char		**split;
	t_myenv_ex	*pr;

	if (!myenv_ex || !*myenv_ex)
		return ;
	pr = *myenv_ex;
	while (pr)
	{
		split = ft_split(pr->data, '=');
		if (ft_strcmp(split[0], "$?"))
		{
			printf("declare -x ");
			if (pr->data != NULL)
				print_export_data(pr->data);
		}
		pr = pr->next;
	}
}

void	ft_export(t_myenv_ex **myenv_ex, t_myenv **myenv, t_cmd **str)
{
	t_cmd	*cmd;
	int		i;

	cmd = *str;
	i = 1;
	sort_export(myenv_ex);
	if (cmd->args[1] != NULL)
	{
		while (cmd->args[i])
		{
			process_export_arg(myenv_ex, myenv, cmd, i);
			i++;
		}
	}
	else
		print_export(myenv_ex);
}
