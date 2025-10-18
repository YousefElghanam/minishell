/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jel-ghna <jel-ghna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 15:07:09 by mchoma            #+#    #+#             */
/*   Updated: 2025/10/08 11:55:18 by jel-ghna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "commands.h"

static int	check_for_var(char ***envp, char **variable)
{
	size_t	i;
	char	*tmp;

	i = 0;
	tmp = ft_strjoin(*variable, "=");
	if (tmp == NULL)
		return (0);
	while ((*envp)[i])
	{
		if (ft_strnstr((*envp)[i], tmp, ft_strlen(tmp)) == NULL)
			i ++;
		else
			return (free(tmp), 1);
	}
	return (free(tmp), 0);
}

void	coppy_env_spc(t_data *data, char *variable, int j, char **new)
{
	int	i;

	i = j;
	while ((data->env)[i])
	{
		if (ft_strnstr((data->env)[i], variable, ft_strlen(variable)) == NULL)
		{
			new[j] = (data->env)[i];
			j ++;
		}
		else
			free((data->env)[i]);
		i ++;
	}
}

int	ft_unset(t_data *data, char *variable)
{
	size_t	i;
	size_t	j;
	char	*tmp;
	char	**new;

	j = check_for_var(&data->env, &variable);
	i = 0;
	tmp = ft_strjoin(variable, "=");
	if (tmp == NULL)
		return (set_rt(&data->rt, 1), 0);
	new = ft_calloc(sizeof(char *), ft_arrlen((void **)data->env) - j + 1);
	if (new == NULL)
		return (free(tmp), set_rt(&data->rt, 1), 0);
	j = 0;
	coppy_env_spc(data, variable, j, new);
	free(data->env);
	data->env = new;
	return (free(tmp), set_rt(&data->rt, 0), 1);
}
