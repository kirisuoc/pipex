/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erikcousillas <erikcousillas@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/02 11:04:23 by erikcousill       #+#    #+#             */
/*   Updated: 2024/11/03 12:34:16 by erikcousill      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	free_args(char **args);
void	process_command(char *command_full, char **envp);

void	execute_command(int in_fd, int out_fd, char *command, char	**envp)
{
		char	**args;
		char	*command_path;

	 	dup2(in_fd, STDIN_FILENO); // Redirigir stdin al archivo de entrada
 		dup2(out_fd, STDOUT_FILENO); // Redirigir stdout al pipe
		close(in_fd); // Cerrar el extremo de escritura después de redirigir
		close(out_fd); // Cerrar el archivo de entrada

		args = get_command_args(command);
		command_path = get_command_path(args[0], envp);
		if (!command_path)
		{
			free_args(args);
			exit(EXIT_FAILURE);
		}

		execve(command_path, args, NULL); // Ejecutar el comando con sus argumentos (Ej: ls -l)
		perror("Error al ejecutar el comando");
		free_args(args);
		free(command_path);
		exit(EXIT_FAILURE);
}

int		open_file(char *filename, int flags, mode_t mode, char *error_message)
{
	int		fd;

	fd = open(filename, flags, mode);
	if (fd < 0)
	{
		perror(error_message);
		exit(EXIT_FAILURE);
	}
	return (fd);
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

void	initialize_files(int *fd_infile, int *fd_outfile, char **argv)
{
	*fd_infile = open_file(argv[1], O_RDONLY, 0, "Error al abrir infile");
	*fd_outfile = open_file(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0644, "Error al abrir outfile");

}

void	create_pipe(int pipe_fd[2])
{
	if (pipe(pipe_fd) == -1)
	{
		perror("Error al crear el pipe");
		exit(EXIT_FAILURE);
	}
}

pid_t		safe_fork(void)
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

void	run_pipeline(char **argv, char **envp)
{
	int	fd_infile;
	int	fd_outfile;
	int	pipe_fd[2];
	pid_t pid1;
	pid_t pid2;

	initialize_files(&fd_infile, &fd_outfile, argv);
	create_pipe(pipe_fd);
	pid1 = safe_fork();
	if (pid1 == 0) // Proceso hijo para el primer comando
	{
		close(pipe_fd[0]); // Cerrar el extremo de lectura
		execute_command(fd_infile, pipe_fd[1], argv[2], envp);
		exit(EXIT_SUCCESS); // Forzar la terminación del proceso hijo
	}
	close(pipe_fd[1]); // Cerrar el extremo de escritura
	waitpid(pid1, NULL, 0); // Esperar a que el primer hijo termine
	pid2 = safe_fork();
	if (pid2 == 0) // Proceso hijo para el segundo comando
	{
		execute_command(pipe_fd[0], fd_outfile, argv[3], envp);
		exit(EXIT_SUCCESS); // Forzar la terminación del proceso hijo
	}
	cleanup(fd_infile, fd_outfile, pipe_fd);
	waitpid(pid2, NULL, 0); // Esperar a que el segundo hijo termine
}

int	main(int argc, char **argv, char **envp)
{
	if (argc < 5)
	{
		ft_printf("Se esperaban los siguientes argumentos: "
		"infile \"comando1\" \"comando2\" outfile\n");
		return (1);
	}
	run_pipeline(argv, envp);
	return (0);
}

/*void	process_command(char *command_full, char **envp)
{
	char	**args;
	char	*command_path;

	args = get_command_args(command_full);
	if (!args)
	{
		perror("Error al obtener argumentos del comando");
		return ;
	}
	command_path = get_command_path(args[0], envp);
	if (command_path)
	{
		ft_printf("Ruta del comando '%s': %s\n", command_full, command_path);
		free(command_path);
	}
	else
	{
		perror("Error: comando no encontrado");
	}
	free_args(args);
}
*/

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
