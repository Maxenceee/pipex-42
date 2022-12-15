/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgama <mgama@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/04 14:46:50 by mgama             #+#    #+#             */
/*   Updated: 2022/12/14 19:08:57 by mgama            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

char	*parse_env(char *envp[], char *cmd)
{
	char	*env;
	char	*path;
	char	**bins;
	char	*joins[3];
	int		i = -1;

	while (envp[++i])
	{
		env = ft_strnstr(envp[i], "PATH", ft_strlen(envp[i]));
		if (env != NULL)
			break;
	}
	if (!env)
		return (NULL);
	path = ft_strtrim(env, "PATH=");
	bins = ft_split(path, ':');
	free(path);
	i = -1;
	while (bins[++i])
	{
		joins[0] = bins[i];
		joins[1] = "/";
		joins[2] = cmd;
		path = ft_strjoin_arr(3, joins, "");
		if (access(path, F_OK) == 0)
			return (path);
	}
	free(path);
	return (0);
}

t_commands parse_commands(char *argv[])
{
	int	i = 0;
	t_commands	commands;
	
	while (argv[++i])
	{
		if (i == 1)
			commands.file1 = argv[i];
		else if (i == 2)
			commands.command1 = ft_split(argv[i], ' ');
		else if (i == 3)
			commands.command2 = ft_split(argv[i], ' ');
		else if (i == 4)
			commands.file2 = argv[i];
	}
	/* print args paring result */
	ft_printf("file1 %s\n", commands.file1);
	i = -1;
	while (commands.command1[++i])
	{
		ft_printf("cmd1 %s\n", commands.command1[i]);
	}
	ft_printf("file2 %s\n", commands.file2);
	i = -1;
	while (commands.command2[++i])
	{
		ft_printf("cmd2 %s\n", commands.command2[i]);
	}
	return (commands);
}

void	pipex_exit(int wait_status)
{
	int	wstatus = WEXITSTATUS(wait_status);
	if (wstatus == 0)
		ft_printf("Success!\n");
	else if (wstatus == 2)
		ft_printf("Could not find command\n");
	else if (wstatus == 3)
		ft_printf("Could not dup\n");
	else if (wstatus == 4)
		ft_printf("Cannot open file\n");
	else
		ft_printf("Failure\n");
}

void	read_pipe(int fdin)
{
	char	buffer[4096];
	
	if (read(fdin, buffer, sizeof(buffer)) < 0)
		ft_printf("cannot read fd %d\n", fdin);
	else
		ft_printf("read fd %d: %s\t end read\n", fdin, buffer);
}

int	main(int argc, char *argv[], char *envp[])
{
	// if (argc < 5 || argc > 5)
	// 	return (1);
	t_commands commands = parse_commands(argv);
	int		fd[2];
	int		file;
	char	*cmd;
	
	if (pipe(fd) == -1)
		return (1);
		
	pid_t	pid = fork();
	if (pid == -1)
		return (1);
	if (pid == 0)
	{	
		file = open(commands.file1, O_RDONLY);
		if (file == -1)
			return (4);
		
		if (dup2(file, STDIN_FILENO) < 0)
			return (3);
		close(file);
		
		if (dup2(fd[1], STDOUT_FILENO) < 0)
		 	return (3);
		close(fd[1]);
		close(fd[0]);
		
		cmd = parse_env(envp, commands.command1[0]);
		if (!cmd)
			return (2);
			
		// ft_printf("Start of execve call %s:\n", cmd);
		// ft_printf("========================================\n");
		if (execve(cmd, commands.command1, envp) == -1)
		{
			perror("Could not execute execve");
			return (1);
		}
	}
	else
	{
		/* reading pipe */
		// read_pipe(fd[0]);
		pid_t	pid2 = fork();
		if (pid2 == -1)
			return (1);

		if (pid2 == 0)
		{
			close(fd[1]);
			int		output = open(commands.file2, O_WRONLY | O_CREAT | O_TRUNC, 0777);
			if (output == -1)
				return (1);
			
			if (dup2(fd[0], STDIN_FILENO) < 0)
				return (3);
			close(fd[0]);
			if (dup2(output, STDOUT_FILENO) < 0)
				return (3);
			close(output);

			char	*cmd = parse_env(envp, commands.command2[0]);
			if (!cmd)
				return (2);

			// ft_printf("Start of execve call %s:\n", cmd);
			// ft_printf("========================================\n");
			if (execve(cmd, commands.command2, envp) == -1)
			{
				perror("Could not execute execve");
				return (1);
			}
		}
		close(fd[0]);
		close(fd[1]);
		int		wait_status;
		int		wait_status2;
		
		waitpid(pid, &wait_status, 0);
		if (WIFEXITED(wait_status))
		{
			ft_printf("Cmd 1 -------------\n");
			pipex_exit(wait_status);
		}
		else if (WIFSIGNALED(wait_status))
		{
			ft_printf("Child exited via signal %d\n", WTERMSIG(wait_status));
			return (1);
		}
		waitpid(pid2, &wait_status2, 0);
		if (WIFEXITED(wait_status2))
		{
			ft_printf("Cmd 2 -------------\n");
			pipex_exit(wait_status2);
		}
		else if (WIFSIGNALED(wait_status))
			ft_printf("Child exited via signal %d\n", WTERMSIG(wait_status));
	}
	return (0);
}