/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akira <akira@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 16:39:55 by akira             #+#    #+#             */
/*   Updated: 2025/08/10 16:44:40 by akira            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_valid_n_flag(const char *str)
{
	int	i;

	i = 1;
	if (str[0] != '-' || str[1] != 'n')
		return (0);
	while (str[i])
	{
		if (str[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

void	ft_echo(t_cmd *cmd, t_myenv **myenv)
{
	int	i;
	int	newline;
	int	j;

	(void)myenv;
	(1) && (i = 1, newline = 1, j = 0);
	while (cmd->args[i] && is_valid_n_flag(cmd->args[i]))
	{
		newline = 0;
		i++;
	}
	while (cmd->args[i++])
	{
		j = 0;
		while (cmd->args[i][j])
		{
			printf("%c", cmd->args[i][j]);
			j++;
		}
		if (cmd->args[i + 1])
			printf(" ");
	}
	if (newline)
		printf("\n");
	set_status(myenv, NULL, 0);
}
