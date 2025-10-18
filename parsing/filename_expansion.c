/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   filename_expansion.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jel-ghna <jel-ghna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 16:26:33 by jel-ghna          #+#    #+#             */
/*   Updated: 2025/10/18 01:16:35 by jel-ghna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

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

static void	star_forward(size_t *i, size_t *j, char *match, char *str)
{
	size_t	tmp;

	tmp = 0;
	if (str[(*i)] != 0 && match[(*j)] == '*' && match[((*j)) + 1] == 0)
	{
		while (str[(*i)])
			(*i)++;
		while (match[(*j)])
			(*j)++;
		return ;
	}
	while (str[(*i) + tmp] == match[(*j) + 1 + tmp] && str[(*i) + tmp]
		&& match[(*j) + 1 + tmp])
		tmp ++;
	if ((match[(*j) + 1 + tmp] == '*' || match[(*j) + 1 + tmp] == 0) && tmp != 0)
	{
		(*j) = (*j) + tmp + 1;
		(*i) = (*i) + tmp;
	}
	else
		((*i)) ++;
}

static int		star_match(char *match, char *str)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	while(1)
	{
		if (match[j] == '*' && match[j + 1] == '*')
			j ++;
		if (match[j] == '*' &&  str[i])
			star_forward(&i, &j, match, str);
		else if (match[j] == str[i] && match[j] && str[i])
		{
			i ++;
			j ++;
		}
		else
		{
			if (str[i] == 0 && match[j] == 0)
				return (1);
			if (match[j] == '*' && match[j + 1] == 0)
				return (1);
			return (0);
		}
	}
}

char	**expand_star_append(char *match, char ***arr)
{
	DIR				*directory;
	struct dirent	*idk;
	char			path[PATH_MAX];
	char			*str;

	if (getcwd(path, PATH_MAX) == NULL)
		return (NULL);
	if (ft_opendir(path, &directory) == NULL)
		return (*arr);
	idk = readdir(directory);
	while (idk)
	{
		if (star_match(match, idk->d_name) == 1)
		{
			str = ft_strdup(idk->d_name);
			if (str == NULL)
				return (closedir(directory), NULL);
			if (ft_append_arr_str(arr, str) == NULL)
				return (free(str), closedir(directory), NULL);
		}
		idk = readdir(directory);
	}
	return (free(idk), closedir(directory), *arr);
}

int	create_files_tokens(t_list **head, char **split_arr, size_t fragment_i, t_list *token_node)
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

int	expand_filename(t_list **node, size_t fragment_i)
{
	char	**files_arr;
	t_list	*files_list;

	files_arr = NULL;
	files_list = NULL;
	files_arr = expand_star_append((*node)->token->str, &files_arr);
	if (!files_arr)
		return (printf("expand_star_append() returned NULL"), 0);
	if (create_files_tokens(&files_list, files_arr, fragment_i, *node))
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
		if (node->token->options & WORD && !(node->token->options & EXPANDED_WORD)
			&& node->token->fragments[0].type == UNQUOTED) // THIS MIGHT NEED TO BE CHANGED, READ THE MANUAL, SEARCH FOR UNQUOTED '*'
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
	for (t_list *cur = *head; cur; cur = cur->next)
		if (cur->token->options & WORD)
			printf("tokens is (%s)\n", cur->token->str);
	return (0);
}
