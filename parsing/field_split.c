
#include "parsing.h"

static int	create_split_tokens(t_list **head, char **split_arr,
	size_t fragment_i, t_list *token_node)
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
		new_token->fragment_i = fragment_i;
		new_token->fragments = malloc(sizeof (t_fragment)); // MAKE FRAGMENTS FOR THIS AND MAKE IT UQNUOTED FRAGMENTS, SO WHEN YOU PASS IT TO FILENAME_EXPANSION IT KNOWS THAT IT CAME FROM AN UNQUOTED EXPANSION
		if (!new_token->fragments)
			return (free(new_token), 1);
		ft_memmove(new_token->fragments, token_node->token->fragments,
			sizeof(t_fragment));
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

static int	insert_split_tokens(t_list **field_split_head, t_list *token_node,
	t_list **target_node, size_t fragment_i)
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

int	handle_str_of_dels(char **split_arr)
{
	if (!*split_arr)
	{
		free(split_arr);
		split_arr = malloc(sizeof(char *) * 2);
		if (!split_arr)
			return (1);
		split_arr[0] = ft_strdup("");
		if (!split_arr[0])
			return (free(split_arr), 1);
		split_arr[1] = NULL;
	}
	return (0);	
}

int	handle_str_of_one_word(char **split_arr, t_expansion_data *xd)
{
	char	**tmp;

	if (split_arr[0] && !split_arr[1]
		&& xd->token_node->token->fragments[xd->i].ends_with_space
		&& xd->i + 1 < xd->token_node->token->fragment_count)
	{
		tmp = malloc(sizeof(char *) * 3);
		if (!tmp)
			return (1);
		tmp[0] = split_arr[0];
		tmp[1] = ft_strdup("");
		if (!tmp[1])
			return (free(tmp), 1);
		tmp[2] = NULL;
		free(split_arr);
		split_arr = tmp;
	}
	return (0);
}

int	field_split(char *fragment_str, char *expanded, t_expansion_data *xd)
{
	t_list	*head;
	char	**split_arr;

	head = NULL;
	// CAREFULL, THIS FT_SPLIT RETURNS 0 STRINGS IN THE ARRAY IF WE PASS A STRING OF ONLY DELIMITERS TO IT.
	// I EXPECTED IT TO RETURN 1 EMPTY STRING, THAT'S WHY I HAVE TO HANDLE IT SEPARATELY NEXT.
	split_arr = ft_split_dels(expanded, " 	");
	if (!split_arr || handle_str_of_dels(split_arr))
		return (1);
	if (handle_str_of_one_word(split_arr, xd))
		return (free_split(split_arr), 1);
	if (!xd->token_node->token->fragments[xd->i].starts_with_space
		|| xd->i == 0)
	{
		append_substr(*(xd->target_node), split_arr[0], 0);
		if (create_split_tokens(&head, split_arr + 1, xd->i, xd->token_node))
			return (del_tokens(head), free_split(split_arr), 1);
	}
	else
		if (create_split_tokens(&head, split_arr, xd->i, xd->token_node))
			return (del_tokens(head), free_split(split_arr), 1);
	free_split(split_arr);
	if (insert_split_tokens(&head, xd->token_node, xd->target_node, xd->i))
		return (del_tokens(head), 1);
	return (0);
}
