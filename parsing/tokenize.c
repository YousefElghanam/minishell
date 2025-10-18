/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jel-ghna <jel-ghna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 21:47:30 by jel-ghna          #+#    #+#             */
/*   Updated: 2025/10/17 18:36:32 by jel-ghna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

static void	set_operator_options(int *options, int op_index)
{
	*options |= (1 << op_index);
	*options |= OPERATOR;
	if (op_index == 3 || op_index == 4 || op_index == 5 || op_index == 6)
		*options |= REDIR_OP;
	else if (7 == op_index || op_index == 8)
		*options |= PARENTHESIS;
}

static int	handle_operator(t_list **head, char *line, char **operators, size_t *i)
{
	t_token	*token;
	int		op_index;

	skip_spaces(line, i);
	op_index = is_operator(&line[*i], operators);
	if (op_index == -1)
		return (0);
	token = create_token();
	if (!token)
		return (palloc_err(), 1);
	set_operator_options(&token->options, op_index);
	if (add_token(head, token))
		return (free(token), 1);
	*i += ft_strlen(operators[op_index]);
	skip_spaces(line, i);
	op_index = is_operator(&line[*i], operators);
	if (op_index > -1)
		return (handle_operator(head, line, operators, i));
	return (0);
}

static int	handle_word(t_list **head, char *line, char **operators, size_t *i)
{
	t_token		*token;
	ssize_t		word_len;

	skip_spaces(line, i);
	token = create_token();
	if (!token)
		return (palloc_err(), 1);
	token->options |= WORD;
	word_len = len_to_unquoted_delimiter(&line[*i], operators);
	if (word_len == 0)
		return (free(token), 0);
	token->fragments = malloc(sizeof(t_fragment)
			* count_fragments(&line[*i], word_len, operators));
	if (!token->fragments)
		return (free(token), palloc_err(), 1);
	token->str = ft_substr(&line[*i], 0, word_len);
	if (handle_fragments(line, operators, token, i))
		return (free(token->fragments), free(token->str), free(token), 1);
	if (add_token(head, token))
		return (free(token->fragments), free(token->str), free(token),
			palloc_err(), 1);
	return (0);
}

t_list	*tokenize(char *line, char **operators)
{
	t_list		*tokens;
	size_t		i;

	i = 0;
	tokens = NULL;
	while (line[i])
	{
		if (handle_operator(&tokens, line, operators, &i))
			return (del_tokens(tokens), NULL);
		if (handle_word(&tokens, line, operators, &i))
			return (del_tokens(tokens), NULL);
	}
	return (tokens);
}
