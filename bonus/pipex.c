/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgama <mgama@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/04 14:46:50 by mgama             #+#    #+#             */
/*   Updated: 2023/01/11 20:29:41 by mgama            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex_bonus.h"

void	pipex_exit(int wait_status)
{
	int	wstatus;
	
	if (WIFEXITED(wait_status))
	{
		wstatus = WEXITSTATUS(wait_status);
		if (wstatus == 0)
			ft_printf("Succes\n");
		if (wstatus == 2)
			ft_printf("Could not find command\n");
		else if (wstatus == 3)
			ft_printf("Could not dup2\n");
		else if (wstatus == 4)
			ft_printf("Cannot open file\n");
	}
	else if (WIFSIGNALED(wait_status))
		ft_printf("Child exited via signal %d\n", WTERMSIG(wait_status));
}

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
    exit(code);
}

void	exit_error_with_msg(t_commands *commands, char *msg)
{
	perror(msg);
	exit_with_code(commands, 1);
}

void	create_pipes(t_commands *commands)
{
	int	i;

	i = -1;
	while (++i < commands->process_count)
	{
		if (pipe(commands->pipes + 2 * i) == -1)
			exit_error_with_msg(commands, PIPE_ERROR);
	}
}

void	close_pipes(t_commands *commands)
{
	int	i;

	i = -1;
	while (++i < commands->pipe_nb)
		close(commands->pipes[i]);
}

int	execcmd(char **command, char *envp[])
{
	char		*cmd;

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

int	dup2_fdinout(int fdin, int fdout)
{
	if (dup2(fdin, STDIN_FILENO) < 0)
		return (3);
	// close(fdin);
	if (dup2(fdout, STDOUT_FILENO) < 0)
		return (3);
	// close(fdout);
	return (0);
}

void	process_child(t_commands *commands, int idx)
{
	pid_t	pid;
	int		fdin;
	int		fdout;
	
	pid = fork();
	if (pid == -1)
		exit_error_with_msg(commands, FORK_ERROR);
	if (pid == 0)
	{
		if (idx == 0)
		{
			fdin = open(commands->input, O_RDONLY);
			if (fdin == -1)
				perror(OPEN_IN_ERROR);
			if (dup2_fdinout(fdin, commands->pipes[1]) < 0)
				return ;
		}
		else if (idx == commands->process_count - 1)
		{
			fdout = open(commands->output, O_WRONLY | O_CREAT | O_TRUNC, 0777);
			if (fdout == -1)
				exit_error_with_msg(commands, OPEN_OUT_ERROR);
			if (dup2_fdinout(commands->pipes[2 * idx - 2], fdout) < 0)
				return ;
		}
		else
			if (dup2_fdinout(commands->pipes[2 * idx - 2], commands->pipes[2 * idx + 1]) < 0)
				return ;
		close_pipes(commands);
		execcmd(commands->command_list[idx], commands->envp);
	}
}

void	fork_processes(t_commands *commands)
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
	if (argc < 5)
	{
		ft_printf("Missing arguments. You must provied at least 4 arguments.");
		return (1);
	}
	t_commands	commands;
	
	commands.envp = envp;
	parse_commands(&commands, argv, argc);
	fork_processes(&commands);
	exit_with_code(&commands, 0);
}