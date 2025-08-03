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