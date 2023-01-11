/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgama <mgama@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/13 19:28:38 by mgama             #+#    #+#             */
/*   Updated: 2022/12/29 12:52:46 by mgama            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

static char	**free_tab(char **tab)
{
	int	i;

	i = 0;
	while (tab[i])
	{
		free(tab[i]);
	}
	free(tab);
	return (0);
}

static int	count_strings(const char *str, char charset)
{
	int	i;
	int	count;

	count = 0;
	i = 0;
	while (str[i] != '\0')
	{
		while (str[i] != '\0' && charset == str[i])
			i++;
		if (str[i] != '\0')
			count++;
		while (str[i] != '\0' && charset != str[i])
			i++;
	}
	return (count);
}

static int	ft_strlen_sep(const char *str, char charset)
{
	int	i;

	i = 0;
	while (str[i] && charset != str[i])
		i++;
	return (i);
}

static char	*ft_word(const char *str, char charset)
{
	int		len_word;
	int		i;
	char	*word;

	i = 0;
	len_word = ft_strlen_sep(str, charset);
	word = (char *)malloc(sizeof(char) * (len_word + 1));
	if (!word)
		return (0);
	while (i < len_word)
	{
		word[i] = str[i];
		i++;
	}
	word[i] = '\0';
	return (word);
}

char	**ft_split(const char *str, char charset)
{
	char	**strings;
	int		i;

	if (str == NULL)
		return (NULL);
	i = 0;
	strings = (char **)malloc(sizeof(char *)
			* (count_strings(str, charset) + 1));
	if (!strings)
		return (0);
	while (*str != '\0')
	{
		while (*str != '\0' && *str == charset)
			str++;
		if (*str != '\0')
		{
			strings[i] = ft_word(str, charset);
			if (strings[i++] == 0)
				return (free_tab(strings));
		}
		while (*str && *str != charset)
			str++;
	}
	strings[i] = 0;
	return (strings);
}
