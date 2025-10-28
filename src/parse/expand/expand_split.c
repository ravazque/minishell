/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_split.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/19 00:00:00 by ravazque          #+#    #+#             */
/*   Updated: 2025/10/19 20:13:55 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

static int	count_words(const char *str)
{
	int	count;
	int	in_word;
	int	i;

	count = 0;
	in_word = 0;
	i = 0;
	while (str[i])
	{
		if (is_space(str[i]))
			in_word = 0;
		else
		{
			if (in_word == 0)
			{
				count++;
				in_word = 1;
			}
		}
		i++;
	}
	return (count);
}

static char	*extract_word(const char *str, int *start)
{
	int		i;
	int		len;
	char	*word;
	int		j;

	while (str[*start] && is_space(str[*start]))
		(*start)++;
	i = *start;
	len = 0;
	while (str[i] && !is_space(str[i]))
	{
		len++;
		i++;
	}
	if (len == 0)
		return (NULL);
	word = (char *)malloc(len + 1);
	if (!word)
		return (NULL);
	j = 0;
	while (j < len)
	{
		word[j] = str[*start + j];
		j++;
	}
	word[len] = '\0';
	*start = i;
	return (word);
}

static void	free_split_result(char **result, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		if (result[i])
			free(result[i]);
		i++;
	}
	free(result);
}

static char	**split_by_spaces(const char *str)
{
	char	**result;
	int		word_count;
	int		pos;
	int		i;
	char	*word;

	if (!str)
		return (NULL);
	word_count = count_words(str);
	if (word_count == 0)
		return (NULL);
	result = (char **)malloc(sizeof(char *) * (word_count + 1));
	if (!result)
		return (NULL);
	pos = 0;
	i = 0;
	while (i < word_count)
	{
		word = extract_word(str, &pos);
		if (!word)
		{
			free_split_result(result, i);
			return (NULL);
		}
		result[i] = word;
		i++;
	}
	result[word_count] = NULL;
	return (result);
}

static int	needs_word_split(t_token *tok)
{
	t_token_part	*curr;

	if (!tok)
		return (0);
	if (tok->is_squote || tok->is_dquote)
		return (0);
	if (is_assignment(tok->raw))
		return (0);
	if (!tok->parts)
		return (1);
	curr = tok->parts;
	while (curr)
	{
		if (!curr->is_squote && !curr->is_dquote)
			return (1);
		curr = curr->next;
	}
	return (0);
}

static int	count_expanded_tokens(t_token *tokens, t_mini *mini)
{
	t_token	*curr;
	char	*expanded;
	char	**split;
	int		total;
	int		i;

	total = 0;
	curr = tokens;
	while (curr)
	{
		if (curr->is_assignment)
		{
			curr = curr->next;
			continue ;
		}
		expanded = exp_tok_parts(curr, mini);
		if (!expanded)
			return (-1);
		if (is_empty_str(expanded) && !tok_has_quotes(curr))
		{
			free(expanded);
			curr = curr->next;
			continue ;
		}
		if (needs_word_split(curr) && !is_empty_str(expanded))
		{
			split = split_by_spaces(expanded);
			free(expanded);
			if (split)
			{
				i = 0;
				while (split[i])
					i++;
				total = total + i;
				free_dblptr(split);
			}
		}
		else
		{
			total++;
			free(expanded);
		}
		curr = curr->next;
	}
	return (total);
}

static int	add_expanded_tokens(char **result, int *j, t_token *tok, t_mini *mini)
{
	char	*expanded;
	char	**split;
	int		i;

	expanded = exp_tok_parts(tok, mini);
	if (!expanded)
		return (1);
	if (is_empty_str(expanded) && !tok_has_quotes(tok))
	{
		free(expanded);
		return (0);
	}
	if (needs_word_split(tok) && !is_empty_str(expanded))
	{
		split = split_by_spaces(expanded);
		free(expanded);
		if (!split)
			return (1);
		i = 0;
		while (split[i])
		{
			result[*j] = split[i];
			(*j)++;
			i++;
		}
		free(split);
	}
	else
	{
		result[*j] = expanded;
		(*j)++;
	}
	return (0);
}

int	exp_cmd_toks_with_split(t_cmd *cmd, t_mini *mini)
{
	t_token	*curr;
	char	**new;
	int		total_count;
	int		j;

	if (!cmd->tokn)
		return (0);
	total_count = count_expanded_tokens(cmd->tokn, mini);
	if (total_count < 0)
		return (1);
	if (total_count == 0)
	{
		new = (char **)malloc(sizeof(char *) * 1);
		if (!new)
			return (1);
		new[0] = NULL;
		if (cmd->tokens)
			free_dblptr(cmd->tokens);
		cmd->tokens = new;
		return (0);
	}
	new = (char **)malloc(sizeof(char *) * (total_count + 1));
	if (!new)
		return (1);
	curr = cmd->tokn;
	j = 0;
	while (curr)
	{
		if (add_expanded_tokens(new, &j, curr, mini))
		{
			free_split_result(new, j);
			return (1);
		}
		curr = curr->next;
	}
	new[j] = NULL;
	if (cmd->tokens)
		free_dblptr(cmd->tokens);
	cmd->tokens = new;
	return (0);
}
