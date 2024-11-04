/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erikcousillas <erikcousillas@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/03 12:45:45 by erikcousill       #+#    #+#             */
/*   Updated: 2024/11/03 12:51:06 by erikcousill      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

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

void	cleanup(int fd_infile, int fd_outfile, int pipe_fd[2])
{
	if (fd_infile != 1)
		close(fd_infile);
	if (fd_outfile != 1)
		close(fd_outfile);
	close(pipe_fd[0]);
	close(pipe_fd[1]);
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

void	initialize_files(int *fd_infile, int *fd_outfile, char **argv)
{
	*fd_infile = open_file(argv[1], O_RDONLY, 0, "Error al abrir infile");
	*fd_outfile = open_file(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0644,
			"Error al abrir outfile");
}
