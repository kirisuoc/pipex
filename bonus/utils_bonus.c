/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erikcousillas <erikcousillas@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/03 12:45:45 by erikcousill       #+#    #+#             */
/*   Updated: 2024/11/03 18:25:55 by erikcousill      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

void	free_args(char **args)
{
	int	i;

	i = 0;
	if (args)
	{
		while (args[i] != NULL)
		{
			free(args[i]);
			i++;
		}
		free(args);
	}
}

void	cleanup(int fd_infile, int fd_outfile, int pipe_fds[][2], int n)
{
	int	i;

	if (fd_infile != 1)
		close(fd_infile);
	if (fd_outfile != 1)
		close(fd_outfile);
	i = 0;
	while (i < n - 1)
	{
		close(pipe_fds[i][0]);
		close(pipe_fds[i][1]);
		i++;
	}
}

int	open_file(char *filename, int flags, mode_t mode, char *error_message)
{
	int	fd;

	fd = open(filename, flags, mode);
	if (fd < 0)
	{
		perror(error_message);
		exit(EXIT_FAILURE);
	}
	return (fd);
}

void	initialize_files(int *fd_infile, int *fd_outfile, char **argv, int argc)
{
	*fd_infile = open_file(argv[1], O_RDONLY, 0, "Error al abrir infile");
	*fd_outfile = open_file(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC, 0644,
			"Error al abrir outfile");
}