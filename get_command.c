/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_command.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erikcousillas <erikcousillas@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/03 02:23:33 by erikcousill       #+#    #+#             */
/*   Updated: 2024/11/03 02:23:58 by erikcousill      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

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

