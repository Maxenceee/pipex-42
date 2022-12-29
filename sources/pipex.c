/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgama <mgama@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/04 14:46:50 by mgama             #+#    #+#             */
/*   Updated: 2022/12/29 17:29:47 by mgama            ###   ########.fr       */
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
	free(bins);
	free(path);
	return (NULL);
}

void	parse_commands(t_commands *commands, char *argv[])
{
	int	i = 0;
	
	while (argv[++i])
	{
		if (i == 1)
			commands->file1 = argv[i];
		else if (i == 2)
			commands->command1 = ft_split(argv[i], ' ');
		else if (i == 3)
			commands->command2 = ft_split(argv[i], ' ');
		else if (i == 4)
			commands->file2 = argv[i];
	}
}

void	pipex_exit(int wait_status)
{
	int	wstatus;
	
	if (WIFEXITED(wait_status))
	{
		wstatus = WEXITSTATUS(wait_status);
		if (wstatus == 2)
			ft_printf("Could not find command\n");
		else if (wstatus == 3)
			ft_printf("Could not dup\n");
		else if (wstatus == 4)
			ft_printf("Cannot open file\n");
	}
	else if (WIFSIGNALED(wait_status))
		ft_printf("Child exited via signal %d\n", WTERMSIG(wait_status));
}

void	exit_with_code(t_commands *commands, int code)
{
	int i;

	i = 0;
    while (commands->command1[i] != NULL)
        free(commands->command1[i++]);
    free(commands->command1);
	i = 0;
    while (commands->command2[i] != NULL)
        free(commands->command2[i++]);
    free(commands->command2);
	if (code > 0)
		perror("An error occurred while executing the program");
    exit(code);
}

int	execcmd(int fdin, int fdout, char **command, char *envp[])
{
	char		*cmd;

	if (dup2(fdin, STDIN_FILENO) < 0)
		return (3);
	close(fdin);
	if (dup2(fdout, STDOUT_FILENO) < 0)
		return (3);
	close(fdout);
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

int	main(int argc, char *argv[], char *envp[])
{
	if (argc < 5 || argc > 5)
		return (1);
	t_commands	commands;
	int			fd[2];
	int			file;
	char		*cmd;
	
	parse_commands(&commands, argv);
	if (pipe(fd) == -1)
		exit_with_code(&commands, 1);

	pid_t	pid = fork();
	if (pid == -1)
		exit_with_code(&commands, 1);
	if (pid == 0)
	{	
		file = open(commands.file1, O_RDONLY);
		if (file == -1)
			return (4);

		close(fd[0]);
		return (execcmd(file, fd[1], commands.command1, envp));
	}
	else
	{
		pid_t	pid2 = fork();
		if (pid2 == -1)
			exit_with_code(&commands, 1);

		if (pid2 == 0)
		{
			int		output = open(commands.file2, O_WRONLY | O_CREAT | O_TRUNC, 0777);
			if (output == -1)
				return (1);
				
			close(fd[1]);
			return (execcmd(fd[0], output, commands.command2, envp));
		}
		close(fd[0]);
		close(fd[1]);
		int		wait_status;
		int		wait_status2;
		
		waitpid(pid, &wait_status, 0);
		pipex_exit(wait_status);
		waitpid(pid2, &wait_status2, 0);
		pipex_exit(wait_status2);
	}
	exit_with_code(&commands, 0);
}