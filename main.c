/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erikcousillas <erikcousillas@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/02 11:04:23 by erikcousill       #+#    #+#             */
/*   Updated: 2024/11/03 23:39:53 by erikcousill      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	execute_command(int in_fd, int out_fd, char *command, char **envp);
void	create_pipe(int pipe_fd[2]);
pid_t	safe_fork(void);
void	run_pipeline(char **argv, char **envp);

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

void	execute_command(int in_fd, int out_fd, char *command, char **envp)
{
	char	**args;
	char	*command_path;

	args = get_command_args(command);
	dup2(in_fd, STDIN_FILENO);
	dup2(out_fd, STDOUT_FILENO);
	close(in_fd);
	close(out_fd);
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

void	create_pipe(int pipe_fd[2])
{
	if (pipe(pipe_fd) == -1)
	{
		perror("Error al crear el pipe");
		exit(EXIT_FAILURE);
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

void	run_pipeline(char **argv, char **envp)
{
	int		fd_infile;
	int		fd_outfile;
	int		pipe_fd[2];
	pid_t	pid1;
	pid_t	pid2;

	initialize_files(&fd_infile, &fd_outfile, argv);
	create_pipe(pipe_fd);
	pid1 = safe_fork();
	if (pid1 == 0)
	{
		close(pipe_fd[0]);
		execute_command(fd_infile, pipe_fd[1], argv[2], envp);
		exit(EXIT_SUCCESS);
	}
	close(pipe_fd[1]);
	waitpid(pid1, NULL, 0);
	pid2 = safe_fork();
	if (pid2 == 0)
	{
		execute_command(pipe_fd[0], fd_outfile, argv[3], envp);
		exit(EXIT_SUCCESS);
	}
	cleanup(fd_infile, fd_outfile, pipe_fd);
	waitpid(pid2, NULL, 0);
}
