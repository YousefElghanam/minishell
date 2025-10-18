/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jel-ghna <jel-ghna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 15:59:58 by jel-ghna          #+#    #+#             */
/*   Updated: 2025/10/17 19:05:15 by jel-ghna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

static int	is_delimiter(char *line, char **operators)
{
	int	op_index;

	op_index = 0;
	if (*line == ' ' || *line == '	')
		return (1);
	while (operators[op_index])
	{
		if (!ft_strncmp(line, operators[op_index],
				ft_strlen(operators[op_index])))
			return (1);
		op_index++;
	}
	return (0);
}

size_t	count_fragments(char *line, ssize_t word_len, char **operators)
{
	size_t	count;
	ssize_t	i;

	count = 0;
	i = -1;
	while (++i < word_len)
	{
		if (line[i] == '\"' && ft_strchr(&line[i + 1], '\"'))
		{
			count++;
			i += (ft_strchr(&line[i + 1], '\"') - &line[i]);
		}
		else if (line[i] == '\'' && ft_strchr(&line[i + 1], '\''))
		{
			count++;
			i += (ft_strchr(&line[i + 1], '\'') - &line[i]);
		}
		else
		{
			count++;
			i += len_to_quote_or_delimiter(&line[i], operators);
		}
	}
	return (count);
}

void	skip_spaces(char *line, size_t *i)
{
	while (line[*i] == ' ' || line[*i] == '	')
		*i += 1;
}
ssize_t	len_to_unquoted_delimiter(char *line, char **operators)
{
	ssize_t	len;
	int		in_quote;

	len = 0;
	in_quote = 0;
	while (line[len])
	{
		if (line[len] == '\"' && !in_quote)
			in_quote = 1;
		else if (line[len] == '\'' && !in_quote)
			in_quote = 2;
		else if (((line[len] == '\"' && in_quote == 1)
				|| (line[len] == '\'' && in_quote == 2)))
			in_quote = 0;
		if (!in_quote)
			if (is_delimiter(&line[len], operators))
				return (len);
		len++;
	}
	return (len);
}

ssize_t	len_to_quote_or_delimiter(char *line, char **operators)
{
	ssize_t	len;
	size_t	op;

	len = 0;
	while (line[len])
	{
		op = 0;
		if (line[len] == ' ' || line[len] == '	'
			|| line[len] == '\'' || line[len] == '\"')
			return (len);
		while (operators[op])
		{
			if (!ft_strncmp(&line[len], operators[op],
					ft_strlen(operators[op])))
				return (len);
			op++;
		}
		len++;
	}
	return (len);
}
