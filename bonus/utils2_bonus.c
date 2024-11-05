/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecousill <ecousill@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/02 11:04:23 by erikcousill       #+#    #+#             */
/*   Updated: 2024/11/05 12:58:19 by ecousill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

void	setup_io(int fds[2], int i, int n, int (*pipe_fds)[2])
{
	int	j;

	if (i == 0)
		dup2(fds[0], STDIN_FILENO);
	else
		dup2(pipe_fds[i - 1][0], STDIN_FILENO);
	if (i == n - 1)
		dup2(fds[1], STDOUT_FILENO);
	else
		dup2(pipe_fds[i][1], STDOUT_FILENO);
	j = 0;
	while (j < n - 1)
	{
		close(pipe_fds[j][0]);
		close(pipe_fds[j][1]);
		j++;
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

void	create_pipes(int n, int (*pipe_fds)[2])
{
	int	i;

	i = 0;
	while (i < n - 1)
	{
		if (pipe(pipe_fds[i]) == -1)
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
