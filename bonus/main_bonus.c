/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erikcousillas <erikcousillas@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/02 11:04:23 by erikcousill       #+#    #+#             */
/*   Updated: 2024/11/03 23:18:24 by erikcousill      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

void	execute_command(char *command, char **envp);
void	create_pipes(int n, int pipe_fd[][2]);
pid_t	safe_fork(void);
void	run_pipeline(int argc, char **argv, char **envp);

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

// Si tenemos 2 comandos, necesitamos 2 procesos y 1 pipe
// Si tenemos 4 comandos, necesitamos 4 procesos y 3 pipes
// Si tenemos n comandos, necesitamos n procesos y n-1 pipes
//			--> fd_infile + fd_outfile + (n-1) pipe_fd[2]
//											pipe_fds[2 x (n-1)]

void	run_pipeline(int argc, char **argv, char **envp)
{
	int		fd_infile;
	int		fd_outfile;
	int		n;
	int		pipe_fds[(argc - 3) - 1][2];
	pid_t	pid;
	int		i;
	int		j;

	n = argc - 3; // Los argumentos son infile, n comandos, outfile

	initialize_files(&fd_infile, &fd_outfile, argv, argc);
	create_pipes(n, pipe_fds); // Creamos n-1 pipes

	i = 0;
	while (i < n)
	{
		pid = safe_fork();


		if (pid == 0)
		{
			// Proceso hijo
			if (i == 0) // Primer comando
			{
				// ????????? close(pipe_fds[i][0]);
				dup2(fd_infile, STDIN_FILENO);
			}
			else // Comandos intermedios o último comando
			{
				dup2(pipe_fds[i - 1][0], STDIN_FILENO);
			}

			if (i == n - 1) // Último comando
			{
				dup2(fd_outfile, STDOUT_FILENO);
			}
			else // Comandos intermedios o primer comando
			{
				dup2(pipe_fds[i][1], STDOUT_FILENO);
			}
			// Cerrar todos los pipes
			j = 0;
			while (j < n - 1)
			{
				close(pipe_fds[j][0]);
				close(pipe_fds[j][1]);
				j++;
			}
			execute_command(argv[i + 2], envp);
			exit(EXIT_SUCCESS);
		}
		i++;

	}
	// Proceso padre
	cleanup(fd_infile, fd_outfile, pipe_fds, n);
	i = 0;
	while (i < n)
	{
		wait(NULL);
		i++;
	}
}

void	execute_command(char *command, char **envp)
{
	char	**args;
	char	*command_path;

	args = get_command_args(command);
	command_path = get_command_path(args[0], envp);
	if (!command_path)
	{
		free_args(args);
		exit(EXIT_FAILURE);
	}
	execve(command_path, args, NULL);
	perror("Error al ejecutar el comando");
	free_args(args);
	free(command_path);
	exit(EXIT_FAILURE);
}

void	create_pipes(int n, int pipe_fds[][2])
{
	int	i;

	i = 0;
	while (i < n - 1)
	{
		if(pipe(pipe_fds[i]))
		{
			perror("Error al crear el pipe");
			exit(EXIT_FAILURE);
		}
		i++;
	}
}

pid_t	safe_fork(void)
{
	int	pid;

	pid = fork();
	if (pid == -1)
	{
		perror("Error al crear el fork");
		exit(EXIT_FAILURE);
	}
	return (pid);
}

