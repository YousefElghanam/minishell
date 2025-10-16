#include "parsing.h"

static int	fragment_quoted(char *line, t_token *token, size_t *i, char quote)
{
	char		*match;

	match = ft_strchr(&line[*i + 1], quote);
	if (!match)
		return (ft_printf(2, "minishell: unclosed quote `%c'\n", quote), 1);
	if (match == &line[*i + 1])
	{
		token->fragments[token->fragment_count].type = EMPTY;
		token->fragment_count += 1;
		*i += 2;
		return (0);
	}
	token->fragments[token->fragment_count].start = *i + 1;
	token->fragments[token->fragment_count].end = *i + 1 + (match - &line[*i] - 2);
	token->fragments[token->fragment_count].starts_with_space = 0;
	token->fragments[token->fragment_count].ends_with_space = 0;
	token->fragments[token->fragment_count].type = (quote == '\"') + 1;
	token->fragment_count += 1;
	*i += 1 + (match - &line[*i]);
	return (0);
}

static void	fragment_unquoted(char *line, t_token *token, size_t *i, char **operators)
{
	size_t	unquoted_len;

	unquoted_len = len_to_quote_or_delimiter(&line[*i], operators);
	token->fragments[token->fragment_count].start = *i;
	token->fragments[token->fragment_count].end = *i + unquoted_len - 1;
	token->fragments[token->fragment_count].starts_with_space = 0;
	token->fragments[token->fragment_count].ends_with_space = 0;
	token->fragments[token->fragment_count].type = UNQUOTED;
	token->fragment_count += 1;
	*i += unquoted_len;
}

int	handle_fragments(char *line, char **operators, t_token *token, size_t *i)
{
	size_t	fragment_count;

	fragment_count = 0;
	while(line[*i] && len_to_unquoted_delimiter(&line[*i], operators) > 0)
	{
		if (line[*i] == '\"' || line[*i] == '\'')
		{
			if (fragment_quoted(line, token, i, line[*i]))
				return (1);
		}
		else if (line[*i])
			fragment_unquoted(line, token, i, operators);
	}
	return (0);
}
