/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecousill <ecousill@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 13:37:23 by ecousill          #+#    #+#             */
/*   Updated: 2024/10/31 14:54:57 by ecousill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	main(void)
{
	int	pipefd[2];
	pid_t pid1, pid2;

	// Crear el pipe
	if(pipe(pipefd) == -1)
	{
		perror("pipe");
		exit(1);
	}

	// Primer proceso
	pid1 = fork();
	if (pid1 == -1)
	{
		perror("fork");
		exit(1);
	}

	if (pid1 == 0) // Proceso hijo
	{
		close(pipefd[0]); // Cerrar el extremo de lectura
		dup2(pipefd[1], STDOUT_FILENO); // Redirigir stdout al pipe
		close(pipefd[1]); // Cerrar el extremo de escritura

		// Preparar argumentos para execve
		char	*argv1[] = { "ls", "-l", NULL }; // Argumentos para el comando ls
		char	*envp[] = { NULL }; // Variables de entorno (opcional)

		execve("/bin/ls", argv1, envp); // Ejecutar el comando ls
		perror("execve"); // Manejo de error si execve falla
		exit(1);
	}

	//Segundo proceso
	pid2 = fork();
	if (pid2 == -1)
	{
		perror("fork");
		exit(1);
	}

	if (pid2 == 0) // Proceso hijo
	{
		close(pipefd[1]); // Cerrar el extremo de lectura
		dup2(pipefd[0], STDIN_FILENO); // Redirigir stdout al pipe
		close(pipefd[0]); // Cerrar el extremo de escritura

		// Preparar argumentos para execve
		char	*argv2[] = { "wc", "-l", NULL }; // Argumentos para el comando ls
		char	*envp[] = { NULL }; // Variables de entorno (opcional)

		execve("/usr/bin/wc", argv2, envp); // Ejecutar el comando ls
		perror("execve"); // Manejo de error si execve falla
		exit(1);
	}

	// Cerrar los extremos del pipe en el proceso padre
	close(pipefd[0]);
	close(pipefd[1]);

	// Esperar a que terminen los hijos
	wait(NULL);
	wait(NULL);

	return (0);
}
