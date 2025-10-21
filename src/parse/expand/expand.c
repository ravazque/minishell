/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 14:00:00 by ravazque          #+#    #+#             */
/*   Updated: 2025/10/20 19:27:23 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

int	is_empty_str(const char *s)
{
	if (!s)
		return (1);
	if (s[0] == '\0')
		return (1);
	return (0);
}

int	tok_has_quotes(t_token *tok)
{
	t_token_part	*curr;

	if (!tok)
		return (0);
	if (!tok->parts)
		return (0);
	curr = tok->parts;
	while (curr)
	{
		if (curr->is_squote || curr->is_dquote)
			return (1);
		curr = curr->next;
	}
	return (0);
}

static char	*str_cat(char *dst, const char *src)
{
	char	*res;
	int		dlen;
	int		slen;
	int		i;
	int		j;

	if (!src)
		return (dst);
	dlen = 0;
	slen = ft_strlen(src);
	if (dst)
		dlen = ft_strlen(dst);
	res = (char *)malloc(dlen + slen + 1);
	if (!res)
	{
		if (dst)
			free(dst);
		return (NULL);
	}
	i = 0;
	while (i < dlen && dst)
	{
		res[i] = dst[i];
		i++;
	}
	j = 0;
	while (j < slen)
	{
		res[i + j] = src[j];
		j++;
	}
	res[dlen + slen] = '\0';
	if (dst)
		free(dst);
	return (res);
}

static char	*str_cat_chr(char *dst, char c)
{
	char	tmp[2];

	tmp[0] = c;
	tmp[1] = '\0';
	return (str_cat(dst, tmp));
}

static char	*get_env_val(const char *key, char **env)
{
	int		i;
	int		j;

	if (!key || !env)
		return (NULL);
	j = ft_strlen(key);
	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], key, j) == 0 && env[i][j] == '=')
			return (ft_strdup(env[i] + j + 1));
		i++;
	}
	return (NULL);
}

static char	*get_exit_sts(int exit_status)
{
	char	*sts_str;

	sts_str = ft_itoa(exit_status);
	if (!sts_str)
		return (NULL);
	return (sts_str);
}

static int	is_valid_var_chr(char c)
{
	if (ft_isalnum(c))
		return (1);
	if (c == '_')
		return (1);
	return (0);
}

static char	*extract_var(const char *str, int start, int *end)
{
	char	*var;
	int		i;
	int		len;

	if (str[start] == '?')
	{
		*end = start + 1;
		return (ft_strdup("?"));
	}
	if (str[start] == '$')
	{
		*end = start + 1;
		return (ft_strdup("$$"));
	}
	if (str[start] == '0')
	{
		*end = start + 1;
		return (ft_strdup("0"));
	}
	i = start;
	while (str[i] && is_valid_var_chr(str[i]))
		i++;
	*end = i;
	len = i - start;
	if (len == 0)
		return (NULL);
	var = (char *)malloc(len + 1);
	if (!var)
		return (NULL);
	i = 0;
	while (i < len)
	{
		var[i] = str[start + i];
		i++;
	}
	var[len] = '\0';
	return (var);
}


static char	*expand_var(const char *var, t_mini *mini)
{
	if (!var)
		return (NULL);
	if (ft_strcmp(var, "?") == 0)
		return (get_exit_sts(mini->exit_sts));
	if (ft_strcmp(var, "$$") == 0)
		return (ft_strdup("80085"));
	if (ft_strcmp(var, "0") == 0)
	{
		if (mini->argv && mini->argv[0])
			return (ft_strdup(mini->argv[0]));
		return (ft_strdup("minishell"));
	}
	return (get_env_val(var, mini->env));
}

char	*exp_str_part(const char *s, t_mini *mini, int exp)
{
	char	*res;
	char	*var;
	char	*val;
	int		i;
	int		vend;

	res = NULL;
	i = 0;
	while (s[i])
	{
		if (s[i] == '$' && exp && s[i + 1])
		{
			if (is_valid_var_chr(s[i + 1]) || s[i + 1] == '?' || s[i + 1] == '$' || s[i + 1] == '0')
			{
				var = extract_var(s, i + 1, &vend);
				if (var)
				{
					val = expand_var(var, mini);
					free(var);
					if (val)
					{
						res = str_cat(res, val);
						free(val);
						if (!res)
							return (NULL);
					}
					else
					{
						res = str_cat(res, "");
						if (!res)
							return (NULL);
					}
					i = vend;
				}
				else
				{
					res = str_cat_chr(res, s[i]);
					if (!res)
						return (NULL);
					i++;
				}
			}
			else
			{
				res = str_cat_chr(res, s[i]);
				if (!res)
					return (NULL);
				i++;
			}
		}
		else
		{
			res = str_cat_chr(res, s[i]);
			if (!res)
				return (NULL);
			i++;
		}
	}
	if (!res)
		return (ft_strdup(""));
	return (res);
}

char	*exp_tok_parts(t_token *tok, t_mini *mini)
{
	char			*res;
	char			*exp_part;
	t_token_part	*curr;
	int				exp;

	if (!tok)
		return (ft_strdup(""));
	if (!tok->parts)
	{
		exp = 0;
		if (!tok->is_squote)
			exp = 1;
		return (exp_str_part(tok->raw, mini, exp));
	}
	res = NULL;
	curr = tok->parts;
	while (curr)
	{
		exp = 0;
		if (!curr->is_squote)
			exp = 1;
		exp_part = exp_str_part(curr->content, mini, exp);
		if (!exp_part)
		{
			if (res)
				free(res);
			return (NULL);
		}
		res = str_cat(res, exp_part);
		free(exp_part);
		if (!res)
			return (NULL);
		curr = curr->next;
	}
	if (!res)
		return (ft_strdup(""));
	return (res);
}

static int	should_exp_redir(t_redir *redir)
{
	if (redir->in_redir == 2)
		return (0);
	return (1);
}

static int	exp_redirs(t_cmd *cmd, t_mini *mini)
{
	t_redir	*curr;
	char	*exp;

	curr = cmd->redirs;
	while (curr)
	{
		if (should_exp_redir(curr))
		{
			exp = exp_str_part(curr->target, mini, 1);
			if (!exp)
				return (1);
			free(curr->target);
			curr->target = exp;
		}
		curr = curr->next;
	}
	return (0);
}

static void	free_tok_parts(t_token_part *parts)
{
	t_token_part	*curr;
	t_token_part	*next;

	curr = parts;
	while (curr)
	{
		next = curr->next;
		if (curr->content)
			free(curr->content);
		free(curr);
		curr = next;
	}
}

static void	free_tok_list(t_token *toks)
{
	t_token	*curr;
	t_token	*next;

	curr = toks;
	while (curr)
	{
		next = curr->next;
		if (curr->raw)
			free(curr->raw);
		if (curr->parts)
			free_tok_parts(curr->parts);
		free(curr);
		curr = next;
	}
}

int	expander(t_mini *mini)
{
	t_cmd	*curr;

	if (!mini || !mini->cmds)
		return (0);
	curr = mini->cmds;
	while (curr)
	{
		if (exp_cmd_toks_with_split(curr, mini))
			return (1);
		if (exp_redirs(curr, mini))
			return (1);
		if (curr->tokn)
		{
			free_tok_list(curr->tokn);
			curr->tokn = NULL;
		}
		curr = curr->next;
	}
	return (0);
}
