/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   p_syntax_error.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: daeha <daeha@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/29 22:03:33 by daeha             #+#    #+#             */
/*   Updated: 2024/05/30 16:56:24 by daeha            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

extern int g_status;

static void	syntax_error_type(t_token *token);

void	*syntax_error_test(t_token *token, t_node **node)
{	
	if (g_status == 0)
	{
		if (token->type == T_EOF)
			ft_putendl_fd("bash: syntax error: unexpected end of file", STDERR_FILENO);
		else
		{
			ft_putstr_fd("bash: syntax error near unexpected token `", 2);
			syntax_error_type(token);
			ft_putendl_fd("'", STDERR_FILENO);
		}
		g_status = 258;
	}
	free_tree(node);
	return (NULL);
}

static void	syntax_error_type(t_token *token)
{
	if(token->type == T_AND_IF)
		ft_putstr_fd("&&", STDERR_FILENO);
	else if (token->type == T_OR_IF)
		ft_putstr_fd("||", STDERR_FILENO);
	else if (token->type == T_GREAT)
		ft_putstr_fd(">", STDERR_FILENO);
	else if (token->type == T_DGREAT)
		ft_putstr_fd(">>", STDERR_FILENO);
	else if (token->type == T_DLESS)
		ft_putstr_fd("<<", STDERR_FILENO);
	else if (token->type == T_LESS)
		ft_putstr_fd("<", STDERR_FILENO);
	else if (token->type == T_PIPE)
		ft_putstr_fd("|", STDERR_FILENO);
	else if (token->type == T_RPAREN)
		ft_putstr_fd(")", STDERR_FILENO);
	else if (token->type == T_LPAREN)
		ft_putstr_fd("()", STDERR_FILENO);
}
