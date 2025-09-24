/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 03:22:36 by ravazque          #+#    #+#             */
/*   Updated: 2025/09/24 16:16:23 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

static int	str_append_char(char **dst, char c)
{
	char	*old;
	size_t	len;
	size_t	i;
	char	*newp;

	old = *dst;
	len = 0;
	if (old)
	{
		while (old[len])
			len++;
	}
	newp = (char *)malloc(len + 2);
	if (!newp)
		return (1);
	i = 0;
	while (i < len)
	{
		newp[i] = old[i];
		i++;
	}
	newp[len] = c;
	newp[len + 1] = '\0';
	if (old)
		free(old);
	*dst = newp;
	return (0);
}

static int	str_append_slice(char **dst, const char *src, size_t start, size_t end_excl)
{
	size_t	i;

	i = start;
	while (i < end_excl)
	{
		if (str_append_char(dst, src[i]))
			return (1);
		i++;
	}
	return (0);
}

static t_token_info	**push_token_info(t_token_info ***dst, const char *content, int is_sq, int is_dq)
{
	size_t		n;
	size_t		i;
	t_token_info	**newp;
	t_token_info	*new_token;

	n = 0;
	if (*dst)
	{
		while ((*dst)[n])
			n++;
	}
	newp = (t_token_info **)malloc(sizeof(t_token_info *) * (n + 2));
	if (!newp)
		return (NULL);
	new_token = (t_token_info *)malloc(sizeof(t_token_info));
	if (!new_token)
	{
		free(newp);
		return (NULL);
	}
	new_token->content = ft_strdup(content);
	if (!new_token->content)
	{
		free(new_token);
		free(newp);
		return (NULL);
	}
	new_token->is_squote = is_sq;
	new_token->is_dquote = is_dq;
	i = 0;
	while (i < n)
	{
		newp[i] = (*dst)[i];
		i++;
	}
	newp[n] = new_token;
	newp[n + 1] = NULL;
	if (*dst)
		free(*dst);
	*dst = newp;
	return (*dst);
}

static int	flush_word(char **buff, int tok_in_progress, int is_sq, int is_dq, t_token_info ***parts)
{
	if (tok_in_progress)
	{
		if (*buff)
		{
			if (!push_token_info(parts, *buff, is_sq, is_dq))
				return (1);
			free(*buff);
			*buff = NULL;
		}
		else
		{
			if (!push_token_info(parts, "", is_sq, is_dq))
				return (1);
		}
	}
	return (0);
}

static int	read_quoted(const char *in, size_t *i, char quote, char **buff)
{
	size_t	start;

	start = *i;
	while (in[*i] && in[*i] != quote)
		(*i)++;
	if (!in[*i])
		return (1);
	if (start < *i)
	{
		if (str_append_slice(buff, in, start, *i))
			return (1);
	}
	(*i)++;
	return (0);
}

static void	free_token_info_array(t_token_info **parts)
{
	int	i;

	if (!parts)
		return ;
	i = 0;
	while (parts[i])
	{
		if (parts[i]->content)
			free(parts[i]->content);
		free(parts[i]);
		i++;
	}
	free(parts);
}

static char	**convert_to_string_array(t_token_info **parts)
{
	char	**result;
	int		count;
	int		i;

	if (!parts)
		return (NULL);
	count = 0;
	while (parts[count])
		count++;
	result = (char **)malloc(sizeof(char *) * (count + 1));
	if (!result)
		return (NULL);
	i = 0;
	while (i < count)
	{
		result[i] = ft_strdup(parts[i]->content);
		if (!result[i])
		{
			free_dblptr(result);
			return (NULL);
		}
		i++;
	}
	result[count] = NULL;
	return (result);
}

static int	build_single_cmd_with_info(t_mini *mini, t_token_info **parts)
{
	t_cmd		*cmd;
	t_token		*token_node;
	t_token		*current_token;
	int			i;

	cmd = (t_cmd *)malloc(sizeof(t_cmd));
	if (!cmd)
	{
		free_token_info_array(parts);
		malloc_error();
		return (1);
	}
	ft_bzero(cmd, sizeof(t_cmd));
	cmd->tokens = convert_to_string_array(parts);
	if (!cmd->tokens && parts[0])
	{
		free(cmd);
		free_token_info_array(parts);
		return (1);
	}
	current_token = NULL;
	i = 0;
	while (parts[i])
	{
		token_node = (t_token *)malloc(sizeof(t_token));
		if (!token_node)
		{
			free_cmds(cmd);
			free_token_info_array(parts);
			return (1);
		}
		token_node->raw = ft_strdup(parts[i]->content);
		token_node->is_squote = parts[i]->is_squote;
		token_node->is_dquote = parts[i]->is_dquote;
		token_node->next = NULL;
		if (!cmd->tokn)
			cmd->tokn = token_node;
		else
			current_token->next = token_node;
		current_token = token_node;
		i++;
	}
	cmd->next = NULL;
	mini->cmds = cmd;
	free_token_info_array(parts);
	return (0);
}

int	tokenizer(t_mini **mini)
{
	char		*in;
	size_t		i;
	char		*buff;
	t_token_info	**parts;
	int			tok_in_progress;
	char		q;
	int			token_is_squote;
	int			token_is_dquote;

	in = (*mini)->input;
	i = 0;
	buff = NULL;
	parts = NULL;
	tok_in_progress = 0;
	token_is_squote = 0;
	token_is_dquote = 0;
	while (in && in[i])
	{
		if (is_space(in[i]))
		{
			if (flush_word(&buff, tok_in_progress, token_is_squote, token_is_dquote, &parts))
				return (free(buff), free_token_info_array(parts), 1);
			tok_in_progress = 0;
			token_is_squote = 0;
			token_is_dquote = 0;
			while (in[i] && is_space(in[i]))
				i++;
		}
		else if (in[i] == '\'' || in[i] == '\"')
		{
			q = in[i];
			i++;
			tok_in_progress = 1;
			if (q == '\'')
				token_is_squote = 1;
			else
				token_is_dquote = 1;
			if (read_quoted(in, &i, q, &buff))
				return (free(buff), free_token_info_array(parts), 1);
		}
		else
		{
			tok_in_progress = 1;
			if (str_append_char(&buff, in[i]))
				return (free(buff), free_token_info_array(parts), 1);
			i++;
		}
	}
	if (flush_word(&buff, tok_in_progress, token_is_squote, token_is_dquote, &parts))
		return (free(buff), free_token_info_array(parts), 1);
	if (!parts)
		return (0);
	return (build_single_cmd_with_info(*mini, parts));
}
