/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_utils_2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jel-ghna <jel-ghna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 21:42:35 by jel-ghna          #+#    #+#             */
/*   Updated: 2025/10/17 18:17:36 by jel-ghna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int	add_token(t_list **head, t_token *token)
{
	t_list	*node;

	node = ft_lstnew(token);
	if (!node)
		return (palloc_err(), 1);
	ft_lstadd_back(head, node);
	return (0);
}

t_token	*create_token(void)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->fragment_count = 0;
	token->fragments = NULL;
	token->options = 0;
	token->str = NULL;
	token->fragment_i = 0;
	token->redir.append = 0;
	token->redir.in = NULL;
	token->redir.out = NULL;
	return (token);
}
