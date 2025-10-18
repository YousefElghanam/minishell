/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   field_split_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jel-ghna <jel-ghna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/18 00:18:14 by jel-ghna          #+#    #+#             */
/*   Updated: 2025/10/18 01:31:03 by jel-ghna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

static int	set_token(t_token *new_token, t_list *token_node, size_t fragment_i,
	t_list **field_split_head)
{
	new_token->fragment_count = 1;
	new_token->fragments = NULL;//token_node->token->fragments;
	new_token->options |= WORD;
	new_token->options |= EXPANDED_WORD;
	new_token->fragment_i = fragment_i;
	new_token->str = ft_strdup("");
	if (!new_token->str)
		return (1);
	if (add_token(field_split_head, new_token))
		return (free(new_token->str), 1);
	return (0);
}

int	create_split_tokens(t_list **head, char **split_arr, t_expansion_data *xd)
{
	t_token	*new_token;
	size_t	arr_counter;

	arr_counter = 0;
	while (split_arr[arr_counter++])
	{
		new_token = create_token();
		if (!new_token)
			return (1);
		new_token->fragment_count = 1;
		new_token->fragment_i = xd->i;
		new_token->fragments = NULL;//malloc(sizeof (t_fragment));
		// if (!new_token->fragments)
		// 	return (free(new_token), 1);
		// ft_memmove(new_token->fragments, xd->token_node->token->fragments,
		// 	sizeof(t_fragment));
		new_token->options = new_token->options | WORD | EXPANDED_WORD;
		new_token->str = ft_strdup(split_arr[arr_counter - 1]);
		if (!new_token->str)
			return (free(new_token->fragments), free(new_token), 1);
		if (add_token(head, new_token))
			return (free(new_token->fragments), free(new_token->str),
				free(new_token), 1);
	}
	return (0);
}

// FIRST CHECK OF NEXT FUNCTION MEANS THE EXPANDED STRING RESULTED IN ONLY SPACES (I ONLY ENTER THIS FUNCTION IF THE EXPANSION HAS SPACES),
// IN THIS CASE WE NEED TO HANDLE IT AS A SPLITTER OF THE WORD.
int	insert_split_tokens(t_list **field_split_head, t_list *token_node,
	t_list **target_node, size_t fragment_i)
{
	t_list	*next;
	t_list	*last_field;
	t_token	*new_token;

	if (!*field_split_head)
		return (0);
	next = (*target_node)->next;
	if (token_node->token->fragments[fragment_i].ends_with_space
		&& fragment_i + 1 < token_node->token->fragment_count
		&& (*field_split_head)->token->str[0])
	{
		new_token = create_token();
		if (!new_token)
			return (1);
		if (set_token(new_token, token_node, fragment_i, field_split_head))
			return (free(new_token), 1);
	}
	last_field = ft_lstlast(*field_split_head);
	(*target_node)->next = *field_split_head;
	last_field->next = next;
	*target_node = last_field;
	return (0);
}
