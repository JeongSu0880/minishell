/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jungslee <jungslee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/25 18:27:00 by daeha             #+#    #+#             */
/*   Updated: 2024/05/28 21:09:44 by jungslee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	g_status;

int	main(int argc, char **argv, char **envp)
{
	t_token	*token;
	t_node	*ast;
	char	*line;

	while (1)
	{
		line = readline("minishell> ");
		add_history(line);
		token = tokenizer(line);
		// ast = parser(token);
		//execution part
		print_all_value(token);
		ms_free_all_token(&token);
		//free_ast();
		free(line);
		line = NULL;
		token = NULL;
	}
	return (0);
}
