/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_format.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecousill <ecousill@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 08:34:17 by erikcousill       #+#    #+#             */
/*   Updated: 2024/10/09 13:04:02 by ecousill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"
#include "../libftprintf.h"

static void	parse_format_flags_width(char **format, t_format *info)
{
	if (**format == '+')
	{
		info->flag_plus = 1;
		(*format)++;
		if (**format == ' ')
			(*format)++;
	}
	else if (**format == ' ')
	{
		info->flag_space = 1;
		(*format)++;
	}
	else if (**format == '#')
	{
		info->flag_hash = 1;
		(*format)++;
	}
}

static void	parse_format_identif(char **format, t_format *info)
{
	if (**format == 'd' || **format == 'i' || **format == 'c'
		|| **format == 's' || **format == 'p' || **format == 'u'
		|| **format == 'x' || **format == 'X' || **format == '%')
		info->specifier = **format;
}

void	parse_format(char **format, t_format *info)
{
	parse_format_flags_width(format, info);
	parse_format_identif(format, info);
}
