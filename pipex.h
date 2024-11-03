/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erikcousillas <erikcousillas@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 13:37:43 by ecousill          #+#    #+#             */
/*   Updated: 2024/11/03 02:23:16 by erikcousill      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <fcntl.h>
# include <string.h>
# include <sys/types.h> // Para pid_t
# include <sys/wait.h> // Para wait y waitpid
# include "libft/libft.h"
# include "libft/libftprintf.h"

char	*get_path_env(char **envp);
char	**get_command_args(char *command_full);
char	*get_command_path(char *command, char **envp);

#endif
