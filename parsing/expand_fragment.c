#include "parsing.h"

static char	*create_expanded_fragment(char *fragment_str, char **target_str,
	t_expansion_data *xd)
{
	char	*var_value;
	size_t	start;
	size_t	i;

	start = 0;
	i = 0;
	while (fragment_str[i])
	{
		if (fragment_str[i] == '$')
		{
			fragment_str[i] = 0;
			if (!safe_strjoin(*target_str, &fragment_str[start], 0))
				return (NULL);
			start = i + 1;
			var_value = create_var_val(&fragment_str[start], &start, xd);
			if (!var_value || !safe_strjoin(*target_str, var_value, 1))
				return (NULL);
			i = start;
		}
		else
			i++;
	}
	return (safe_strjoin(*target_str, &fragment_str[start], 0));
}

static int	append_substr(t_list *target_node, char *str, int free_second_str)
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

int	expand_single_quoted_fragment(char *fragment_str, t_expansion_data *xd)
{
	char	*tmp;

	tmp = ft_strjoin((*xd->target_node)->token->str, fragment_str);
	if (!tmp)
		return (1);
	free((*xd->target_node)->token->str);
	(*xd->target_node)->token->str = tmp;
	return (0);
}

int	expand_double_quoted_fragment(char *fragment_str, t_expansion_data *xd)
{
	char	*var_val;
	size_t	start;
	size_t	i;

	start = 0;
	i = 0;
	while (fragment_str[i])
	{
		if (fragment_str[i] == '$')
		{
			fragment_str[i] = 0;
			if (append_substr(*xd->target_node, &fragment_str[start], 0))
				return (1);
			start = i + 1;
			var_val = create_var_val(&fragment_str[start], &start, xd->d->data);
			if (!var_val)
				return (1);
			if (append_substr(*xd->target_node, var_val, 1))
				return (1);
			i = start;
		}
		else
			i++;
	}
	return (append_substr(*xd->target_node, &fragment_str[start], 0));
}

int	expand_unquoted_fragment(char *fragment_str, t_expansion_data *xd)
{
	t_token	*token;
	char	*expanded;

	token = xd->token_node->token;
	expanded = ft_strdup("");
	if (!expanded)
		return (1);
	expanded = create_expanded_fragment(fragment_str, &expanded, xd);
	if (!expanded)
		return (1);
	if (!ft_strchr(expanded, ' ') && !ft_strchr(expanded, '	'))
		return (append_substr(*(xd->target_node), expanded, 1), 0);
	token->fragments[xd->i].starts_with_space = (ft_strchr(" 	", expanded[0])
		!= NULL);
	token->fragments[xd->i].ends_with_space = (*expanded
		&& ft_strchr(" 	", expanded[ft_strlen(expanded) - 1]));
	if (field_split(fragment_str, expanded, xd))
		return (free(expanded), 1);
	return (free(expanded), 0);
}
