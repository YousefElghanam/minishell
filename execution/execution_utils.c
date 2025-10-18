/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jel-ghna <jel-ghna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 18:59:46 by mchoma            #+#    #+#             */
/*   Updated: 2025/10/17 15:04:41 by mchoma           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

int	wait_and_get_exit_value(t_ids *list)
{
	int	rt;

	if (list == NULL)
		return (0);
	if (list->pid == -1)
		return (wait_and_get_exit_value(list->next));
	if (list->next == NULL)
	{
		waitpid(list->pid, &rt, 0);
		if (WIFSIGNALED(rt))
			return (WTERMSIG(rt) + 128);
		WEXITSTATUS(rt);
		return (WEXITSTATUS(rt));
	}
	waitpid(list->pid, NULL, 0);
	return (wait_and_get_exit_value(list->next));
}
