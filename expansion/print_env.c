/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_env.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jungslee <jungslee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/02 10:07:27 by jungslee          #+#    #+#             */
/*   Updated: 2024/06/06 23:04:19 by jungslee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expansion.h"

char	*replace_str(char *str, int start, int *end, char *content)
{
	char	*ret;
	char	*tmp1;
	char	*tmp2;
	int		end_tmp;

	tmp1 = env_strcpy(0, start - 1, str);
	// printf("^^^^^^^^^");
	tmp2 = ft_strjoin(tmp1, content);
	if (tmp2 == NULL)
		handle_error("exit : malloc error1", 1, 0);
	free(tmp1);
	end_tmp = ft_strlen(tmp2);
	// printf("end + 1 :::::::: %d\n", *end + 1);
	// printf("ft_strlen(str) - 1 :::::::: %d\n", ft_strlen(str) - 1);
	tmp1 = env_strcpy(*end + 1, ft_strlen(str) - 1, str);


	ret = ft_strjoin(tmp2, tmp1);
	if (ret == NULL)
		handle_error("exit : malloc error2", 1, 0);
	free(tmp1);
	free(tmp2);
	*end = end_tmp;
	return (ret);
}

char	*is_valid_env(char *ori, int *idx, t_env *env)
{
	int		start;
	int		end;
	char	*ret;

	if (ori[*idx + 1] == '\"' || \
				ori[*idx + 1] == '\0' || ori[*idx + 1] == '$')
		return (0);
	start = (*idx)++;
	ret = NULL;
	while (ft_isalnum(*(ori + *idx)) || ori[*idx] == '_')
		(*idx)++;
	end = *idx - 1;
	while (env != NULL)
	{
		if (env_strncmp(ori + start + 1, env->key, end - start) == 0)
		{
			ret = replace_str(ori, start, &end, env->value);
			break ;
		}
		env = env->next;
	}
	if (ret == NULL)
		ret = replace_str(ori, start, &end, "");
	*idx = end - 1;
	return (ret);
}

// void	is_var(t_node *node, t_env *env)
// {
// 	int		i;
// 	int		j;
// 	char	*content;
// 	int		single_quote;

// 	i = 0;
// 	single_quote = -1;
// 	while (node->cmd[i] != NULL)
// 	{
// 		j = 0;
// 		while (node->cmd[i][j] != '\0')
// 		{
// 			if (node->cmd[i][j] == '\'')
// 				single_quote *= -1;
// 			if (single_quote == -1 && node->cmd[i][j] == '$')
// 			{
// 				if (!(node->cmd[i][j + 1] == '\"' || \
// 						node->cmd[i][j + 1] == '\0' || node->cmd[i][j + 1] == '$'))
// 					node->cmd[i] = is_valid_env(node->cmd[i], &j, env);
// 				continue ;
// 			}
// 			j++;
// 		}
// 		i++;
// 	}
// }

char	*is_var(char *cmd, int idx, t_env *env)
{
	int		i;
	char	*ret;

	i = idx;
	ret = NULL;
	while (cmd[i] != '\0')
	{
		if (cmd[i] == '$')
			ret = is_valid_env(cmd, &i, env);
		i++;
	}
	if (ret == NULL)
		return (cmd);
	else
		free(cmd);
	return (ret);
}

// char *single_quote(char *no_quote, char *cmd, int *start)
// {
// 	int		len;
// 	int		i;
// 	int		j;
// 	char	*del_quote;

// 	i = 1;
// 	while (cmd[*start + i] != '\'')
// 		i++;
// 	len = ft_strlen(no_quote) + i;
// 	del_quote = (char *)malloc(sizeof(char) * len);
// 	i = -1;
// 	while (no_quote[++i] != '\0')
// 		del_quote[i] = no_quote[i];
// 	j = 1;
// 	while (cmd[*start + j] != '\'')
// 	{
// 		del_quote[i] = cmd[*start + j];
// 		i++;
// 		j++;
// 	}
// 	del_quote[i] = '\0';
// 	free(no_quote);
// 	*start = *start + j + 1;
// 	return (del_quote);
// }


// char *single_quote(char *cmd, int *start)
// {
// 	char	*tmp;
// 	int		i;
// 	int		len;

// 	i = 0;
// 	tmp = (char *)malloc(sizeof(char) * (ft_strlen(cmd) - 1));
// 	if (tmp == NULL)
// 		handle_error("exit : malloc error", 1, 0);
// 	ft_memcpy(tmp, cmd, *start);
// 	while (cmd[*start + i] != '\'')
// 		i++;
// 	ft_memcpy(tmp + (*start), cmd + (*start) + 1, i - 1);
// 	// ft_memcpy(tmp + (*start) + i - 1, cmd + (*start) + i + 1, );
// 	*start = *start + i - 1;
// 	tmp[ft_strlen(cmd) - 3] = '\0';
// 	free(cmd);
// 	return (tmp);
// }

char *single_quote(char *cmd, int *idx)
{
	int		i;
	int		quote_start;
	int		quote_close;
	char	*in_quote;
	char	*ret;

	i = 1;
	quote_start = *idx;
	while (cmd[quote_start + i] != '\'')
		i++;
	quote_close = quote_start + i;
	in_quote = env_strcpy(quote_start + 1, quote_close - 1, cmd);
	ret = replace_str(cmd, quote_start, &quote_close, in_quote);
	*idx = quote_close;
	free(in_quote);
	free(cmd);
	return (ret);
}

char	*double_quote(char *cmd, int *idx, t_env *env)
{
	int		i;
	int		quote_start;
	int		quote_close;
	char	*in_quote;
	char	*ret;

	i = 1;
	quote_start = *idx;
	cmd = is_var(cmd, *idx, env);
	while (cmd[quote_start + i] != '\"')
		i++;
	quote_close = quote_start + i;
	in_quote = env_strcpy(quote_start + 1, quote_close - 1, cmd);
	ret = replace_str(cmd, quote_start, &quote_close, in_quote);
	*idx = quote_close - 1;
	free(in_quote);
	free(cmd);
	return (ret);
}

// char *normal_words(char *cmd, int *idx, t_env *env)
// {
// 	int		i;
// 	char	*ret;

// 	i = *idx;
// 	while (!(cmd[i] == '\'' || cmd[i] == '\"' || cmd[i] == '\-'))
// 	{
// 		if (cmd[i] == '$')
// 		{
// 			if (!(cmd[i + 1] == '\"' || \
// 					cmd[i + 1] == '\0' || cmd[i + 1] == '$'))
// 				ret = is_valid_env(cmd, &i, env);
// 			continue ;
// 		}
// 		i++;
// 	}
// 	if (ret == NULL)
// 		return (cmd);
// 	else
// 		free(cmd);
// 	*idx = i - 1;
// 	return (ret);
// }

void	handle_quote(t_node *node, t_env *env)
{
	int		i;
	int		j;
	char	*words_tmp;

	i = 0;
	while (node->cmd[i] != NULL)
	{
		j = 0;
		while (node->cmd[i][j] != '\0')
		{
			if (node->cmd[i][j] == '\'')
				node->cmd[i] = single_quote(node->cmd[i], &j);
			else if (node->cmd[i][j] == '\"')
				node->cmd[i] = double_quote(node->cmd[i], &j, env);
			else if (node->cmd[i][j] == '$')
				node->cmd[i] = is_valid_env(node->cmd[i], &j, env);
			j++;
		}
		i++;
	}
}



void	check_cmd_node(t_node *ast, t_env *env)//TODO exit status 구현하기
{
	t_node	*node;
	char	*var;

	node = ast;
	if (node->type == N_CMD)
	{
		// is_var(node, env);
		handle_quote(node, env);
		
		// is_wild_card(node);
		return ;
	}
	check_cmd_node(node->right, env);
	check_cmd_node(node->left, env);
}

//TODO 중요한점 : 서브쉘을 실행할 때 우리가 수정한 환경변수가 자식 프로세스?? 에  전달이 되어야함. 그그니니까  할당한  env가 전달되어야함.

//TODO
//1. 