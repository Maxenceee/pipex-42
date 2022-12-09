/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgama <mgama@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/04 14:46:50 by mgama             #+#    #+#             */
/*   Updated: 2022/12/09 17:11:53 by mgama            ###   ########.fr       */
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
	/* tests */
	ft_printf("file1 %s\n", commands.file1);
	i = -1;
	while (commands.command1[++i])
	{
		ft_printf("cmd1 %s\n", commands.command1[i]);
	}
	ft_printf("file2 %s\n", commands.file2);
	// i = -1;
	// while (commands.command2[++i])
	// {
	// 	ft_printf("cmd2 %s\n", commands.command2[i]);
	// }
	return (commands);
}

int	main(int argc, char *argv[], char *envp[])
{
	// if (argc < 5 || argc > 5)
	// 	return (1);
	t_commands commands = parse_commands(argv);
	int		pid = fork();
	int		fd[2];
	
	if (pid == -1)
		return (1);
	if (pipe(fd) == -1)
		return (1);
		
	if (pid == 0)
	{
		close(fd[0]);
		int		file = open(commands.file1, O_RDONLY, 0777);
		if (file == -1)
			return (1);
		
		if (dup2(file, STDIN_FILENO) < 0)
			return (3);
		close(file);
		// if (dup2(STDOUT_FILENO, fd[1]) < 0)
		if (dup2(fd[1], STDOUT_FILENO) < 0)
			return (3);
		close(fd[1]);
		
		char	*cmd = parse_env(envp, commands.command1[0]);
		if (!cmd)
			return (2);
		// char	*argVec[] = {cmd, "-c", "5", "google.com", NULL};
		// char	*argVec[] = commands.command1;
		// char	*envVec[] = {NULL};
		ft_printf("Start of execve call %s:\n", cmd);
		ft_printf("========================================\n");
		if (execve(cmd, commands.command1, envp) == -1)
		{
			perror("Could not execute execve");
			return (1);
		}
	}
	else
	{
		int		wait_status;
		int		wait_status2;
		int		pid2 = fork();
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

			char	*cmd = parse_env(envp, commands.command2[0]);
			if (!cmd)
				return (2);
			
			ft_printf("Start of execve call %s:\n", cmd);
			ft_printf("========================================\n");
			if (execve(cmd, commands.command2, envp) == -1)
			{
				perror("Could not execute execve");
				return (1);
			}
		}
		else
		{
			close(fd[0]);
			close(fd[1]);
			waitpid(pid, &wait_status, 0);
			waitpid(pid2, &wait_status2, 0);
			if (WIFEXITED(wait_status))
			{
				ft_printf("Cmd 1 -------------\n");
				int	wstatus = WEXITSTATUS(wait_status);
				if (wstatus == 0)
					ft_printf("Success!\n");
				else if (wstatus == 2)
					ft_printf("Could not find command\n");
				else if (wstatus == 3)
					ft_printf("Could not dup\n");
				else
					ft_printf("Failure\n");
			}
			if (WIFEXITED(wait_status2))
			{
				ft_printf("Cmd 2 -------------\n");
				int	wstatus = WEXITSTATUS(wait_status2);
				if (wstatus == 0)
					ft_printf("Success!\n");
				else if (wstatus == 2)
					ft_printf("Could not find command\n");
				else if (wstatus == 3)
					ft_printf("Could not dup\n");
				else
					ft_printf("Failure\n");
			}
		}
	}
	return (0);
}