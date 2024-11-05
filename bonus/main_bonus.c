/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecousill <ecousill@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 12:50:18 by ecousill          #+#    #+#             */
/*   Updated: 2024/11/05 12:58:12 by ecousill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

static int	(*allocate_pipe_fds(int n)) [2]
{
	int	(*pipe_fds)[2];

	pipe_fds = malloc((n - 1) * sizeof(int [2]));
	if (!pipe_fds)
	{
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	return (pipe_fds);
}

static void	run_pipeline(int argc, char **argv, char **envp)
{
	int		fds[2];
	int		n;
	int		(*pipe_fds)[2];
	pid_t	pid;
	int		i;

	n = argc - 3;
	pipe_fds = allocate_pipe_fds(n);
	initialize_files(&fds[0], &fds[1], argv, argc);
	create_pipes(n, pipe_fds);
	i = 0;
	while (i < n)
	{
		pid = safe_fork();
		if (pid == 0)
		{
			setup_io(fds, i, n, pipe_fds);
			execute_command(argv[i + 2], envp);
			exit(EXIT_SUCCESS);
		}
		i++;
	}
	cleanup(fds[0], fds[1], pipe_fds, n);
	while (n--)
		wait(NULL);
}

int	main(int argc, char **argv, char **envp)
{
	if (argc < 4)
	{
		ft_printf("Se esperaban al menos 3 argumentos\n");
		return (1);
	}
	run_pipeline(argc, argv, envp);
	return (0);
}
