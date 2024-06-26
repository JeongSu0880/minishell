/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   e_utils.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: daeha <daeha@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/06 16:04:44 by daeha             #+#    #+#             */
/*   Updated: 2024/06/24 23:57:27 by jungslee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"
#include "expansion.h"
#include "minishell.h"

void	push_pid_list(pid_t pid, t_stat *stat)
{
	if (stat->n_pid == MAX_PID)
	{	
		ft_putendl_fd("minishell : maximum number of process exceeded", \
					STDERR_FILENO);
		wait_pid_list(stat);
		ft_putendl_fd("exit...", STDERR_FILENO);
		exit(EXIT_FAILURE);
	}
	stat->pid[stat->n_pid++] = pid;
}

void	wait_pid_list(t_stat *stat)
{	
	int	pid;
	int	*status;

	pid = 0;
	status = get_status();
	if (stat->n_pid == 0)
		return ;
	while (pid < stat->n_pid)
	{
		waitpid(stat->pid[pid], status, 0);
		pid++;
	}
	stat->n_pid = 0;
	if (WIFEXITED(*status))
		set_status(WEXITSTATUS(*status));
	else if (WIFSIGNALED(*status))
	{
		set_status(WTERMSIG(*status) + 128);
		if (*status == 131)
			write(1, "Quit: 3", 7);
		write(1, "\n", 1);
	}
}
