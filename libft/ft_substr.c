/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akira <akira@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 16:59:04 by adzahrao          #+#    #+#             */
/*   Updated: 2025/08/14 16:26:10 by akira            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	size_t	i;
	char	*p;

	if (s == NULL)
		return (NULL);
	if (start > ft_strlen(s))
		return (ft_strdup_gc(""));
	if (len > ft_strlen(s) - start)
		len = ft_strlen(s) - start;
	p = ft_malloc((len + 1) * sizeof(char));
	if (!p)
		return (NULL);
	i = 0;
	while (s[start + i] != '\0' && i < len)
	{
		p[i] = s[start + i];
		i++;
	}
	p[i] = '\0';
	return (p);
}
