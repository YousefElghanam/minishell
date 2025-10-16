#include "parsing.h"
// x &= ~(1 << 2)
// 000101
// 111011
//=000001

// DON't FORGET FUNCHECK

// #include <fcntl.h>
// void	signal_handler(int sig)
// {
// 	int fd = open("test", O_RDWR);
// 	write(fd, "sig recieved\n", 13);
// 	exit(1);
// 	// printf("sig recieved\n");
// }

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

void	insert_expanded_filenames(t_list **files_list, t_list **node)
{
	free((*node)->token->fragments);
	// free((*node)->token->str);
	// free((*node)->token);
	ft_lstlast(*files_list)->next = (*node)->next;
	(*node)->token = (*files_list)->token;
	(*node)->next = (*files_list)->next;
	// free(*files_list);
}

int	expand_filename(t_list **node, size_t fragment_i)
{
	char	**files_arr;
	t_list	*files_list;

	files_arr = NULL;
	files_list = NULL;
	files_arr = expand_star_append((*node)->token->str, &files_arr);
	if (!files_arr)
		return (printf("expand_star_append() returned NULL"), 0);
	if (create_field_split_tokens(&files_list, files_arr, fragment_i, *node))
		return (free_split(files_arr), del_tokens(files_list), 1);
	free_split(files_arr);
	insert_expanded_filenames(&files_list, node);
	return (0);
}

int	filename_expansion(t_list **head, char *line)
{
	t_list	*node;

	node = *head;
	while (node)
	{
		if (node->token->options & WORD && node->token->fragments[0].type == UNQUOTED) // THIS MIGHT NEED TO BE CHANGED, READ THE MANUAL, SEARCH FOR UNQUOTED '*'
		{
			// printf("fragment_i is (%zi) --- for token (%s), it's type is (%d)\n", node->token->fragment_i, node->token->str, node->token->fragments[node->token->fragment_i].type);
			// if (node->token->options & EXPANDED_WORD && node->token->fragments && node->token->fragments[node->token->fragment_i].type != UNQUOTED)
			// 	;
			// else if (ft_strchr(node->token->str, '*') == NULL)
			// 	;
			// else if (expand_filename(&node, node->token->fragment_i))
			// 	return (1);
			if (ft_strchr(node->token->str, '*'))
			{
				if (expand_filename(&node, node->token->fragment_i))
					return (1);
			}
		}
		node = node->next;
	}
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

t_btree	*parse(t_parse_data *d, t_data *data)
{
	t_list		*tokens;
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
	d->exec_tree = create_tree(d->tokens, d->line_count, &d->here_list);
	del_tokens(d->tokens);
	return (d->exec_tree);
}
