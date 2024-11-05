/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecousill <ecousill@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 13:37:43 by ecousill          #+#    #+#             */
/*   Updated: 2024/11/05 12:59:33 by ecousill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_BONUS_H
# define PIPEX_BONUS_H

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <fcntl.h>
# include <string.h>
# include <sys/types.h> // Para pid_t
# include <sys/wait.h> // Para wait y waitpid
# include "../libft/libft.h"
# include "../libft/libftprintf.h"

char	*get_path_env(char **envp);
char	**get_command_args(char *command_full);
char	*get_command_path(char *command, char **envp);

void	free_args(char **args);
void	cleanup(int fd_infile, int fd_outfile, int pipe_fd[][2], int n);
int		open_file(char *filename, int flags, mode_t mode, char *error_message);
void	initialize_files(int *fd_infile, int *fd_outfile,
			char **argv, int argc);

void	setup_io(int fds[2], int i, int n, int (*pipe_fds)[2]);
void	execute_command(char *command, char **envp);
void	create_pipes(int n, int (*pipe_fds)[2]);
pid_t	safe_fork(void);

#endif
