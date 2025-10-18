#include "parsing.h"

/*
and_or:			pipeline { "&&" | "||" pipeline }
pipeline:		command { "|" command }
command:		simple_command | subshell
simple_command:	WORD { WORD }
subshell:		"(" and_or ")"
*/

t_btree	*parse_and_or(t_list **tokens, t_here_doc **here_list);

t_list	*consume_token(t_list **tokens)
{
	t_list	*cur;

	cur = *tokens;
	if (cur)
		*tokens =  cur->next;
	return (cur);
}

t_btree	*make_bnode(t_bnode_type type, t_btree *left, t_btree *right)
{
	t_btree	*node;

	node = malloc(sizeof(t_btree));
	if (!node)
		return (ft_printf(2, "malloc fail in make_bnode()\n"), NULL);
	node->cmd_argv = NULL;
	node->redir.append = 0;
	node->redir.in = NULL;
	node->redir.out = NULL;
	node->type = type;
	node->left = left;
	node->right = right;
	return (node);
}

int	store_words(t_list **tokens, char **cmd_argv)
{
	t_list	*cur;
	size_t	word_count;
	char	*word;

	cur = *tokens;
	word_count = 0;
	while (cur && (cur->token->options & WORD || cur->token->options & REDIR_OP))
	{
		if (cur->token->options & EMPTY_WORD)
		{
			cur = cur->next;
			consume_token(tokens);
			continue ;
		}
		if (cur->token->options & REDIR_OP)
		{
			cur = cur->next->next; // MAYBE WE CAN HANDLE AMBIGUOUS REDIRECTIONS HERE??
			continue ;
		}
		word = ft_strdup(cur->token->str);
		if (!word)
		{
			cmd_argv[word_count] = NULL;
			return (free_split(cmd_argv), 1);
		}
		cmd_argv[word_count++] = word;
		cur = cur->next;
		consume_token(tokens);
	}
	cmd_argv[word_count] = NULL;
	return (0);
}

char	**create_cmd_argv(t_list **tokens)
{
	t_list	*cur;
	size_t	word_count;
	char	**cmd_argv;

	word_count = 0;
	cur = *tokens;
	while (cur && (cur->token->options & WORD || cur->token->options & REDIR_OP))
	{
		if (cur->token->options & REDIR_OP)
			cur = cur->next;
		else if (!(cur->token->options & EMPTY_WORD))
			word_count++;
		cur = cur->next;
	}
	cmd_argv = malloc(sizeof(char *) * (word_count + 1));
	if (!cmd_argv)
		return (NULL);
	if (store_words(tokens, cmd_argv))
		return (ft_printf(2, "malloc failed in store_words()\n"), NULL);
	return (cmd_argv);
}

int	add_here_node(t_here_doc **here_list, char *delimiter, t_btree *bnode)
{
	t_here_doc	*here_node;
	t_here_doc	*cur;

	// printf(">>>>> delimiter is (%s)\n>>>>> bnode is (%s)\n", delimiter, bnode->cmd_argv[0]);
	here_node = malloc(sizeof(t_here_doc));
	if (!here_node)
		return (1);
	here_node->bnode = bnode;
	here_node->delimiter = ft_strdup(delimiter);
	here_node->file_name = NULL;
	if (!here_node->delimiter)
		return (free(here_node), 1);
	here_node->next = NULL;
	if (!*here_list)
		*here_list = here_node;
	else
	{
		cur = *here_list;
		while (cur->next)
			cur = cur->next;
		cur->next = here_node;
	}
	return (0);
}

int	create_redirections(t_list **tokens, t_btree *bnode, t_here_doc **here_list)
{
	t_list	*cur;

	cur = *tokens;
	bnode->redir.in = NULL;
	bnode->redir.out = NULL;
	bnode->redir.append = 0;
	while (cur && (cur->token->options & WORD || cur->token->options & REDIR_OP))
	{
		if (cur->token->options & REDIR_OP)
		{
			if (cur->token->options & HERE_DOC)
			{
				if (add_here_node(here_list, cur->next->token->str, bnode))
					return (printf("add_here_node failed in create_redirections()\n"), 1);
			}
			else if (cur->token->options & OUTPUT_REDIR_APPEND)
			{
				bnode->redir.append = 1;
				free(bnode->redir.out);
				bnode->redir.out = ft_strdup(cur->next->token->str);
				if (!bnode->redir.out)
					return (free(bnode->redir.in), 1);
			}
			else if (cur->token->options & INPUT_REDIR)
			{
				free(bnode->redir.in);
				bnode->redir.in = ft_strdup(cur->next->token->str);
				if (!bnode->redir.in)
					return (free(bnode->redir.out), 1);
			}
			else if (cur->token->options & OUTPUT_REDIR)
			{
				free(bnode->redir.out);
				bnode->redir.out = ft_strdup(cur->next->token->str);
				if (!bnode->redir.out)
					return (free(bnode->redir.in), 1);
			}
			cur = cur->next;
			consume_token(tokens);
		}
		cur = cur->next;
		consume_token(tokens);
	}
	// printf("for bnode (%s), redirect input to (%s) and output to (%s)\n", bnode->cmd_argv[0], bnode->redir.in, bnode->redir.out);
	return (0);
}

t_btree	*parse_command(t_list **tokens, t_here_doc **here_list)
{
	t_list	*cur;
	t_btree	*bnode;

	cur = *tokens;
	if (!cur)
		return (NULL);
	if (cur->token->options & OPEN_PARENTHESIS)
	{
		consume_token(tokens);
		bnode = parse_and_or(tokens, here_list);
		cur = *tokens;
		if (!cur || !(cur->token->options & CLOSE_PARENTHESIS))
			return (ft_printf(2, "Error: Expected `)'\n"), NULL);
		consume_token(tokens);
		return (make_bnode(BNODE_SUBSHELL, bnode, NULL));
	}
	else if (cur->token->options & WORD || cur->token->options & REDIR_OP)
	{
		bnode = make_bnode(BNODE_COMMAND, NULL, NULL);
		if (!bnode)
			return (ft_printf(2, "malloc failed in make_bnode(): parse_command()\n"), NULL);
		bnode->cmd_argv = create_cmd_argv(tokens);
		if (!bnode->cmd_argv)
			return (ft_printf(2, "malloc failed in create_cmd_argv()\n"), NULL);
		if (create_redirections(tokens, bnode, here_list))
			return (ft_printf(2, "create_redirections() failed\n"), free_split(bnode->cmd_argv), NULL);
		return (bnode);
	}
	return (ft_printf(2, "Error: Unexpected token\n"), NULL);
}

t_btree	*parse_pipeline(t_list **tokens, t_here_doc **here_list)
{
	t_btree	*left;
	t_btree	*right;
	t_list	*cur;

	left = parse_command(tokens, here_list);
	while (*tokens && (*tokens)->token->options & PIPE)
	{
		cur = *tokens;
		consume_token(tokens);
		right = parse_command(tokens, here_list);
		left = make_bnode(BNODE_PIPE, left, right);
	}
	return (left);
}

t_btree	*parse_and_or(t_list **tokens, t_here_doc **here_list)
{
	t_btree	*left;
	t_btree	*right;
	t_list	*cur;

	left = parse_pipeline(tokens, here_list);
	while ((*tokens)
		&& ((*tokens)->token->options & AND || (*tokens)->token->options & OR))
	{
		cur = *tokens;
		consume_token(tokens);
		right = parse_pipeline(tokens, here_list);
		if (cur->token->options & AND)
			left = make_bnode(BNODE_AND, left, right);
		else if (cur->token->options & OR)
			left = make_bnode(BNODE_OR, left, right);
	}
	return (left);
}

char	*here_name(int *here_i)
{
	char	*name;
	char	*name_index;

	name_index = ft_itoa(*here_i);
	if (!name_index)
		return (NULL);
	name = ft_strjoin(".tmp", name_index); // RECHECK WHERE TO SAVE THE FILE!! ALSO CAN WE DO PIPE HERE_DOC LIKE BASH?
	free(name_index);
	if (!name)
		return (NULL);
	if (access(name, F_OK) == 0)
	{
		*here_i += 1;
		return (here_name(here_i));
	}
	return (name);
}

int	open_here_docs(t_here_doc **here_list, int *line_count)
{
	static	int	here_i;
	char		*file_name;
	int			here_fd;
	t_here_doc	*cur;
	char		*line;
	size_t		delimiter_len;

	cur = *here_list;
	here_fd = -1;
	while (cur)
	{
		if (here_fd > 0)
			close(here_fd);
		file_name = here_name(&here_i);
		if (!file_name)
			return (printf("here_name() failed\n"), 1);
		cur->file_name = file_name;
		here_fd = open(file_name, O_WRONLY | O_CREAT | O_EXCL, 0666);
		if (here_fd < 0)
			return (printf("open failed in open_here_docs\n"), 1);
		delimiter_len = ft_strlen(cur->delimiter);
		while (1)
		{
			line = readline(">");
			if (!line)
			{
				ft_printf(2, "bash: warning: here-document at line %d delimited by end-of-file (wanted `%s')\n", *line_count, cur->delimiter);
				free(line);
				break ;
			}
			if (ft_strncmp(line, cur->delimiter, delimiter_len) == 0 && line[delimiter_len] == 0)
			{
				printf("delimiter met for (%s)\n", cur->delimiter);
				free(line);
				break ;
			}
			(write(here_fd, line, ft_strlen(line)), write(here_fd, "\n", 1), free(line));
			*line_count += 1;
		}
		cur->bnode->redir.in = file_name;
		cur = cur->next;
	}
	return (0);
}

// BIG ISSUE FOUND: WHAT HAPPENS WHEN MALLOC FAILS DEEP INSIDE ONE OF THESE FUNCTIONS?? YOU RETURN NULL, WHICH IS A VALID RETURN VALUE??

t_btree	*create_exec_tree(t_parse_data *d) //t_list *tokens, int *line_count, t_here_doc **here_list)
{
	t_btree	*tree;
	t_list	*tokens;

	tokens = d->tokens;
	tree = parse_and_or(&tokens, d->here_list);
	if (open_here_docs(&d->here_list, d->line_count))
		return (printf("run_here_doc() failed\n"), NULL);
	// for (t_list *cur = d->tokens; cur; cur = cur->next)
	// 	printf("node:(%s)\n", cur->token->str);
	// printf("redirect input to (%s) and output to (%s)\n", tree->redir.in, tree->redir.out);
	return (tree);
}
