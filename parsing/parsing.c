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

void	del_token(void *ptr)
{
	free(((t_token *)ptr)->fragments);
	free(((t_token *)ptr)->str);
	free(ptr);
}

void	del_tokens(t_list *tokens)
{
	ft_lstclear(&tokens, del_token);
}

char	*create_env_var_value(char *str, size_t *start, t_data *data)
{
	size_t	j;
	char	*key;
	char	*value;

	j = 0;
	if (str[j] == '$')
		return (*start += 1, ft_strdup("$$"));
	if (str[j] == '?')
		return (*start += 1, ft_itoa(data->rt));
	if (str[j] == 0 || (!ft_isalpha(str[j]) && str[j] != '_'))
		return (ft_strdup("$"));
	while (ft_isalnum(str[j]) || str[j] == '_')
		j++;
	*start += j;
	key = ft_substr(str, 0, j);
	if (!key)
		return (NULL);
	value = ft_get_env_value(data->env, key);
	free(key);
	return (value);
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

char	*safe_strjoin(char **str1, char *str2, int free_second_str)
{
	char	*tmp;

	tmp = ft_strjoin(*str1, str2);
	free(*str1);
	if (free_second_str)
		free(str2);
	if (!tmp)
		return (NULL);
	*str1 = tmp;
	return (*str1);
}

int	expand_double_quoted_fragment(char *fragment_str, t_list *target_node, t_data *data)
{
	char	*env_var_value;
	size_t	start;
	size_t	i;

	start = 0;
	i = 0;
	while (fragment_str[i])
	{
		if (fragment_str[i] == '$')
		{
			fragment_str[i] = 0;
			if (append_substr(target_node, &fragment_str[start], 0))
				return (1);
			start = i + 1;
			env_var_value = create_env_var_value(&fragment_str[start], &start, data);
			if (!env_var_value)
				return (1);
			if (append_substr(target_node, env_var_value, 1))
				return (1);
			i = start;
		}
		else
			i++;
	}
	return (append_substr(target_node, &fragment_str[start], 0));
}

char	*create_expanded_fragment(char *fragment_str, char **target_str, t_data *data, t_list *token_node, size_t fragment_i)
{
	char	*env_var_value;
	size_t	start;
	size_t	i;

	start = 0;
	i = 0;
	while (fragment_str[i])
	{
		if (fragment_str[i] == '$')
		{
			fragment_str[i] = 0;
			if (!safe_strjoin(target_str, &fragment_str[start], 0))
				return (NULL);
			start = i + 1;
			if (fragment_str[start] == 0 && !(fragment_i + 1 == token_node->token->fragment_count))
				env_var_value = ft_strdup("");
			else
				env_var_value = create_env_var_value(&fragment_str[start], &start, data);
			if (!env_var_value)
				return (NULL);
			if (!safe_strjoin(target_str, env_var_value, 1))
				return (NULL);
			i = start;
		}
		else
			i++;
	}
	return (safe_strjoin(target_str, &fragment_str[start], 0));
}

int	create_field_split_tokens(t_list **head, char **split_arr, size_t fragment_i, t_list *token_node)
{
	t_token	*new_token;
	size_t	arr_counter;

	arr_counter = 0;
	while (split_arr[arr_counter])
	{
		new_token = create_token();
		if (!new_token)
			return (1);
		new_token->fragment_count = 1;
		new_token->fragment_i = fragment_i;
		new_token->fragments = token_node->token->fragments; // MAKE FRAGMENTS FOR THIS AND MAKE IT UQNUOTED FRAGMENTS, SO WHEN YOU PASS IT TO FILENAME_EXPANSION IT KNOWS THAT IT CAME FROM AN UNQUOTED EXPANSION
		new_token->options |= WORD;
		new_token->options |= EXPANDED_WORD;
		new_token->str = ft_strdup(split_arr[arr_counter]);
		if (!new_token->str)
			return (free(new_token), 1);
		if (add_token(head, new_token))
			return (free(new_token->str), free(new_token), 1);
		arr_counter++;
	}
	// if (arr_counter >= 2)
	// 	(*head)->token->options |= 
	return (0);
}

int	insert_field_split_tokens(t_list **field_split_head, t_list *token_node, t_list **target_node, size_t fragment_i)
{
	t_list	*next;
	t_list	*last_field;
	t_token	*new_token;

	// NEXT CHECK MEANS THE EXPANDED STRING RESULTED IN ONLY SPACES (I ONLY ENTER THIS FUNCTION IF THE EXPANSION HAS SPACES),
	// IN THIS CASE WE NEED TO HANDLE IT AS A SPLITTER OF THE WORD.
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
		new_token->fragment_count = 1;
		new_token->fragments = token_node->token->fragments;
		new_token->options |= WORD;
		new_token->options |= EXPANDED_WORD;
		new_token->fragment_i = fragment_i;
		new_token->str = ft_strdup("");
		if (!new_token->str)
			return (free(new_token), 1);
		if (add_token(field_split_head, new_token))
			return (free(new_token->str), free(new_token), 1);
	}
	last_field = ft_lstlast(*field_split_head);
	(*target_node)->next = *field_split_head;
	last_field->next = next;
	*target_node = last_field;
	return (0);
}

int	field_split(char *fragment_str, char *expanded, t_list *token_node, t_list **target_node, size_t fragment_i)
{
	t_list	*head;
	char	**split_arr;

	head = NULL;
	// CAREFULL, THIS FT_SPLIT RETURNS 0 STRINGS IN THE ARRAY IF WE PASS A STRING OF ONLY DELIMITERS TO IT.
	// I EXPECTED IT TO RETURN 1 EMPTY STRING, THAT'S WHY I HAVE TO HANDLE IT SEPARATELY NEXT.
	split_arr = ft_split_dels(expanded, " 	");
	if (!split_arr)
		return (1);
	// WHEN 
	// if (!*split_arr && !(fragment_i > 0 && fragment_i + 1 < token_node->token->fragment_count))
	// 	return (free_split(split_arr), 0);
	if (!*split_arr)
	{
		free(split_arr);
		split_arr = malloc(sizeof(char *) * 2);
		split_arr[0] = ft_strdup("");
		if (!split_arr[0])
			return (free_split(split_arr), 1);
		split_arr[1] = NULL;
	}
	else if (split_arr[0] && !split_arr[1] && token_node->token->fragments[fragment_i].ends_with_space && fragment_i + 1 < token_node->token->fragment_count)
	{
		char	**tmp = malloc(sizeof(char *) * 3);
		if (!tmp)
			return (free(split_arr), 1);
		tmp[0] = split_arr[0];
		tmp[1] = ft_strdup("");
		if (!tmp[1])
			return (free_split(split_arr), 1);
		tmp[2] = NULL;
		free(split_arr);
		split_arr = tmp;
	}
	if (!(token_node)->token->fragments[fragment_i].starts_with_space
		|| fragment_i == 0)
	{
		append_substr(*target_node, split_arr[0], 0);
		if (create_field_split_tokens(&head, split_arr + 1, fragment_i, token_node))
			return (del_tokens(head), free_split(split_arr), 1);
	}
	else
		if (create_field_split_tokens(&head, split_arr, fragment_i, token_node))
			return (del_tokens(head), free_split(split_arr), 1);
	free_split(split_arr);
	if (insert_field_split_tokens(&head, token_node, target_node, fragment_i))
		return (del_tokens(head), 1);
	return (0);
}

int	expand_unquoted_fragment(char *fragment_str, t_list *token_node, t_list **target_node, size_t i, t_data *data)
{
	t_token	*token;
	char	*expanded;

	token = token_node->token;
	expanded = ft_strdup("");
	if (!expanded)
		return (1);
	expanded = create_expanded_fragment(fragment_str, &expanded, data, token_node, i);
	if (!expanded)
		return (1);
	if (!ft_strchr(expanded, ' ') && !ft_strchr(expanded, '	'))
		return (append_substr(*target_node, expanded, 1), 0);
	// NEED TO SEE IF I HAVE SPACE ONLY STRING!! HOW TO HANDLE IT?
	token->fragments[i].starts_with_space = (ft_strchr(" 	", expanded[0])
		!= NULL);
	token->fragments[i].ends_with_space = (*expanded
		&& ft_strchr(" 	", expanded[ft_strlen(expanded) - 1]));
	if (field_split(fragment_str, expanded, token_node, target_node, i))
		return (free(expanded), 1);
	return (free(expanded), 0);
}

int	expand_single_quoted_fragment(char *fragment_str, t_list *target_node)
{
	char	*tmp;

	tmp = ft_strjoin(target_node->token->str, fragment_str);
	if (!tmp)
		return (1);
	free(target_node->token->str);
	target_node->token->str = tmp;
	return (0);
}

int	expand_fragment(char *line, t_list *token_node, t_list **target_node, size_t i, t_data *data)
{
	t_token	*token;
	int		res;
	char	*fragment_str;

	res = 1;
	token = token_node->token;
	fragment_str = ft_substr(line, token->fragments[i].start,
		token->fragments[i].end - token->fragments[i].start + 1);
	// while ((*target_node) &&(*target_node)->token->options & EXPANDED_WORD)
		// *target_node = (*target_node)->next;
	if (!fragment_str)
		return (1);
	if (token->fragments[i].type == SINGLE)
		res = expand_single_quoted_fragment(fragment_str, *target_node);
	else if (token->fragments[i].type == DOUBLE)
		res = expand_double_quoted_fragment(fragment_str, *target_node, data);
	else if (token->fragments[i].type == UNQUOTED)
		res = expand_unquoted_fragment(fragment_str, token_node, target_node, i, data);
	free(fragment_str);
	return (res);
}

int	expand_word(char *line, t_list *target_node, t_data *data)
{
	size_t	fragment_i;
	t_list	*token_node;

	fragment_i = 0;
	token_node = target_node;
	free(token_node->token->str);
	token_node->token->str = ft_strdup("");
	if (!token_node->token->str)
		return (1);
	while (fragment_i < token_node->token->fragment_count)
	{
		if (token_node->token->fragments[fragment_i].type == EMPTY)
		{
			fragment_i++;
			continue ;
		}
		// print_fragment_str(line, token_node->token, fragment_i);
		if(expand_fragment(line, token_node, &target_node, fragment_i, data))
			return (1);
		fragment_i++;
	}
	// ft_printf(1, "to (%s)\n", token_node->token->str);
	if (!*token_node->token->str && token_node->token->fragments && token_node->token->fragments[0].type == UNQUOTED && token_node->token->fragment_count == 1)
		return (token_node->token->options |= EMPTY_WORD, 0);
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
		return (free_split(files_arr), ft_lstclear(&files_list, del_token), 1);
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

int	expand(t_list **head, char *line, t_data *data)
{
	t_list	*node;

	node = *head;
	while (node)
	{
		if (node->token->options & WORD
			&& !(node->token->options & EXPANDED_WORD))
			{
				if (expand_word(line, node, data))
					return (ft_printf(2, "expand_token() failed: (%s)\n", node->token->str), 1);
			}
		node = node->next;
	}
	// if (filename_expansion(head, line))
	// 	return (ft_printf(2, "expand_filename() failed\n"), 1);
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

t_btree	*create_exec_tree(t_parse_data *d, t_data *data)
{
	t_list		*tokens;
	t_print_d	print_data;

	print_data.line = d->line;
	print_data.operators = d->operators;
	tokens = tokenize(d->line, d->operators);
	if (!tokens)
		return (ft_printf(2, "minishell: tokenize() returned NULL\n"), NULL);
	if (validate_tokens(tokens, d->operators))
		return (ft_printf(2, "minishell: validate_tokens() failed\n"), del_tokens(tokens), NULL);
	if (expand(&tokens, d->line, data))
		return (ft_printf(2, "minishell: parse() failed\n"), del_tokens(tokens), NULL);
	// ft_lstiter(tokens, print_tokens, &print_data);
	d->exec_tree = create_tree(tokens, &d->line_count, &d->here_list);
	del_tokens(tokens);
	return (d->exec_tree);
}
