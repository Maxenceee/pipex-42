/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgama <mgama@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/04 14:46:57 by mgama             #+#    #+#             */
/*   Updated: 2022/12/05 17:19:24 by mgama            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/wait.h>
# include <errno.h>
# include <fcntl.h>
# include "../printf/ft_printf.h"

typedef struct	s_commands {
	char	*path_command1;
	char	**command1;
	char	*file1;
	char	*path_command2;
	char	**command2;
	char	*file2;
}				t_commands;

char	*ft_strnstr(const char *str, const char *to_find, size_t n);

size_t	ft_strlen(const char *str);
char	*ft_strjoin_arr(int size, char **strs, char *sep);
char	*ft_strtrim(char const *s1, char const *set);
char	**ft_split(const char *str, char charset);

#endif