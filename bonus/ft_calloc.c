/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgama <mgama@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/07 19:40:47 by mgama             #+#    #+#             */
/*   Updated: 2023/01/11 19:08:55 by mgama            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex_bonus.h"

// static void	*ft_memset(void *b, int c, size_t len)
// {
// 	size_t	i;

// 	i = 0;
// 	while (i < len)
// 	{
// 		((char *)b)[i] = (unsigned char)c;
// 		i++;
// 	}
// 	return (b);
// }

// static void	ft_bzero(void *s, size_t n)
// {
// 	ft_memset(s, '\0', n);
// }

void	*ft_calloc(size_t count, size_t size)
{
	void	*ptr;

	if (count == 0 || size == 0)
	{
		count = 1;
		size = 1;
	}
	if (SIZE_MAX / count < size)
		return (NULL);
	ptr = malloc(count * size);
	if (ptr)
		ft_bzero(ptr, count * size);
	return (ptr);
}
