/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgama <mgama@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/04 14:46:50 by mgama             #+#    #+#             */
/*   Updated: 2023/01/13 15:26:09 by mgama            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

int	execcmd(int fdin, int fdout, char **command, char *envp[])
{
	char		*cmd;

	if (dup2(fdin, STDIN_FILENO) < 0)
		return (3);
	close(fdin);
	if (dup2(fdout, STDOUT_FILENO) < 0)
		return (3);
	close(fdout);
	if (command[0] == NULL)
		return (5);
	cmd = parse_env(envp, command[0]);
	if (!cmd)
		return (2);
	if (execve(cmd, command, envp) == -1)
	{
		perror("Could not execute execve");
		return (1);
	}
	return (0);
}

void	fork_process(t_commands *commands)
{
	if (pipe(commands->pipe) == -1)
		exit_with_code(commands, 2);
	commands->pid1 = fork();
	if (commands->pid1 == -1)
		exit_with_code(commands, 1);
	if (commands->pid1 == 0)
		first_child(commands);
	commands->pid2 = fork();
	if (commands->pid2 == -1)
		exit_with_code(commands, 1);
	if (commands->pid2 == 0)
		second_child(commands);
	close(commands->pipe[0]);
	close(commands->pipe[1]);
	waitpid(commands->pid1, NULL, 0);
	waitpid(commands->pid2, NULL, 0);
}

int	main(int argc, char *argv[], char *envp[])
{
	t_commands	commands;
	int			file;
	char		*cmd;

	if (argc < 5)
	{
		ft_printf("Missing arguments. You must provied at least 4 arguments.");
		return (1);
	}
	parse_commands(&commands, argv, argc);
	commands.envp = envp;
	fork_process(&commands);
	exit_with_code(&commands, 0);
}
