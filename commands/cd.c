/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jel-ghna <jel-ghna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 13:07:28 by mchoma            #+#    #+#             */
/*   Updated: 2025/10/10 14:45:11 by mchoma           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "commands.h"

int	cd_path(t_data *data, char **path)
{
	char	*str;
	char	**arr;
	ssize_t	i;

	i = -1;
	str = ft_get_env_value(data->env, "CDPATH");
	if (str == NULL)
		return (0);
	arr = ft_split(str, ':');
	if (arr == NULL)
		return (free(str), 0);
	free(str);
	while (arr[i] && i++)
	{
		str = ft_strjoin(arr[i], "/");
		if (str == NULL)
			return (free_arr((void ***)&arr), 0);
		str = ft_strjoinf1(str, *path);
		if (str == NULL)
			return (free_arr((void ***)&arr), 0);
		if (chdir (str) != -1)
			return (free_arr((void ***) &arr), free(str), 2);
		free(str);
	}
	return (free_arr((void ***) &arr), 1);
}

int	ft_cd(t_data *data, char *path)
{
	size_t	i;

	if (*path != '/')
	{
		i = cd_path(data, &path);
		if (i == 0)
			return (set_rt(&data->rt, 1), 0);
		if (i == 2)
			return (set_rt(&data->rt, 0), 2);
	}
	if (*path == 0)
	{
		free(path);
		path = ft_get_env_value(data->env, "HOME");
		if (path == NULL)
			return (set_rt(&data->rt, 1), 0);
	}
	if (chdir(path) == -1)
	{
		ft_putstr_fd("bash: cd: ", 2);
		ft_putstr_fd(path, 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		return (set_rt(&data->rt, 1), 0);
	}
	return (set_rt(&data->rt, 0), 1);
}
