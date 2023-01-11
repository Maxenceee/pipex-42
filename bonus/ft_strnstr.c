/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgama <mgama@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/07 14:03:02 by mgama             #+#    #+#             */
/*   Updated: 2023/01/11 19:08:45 by mgama            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex_bonus.h"

static char	*substrnstr(const char *str, const char *to_find, size_t n)
{
	size_t		i;
	size_t		j;
	size_t		k;
	int			found;

	i = -1;
	found = 1;
	while (*(str + ++i) && i < n)
	{
		j = 0;
		if (*(str + i) == *(to_find + 0))
		{
			k = i;
			found = 1;
			while (*(str + k) && *(to_find + j) && j < n && k < n)
				if (*(str + k++) != *(to_find + j++))
					found = 0;
			if (found && !*(to_find + j))
				return ((char *)str + i);
		}
	}
	return (NULL);
}

char	*ft_strnstr(const char *str, const char *to_find, size_t n)
{
	if (n == 0 && !str)
		return (0);
	if (!ft_strlen(to_find))
		return ((char *)str);
	return (substrnstr(str, to_find, n));
}
