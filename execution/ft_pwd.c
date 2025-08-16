/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adzahrao <adzahrao@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 15:47:09 by adzahrao          #+#    #+#             */
/*   Updated: 2025/08/16 16:51:30 by adzahrao         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_pwd(t_myenv **myenv)
{
	char	*pwd;

	(void)myenv;
	pwd = getcwd(NULL, 0);
	if (!pwd)
	{
		set_status(1);
		perror("error in pwd\n");
		return ;
	}
	printf("%s\n", pwd);
	free(pwd);
	set_status(0);
}
