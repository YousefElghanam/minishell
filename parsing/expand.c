#include "parsing.h"

static void	set_xd(t_expansion_data *xd, t_parse_data *d, t_list *target_node)
{
	xd->d = d;
	xd->i = 0;
	xd->token_node = target_node;
	xd->target_node = target_node;
}

static int	expand_word(t_parse_data *d, t_list *target_node)
{
	t_expansion_data	xd;

	set_xd(&xd, d, target_node);
	free(xd.token_node->token->str);
	xd.token_node->token->str = ft_strdup("");
	if (!xd.token_node->token->str)
		return (1);
	while (xd.i < xd.token_node->token->fragment_count)
	{
		if (xd.token_node->token->fragments[xd.i].type == EMPTY)
		{
			xd.i++;
			continue ;
		}
		if (expand_fragment(&xd))
			return (1);
		xd.i++;
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
				return (ft_printf(2, "expand_token() failed: (%s)\n",
						node->token->str), 1);
		}
		node = node->next;
	}
	// if (filename_expansion(&d->tokens, d->line))
	// 	return (ft_printf(2, "expand_filename() failed\n"), 1);
	return (0);
}
