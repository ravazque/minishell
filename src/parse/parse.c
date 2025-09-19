/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 17:15:38 by ravazque          #+#    #+#             */
/*   Updated: 2025/09/19 06:42:00 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	quotes_balanced(const char *s)
{
	size_t	i;
	int		in_sq;
	int		in_dq;

	i = 0;
	in_sq = 0;
	in_dq = 0;
	while (s[i])
	{
		if (s[i] == '\'' && in_dq == 0)
			in_sq = !in_sq;
		else if (s[i] == '\"' && in_sq == 0)
			in_dq = !in_dq;
		i++;
	}
	return (in_sq == 0 && in_dq == 0);
}

static void	simple_mark(char *s, size_t *i)
{
	size_t	k;

	k = *i + 1;
	while (s[k] && s[k] != '\'')
	{
		if (s[k] == ' ' || s[k] == '\t')
			s[k] = QPAD;
		k++;
	}
	if (s[k] == '\'')
		k++;
	*i = k;
}

static void	double_mark(char *s, size_t *i)
{
	size_t	k;

	k = *i + 1;
	while (s[k] && s[k] != '\"')
	{
		if (s[k] == ' ' || s[k] == '\t')
			s[k] = QPAD;
		k++;
	}
	if (s[k] == '\"')
		k++;
	*i = k;
}

static char	*unquote_token(const char *s)
{
	size_t	i;
	size_t	j;
	int		in_sq;
	int		in_dq;
	char	*out;

	out = (char *)malloc((ft_strlen(s) + 1) * sizeof(char));
	if (!out)
		return (NULL);
	i = 0;
	j = 0;
	in_sq = 0;
	in_dq = 0;
	while (s[i])
	{
		if (s[i] == '\'' && in_dq == 0)
		{
			in_sq = !in_sq;
			i++;
			continue ;
		}
		if (s[i] == '\"' && in_sq == 0)
		{
			in_dq = !in_dq;
			i++;
			continue ;
		}
		if (s[i] == QPAD)
			out[j++] = ' ';
		else
			out[j++] = s[i];
		i++;
	}
	out[j] = '\0';
	return (out);
}

void	parse(t_mini *mini)
{
	char	*buf;
	char	**parts;
	size_t	k;
	size_t	i;
	t_cmd	*cmd;
	char	*clean;

	if (!mini || !mini->input)
		return ;
	if (!quotes_balanced(mini->input))
	{
		ft_putstr_fd(ERR_QUO, STDERR_FILENO);
		mini->exit_sts = 2;
		return ;
	}
	buf = ft_strdup(mini->input);
	if (!buf)
		return (malloc_error());
	k = 0;
	while (buf[k])
	{
		if (buf[k] == '\'')
			simple_mark(buf, &k);
		else if (buf[k] == '\"')
			double_mark(buf, &k);
		else
		{
			if (buf[k] == '\t')
				buf[k] = ' ';
			k++;
		}
	}
	parts = ft_split(buf, ' ');
	free(buf);
	if (!parts)
		return (split_error());
	i = 0;
	while (parts[i])
	{
		clean = unquote_token(parts[i]);
		if (!clean)
		{
			free_dblptr(parts);
			return (malloc_error());
		}
		free(parts[i]);
		parts[i] = clean;
		i++;
	}
	cmd = (t_cmd *)malloc(sizeof(t_cmd));
	if (!cmd)
	{
		free_dblptr(parts);
		return (malloc_error());
	}
	ft_bzero(cmd, sizeof(t_cmd));
	cmd->tokens = parts;
	cmd->next = NULL;
	mini->cmds = cmd;
}
