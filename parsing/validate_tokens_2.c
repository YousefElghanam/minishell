/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_tokens_2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jel-ghna <jel-ghna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 21:53:46 by jel-ghna          #+#    #+#             */
/*   Updated: 2025/10/16 21:57:38 by jel-ghna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int	wrong_first_token(t_list *cur, char **operators)
{
	int	options;

	options = cur->token->options;
	if (options & OR || options & AND || options & PIPE
		|| options & CLOSE_PARENTHESIS)
	{
		ft_printf(2, "minishell: syntax error near unexpected token `%s'\n",
			operators[btoindex(options)]);
		return (1);
	}
	return (0);
}

int	validate_word(t_list *cur)
{
	if (cur->next && cur->next->token->options & OPEN_PARENTHESIS)
	{
		ft_printf(2, "minishell: syntax error near unexpected token `('\n");
		return (1);
	}
	return (0);
}

int	validate_open_paren(t_list *cur, long *paren_count, char **operators)
{
	if (cur->next == NULL)
	{
		ft_printf
		(2, "minishell: syntax error near unexpected token `newline'\n");
		return (1);
	}
	*paren_count += 1;
	if (cur->next->token->options & AND
		|| cur->next->token->options & OR
		|| cur->next->token->options & PIPE
		|| cur->next->token->options & CLOSE_PARENTHESIS)
	{
		ft_printf(2, "minishell: syntax erorr near unexpected token `%s'\n",
			operators[btoindex(cur->next->token->options)]);
		return (1);
	}
	return (0);
}

int	validate_close_paren(t_list *cur, long *paren_count)
{
	if (*paren_count == 0)
	{
		ft_printf(2, "minishell: syntax error near unexpected token `)'\n");
		return (1);
	}
	*paren_count -= 1;
	if (cur->next && cur->next->token->options & WORD)
	{
		ft_printf(2, "minishell: syntax error near unexpected token `%s'\n",
			cur->next->token->str);
		return (1);
	}
	if (cur->next && cur->next->token->options & OPEN_PARENTHESIS)
	{
		ft_printf(2, "minishell: syntax error near unexpected token `('\n");
		return (1);
	}
	return (0);
}
