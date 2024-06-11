/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   e_cmd_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: daeha <daeha@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 17:10:55 by daeha             #+#    #+#             */
/*   Updated: 2024/06/11 21:21:30 by daeha            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

void	exit_cmd_error(char *cmd, int error_type)
{
	ft_putstr_fd("bash: ", STDERR_FILENO);
	ft_putstr_fd(cmd, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	if (error_type != ENOCMD)
		ft_putendl_fd(strerror(error_type), STDERR_FILENO);
	else
		ft_putendl_fd("command not found", STDERR_FILENO);
	if (error_type == ENOENT || error_type == ENOCMD)
		exit(127);
	else if (error_type == EACCES || error_type == EISDIR)
		exit(126);
}

void redirect_to_cmd(t_stat *stat)
{
	if (stat->fd[INPUT] == -1 || stat->fd[OUTPUT] == -1)
	{
		if (stat->fd[INPUT] != -1 && stat->fd[INPUT] != STDIN_FILENO)
			close(stat->fd[INPUT]);
		if (stat->fd[OUTPUT] != -1 && stat->fd[INPUT] != STDOUT_FILENO)
			close(stat->fd[OUTPUT]);
		exit(EXIT_FAILURE);
	}
	if (stat->fd[OUTPUT] != STDOUT_FILENO)
	{
		dup2(stat->fd[OUTPUT], STDOUT_FILENO);
		close(stat->fd[OUTPUT]);
	}
	if (stat->fd[INPUT] != STDIN_FILENO)
	{
		dup2(stat->fd[INPUT], STDIN_FILENO);
		close(stat->fd[INPUT]);
	}
}

void	close_pipe_fds(t_stat *stat)
{
	int	i;

	i = 0;
	while (i < stat->n_pipe)
	{
		if (stat->pipe[i] != stat->fd[INPUT] \
			&& stat->pipe[i] != stat->fd[OUTPUT])
			close(stat->pipe[i]);
		i++;
	}
}

void	if_not_executable_then_exit(char *cmd)
{
	struct stat	buf;
	
	stat(cmd, &buf);
	if (access(cmd, F_OK))
		exit_cmd_error(cmd, ENOENT);
	else if (access(cmd, X_OK))
		exit_cmd_error(cmd, EACCES);
	else if ((buf.st_mode & S_IFMT) == S_IFDIR)
		exit_cmd_error(cmd, EISDIR);
}

char	*change_as_absolute_path(char *cmd)
{
	size_t		str_len;
	size_t		file_len;
	char		*file;
	char		*path;
	char		*ret;

	str_len = ft_strlen(cmd) - 1;
	file_len = 0;
	while (cmd[str_len - file_len] != '/')
		file_len++;
	path = (char *)malloc(sizeof(char) * (str_len - file_len + 1));
	file = (char *)malloc(sizeof(char) * (file_len + 2));
	ft_strlcpy(path, cmd, str_len - file_len + 1);
	ft_strlcpy(file, cmd + str_len - file_len, file_len + 2);
	if (chdir(path) == -1)
		exit_cmd_error(cmd, ENOENT);
	free(path);
	path = getcwd(NULL, 0);
	ret = ft_strjoin(path, file);
	free(file);
	return (ret);
}