/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   binsearch.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jel-ghna <jel-ghna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 17:21:55 by mchoma            #+#    #+#             */
/*   Updated: 2025/10/17 15:04:15 by mchoma           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"

char	*isbin(char *path, char *command)
{
	char	*binpath;
	char	*tmp;

	if (command == NULL || path == NULL)
		return (NULL);
	tmp = ft_strjoin("/", command);
	if (!tmp)
		return (ft_putstr_fd("Malloc failed\n", 2), NULL);
	binpath = ft_strjoin(path, tmp);
	free(tmp);
	if (binpath)
	{
		if (access(binpath, F_OK) == 0)
			return (binpath);
	}
	free(binpath);
	return (NULL);
}

void	ft_free_split(char ***split)
{
	int		i;

	i = 0;
	while (split && (*split) && (*split)[i])
	{
		free((*split)[i]);
		i++;
	}
	if (split)
		free(*split);
	*split = NULL;
}

char	*get_env_path(char **envp)
{
	int		i;

	i = 0;
	while (envp[i])
	{
		if (ft_strnstr(envp[i], "PATH=", 5))
			return (envp[i]);
		i ++;
	}
	return (NULL);
}

char	*get_path_path(char**envp, char *command)
{
	int		j;
	char	**arr;
	char	*path;
	char	*tmp;

	arr = NULL;
	tmp = get_env_path(envp);
	arr = ft_split(tmp + 5, ':');
	tmp = NULL;
	j = 0;
	while (arr && arr[j])
	{
		path = isbin(arr[j], command);
		if (path && access(path, X_OK) == 0)
			return (ft_free_split(&arr), path);
		tmp = path;
		j++;
	}
	return (ft_free_split(&arr), tmp);
}

char	*get_path(char**envp, char *command)
{
	if (ft_strchr(command, '/'))
	{
		if (access(command, F_OK) == 0)
			return (ft_strdup(command));
		ft_putstr_fd("Didn't find executable binary\n", 2);
		return (NULL);
	}
	return (get_path_path(envp, command));
}
