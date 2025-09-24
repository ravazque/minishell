/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 03:22:36 by ravazque          #+#    #+#             */
/*   Updated: 2025/09/24 03:25:13 by ravazque         ###   ########.fr       */
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

static int	flush_word(char **buff, int tok_in_progress, char ***parts)
{
	if (tok_in_progress)
	{
		if (*buff)
		{
			if (!dbpt_push(parts, *buff))
				return (1);
			free(*buff);
			*buff = NULL;
		}
		else
		{
			if (!dbpt_push(parts, ""))
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

static int	build_single_cmd(t_mini *mini, char **parts)
{
	t_cmd	*cmd;

	cmd = (t_cmd *)malloc(sizeof(t_cmd));
	if (!cmd)
	{
		free_dblptr(parts);
		malloc_error();
		return (1);
	}
	ft_bzero(cmd, sizeof(t_cmd));
	cmd->tokens = parts;
	cmd->next = NULL;
	mini->cmds = cmd;
	return (0);
}

int	tokenizer(t_mini **mini)
{
	char	*in;
	size_t	i;
	char	*buff;
	char	**parts;
	int		tok_in_progress;
	char	q;

	in = (*mini)->input;
	i = 0;
	buff = NULL;
	parts = NULL;
	tok_in_progress = 0;
	while (in && in[i])
	{
		if (is_space(in[i]))
		{
			if (flush_word(&buff, tok_in_progress, &parts))
				return (free(buff), free_dblptr(parts), 1);
			tok_in_progress = 0;
			while (in[i] && is_space(in[i]))
				i++;
		}
		else if (in[i] == '\'' || in[i] == '\"')
		{
			q = in[i];
			i++;
			tok_in_progress = 1;
			if (read_quoted(in, &i, q, &buff))
				return (free(buff), free_dblptr(parts), 1);
		}
		else
		{
			tok_in_progress = 1;
			if (str_append_char(&buff, in[i]))
				return (free(buff), free_dblptr(parts), 1);
			i++;
		}
	}
	if (flush_word(&buff, tok_in_progress, &parts))
		return (free(buff), free_dblptr(parts), 1);
	if (!parts)
		return (0);
	return (build_single_cmd(*mini, parts));
}
