/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exits.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgama <mgama@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/12 18:21:18 by mgama             #+#    #+#             */
/*   Updated: 2023/01/25 23:50:39 by mgama            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex_bonus.h"

void	exit_with_code(t_commands *commands, int code)
{
	int	i;
	int	j;

	i = 0;
	if (commands->command_list != NULL)
	{
		while (commands->command_list[i] != NULL)
		{
			j = 0;
			while (commands->command_list[i][j])
				free(commands->command_list[i][j++]);
			free(commands->command_list[i++]);
		}
		free(commands->command_list);
	}
	exit(code);
}

void	exit_error_with_msg(t_commands *commands, char *msg)
{
	perror(msg);
	exit_with_code(commands, 1);
}
