/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   field_split.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jel-ghna <jel-ghna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/18 00:11:53 by jel-ghna          #+#    #+#             */
/*   Updated: 2025/10/18 00:15:23 by jel-ghna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

static int	handle_str_of_dels(char ***split_arr)
{
	free(*split_arr);
	*split_arr = malloc(sizeof(char *) * 2);
	if (!*split_arr)
		return (1);
	(*split_arr)[0] = ft_strdup("");
	if (!(*split_arr)[0])
		return (free(*split_arr), 1);
	(*split_arr)[1] = NULL;
	return (0);
}

static int	handle_str_of_one_word(char ***split_arr, t_expansion_data *xd)
{
	char	**tmp;

	tmp = malloc(sizeof(char *) * 3);
	if (!tmp)
		return (1);
	tmp[0] = (*split_arr)[0];
	tmp[1] = ft_strdup("");
	if (!tmp[1])
		return (free(tmp), 1);
	tmp[2] = NULL;
	free(*split_arr);
	*split_arr = tmp;
	return (0);
}

static int	handle_one_str_arr(char ***split_arr, t_expansion_data *xd)
{
	if (!**split_arr)
	{
		if (handle_str_of_dels(split_arr))
			return (1);
	}
	else if ((*split_arr)[0] && !(*split_arr)[1]
		&& xd->token_node->token->fragments[xd->i].ends_with_space
		&& xd->i + 1 < xd->token_node->token->fragment_count)
	{
		if (handle_str_of_one_word(split_arr, xd))
			return (free_split(*split_arr), 1);
	}
	return (0);
}

int	field_split(char *fragment_str, char *expanded, t_expansion_data *xd)
{
	t_list	*head;
	char	**split_arr;

	head = NULL;
	split_arr = ft_split_dels(expanded, " 	");
	if (!split_arr || handle_one_str_arr(&split_arr, xd))
		return (1);
	if (!xd->token_node->token->fragments[xd->i].starts_with_space
		|| xd->i == 0)
	{
		append_substr(xd->target_node, split_arr[0], 0);
		if (create_split_tokens(&head, split_arr + 1, xd))
			return (del_tokens(head), free_split(split_arr), 1);
	}
	else
		if (create_split_tokens(&head, split_arr, xd))
			return (del_tokens(head), free_split(split_arr), 1);
	free_split(split_arr);
	if (insert_split_tokens(&head, xd->token_node, &xd->target_node, xd->i))
		return (del_tokens(head), 1);
	return (0);
}
