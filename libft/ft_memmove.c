/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jel-ghna <jel-ghna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 21:54:00 by josefelghna       #+#    #+#             */
/*   Updated: 2025/04/22 13:58:12 by jel-ghna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	unsigned char	*p_src;
	unsigned char	*p_dest;

	p_src = (unsigned char *)src;
	p_dest = (unsigned char *)dest;
	if (!n)
		return (dest);
	if (p_src > p_dest)
		while (n--)
			*p_dest++ = *p_src++;
	else if (p_src < p_dest)
		while (n--)
			p_dest[n] = p_src[n];
	return (dest);
}

// #include <stdio.h>
// #include <unistd.h>

// int	main(void)
// {
// 	char	*str = ft_strdup("abcdefghijklmnopqrstuvwxyz!!!");
// 	char	*res = (char *)ft_memmove(str + 5, str, 5);
// 	printf("%s\n", res);
// 	write(1, str, 30);
// 	return (0);
// }