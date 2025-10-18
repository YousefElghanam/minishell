/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   idlist.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jel-ghna <jel-ghna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 18:30:29 by mchoma            #+#    #+#             */
/*   Updated: 2025/10/17 15:05:46 by mchoma           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IDLIST_H
# define IDLIST_H

# include "../minishell.h"

//this contains ids of the child processes that are currently running
// typedef struct s_ids
// {
// 	struct s_ids	*next;
// 	int				pid;
// }	t_ids;

//returns -1 on failiure and 0 on succsess doesn't free the list 
int		add_last_id(t_ids **list, int id);
//needs testing
void	free_pids(t_ids **list);
#endif
