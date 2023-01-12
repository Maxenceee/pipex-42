/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgama <mgama@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/04 14:46:57 by mgama             #+#    #+#             */
/*   Updated: 2023/01/12 18:31:37 by mgama            ###   ########.fr       */
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

typedef struct s_commands {
	char	***command_list;
	pid_t	pid1;
	pid_t	pid2;
	int		pipe[2];
	char	*input;
	char	*output;
	int		process_count;
	char	**envp;
}				t_commands;

int		execcmd(int fdin, int fdout, char **command, char *envp[]);
void	fork_process(t_commands *commands);

void	first_child(t_commands *commands);
void	second_child(t_commands *commands);
void	exit_with_code(t_commands *commands, int code);

char	*parse_env(char *envp[], char *cmd);
void	parse_commands(t_commands *commands, char *argv[], int argc);

char	*ft_strnstr(const char *str, const char *to_find, size_t n);
void	*ft_calloc(size_t count, size_t size);
char	*ft_strjoin_arr(int size, char **strs, char *sep);
char	*ft_strtrim(char const *s1, char const *set);
char	**ft_split(const char *str, char charset);

#endif