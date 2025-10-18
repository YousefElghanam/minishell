/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jel-ghna <jel-ghna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 16:37:40 by mchoma            #+#    #+#             */
/*   Updated: 2025/10/08 11:55:08 by jel-ghna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "commands.h"

int	ft_pwd(t_data *data)
{
	char	*path;

	path = malloc(PATH_MAX);
	if (path == NULL)
		return (ft_putstrerr("malloc error \n"),
			free(path), set_rt(&data->rt, 1), 0);
	if (getcwd(path, PATH_MAX) == NULL)
		return (ft_putstrerr("get_cwd error \n"),
			free(path), set_rt(&data->rt, 1), 0);
	if (printf("%s\n", path) == -1)
		return (ft_putstrerr("printf_eror\n"),
			free(path), set_rt(&data->rt, 1), 0);
	return (free(path), set_rt(&data->rt, 0), 1);
}
