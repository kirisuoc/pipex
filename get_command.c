/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_command.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecousill <ecousill@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/03 02:23:33 by erikcousill       #+#    #+#             */
/*   Updated: 2024/11/04 14:58:44 by ecousill         ###   ########.fr       */
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

int	extract_argument(char *command_full, int *start, char **arg)
{
	int	in_qt;
	int	i;
	int	length;

	i = *start;
	in_qt = 0;
	while (command_full[i] != '\0')
	{
		if ((command_full[i] == '\'' || command_full[i] == '\"')
			&& (i == 0 || command_full[i - 1] != '\\'))
			in_qt = !in_qt;
		if ((command_full[i] == ' ' && !in_qt) || command_full[i + 1] == '\0')
		{
			if (command_full[i + 1] == '\0')
				length = i - *start + 1;
			else
				length = i - *start;
			*arg = malloc(length + 1);
			ft_strlcpy(*arg, &command_full[*start], length + 1);
			*start = i + 1;
			return (1);
		}
		i++;
	}
	return (0);
}

char	**get_command_args(char *command_full)
{
	char	**args;
	char	*arg;
	int		start;
	int		arg_count;

	args = malloc((ft_strlen(command_full) + 2) * sizeof(char *));
	if (!args)
		return (NULL);
	start = 0;
	arg_count = 0;
	while (1)
	{
		if (extract_argument(command_full, &start, &arg) == 0)
			break ;
		args[arg_count++] = remove_quotes(arg);
		free(arg);
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
