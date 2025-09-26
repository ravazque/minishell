/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 03:22:36 by ravazque          #+#    #+#             */
/*   Updated: 2025/09/26 12:31:01 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	free_token_parts(t_token_part *parts)
{
	t_token_part	*current;
	t_token_part	*next;

	current = parts;
	while (current)
	{
		next = current->next;
		if (current->content)
			free(current->content);
		free(current);
		current = next;
	}
}

static t_token_part	*create_token_part(const char *content, int is_sq, int is_dq)
{
	t_token_part	*part;

	part = (t_token_part *)malloc(sizeof(t_token_part));
	if (!part)
		return (NULL);
	part->content = ft_strdup(content);
	if (!part->content)
	{
		free(part);
		return (NULL);
	}
	part->is_squote = is_sq;
	part->is_dquote = is_dq;
	part->next = NULL;
	return (part);
}

static void	add_token_part(t_token_part **list, t_token_part *new_part)
{
	t_token_part	*current;

	if (!*list)
	{
		*list = new_part;
		return ;
	}
	current = *list;
	while (current->next)
		current = current->next;
	current->next = new_part;
}

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

static int	flush_current_part(char **buff, int is_sq, int is_dq, t_token_part **parts)
{
	t_token_part	*new_part;

	if (!*buff)
		return (0);
	new_part = create_token_part(*buff, is_sq, is_dq);
	if (!new_part)
		return (1);
	add_token_part(parts, new_part);
	free(*buff);
	*buff = NULL;
	return (0);
}

static int	read_quoted_content(const char *in, size_t *i, char quote, char **buff)
{
	size_t	start;

	start = *i;
	while (in[*i] && in[*i] != quote)
		(*i)++;
	if (!in[*i])
		return (1);
	while (start < *i)
	{
		if (str_append_char(buff, in[start]))
			return (1);
		start++;
	}
	(*i)++;
	return (0);
}

static char	*join_token_parts(t_token_part *parts)
{
	char			*result;
	t_token_part	*current;

	result = NULL;
	current = parts;
	while (current)
	{
		result = ft_strjoin(result, current->content);
		if (!result)
			return (NULL);
		current = current->next;
	}
	if (!result)
		result = ft_strdup("");
	return (result);
}

static t_token	*create_token_from_parts(t_token_part *parts)
{
	t_token	*token;
	char	*joined_content;

	if (!parts)
		return (NULL);
	token = (t_token *)malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	joined_content = join_token_parts(parts);
	if (!joined_content)
	{
		free(token);
		return (NULL);
	}
	token->raw = joined_content;
	token->is_squote = 0;
	token->is_dquote = 0;
	token->parts = parts;
	token->next = NULL;
	return (token);
}

static void	cleanup_tokenizer_data(char **buff, t_token_part **parts, t_cmd **cmd)
{
	if (buff && *buff)
	{
		free(*buff);
		*buff = NULL;
	}
	if (parts && *parts)
	{
		free_token_parts(*parts);
		*parts = NULL;
	}
	if (cmd && *cmd)
	{
		free_cmds(*cmd);
		*cmd = NULL;
	}
}

static int	finalize_token(char **buff, t_token_part **current_token_parts, t_cmd *cmd)
{
	t_token	*new_token;
	t_token	*current;

	if (*buff)
	{
		if (flush_current_part(buff, 0, 0, current_token_parts))
			return (1);
	}
	if (*current_token_parts)
	{
		new_token = create_token_from_parts(*current_token_parts);
		if (!new_token)
			return (1);
		if (!cmd->tokn)
			cmd->tokn = new_token;
		else
		{
			current = cmd->tokn;
			while (current->next)
				current = current->next;
			current->next = new_token;
		}
		*current_token_parts = NULL;
	}
	return (0);
}

int	tokenizer(t_mini **mini)
{
	char			*in;
	size_t			i;
	char			*buff;
	t_token_part	*current_token_parts;
	t_cmd			*cmd;
	int				in_token;

	in = (*mini)->input;
	i = 0;
	buff = NULL;
	current_token_parts = NULL;
	in_token = 0;
	
	cmd = (t_cmd *)malloc(sizeof(t_cmd));
	if (!cmd)
		return (1);
	ft_bzero(cmd, sizeof(t_cmd));
	
	while (in && in[i])
	{
		if (is_space(in[i]))
		{
			if (in_token)
			{
				if (finalize_token(&buff, &current_token_parts, cmd))
				{
					cleanup_tokenizer_data(&buff, &current_token_parts, &cmd);
					return (1);
				}
				in_token = 0;
			}
			while (in[i] && is_space(in[i]))
				i++;
		}
		else if (in[i] == '\'' || in[i] == '\"')
		{
			char	quote;
			int		is_sq;
			int		is_dq;

			quote = in[i];
			is_sq = (quote == '\'');
			is_dq = (quote == '\"');
			i++;
			in_token = 1;
			
			if (buff)
			{
				if (flush_current_part(&buff, 0, 0, &current_token_parts))
				{
					cleanup_tokenizer_data(&buff, &current_token_parts, &cmd);
					return (1);
				}
			}
			
			if (read_quoted_content(in, &i, quote, &buff))
			{
				cleanup_tokenizer_data(&buff, &current_token_parts, &cmd);
				return (1);
			}
			
			if (flush_current_part(&buff, is_sq, is_dq, &current_token_parts))
			{
				cleanup_tokenizer_data(&buff, &current_token_parts, &cmd);
				return (1);
			}
		}
		else
		{
			in_token = 1;
			if (str_append_char(&buff, in[i]))
			{
				cleanup_tokenizer_data(&buff, &current_token_parts, &cmd);
				return (1);
			}
			i++;
		}
	}
	
	if (in_token)
	{
		if (finalize_token(&buff, &current_token_parts, cmd))
		{
			cleanup_tokenizer_data(&buff, &current_token_parts, &cmd);
			return (1);
		}
	}
	
	(*mini)->cmds = cmd;
	return (0);
}
