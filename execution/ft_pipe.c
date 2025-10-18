/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pipe.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchoma <your@mail.com>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 19:04:26 by mchoma            #+#    #+#             */
/*   Updated: 2025/10/17 15:14:43 by mchoma           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "../libft/libft.h"
#include "../libft/idlist.h"

void	ft_putstrerr(char *str);

void	read_pipe(t_btree *tree, t_data *data, int *fd)
{
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	close(STDIN_FILENO);
	data->subshell = 1;
	dup2(fd[0], STDIN_FILENO);
	close(fd[0]);
	close(fd[1]);
	free_pids(&data->pids);
	execute(tree->right, data);
}

void	write_pipe(t_btree *tree, t_data *data, int *fd)
{
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	close(STDOUT_FILENO);
	data->subshell = 1;
	dup2(fd[1], STDOUT_FILENO);
	close(fd[0]);
	close(fd[1]);
	free_pids(&data->pids);
	execute(tree->left, data);
}

void	ft_pipe(t_btree *tree, t_data *data)
{
	int		fd[2];
	int		pid;

	if (pipe(fd))
	{
		ft_putstrerr("Pipe has failed\n");
		data->rt = 1;
		return ;
	}
	pid = fork();
	if (pid == 0)
		read_pipe(tree, data, fd);
	if (add_last_id(&data->pids, pid))
		fprintf(stderr, "error add_last_id\n");
	pid = fork();
	if (pid == 0)
		write_pipe(tree, data, fd);
	if (add_last_id(&data->pids, pid))
		fprintf(stderr, "error add_last_id\n");
	close (fd [0]);
	close (fd [1]);
	data->rt = wait_and_get_exit_value(data->pids);
	free_pids(&data->pids);
}
