#include "parsing.h"

// DON't FORGET FUNCHECK

// What if we have 2 bits of 2, and we want to check the 1 bit that is to the left?
// we read the one to the right first, which is an incorrect result.
// when calling it for token types, this function only works up to bit 8 (CLOSE_PAREN). Bigger is undefined.
//		||
//		||
//		\/
int	btoindex(int options)
{
	int	i;

	i = 0;
	while (!((options >> i) & 1))
		i++;
	return (i);
}

int	print_fragment_str(char *line, t_token *token, size_t fragment_i)
{
	char	*fragment_str;
	
	fragment_str = ft_substr(line, token->fragments[fragment_i].start,
		token->fragments[fragment_i].end - token->fragments[fragment_i].start + 1);
	if (!fragment_str)
		return (1);
	ft_printf(1, "expanded (%s) ", fragment_str);
	free(fragment_str);
	return (0);
}

int	token_count = 0;
static void	print_tokens(t_print_d *data)
{
	char		**operators;
	t_token		*token;
	char		*line;
	t_fragment	fragment;
	size_t		i;
	size_t		len;

	i = 0;
	operators = data->operators;
	token = data->token;
	line = data->line;
	if (token->options & WORD)
	{
		printf("token (%d) type: (WORD)\n", token_count++);
		if (token->fragment_count == 0)
			printf("[%s] EMPTY WORD\n", token->str);
        printf("[%s]", token->str);
		// while (i < token->fragment_count)
		// {
        //     if (token->fragments[i].type == EMPTY)
        //     {
        //         i++;
        //         continue ;
        //     }
		// 	fragment = (token->fragments)[i];
		// 	len = fragment.end - fragment.start + 1;
		// 	write(1, "[", 1);
		// 	write(1, &line[fragment.start], len);
		// 	write(1, "]", 1);
		// 	if (len)
		// 		print_fragment_type(&fragment);
		// 	write(1, "\n", 1);
		// 	i++;
		// }
	}
	else if (token->options & OPERATOR)
	{
		printf("token (%d) type: (OPERATOR)\n", token_count++);		
		printf("[%s]\n", operators[btoindex(token->options)]);
	}
	// print_bits(token->options, 0);
	printf("\n\n");
	// free(token->fragments);
}

t_btree	*parse(t_parse_data *d)
{
	t_print_d	print_data;

	print_data.line = d->line;
	print_data.operators = d->operators;
	d->tokens = tokenize(d->line, d->operators);
	if (!d->tokens)
		return (ft_printf(2, "minishell: tokenize() returned NULL\n"),
		NULL);
	if (validate_tokens(d->tokens, d->operators))
		return (ft_printf(2, "minishell: validate_tokens() failed\n"),
		del_tokens(d->tokens), NULL);
	if (expand(d))
		return (ft_printf(2, "minishell: expand() failed\n"),
		del_tokens(d->tokens), NULL);
	// ft_lstiter(d->tokens, print_tokens, &print_data);
	d->exec_tree = create_exec_tree(d);
	del_tokens(d->tokens);
	return (d->exec_tree);
}
