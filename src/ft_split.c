/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: walwin <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 19:08:30 by walwin            #+#    #+#             */
/*   Updated: 2026/03/23 13:28:33 by nkham            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static int	char_is_separator(char c, char set)
{
	if (c == set)
		return (1);
	if (c == '\0')
		return (1);
	return (0);
}

static int	count_words(char const *str, char c)
{
	int	i;
	int	words;

	words = 0;
	i = 0;
	while (str[i] != '\0')
	{
		if (char_is_separator(str[i], c) == 0
			&& char_is_separator(str[i + 1], c) == 1)
			words++;
		i++;
	}
	return (words);
}

static void	write_word(char *dest, char const *from, char c)
{
	int	i;

	i = 0;
	while (char_is_separator(from[i], c) == 0)
	{
		dest[i] = from[i];
		i++;
	}
	dest[i] = '\0';
}

static void	write_split(char **split, char const *s, char c)
{
	int	i;
	int	j;
	int	word;

	word = 0;
	i = 0;
	while (s[i] != '\0')
	{
		if (s[i] == c)
			i++;
		else
		{
			j = 0;
			while (char_is_separator(s[i + j], c) == 0)
			{
				j++;
			}
			split[word] = (char *)malloc(sizeof(char) * (j + 1));
			if (!split[word])
				return ;
			write_word(split[word], s + i, c);
			i += j;
			word++;
		}
	}
}

char	**ft_split(char const *s, char c)
{
	char	**res;
	int		words;

	if (!s)
		return (NULL);
	words = count_words(s, c);
	res = (char **)malloc(sizeof(char *) * (words + 1));
	if (!res)
		return (NULL);
	res[words] = NULL;
	write_split(res, s, c);
	return (res);
}
