/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_buildins2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchoma <your@mail.com>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/17 15:17:42 by mchoma            #+#    #+#             */
/*   Updated: 2025/10/17 15:19:03 by mchoma           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "executor.h"
#include "../commands/commands.h"
#include "../libft/libft.h"
#include "../libft/idlist.h"

void	env_wrap(t_btree *tree, t_data *data)
{
	int		pid;

	pid = -1;
	if (tree->redir.in || tree->redir.out)
	{
		pid = fork();
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_IGN);
		redirection(tree);
	}
	if (pid <= 0)
		ft_env(data);
	if (pid == 0)
		ft_exit(data, NULL);
	if (pid != 0)
		add_last_id(&data->pids, pid);
}

void	pwd_wrap(t_btree *tree, t_data *data)
{
	int		pid;

	pid = -1;
	if (tree->redir.in || tree->redir.out)
	{
		pid = fork();
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_IGN);
		redirection(tree);
	}
	if (pid <= 0)
		ft_pwd(data);
	if (pid == 0)
		ft_exit(data, NULL);
	if (pid != 0)
		add_last_id(&data->pids, pid);
}

void	exit_wrap(t_btree *tree, t_data *data)
{
	ft_exit(data, tree->cmd_argv[1]);
	add_last_id(&data->pids, -1);
}

int	is_buildin(t_btree *tree, t_data *data)
{
	if (ft_strncmp(tree->cmd_argv[0], "cd", 3) == 0)
		return (cd_wrap(tree, data), 1);
	if (ft_strncmp(tree->cmd_argv[0], "pwd", 4) == 0)
		return (pwd_wrap(tree, data), 1);
	if (ft_strncmp(tree->cmd_argv[0], "echo", 5) == 0)
		return (echo_wrap(tree, data), 1);
	if (ft_strncmp(tree->cmd_argv[0], "env", 4) == 0)
		return (env_wrap(tree, data), 1);
	if (ft_strncmp(tree->cmd_argv[0], "unset", 6) == 0)
		return (unset_wrap(tree, data), 1);
	if (ft_strncmp(tree->cmd_argv[0], "export", 7) == 0)
		return (export_wrap(tree, data), 1);
	if (ft_strncmp(tree->cmd_argv[0], "exit", 5) == 0)
		return (exit_wrap(tree, data), 1);
	return (0);
}
