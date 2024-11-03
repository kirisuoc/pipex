/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erikcousillas <erikcousillas@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/02 11:04:23 by erikcousill       #+#    #+#             */
/*   Updated: 2024/11/03 11:06:24 by erikcousill      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	free_args(char **args);
void	process_command(char *command_full, char **envp);

int	main(int argc, char **argv, char **envp)
{
	if (argc < 4)
	{
		ft_printf("Se esperaban los siguientes argumentos: infile \"comando1\" \"comando2\" outfile\n");
		return (1);
	}

	int	fd_infile;
	int	fd_outfile;
	int	pipe_fd[2];
	pid_t pid1;
	pid_t pid2;
	char	**args;
	char	*command_path;

	// Abre el archivo de entrada
	fd_infile = open(argv[1], O_RDONLY);
	if (fd_infile < 0)
	{
		perror("Error al abrir infile");
		exit(EXIT_FAILURE);
	}

	// Crea el pipe
	if (pipe(pipe_fd) == -1)
	{
		perror("Error al crear el pipe");
		close(fd_infile); // Se cierra el archivo si hay un error
		exit(EXIT_FAILURE);
	}


	// Abre el archivo de salida
	fd_outfile = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd_outfile < 0)
	{
		perror("Error al abrir outfile");
		close(fd_infile);
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		exit(1);
	}

	// Primer proceso
	pid1 = fork();
	if (pid1 == -1)
	{
		perror("Error al crear el fork1");
		exit(EXIT_FAILURE);
	}

	if (pid1 == 0) // Proceso hijo
	{
		close(pipe_fd[0]); // Cerrar el extremo de lectura
 		dup2(fd_infile, STDIN_FILENO); // Redirigir stdin al archivo de entrada
 		dup2(pipe_fd[1], STDOUT_FILENO); // Redirigir stdout al pipe
		close(pipe_fd[1]); // Cerrar el extremo de escritura después de redirigir
		close(fd_infile); // Cerrar el archivo de entrada

		args = get_command_args(argv[2]);

		command_path = get_command_path(args[0], envp);
		if (!command_path)
		{
			free_args(args);
			exit(EXIT_FAILURE);
		}

		execve(command_path, args, NULL); // Ejecutar el comando con sus argumentos (Ej: ls -l)
		perror("Error al ejecutar el comando1");
		free_args(args);
		free(command_path);
		exit(EXIT_FAILURE);
	}
	else // Proceso padre
	{
		close(pipe_fd[1]); // Cerrar el extremo de escritura
		waitpid(pid1, NULL, 0); // Esperar a que el primer hijo termine

		// Segundo proceso
		pid2 = fork();
		if (pid2 == -1)
		{
			perror("Error al crear el fork2");
			exit(EXIT_FAILURE);
		}
		if (pid2 == 0) // Proceso hijo para el segundo comando
		{
			dup2(pipe_fd[0], STDIN_FILENO); // Redirigir stdin al pipe
			dup2(fd_outfile, STDOUT_FILENO); // Redirigir stdout al archivo de salida
			close(pipe_fd[0]); // Cerrar el extremo de lectura
			close(fd_outfile); // Cerrar el fd del archivo de salida

			args = get_command_args(argv[3]);
			command_path = get_command_path(args[0], envp);
			if (!command_path)
			{
				free_args(args);
				exit(EXIT_FAILURE);
			}

			execve(command_path, args, NULL); // Ejecutar el comando con sus argumentos (Ej: ls -l)
			perror("Error al ejecutar el comando2");
			free_args(args);
			free(command_path);
			exit(EXIT_FAILURE);
		}
		else // Proceso padre
		{
			close(pipe_fd[0]); // Cerrar extremo de lectura del pipe
			close(fd_outfile); // Cerrar fd del archivo de salida
			waitpid(pid2, NULL, 0); // Esperar a que el segundo hijo termine
		}
	}


	close(fd_infile); // Se cierra el archivo al final
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
