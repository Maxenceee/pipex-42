/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgama <mgama@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/04 14:46:50 by mgama             #+#    #+#             */
/*   Updated: 2023/01/08 16:03:03 by mgama            ###   ########.fr       */
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

void	print_tab(char **tab)
{
	int	i = 0;
	while (tab[i])
	{
		ft_printf("%s\n", tab[i++]);	
	}
}

void	print_tab_int(int **tab)
{
	int	i = 0;
	while (tab[i])
	{
		ft_printf("%d\n", tab[i++]);	
	}
}

void	parse_commands(t_commands *commands, char *argv[], int argc)
{
	int		i = 0;
	char	***comds;

	comds = (char ***)ft_calloc((argc - 2), sizeof(char **));
	if (!comds)
		exit(EXIT_FAILURE);
	while (++i < argc)
	{
		printf("%d %s\n", i, argv[i]);
		if (i == 1)
			commands->input = argv[i];
		else if (i == argc - 1)
			commands->output = argv[i];
		else
			comds[i - 2] = ft_split(argv[i], ' ');
	}
	commands->command_list = comds;
	commands->process_count = i-3;
	ft_printf("\nprocess_count %d\n", commands->process_count);
	int j = -1;
	while (commands->command_list[++j])
	{
		print_tab(commands->command_list[j]);
		fflush(NULL);
	}
}

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
	i = 0;
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

void	read_pipe(int fdin)
{
	char	buffer[4096];

	if (read(fdin, buffer, sizeof(buffer)) < 0)
		ft_printf("cannot read fd %d\n", fdin);
	else
		ft_printf("read fd %d: %s\t end read\n", fdin, buffer);
}

void	fork_processes(t_commands *commands)
{
	pid_t	pid;
	int		file;
	int		**fd;
	int		fdin;
	int		fdout;
	int		wait_status;
	int		i;
	int		j;
	
	// if (pipe(fd) == -1)
	// 	exit_with_code(commands, 1);
	
	fd = malloc(sizeof(int *) * commands->process_count);
	if (!fd)
		exit_with_code(commands, 1);
	i = -1;
	while (++i < commands->process_count)
		fd[i] = ft_calloc(2, sizeof(int));
	i = -1;
	while (++i < commands->process_count)
	{
		if (pipe(fd[i]) == -1)
			exit_with_code(commands, 1);
	}
	// print_tab_int(fd);
	i = -1;
	while (++i < commands->process_count)
	{
		pid = fork();
		if (pid == -1)
			exit_with_code(commands, 1);
		if (pid == 0)
		{
			j = -1;

			while (++j < commands->process_count)
			{
				if (i != j)
					close(fd[j][0]);
				if (i + 1 != j)
					close(fd[j][1]);
			}
			
			// write(fd[i + 1][1], "hello", 6);
			if (i == 0)
			{
				fdin = open(commands->input, O_RDONLY);
				if (fdin == -1)
					exit_with_code(commands, 1);
				ft_printf("read file\n");
				close(fd[i][0]);
			}
			else
				fdin = fd[i][0];
			if (i < commands->process_count - 1)
				fdout = fd[i + 1][1];
			else
			{
				fdout = open(commands->output, O_WRONLY | O_CREAT | O_TRUNC, 0777);
				if (fdout == -1)
					exit_with_code(commands, 1);
				ft_printf("write file\n");
				close(fd[i + 1][1]);
			}
			ft_printf("fdin %d fdout %d\n\n", fdin, fdout);
			read_pipe(fdin);
				
			// if (fdin == -1 || fdout == -1)
			// 	exit_with_code(commands, 4);

			char	buffer[4096];

			if (read(fdin, buffer, sizeof(buffer)) < 0)
				ft_printf("cannot read fd %d\n", fdin);

			if (write(fdout, buffer, ft_strlen(buffer)) < 0)
				ft_printf("cannot write fd %d\n", fdout);
			
			// execcmd(fdin, fdout, commands->command_list[i], commands->envp);
		}
		else
		{
			// close(fd[0]);
			// close(fd[1]);
			j = -1;
			while (++j < commands->process_count)
			{
				close(fd[j][0]);
				close(fd[j][1]);
			}
			waitpid(pid, &wait_status, 0);
			pipex_exit(wait_status);
		}
	}
	
}

int	main(int argc, char *argv[], char *envp[])
{
	if (argc < 5)
	{
		ft_printf("Missing arguments. You must provied at least 4 arguments.");
		return (1);
	}
	t_commands	commands;
	int			file;
	char		*cmd;
	
	parse_commands(&commands, argv, argc);
	commands.envp = envp;	
	
	fork_processes(&commands);
	// if (1)
	// {
	// 	if (pipe(fd+2) == -1)
	// 		exit_with_code(&commands, 1);
	// 	pid_t	pid2 = fork();
	// 	if (pid2 == -1)
	// 		exit_with_code(&commands, 1);

	// 	if (pid2 == 0)
	// 	{
	// 		close(fd[1]);
	// 		close(fd[2]);
	// 		ft_printf("pid2\n");
	// 		return (execcmd(fd[0], fd[3], commands.command_list[1], envp));
	// 	}
	// 	else
	// 	{
	// 		pid_t	pid3 = fork();
	// 		if (pid3 == -1)
	// 			exit_with_code(&commands, 1);

	// 		if (pid3 == 0)
	// 		{
	// 			int		output = open(commands.output, O_WRONLY | O_CREAT | O_TRUNC, 0777);
	// 			if (output == -1)
	// 				return (1);
					
	// 			close(fd[0]);
	// 			close(fd[1]);
	// 			close(fd[3]);
	// 			ft_printf("pid3\n");
	// 			char *cmd[] = {"cat", "-e", NULL};
	// 			return (execcmd(fd[2], output, cmd, envp));
	// 		}
	// 		close(fd[0]);
	// 		close(fd[1]);
	// 		close(fd[2]);
	// 		close(fd[3]);
	// 		int		wait_status;
	// 		int		wait_status2;
	// 		int		wait_status3;
			
	// 		waitpid(pid, &wait_status, 0);
	// 		pipex_exit(wait_status);
	// 		waitpid(pid2, &wait_status2, 0);
	// 		pipex_exit(wait_status2);
	// 		waitpid(pid3, &wait_status3, 0);
	// 		pipex_exit(wait_status3);
	// 		// ft_printf("\n");
	// 	}
	// }
	exit_with_code(&commands, 0);
}