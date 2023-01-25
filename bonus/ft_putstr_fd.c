/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putstr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgama <mgama@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/08 12:57:23 by mgama             #+#    #+#             */
/*   Updated: 2023/01/25 21:33:08 by mgama            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

void	ft_putstr_fd(char *s, char *t, int fd)
{
	if (!s || !t)
		return ;
	write(fd, s, ft_strlen(s));
	write(fd, ": ", 2);
	write(fd, t, ft_strlen(t));
	write(fd, "\n", 1);
}
