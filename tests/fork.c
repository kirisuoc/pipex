/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erikcousillas <erikcousillas@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 19:35:47 by erikcousill       #+#    #+#             */
/*   Updated: 2024/11/01 00:54:59 by erikcousill      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	main(int argc, char **argv)
{
	int id = fork();
	int	n;
	int	i;

	if (id == 0)
		n = 1;
	else
		n = 6;

	if (id == 0)
	{
		sleep(3);
	}
	for (i = n; i < n + 5; i++)
	{
		printf("%d ", i);
	}
	if (id != 0)
		printf("\n");


	wait(NULL);
	return (0);
}
