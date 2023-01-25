/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgama <mgama@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/10 19:19:32 by mgama             #+#    #+#             */
/*   Updated: 2023/01/26 00:03:07 by mgama            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

char	*get_path(char *envp[])
{
	char	*env;
	int		i;

	i = -1;
	while (envp[++i])
	{
		env = ft_strnstr(envp[i], "PATH", ft_strlen(envp[i]));
		if (env != NULL)
			return (env + 5);
	}
	return (NULL);
}

char	*parse_env(char *envp[], char *cmd)
{
	char	*path;
	char	**bins;
	char	*joins[3];
	int		i;

	path = get_path(envp);
	if (!path)
		return (NULL);
	bins = ft_split(path, ':');
	if (!bins)
		return (NULL);
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

static void	parse_commands_2(t_commands *commands, char *argv[],
	int argc)
{
	int		i;

	i = 1;
	while (++i < argc - 1)
	{
		commands->command_list[i - 2] = ft_split(argv[i], ' ');
		if (commands->command_list[i - 2] && !commands->command_list[i - 2][0])
		{
			free(commands->command_list[i - 2]);
			commands->command_list[i - 2] = ft_calloc(2, sizeof(char *));
			if (!commands->command_list[i - 2])
				exit_with_code(commands, 1);
			commands->command_list[i - 2][0] = ft_strdup("");
			if (!commands->command_list[i - 2])
				exit_with_code(commands, 1);
		}
		else if (!commands->command_list[i - 2])
			exit_with_code(commands, 1);
	}
	commands->process_count = i - 2;
}

void	parse_commands(t_commands *commands, char *argv[], int argc)
{
	char	***comds;

	comds = (char ***)ft_calloc((argc - 2), sizeof(char **));
	if (!comds)
		exit(EXIT_FAILURE);
	commands->input = argv[1];
	commands->output = argv[argc - 1];
	commands->command_list = comds;
	parse_commands_2(commands, argv, argc);
}
