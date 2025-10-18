/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jel-ghna <jel-ghna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 18:07:20 by jel-ghna          #+#    #+#             */
/*   Updated: 2025/10/08 10:34:03 by jel-ghna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <stdarg.h>
# include <unistd.h>
# include "libft.h"

typedef struct s_base
{
	char	*base;
	size_t	b_len;
}	t_base;

void	handle_sign(int fd, char c, va_list args, int *count);
int		parse_format(int fd, const char *format, va_list args, int *count);
int		ft_printf(int fd, const char *format, ...);
int		is_valid_sign(char c);
int		ft_putnbr_base(int fd, long nbr, char *base);
int		ft_putnbr_base_p(int fd, uintptr_t nbr, char *base);
int		write_c(int fd, char c);
int		write_s(int fd, char *str);
int		write_p(int fd, void *ptr);
int		write_di(int fd, int num);
int		write_u(int fd, unsigned int num);
int		write_x(int fd, unsigned int num);
int		write_cap_x(int fd, unsigned int num);

#endif
