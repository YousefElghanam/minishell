#include "parsing.h"

static int	expand_fragment(t_expansion_data *xd)
{
	t_token	*token;
	int		res;
	char	*fragment_str;

	res = 1;
	token = xd->token_node->token;
	fragment_str = ft_substr(xd->d->line, token->fragments[xd->i].start,
		token->fragments[xd->i].end - token->fragments[xd->i].start + 1);
	if (!fragment_str)
		return (1);
	if (token->fragments[xd->i].type == SINGLE)
		res = expand_single_quoted_fragment(fragment_str, xd);
	else if (token->fragments[xd->i].type == DOUBLE)
		res = expand_double_quoted_fragment(fragment_str, xd);
	else if (token->fragments[xd->i].type == UNQUOTED)
		res = expand_unquoted_fragment(fragment_str, xd);
	free(fragment_str);
	return (res);
}

static void	set_xd(t_expansion_data *xd, t_parse_data *d, t_list *target_node)
{
	xd->d = d;
	xd->i = 0;
	xd->token_node = target_node;
	xd->target_node = &target_node;
}

static int	expand_word(t_parse_data *d, t_list *target_node)
{
	size_t				fragment_i;
	t_expansion_data	xd;

	set_xd(&xd, d, target_node);
	free(xd.token_node->token->str);
	xd.token_node->token->str = ft_strdup("");
	if (!xd.token_node->token->str)
		return (1);
	while (fragment_i < xd.token_node->token->fragment_count)
	{
		if (xd.token_node->token->fragments[fragment_i].type == EMPTY)
		{
			fragment_i++;
			continue ;
		}
		if(expand_fragment(&xd))
			return (1);
		fragment_i++;
	}
	if (!*xd.token_node->token->str && xd.token_node->token->fragments
		&& xd.token_node->token->fragments[0].type == UNQUOTED
		&& xd.token_node->token->fragment_count == 1)
		return (xd.token_node->token->options |= EMPTY_WORD, 0);
	return (0);
}

int	expand(t_parse_data *d)
{
	t_list	*node;

	node = d->tokens;
	while (node)
	{
		if (node->token->options & WORD
			&& !(node->token->options & EXPANDED_WORD))
			{
				if (expand_word(d, node))
					return (ft_printf(2, "expand_token() failed: (%s)\n", node->token->str), 1);
			}
		node = node->next;
	}
	// if (filename_expansion(head, line))
	// 	return (ft_printf(2, "expand_filename() failed\n"), 1);
	return (0);
}
