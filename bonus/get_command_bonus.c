/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_command_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erikcousillas <erikcousillas@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/03 02:23:33 by erikcousill       #+#    #+#             */
/*   Updated: 2024/11/03 23:17:22 by erikcousill      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

char	*get_path_env(char **envp)
{
	char	*path;
	int		i;

	i = 0;
	path = NULL;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
		{
			path = envp[i] + 5;
			break ;
		}
		i++;
	}
	return (path);
}

char	*remove_quotes(char *str)
{
	size_t	len;
	char	*new_str;

	len = ft_strlen(str);
	if (str[0] == '\'' && str[len - 1] == '\'')
	{
		new_str = malloc(len - 1);
		if (!new_str)
			return (NULL);
		ft_strlcpy(new_str, str + 1, len - 1);
		return (new_str);
	}
	return (ft_strdup(str));
}

char	**get_command_args(char *command_full)
{
	char	**args;
	int		i;
	char	*new_arg;

	args = ft_split(command_full, ' ');

	i = 0;
	while (args[i] != NULL)
	{
		new_arg = remove_quotes(args[i]);
		free(args[i]);
		args[i] = new_arg;
		i++;
	}
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
	paths = ft_split(path, ':');
	if (!paths)
		return (NULL);
	i = 0;
	while (paths[i])
	{
		full_path = ft_strjoin(ft_strjoin(paths[i], "/"), command);
		if (access(full_path, X_OK) == 0)
		{
			free(paths);
			return (full_path);
		}
		free(full_path);
		i++;
	}
	free(paths);
	return (NULL);
}
