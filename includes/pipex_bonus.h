/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgama <mgama@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/04 14:46:57 by mgama             #+#    #+#             */
/*   Updated: 2023/01/11 20:18:27 by mgama            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_BONUS_H
# define PIPEX_BONUS_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/wait.h>
# include <signal.h>
# include <errno.h>
# include <fcntl.h>
# include "../printf/ft_printf.h"

# define PIPE_ERROR "Pipe error"
# define EXEVE_ERROR "Could not execute execve"
# define OPEN_IN_ERROR "Infile"
# define OPEN_OUT_ERROR "Outfile"
# define FORK_ERROR "Fork"
# define PERM_DENIED "permission denied"

typedef struct	s_commands {
	char	***command_list;
	char	*input;
	char	*output;
	int		process_count;
	int		*pipes;
	int		pipe_nb;
	char	**envp;
}				t_commands;

char	*parse_env(char *envp[], char *cmd);
void	parse_commands(t_commands *commands, char *argv[], int argc);
void	exit_error_with_msg(t_commands *commands, char *msg);
void	exit_with_code(t_commands *commands, int code);

char	*ft_strnstr(const char *str, const char *to_find, size_t n);
void	*ft_calloc(size_t count, size_t size);
char	*ft_strjoin_arr(int size, char **strs, char *sep);
char	**ft_split(const char *str, char charset);

#endif