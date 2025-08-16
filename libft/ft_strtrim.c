/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adzahrao <adzahrao@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 18:31:39 by adzahrao          #+#    #+#             */
/*   Updated: 2025/08/16 16:52:55 by adzahrao         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	chack_set(char c, const char *set)
{
	int	i;

	i = 0;
	while (set[i] != '\0')
	{
		if (set[i] == c)
			return (1);
		i++;
	}
	return (0);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	char	*p;
	int		s;
	int		e;
	size_t	l;

	if (!s1 || !set)
		return (NULL);
	s = 0;
	while (s1[s] && chack_set(s1[s], set))
		s++;
	e = ft_strlen(s1) - 1;
	while (e > s && chack_set(s1[e], set))
		e--;
	l = e - s + 1;
	p = ft_malloc((l + 1) * sizeof(char));
	ft_strlcpy(p, &s1[s], l + 1);
	return (p);
}
