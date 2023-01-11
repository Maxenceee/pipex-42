/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgama <mgama@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/04 14:46:57 by mgama             #+#    #+#             */
/*   Updated: 2023/01/08 15:09:48 by mgama            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/wait.h>
# include <signal.h>
# include <errno.h>
# include <fcntl.h>
# include "../printf/ft_printf.h"

typedef struct	s_commands {
	char	***command_list;
	char	*input;
	char	*output;
	int		process_count;
	char	**envp;
}				t_commands;

char	*ft_strnstr(const char *str, const char *to_find, size_t n);

void	*ft_calloc(size_t count, size_t size);
// size_t	ft_strlen(const char *str);
char	*ft_strjoin_arr(int size, char **strs, char *sep);
char	*ft_strtrim(char const *s1, char const *set);
char	**ft_split(const char *str, char charset);

#endif