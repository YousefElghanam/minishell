/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchoma <your@mail.com>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/10 19:00:46 by mchoma            #+#    #+#             */
/*   Updated: 2025/10/12 14:39:22 by mchoma           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "commands.h"

//check for valid string that will be passed to this funcion
static int	arg_check(char *arg)
{
	size_t	i;

	i = 0;
	if (!arg[i] && !ft_isalpha(arg[i]) && !(arg[i] == '_'))
		return (0);
	i ++;
	while (arg[i])
	{
		if (arg[i] == '=')
			break ;
		if (!ft_isalnum(arg[i]) && !(arg[i] == '_'))
			return (0);
		i++;
	}
	return (1);
}

int	ft_export(t_data *data, char *arg)
{
	char	*var;

	if (arg_check(arg) == 0)
	{
		data->rt = 1;
		return (ft_putstr_fd("bash: export: '", 2),
			ft_putstrerr(arg), ft_putstr_fd("': not a valid identifier\n", 2),
			set_rt(&data->rt, 1), 0);
	}
	if (ft_strchr(arg, '='))
	{
		var = ft_substr(arg, 0, (size_t)(ft_strchr(arg, '=') - arg));
		if (var == NULL)
			return (set_rt(&data->rt, 1), 0);
		ft_unset(data, var);
	}
	else
		ft_unset(data, arg);
	if (ft_append_arr_str(&data->env, ft_strdup(arg)) == NULL)
		return (free(var), set_rt(&data->rt, 1), 0);
	return (set_rt(&data->rt, 0), 1);
}
