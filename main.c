/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erikcousillas <erikcousillas@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/02 11:04:23 by erikcousill       #+#    #+#             */
/*   Updated: 2024/11/02 14:35:38 by erikcousill      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	*get_path_env(char **envp);
char	**get_command_args(char *command_full);
char	*get_command_path(char *command, char **envp);
void	free_args(char **args);
void	process_command(char *command_full, char **envp);


/*int	main(int argc, char **argv, char **envp)
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
	char	**args1;
	char	**args2;
	char	*command_path1;
	char	*command_path2;

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
		exit(EXIT_FAILURE);
	}

	// Primer proceso
	pid1 = fork();
	if (pid1 == -1)
	{
		perror("Error al crear el fork1");
		exit(EXIT_FAILURE);
	}
											ft_printf("Fork creado\n");


	if (pid1 == 0) // Proceso hijo
	{
		close(pipe_fd[0]); // Cerrar el extremo de lectura
 		dup2(pipe_fd[1], STDOUT_FILENO); // Redirigir stdout al pipe
		close(pipe_fd[1]);

		args1 = get_command_args(argv[2]);

		command_path1 = get_command_path(args1[0], envp);
		if (!command_path1)
		{
			free_args(args1);
			exit(EXIT_FAILURE);
		}

		execve(command_path1, args1, NULL); // Ejecutar el comando con sus argumentos (Ej: ls -l)
		perror("Error al ejecutar el comando1");
		free_args(args1);
		free(command_path1);
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
			close(pipe_fd[0]); // Cerrar el extremo de lectura
			dup2(fd_outfile, STDOUT_FILENO); // Redirigir stdout al archivo de salida
			close(fd_outfile); // Cerrar el fd del archivo de salida

			args2 = get_command_args(argv[3]);
			command_path2 = get_command_path(args2[0], envp);
			if (!command_path2)
			{
				free_args(args2);
				exit(EXIT_FAILURE);
			}

			execve(command_path2, args2, NULL); // Ejecutar el comando con sus argumentos (Ej: ls -l)
			perror("Error al ejecutar el comando2");
			free_args(args2);
			free(command_path2);
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
*/

int main(int argc, char **argv, char **envp)
{
    if (argc < 4)
    {
        ft_printf("Se esperaban los siguientes argumentos: infile \"comando1\" \"comando2\" outfile\n");
        return (1);
    }

    int fd_infile;
    int fd_outfile;
    int pipe_fd[2];
    pid_t pid1;
    pid_t pid2;
    char **args1;
    char **args2;
    char *command_path1;
    char *command_path2;

    // Abre el archivo de entrada
    fd_infile = open(argv[1], O_RDONLY);
    ft_printf("Infile abierto\n");
    if (fd_infile < 0)
    {
        perror("Error al abrir infile");
        return (1);
    }

    // Crea el pipe
    if (pipe(pipe_fd) == -1)
    {
        perror("Error al crear el pipe");
        close(fd_infile);
        return (1);
    }
    ft_printf("Pipe creado\n");

    // Abre el archivo de salida
    fd_outfile = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd_outfile < 0)
    {
        perror("Error al abrir outfile");
        close(fd_infile);
        close(pipe_fd[0]);
        close(pipe_fd[1]);
        return (1);
    }
    ft_printf("Outfile creado/abierto sin problema\n");

    // Primer proceso
    pid1 = fork();
    if (pid1 == -1)
    {
        perror("Error al crear el fork1");
        return (1);
    }
    ft_printf("Fork creado\n");

    if (pid1 == 0) // Proceso hijo
    {
        close(pipe_fd[0]); // Cerrar el extremo de lectura
        dup2(pipe_fd[1], STDOUT_FILENO); // Redirigir stdout al pipe
        close(pipe_fd[1]); // Cerrar el extremo de escritura

        args1 = get_command_args(argv[2]);
        ft_printf("Ejecutando comando: %s\n", args1[0]);

        command_path1 = get_command_path(args1[0], envp);
        if (!command_path1)
        {
            free_args(args1);
            exit(EXIT_FAILURE);
        }

        execve(command_path1, args1, envp); // Pasar envp a execve
        perror("Error al ejecutar el comando1");
        free_args(args1);
        free(command_path1);
        exit(EXIT_FAILURE);
    }

    // Proceso padre
    close(pipe_fd[1]); // Cerrar el extremo de escritura

    // Esperar a que el primer hijo termine
    waitpid(pid1, NULL, 0);

    // Segundo proceso
    pid2 = fork();
    if (pid2 == -1)
    {
        perror("Error al crear el fork2");
        return (1);
    }

    if (pid2 == 0) // Proceso hijo para el segundo comando
    {
        dup2(pipe_fd[0], STDIN_FILENO); // Redirigir stdin al pipe
        close(pipe_fd[0]); // Cerrar el extremo de lectura
        dup2(fd_outfile, STDOUT_FILENO); // Redirigir stdout al archivo de salida
        close(fd_outfile); // Cerrar el fd del archivo de salida

        args2 = get_command_args(argv[3]);
        command_path2 = get_command_path(args2[0], envp);
        if (!command_path2)
        {
            free_args(args2);
            exit(EXIT_FAILURE);
        }

        execve(command_path2, args2, envp); // Pasar envp a execve
        perror("Error al ejecutar el comando2");
        free_args(args2);
        free(command_path2);
        exit(EXIT_FAILURE);
    }
    else // Proceso padre
    {
        close(pipe_fd[0]); // Cerrar extremo de lectura del pipe
        close(fd_outfile); // Cerrar fd del archivo de salida
        waitpid(pid2, NULL, 0); // Esperar a que el segundo hijo termine
    }

    close(fd_infile); // Cerrar infile al final
    return (0);
}



void	process_command(char *command_full, char **envp)
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

char	*get_path_env(char **envp)
{
	char	*path;
	int		i;

	i = 0;
	path = NULL;
	// Buscar PATH en envp
	while (envp[i])
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
		{
			path = envp[i] + 5; // Salta "PATH="
			break ;
		}
		i++;
	}
	return (path);
}

char	**get_command_args(char *command_full)
{
	char	**args;

	args = ft_split(command_full, ' ');
	return (args);
}

char	*get_command_path(char *command, char **envp)
{
	char	*path;
	char	**paths;
	char	*full_path;
	int		i;

	path = get_path_env(envp);
	if (!path)
		return (NULL);

	// Dividir PATH en una lista de rutas
	paths = ft_split(path, ':');
	if (!paths)
		return (NULL);

	i = 0;
	while (paths[i])
	{
		full_path = ft_strjoin(paths[i], "/"); // Añadir '/' al final del directorio
		full_path = ft_strjoin(full_path, command); // Concatenar con el nombre del comando

		// Verificar si es un ejecutable
		if (access(full_path, X_OK) == 0)
		{
			free(paths);
			return (full_path);
		}
		free(full_path);
		i++;
	}
	free(paths);
	return (NULL); // Retorna NULL si no encuentra el comando
}

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
