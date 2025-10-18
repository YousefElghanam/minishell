/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_buldins.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchoma <your@mail.com>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/17 15:00:31 by mchoma            #+#    #+#             */
/*   Updated: 2025/10/17 15:20:14 by mchoma           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../commands/commands.h"
#include "../libft/idlist.h"
#include "executor.h"

int	echo_maker(t_btree *tree, t_data *data)
{
	size_t	i;
	int		flag;
	char	*str;

	flag = 0;
	i = 1;
	str = NULL;
	if ((tree->cmd_argv)[i] != 0)
		if (ft_strncmp(tree->cmd_argv[i], "-n", 3) == 0 && ++flag)
			i++;
	while (tree->cmd_argv[i])
	{
		if (!(i == 1 || (i == 2 && flag == 1)))
		{
			str = ft_strjoinf1(str, " ");
			if (str == NULL)
				return (ft_putstrerr("fail\n"), add_last_id(&data->pids, -1));
		}
		str = ft_strjoinf1(str, tree->cmd_argv[i]);
		if (str == NULL)
			return (ft_putstrerr("fail\n"), add_last_id(&data->pids, -1));
		i ++;
	}
	return (ft_echo(data, str, flag), free(str), add_last_id(&data->pids, -1));
}

void	echo_wrap(t_btree *tree, t_data *data)
{
	int		pid;

	pid = -1;
	if (tree->redir.in || tree->redir.out)
	{
		pid = fork();
		if (pid == 0)
		{
			signal(SIGINT, SIG_DFL);
			signal(SIGQUIT, SIG_IGN);
			redirection(tree);
		}
	}
	if (pid <= 0)
		echo_maker(tree, data);
	if (pid == 0)
		ft_exit(data, NULL);
	if (pid != 0)
		add_last_id(&data->pids, pid);
}

void	cd_wrap(t_btree *tree, t_data *data)
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
	{
		if (!tree->cmd_argv[1])
			ft_cd(data, ft_strdup(""));
		else
			ft_cd(data, tree->cmd_argv[1]);
	}
	if (pid == 0)
		ft_exit(data, NULL);
	if (pid != 0)
		add_last_id(&data->pids, pid);
}

void	export_wrap(t_btree *tree, t_data *data)
{
	int		pid;

	pid = -1;
	if (tree->redir.in || tree->redir.out)
	{
		pid = fork();
		if (pid == 0)
		{
			signal(SIGINT, SIG_DFL);
			signal(SIGQUIT, SIG_IGN);
			redirection(tree);
		}
	}
	if (pid <= 0)
	{
		if (!tree->cmd_argv[1])
			ft_export(data, "");
		else
			ft_export(data, tree->cmd_argv[1]);
	}
	if (pid == 0)
		ft_exit(data, NULL);
	if (pid != 0)
		add_last_id(&data->pids, pid);
}

void	unset_wrap(t_btree *tree, t_data *data)
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
	{
		if (!tree->cmd_argv[1])
			ft_unset(data, ft_strdup(""));
		else
			ft_unset(data, tree->cmd_argv[1]);
	}
	if (pid == 0)
		ft_exit(data, NULL);
	if (pid != 0)
		add_last_id(&data->pids, pid);
}
