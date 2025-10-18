/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jel-ghna <jel-ghna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/10 15:33:57 by mchoma            #+#    #+#             */
/*   Updated: 2025/10/12 22:46:35 by jel-ghna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "commands.h"
#include "../minishell.h"

int	ft_exit(t_data *data, char *str)
{
	int	i;

	if (str)
		i = ft_atoi(str);
	else
		i = data->rt;
	if (data->head != NULL)
		btree_apply_suffix(data->head, delete_bnode);
	exit(i);
}
