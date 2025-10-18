/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_command.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchoma <your@mail.com>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/17 15:16:38 by mchoma            #+#    #+#             */
/*   Updated: 2025/10/17 15:18:22 by mchoma           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "../commands/commands.h"
#include "../libft/libft.h"
#include "../libft/idlist.h"
#include <signal.h>

//todo implement build ins
//if it is not found exit with 127 
//if found and not executable exit with 126

int	redirection(t_btree *tree)
{
	int		in;
	int		out;

	if (tree->redir.in)
	{
		in = open(tree->redir.in, O_RDONLY);
		if (in > 0)
			return (ft_putstrerr("failed to open file in redirection\n"), -1);
		dup2(in, STDIN_FILENO);
		close(in);
	}
	if (tree->redir.out)
	{
		if (tree->redir.append)
			out = open(tree->redir.out, O_WRONLY | O_CREAT | O_APPEND, 0777);
		else
			out = open(tree->redir.out, O_WRONLY | O_CREAT, 0777);
		if (out == -1)
			return (ft_putstrerr("failed to open file in redirection\n"), -1);
		dup2(out, STDOUT_FILENO);
		close(out);
	}
	return (0);
}

void	ft_execve(t_btree *tree, t_data *data)
{
	char	*path;
	int		out;
	int		in;

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_IGN);
	if (redirection(tree))
		ft_exit(data, "1");
	path = get_path(data->env, tree->cmd_argv[0]);
	if (path == NULL)
	{
		ft_putstrerr("Binary ");
		ft_putstrerr(tree->cmd_argv[0]);
		ft_putstrerr(" not found\n");
		ft_exit(data, "127");
	}
	execve(path, tree->cmd_argv, data->env);
	ft_putstrerr("Binary ");
	ft_putstrerr(path);
	ft_putstrerr(" not executable\n");
	free(path);
	ft_exit(data, "126");
}

void	ft_command(t_btree *tree, t_data *data)
{
	int		pid;

	if (is_buildin(tree, data) == 1)
		return ;
	pid = fork();
	if (pid == 0)
		ft_execve(tree, data);
	else if (add_last_id(&data->pids, pid))
		data->rt = 1;
	if (data->subshell == 1)
	{
		close(STDIN_FILENO);
		close(STDOUT_FILENO);
		exit(wait_and_get_exit_value(data->pids));
	}
}
