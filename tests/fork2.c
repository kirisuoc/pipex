/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erikcousillas <erikcousillas@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 19:35:47 by erikcousill       #+#    #+#             */
/*   Updated: 2024/11/01 18:37:59 by erikcousill      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	main(int argc, char **argv)
{
	int	fd[2];
	// fd[0] - read
	// fd[1] - write
	if (pipe(fd) == -1)
	{
		printf("An error occured while trying opening the pipe.\n");
		return (1);
	}
	int	id = fork();
	if (id == -1)
	{
		printf("An error occured while fork.\n");
		return (4);
	}
	if (id == 0)
	{
		close(fd[0]);
		int x;
		printf("Enter a number: ");
		scanf("%d", &x);
		if (write(fd[1], &x, sizeof(int)) == -1)
		{
			printf("An error occured while trying to write to the pipe.\n");
			return (2);
		}
		close(fd[1]);
	}
	else
	{
		close(fd[1]);
		int y;
		if (read(fd[0], &y, sizeof(int)) == -1)
		{
			printf("An error occured while trying to read from the pipe.\n");
			return (3);
		}
		y = y * 3;
		close(fd[0]);
		printf("Got from child process: That number x 3 = %d\n", y);
	}
	return (0);
}
