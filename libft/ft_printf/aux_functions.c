/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aux_functions.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecousill <ecousill@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 13:29:45 by erikcousill       #+#    #+#             */
/*   Updated: 2024/10/09 13:03:55 by ecousill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"
#include "../libftprintf.h"

int	manage_flags_d(t_format *info, int number)
{
	int		printed_chars;
	char	flag_to_write;

	printed_chars = 0;
	flag_to_write = '\0';
	if (info->flag_plus && number > 0)
		flag_to_write = '+';
	else if (info->flag_space && number > 0)
		flag_to_write = ' ';
	else if (number < 0)
		flag_to_write = '-';
	if (flag_to_write)
		printed_chars += ft_putchar(flag_to_write) + ft_putnbr(number);
	else
		printed_chars += ft_putnbr(number);
	return (printed_chars);
}
