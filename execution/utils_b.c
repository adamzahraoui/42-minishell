/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_b.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adzahrao <adzahrao@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 12:51:55 by adzahrao          #+#    #+#             */
/*   Updated: 2025/08/02 18:50:36 by adzahrao         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    set_status(t_myenv **myenv, char *str, int status)
{
    t_myenv *env;
   
    env = *myenv;
	if(str != NULL)
    	perror(str);
    env->i = status;
	check_double_env(myenv, ft_strjoin("?=", ft_itoa((*myenv)->i)));
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
    ft_putstr_fd(" :not a valid identifier\n", 2);
}