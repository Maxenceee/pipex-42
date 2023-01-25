/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   childs.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgama <mgama@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/10 19:21:21 by mgama             #+#    #+#             */
/*   Updated: 2023/01/25 21:32:57 by mgama            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

void	exit_with_code(t_commands *commands, int code)
{
	int	i;
	int	j;

	i = 0;
	while (commands->command_list[i] != NULL)
	{
		j = 0;
		while (commands->command_list[i][j])
			free(commands->command_list[i][j++]);
		free(commands->command_list[i++]);
	}
	free(commands->command_list);
	i = 0;
	if (code > 0)
		perror("An error occurred while executing the program");
	exit(code);
}

void	first_child(t_commands *commands)
{
	int		fdin;
	int		fdout;
	int		res;

	fdin = open(commands->input, O_RDONLY);
	if (fdin == -1)
		exit_with_code(commands, 1);
	close(commands->pipe[0]);
	fdout = commands->pipe[1];
	res = execcmd(fdin, fdout, commands->command_list[0], commands->envp);
	if (res == 5)
		perror(PERM_DENIED);
	else if (res == 2)
		ft_putstr_fd(commands->command_list[0][0], NO_COMMAND, 2);
}

void	second_child(t_commands *commands)
{
	int		fdin;
	int		fdout;
	int		res;

	fdin = commands->pipe[0];
	fdout = open(commands->output, O_WRONLY | O_CREAT | O_TRUNC, 0777);
	if (fdout == -1)
		exit_with_code(commands, 1);
	close(commands->pipe[1]);
	res = execcmd(fdin, fdout, commands->command_list[1], commands->envp);
	if (res == 5)
		perror(PERM_DENIED);
	else if (res == 2)
		ft_putstr_fd(commands->command_list[1][0], NO_COMMAND, 2);
}
