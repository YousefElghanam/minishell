/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_fragment_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jel-ghna <jel-ghna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 22:06:53 by jel-ghna          #+#    #+#             */
/*   Updated: 2025/10/18 01:26:41 by jel-ghna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

char	*create_var_val(char *str, size_t *start, t_expansion_data *xd)
{
	size_t	j;
	char	*key;
	char	*value;

	j = 0;
	if (str == 0 && !(xd->i + 1 == xd->token_node->token->fragment_count))
		return(ft_strdup(""));
	if (str[j] == '*')
		return (*start += 1, ft_strdup(""));
	if (str[j] == '$')
		return (*start += 1, ft_strdup("$$"));
	if (str[j] == '?')
		return (*start += 1, ft_itoa(xd->d->data->rt));
	if (str[j] == 0 || (!ft_isalpha(str[j]) && str[j] != '_'))
		return (ft_strdup("$"));
	while (ft_isalnum(str[j]) || str[j] == '_')
		j++;
	*start += j;
	key = ft_substr(str, 0, j);
	if (!key)
		return (NULL);
	value = ft_get_env_value(xd->d->data->env, key);
	free(key);
	return (value);
}

char	*safe_strjoin(char **str, char *str2, int free_second_str)
{
	char	*tmp;

	tmp = ft_strjoin(*str, str2);
	free(*str);
	if (free_second_str)
		free(str2);
	if (!tmp)
		return (NULL);
	*str = tmp;
	return (*str);
}

int	append_substr(t_list *target_node, char *str, int free_second_str)
{
	char	*tmp;

	tmp = ft_strjoin(target_node->token->str, str);
	free(target_node->token->str);
	if (free_second_str)
		free(str);
	if (!tmp)
		return (1);
	target_node->token->str = tmp;
	return (0);
}
