/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_command.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecousill <ecousill@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/03 02:23:33 by erikcousill       #+#    #+#             */
/*   Updated: 2024/11/04 12:29:19 by ecousill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

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
	char	*arg;
	int		in_quotes;
	int		i;
	int		start;
	int		arg_count;
	int		length;

	args = malloc((ft_strlen(command_full) + 2) * sizeof(char *));
	if (!args)
		return (NULL);
	in_quotes = 0;
	i = 0;
	start = 0;
	arg_count = 0;

	while (command_full[i] != '\0')
	{
		if ((command_full[i] == '\'' && (i == 0 || command_full[i - 1] != '\\'))
			|| (command_full[i] == '\"' && (i == 0 || command_full[i - 1] != '\\')))
			in_quotes = !in_quotes;

		if ((command_full[i] == ' ' && !in_quotes) || command_full[i + 1] == '\0')
		{
			if (command_full[i + 1] == '\0')
			{
				length = i - start + 1;
			}
			else
			{
				length = i - start;
			}
			arg = malloc(length + 1);
			if (!arg)
				return (NULL);
			ft_strlcpy(arg, &command_full[start], length + 1);
			args[arg_count++] = remove_quotes(arg);
			free(arg);

			start = i + 1;
		}
		i++;
	}
	printf("%s\n", args[0]);
	printf("%s\n", args[1]);
	printf("%s\n", args[2]);

	args[arg_count] = NULL;
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
