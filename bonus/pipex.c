/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgama <mgama@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/04 14:46:50 by mgama             #+#    #+#             */
/*   Updated: 2023/01/25 23:39:55 by mgama            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex_bonus.h"

static int	execcmd(char **command, char *envp[])
{
	char		*cmd;

	if (command[0] == NULL)
		return (5);
	cmd = parse_env(envp, command[0]);
	if (!cmd || ft_strcmp(command[0], "") == 0)
		return (2);
	if (execve(cmd, command, envp) == -1)
	{
		perror("Could not execute execve");
		return (1);
	}
	return (0);
}

static int	open_fdinout(int idx, t_commands *c)
{
	int	fdin;
	int	fdout;

	if (idx == 0)
	{
		fdin = open(c->input, O_RDONLY);
		if (fdin == -1)
			perror(OPEN_IN_ERROR);
		if (dup2_fdinout(fdin, c->pipes[1]) < 0)
			return (1);
	}
	else if (idx == c->process_count - 1)
	{
		fdout = open(c->output, O_WRONLY | O_CREAT | O_TRUNC, 0777);
		if (fdout == -1)
			exit_error_with_msg(c, OPEN_OUT_ERROR);
		if (dup2_fdinout(c->pipes[2 * idx - 2], fdout) < 0)
			return (1);
	}
	else
		if (dup2_fdinout(c->pipes[2 * idx - 2],
				c->pipes[2 * idx + 1]) < 0)
			return (1);
	return (0);
}

static void	process_child(t_commands *commands, int idx)
{
	pid_t	pid;
	int		res;

	pid = fork();
	if (pid == -1)
		exit_error_with_msg(commands, FORK_ERROR);
	if (pid == 0)
	{
		if (open_fdinout(idx, commands))
			return ;
		close_pipes(commands);
		res = execcmd(commands->command_list[idx], commands->envp);
		if (res == 5)
			exit_error_with_msg(commands, PERM_DENIED);
		else if (res == 2)
			ft_putstr_fd(commands->command_list[idx][0], NO_COMMAND, 2);
		exit(1);
	}
}

static void	fork_processes(t_commands *commands)
{
	int		i;

	commands->pipe_nb = 2 * (commands->process_count - 1);
	commands->pipes = malloc(sizeof(int) * commands->pipe_nb);
	if (!commands->pipes)
		exit_error_with_msg(commands, PIPE_ERROR);
	create_pipes(commands);
	i = -1;
	while (++i < commands->process_count)
		process_child(commands, i);
	close_pipes(commands);
	waitpid(-1, NULL, 0);
}

int	main(int argc, char *argv[], char *envp[])
{
	t_commands	commands;

	if (argc < 5)
	{
		ft_printf("Missing arguments. You must provied at least 4 arguments.");
		return (1);
	}
	commands.envp = envp;
	parse_commands(&commands, argv, argc);
	fork_processes(&commands);
	exit_with_code(&commands, 0);
}
