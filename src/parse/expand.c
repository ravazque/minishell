/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 14:00:00 by ravazque          #+#    #+#             */
/*   Updated: 2025/09/24 16:31:39 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static char	*get_env_value(const char *key, char **env)
{
	int		i;
	int		key_len;
	char	*env_var;

	if (!key || !env)
		return (NULL);
	key_len = ft_strlen(key);
	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], key, key_len) == 0 && env[i][key_len] == '=')
		{
			env_var = ft_strdup(env[i] + key_len + 1);
			return (env_var);
		}
		i++;
	}
	return (ft_strdup(""));
}

static char	*get_exit_status(int exit_status)
{
	char	*status_str;

	status_str = ft_itoa(exit_status);
	if (!status_str)
		return (ft_strdup(""));
	return (status_str);
}

static int	is_valid_var_char(char c)
{
	if (ft_isalnum(c))
		return (1);
	if (c == '_')
		return (1);
	return (0);
}

static char	*extract_var_name(const char *str, int start, int *end)
{
	char	*var_name;
	int		i;
	int		len;

	if (str[start] == '?')
	{
		*end = start + 1;
		return (ft_strdup("?"));
	}
	i = start;
	while (str[i] && is_valid_var_char(str[i]))
		i++;
	*end = i;
	len = i - start;
	if (len == 0)
		return (NULL);
	var_name = (char *)malloc(len + 1);
	if (!var_name)
		return (NULL);
	i = 0;
	while (i < len)
	{
		var_name[i] = str[start + i];
		i++;
	}
	var_name[len] = '\0';
	return (var_name);
}

static char	*str_append(char *dst, const char *src)
{
	char	*result;
	int		dst_len;
	int		src_len;
	int		i;
	int		j;

	if (!src)
		return (dst);
	dst_len = 0;
	src_len = ft_strlen(src);
	if (dst)
		dst_len = ft_strlen(dst);
	result = (char *)malloc(dst_len + src_len + 1);
	if (!result)
	{
		if (dst)
			free(dst);
		return (NULL);
	}
	i = 0;
	while (i < dst_len)
	{
		result[i] = dst[i];
		i++;
	}
	j = 0;
	while (j < src_len)
	{
		result[i + j] = src[j];
		j++;
	}
	result[dst_len + src_len] = '\0';
	if (dst)
		free(dst);
	return (result);
}

static char	*str_append_char(char *dst, char c)
{
	char	temp[2];

	temp[0] = c;
	temp[1] = '\0';
	return (str_append(dst, temp));
}

static char	*expand_variable(const char *var_name, t_mini *mini)
{
	char	*value;

	if (!var_name)
		return (ft_strdup(""));
	if (ft_strcmp(var_name, "?") == 0)
		return (get_exit_status(mini->exit_sts));
	value = get_env_value(var_name, mini->env);
	if (!value)
		return (ft_strdup(""));
	return (value);
}

static char	*expand_string(const char *str, t_mini *mini, int should_expand)
{
	char	*result;
	char	*var_name;
	char	*var_value;
	int		i;
	int		var_end;

	result = NULL;
	i = 0;
	while (str[i])
	{
		if (str[i] == '$' && should_expand && str[i + 1] 
			&& (is_valid_var_char(str[i + 1]) || str[i + 1] == '?'))
		{
			var_name = extract_var_name(str, i + 1, &var_end);
			if (var_name)
			{
				var_value = expand_variable(var_name, mini);
				result = str_append(result, var_value);
				free(var_name);
				free(var_value);
				if (!result)
					return (NULL);
				i = var_end;
			}
			else
			{
				result = str_append_char(result, str[i]);
				if (!result)
					return (NULL);
				i++;
			}
		}
		else
		{
			result = str_append_char(result, str[i]);
			if (!result)
				return (NULL);
			i++;
		}
	}
	if (!result)
		return (ft_strdup(""));
	return (result);
}

static char	*expand_token_with_quotes(t_token *token, t_mini *mini)
{
	char	*expanded;
	int		should_expand;

	if (!token || !token->raw)
		return (ft_strdup(""));
	
	should_expand = 0;
	if (!token->is_squote)
		should_expand = 1;
	
	expanded = expand_string(token->raw, mini, should_expand);
	if (!expanded)
		return (ft_strdup(""));
	return (expanded);
}

static int	expand_cmd_tokens(t_cmd *cmd, t_mini *mini)
{
	t_token	*current_token;
	char	**new_tokens;
	char	*expanded;
	int		count;
	int		i;

	if (!cmd->tokn)
		return (0);
	count = 0;
	current_token = cmd->tokn;
	while (current_token)
	{
		count++;
		current_token = current_token->next;
	}
	new_tokens = (char **)malloc(sizeof(char *) * (count + 1));
	if (!new_tokens)
		return (1);
	current_token = cmd->tokn;
	i = 0;
	while (current_token)
	{
		expanded = expand_token_with_quotes(current_token, mini);
		if (!expanded)
		{
			while (i > 0)
			{
				i--;
				free(new_tokens[i]);
			}
			free(new_tokens);
			return (1);
		}
		new_tokens[i] = expanded;
		current_token = current_token->next;
		i++;
	}
	new_tokens[count] = NULL;
	if (cmd->tokens)
		free_dblptr(cmd->tokens);
	cmd->tokens = new_tokens;
	return (0);
}

static int	should_expand_redir_target(t_redir *redir)
{
	if (redir->i_redir == 2)
		return (redir->hd_expand);
	return (1);
}

static int	expand_redirections(t_cmd *cmd, t_mini *mini)
{
	t_redir	*current;
	char	*expanded;

	current = cmd->redirs;
	while (current)
	{
		if (should_expand_redir_target(current))
		{
			expanded = expand_string(current->target, mini, 1);
			if (!expanded)
				return (1);
			free(current->target);
			current->target = expanded;
		}
		current = current->next;
	}
	return (0);
}

static void	free_token_list(t_token *tokens)
{
	t_token	*current;
	t_token	*next;

	current = tokens;
	while (current)
	{
		next = current->next;
		if (current->raw)
			free(current->raw);
		free(current);
		current = next;
	}
}

int	expander(t_mini *mini)
{
	t_cmd	*current;

	if (!mini || !mini->cmds)
		return (0);
	current = mini->cmds;
	while (current)
	{
		if (expand_cmd_tokens(current, mini))
			return (1);
		if (expand_redirections(current, mini))
			return (1);
		if (current->tokn)
		{
			free_token_list(current->tokn);
			current->tokn = NULL;
		}
		current = current->next;
	}
	return (0);
}
