/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   e_pipe.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: daeha <daeha@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 21:03:01 by daeha             #+#    #+#             */
/*   Updated: 2024/06/12 16:42:17 by daeha            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

static void	push_pipe_list(int fd[2], t_stat *stat);

void	exec_pipe(t_node *node, t_stat *stat)
{
	int	fd[2];
	int	temp[2];

	temp[INPUT] = stat->fd[INPUT];
	temp[OUTPUT] = stat->fd[OUTPUT];
	pipe(fd);
	stat->fd[OUTPUT] = fd[OUTPUT];
	push_pipe_list(fd, stat);
	execution(node->left, stat);
	stat->n_pipe--;
	stat->fd[OUTPUT] = temp[OUTPUT];
	stat->fd[INPUT] = fd[INPUT];
	execution(node->right, stat);
	stat->n_pipe--;
}

static void	push_pipe_list(int fd[2], t_stat *stat)
{
	if (stat->n_pipe == MAX_PIPE - 1)
	{
		ft_putendl_fd("minishell : maximum number of process exceeded", \
					STDERR_FILENO);
		wait_pid_list(stat);
		ft_putendl_fd("exit...", STDERR_FILENO);
	}
	stat->pipe[stat->n_pipe++] = fd[OUTPUT];
	stat->pipe[stat->n_pipe++] = fd[INPUT];
}
