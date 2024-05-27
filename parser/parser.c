/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: daeha <daeha@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/25 19:39:39 by daeha             #+#    #+#             */
/*   Updated: 2024/05/27 17:46:50 by daeha            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

//list ::= pipeline 
//     | list '&&' pipeline
//     | list '||' pipeline
t_node	*list(t_token **token)
{
	t_node		*ast;
	t_node_type	type;

	ast = pipeline(token);
	while ((*token)->type == T_AND_IF || (*token)->type == T_OR_IF)
	{
		if ((*token)->type == T_AND_IF)
			type = N_AND;
		else
			type = N_OR;
		ast = new_parent_node(type, ast, NULL);
		*token = (*token)->next;
		ast->right = pipeline(token);
	}
	return (ast);
}
