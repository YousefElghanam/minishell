#ifndef MINISHELL_H
# define MINISHELL_H

//this contains ids of the child processes that are currently running
typedef struct s_ids
{
	struct s_ids	*next;
	int				pid;
}	t_ids;

typedef struct s_data
{
	char			**env;
	int				rt;
	t_ids			*pids;
	struct s_btree	*head;
	int				subshell;
}	t_data;

typedef enum	e_bnode_type
{
	BNODE_COMMAND,
	BNODE_PIPE,
	BNODE_AND,
	BNODE_OR,
	BNODE_SUBSHELL
}	t_bnode_type;

//if append == 0 truncate 
//if append == 1 append
typedef struct s_redir
{
	char	*in;
	char	*out;
	int		append;
}	t_redir;

typedef struct	s_btree
{
	struct s_btree	*left;
	struct s_btree	*right;
	char			**cmd_argv;
	t_bnode_type	type;
	t_redir			redir;
}	t_btree;

typedef struct	s_here_doc
{
	char				*delimiter;
	t_btree				*bnode;
	char				*file_name;
	struct s_here_doc	*next;
}	t_here_doc;
//this will contain envp
typedef struct	s_parse_data
{
	char		*line;
	char		*operators[10];
	t_list		*tokens;
	int			*line_count;
	t_here_doc	*here_list;
	t_btree		*exec_tree;
	t_data		*data;
}	t_parse_data;


# include "libft/libft.h"
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include <sys/wait.h>
# include <errno.h>
# include <fcntl.h>


/* DEV print.c */
void	btree_apply_suffix(t_btree *root, void (*applyf)(void *));
void	print_btree_pyramid(const t_btree *node);

/* parsing/parsing.c */
t_btree	*parse(t_parse_data *d, t_data *data);
// int		btoindex(int options);

// execute
void	execute(t_btree *tree, t_data *data);

int		wait_and_get_exit_value(t_ids *list);
void	delete_bnode(void *ptr);
int		ft_exit(t_data *data, char *arg);
#endif
