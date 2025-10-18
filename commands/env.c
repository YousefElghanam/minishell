/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchoma <your@mail.com>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/10 18:59:56 by mchoma            #+#    #+#             */
/*   Updated: 2025/10/10 19:00:57 by mchoma           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "commands.h"

int	ft_env(t_data *data)
{
	size_t	i;

	i = 0;
	if (data->env == NULL)
		return (set_rt(&data->rt, 0), 1);
	while (data->env[i] != NULL)
	{
		if (printf("%s\n", data->env[i]) == -1)
		{
			data->rt = 1;
			return (set_rt(&data->rt, 1), 0);
		}
		i ++;
	}
	return (set_rt(&data->rt, 0), 1);
}
