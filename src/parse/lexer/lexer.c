/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 17:06:13 by ravazque          #+#    #+#             */
/*   Updated: 2025/10/20 19:26:00 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

static int	is_pipe(const char *str)
{
	if (!str)
		return (0);
	if (ft_strcmp(str, "|") == 0)
		return (1);
	return (0);
}

static int	is_redir(const char *str)
{
	if (!str)
		return (0);
	if (ft_strcmp(str, "<") == 0)
		return (1);
	if (ft_strcmp(str, ">") == 0)
		return (1);
	if (ft_strcmp(str, ">>") == 0)
		return (1);
	if (ft_strcmp(str, "<<") == 0)
		return (1);
	return (0);
}

static t_redir	*mk_redir(const char *tgt, const char *op, t_token *tgt_tok)
{
	t_redir			*r;
	t_token_part	*part;

	r = (t_redir *)malloc(sizeof(t_redir));
	if (!r)
		return (malloc_error(), NULL);
	ft_bzero(r, sizeof(t_redir));
	r->target = ft_strdup(tgt);
	if (!r->target)
	{
		free(r);
		return (malloc_error(), NULL);
	}
	if (ft_strcmp(op, "<") == 0)
		r->in_redir = 1;
	else if (ft_strcmp(op, "<<") == 0)
	{
		r->in_redir = 2;
		if (tgt_tok && (tgt_tok->is_squote || tgt_tok->is_dquote))
			r->hd_expand = 0;
		else if (tgt_tok && tgt_tok->parts)
		{
			part = tgt_tok->parts;
			r->hd_expand = 1;
			while (part)
			{
				if (part->is_squote || part->is_dquote)
				{
					r->hd_expand = 0;
					break ;
				}
				part = part->next;
			}
		}
		else
			r->hd_expand = 1;
	}
	else if (ft_strcmp(op, ">") == 0)
		r->out_redir = 1;
	else if (ft_strcmp(op, ">>") == 0)
		r->out_redir = 2;
	r->next = NULL;
	return (r);
}

static void	add_redir(t_cmd *cmd, t_redir *new)
{
	t_redir	*curr;

	if (!cmd->redirs)
	{
		cmd->redirs = new;
		return ;
	}
	curr = cmd->redirs;
	while (curr->next)
		curr = curr->next;
	curr->next = new;
}

static t_token	*mk_tok_node(const char *raw, int sq, int dq)
{
	t_token	*tok;

	tok = (t_token *)malloc(sizeof(t_token));
	if (!tok)
		return (malloc_error(), NULL);
	tok->raw = ft_strdup(raw);
	if (!tok->raw)
	{
		free(tok);
		return (malloc_error(), NULL);
	}
	tok->is_squote = sq;
	tok->is_dquote = dq;
	tok->parts = NULL;
	tok->next = NULL;
	return (tok);
}

static void	add_tok(t_cmd *cmd, t_token *new)
{
	t_token	*curr;

	if (!cmd->tokn)
	{
		cmd->tokn = new;
		return ;
	}
	curr = cmd->tokn;
	while (curr->next)
		curr = curr->next;
	curr->next = new;
}

static char	**toks_to_arr(t_token *toks)
{
	char	**arr;
	t_token	*curr;
	int		cnt;
	int		i;

	cnt = 0;
	curr = toks;
	while (curr)
	{
		cnt++;
		curr = curr->next;
	}
	arr = (char **)malloc(sizeof(char *) * (cnt + 1));
	if (!arr)
		return (malloc_error(), NULL);
	curr = toks;
	i = 0;
	while (curr)
	{
		arr[i] = ft_strdup(curr->raw);
		if (!arr[i])
		{
			while (i > 0)
			{
				i--;
				free(arr[i]);
			}
			free(arr);
			return (malloc_error(), NULL);
		}
		curr = curr->next;
		i++;
	}
	arr[cnt] = NULL;
	return (arr);
}

static int	rm_redir_toks(t_token **head, t_token *curr, t_token *prev)
{
	t_token	*next;

	if (!curr || !curr->next)
		return (1);
	next = curr->next;
	if (prev)
		prev->next = next->next;
	else
		*head = next->next;
	if (curr->raw)
		free(curr->raw);
	if (curr->parts)
		free_token_parts(curr->parts);
	free(curr);
	if (next->raw)
		free(next->raw);
	if (next->parts)
		free_token_parts(next->parts);
	free(next);
	return (0);
}

static int	proc_redirs(t_cmd *cmd)
{
	t_token	*curr;
	t_token	*next;
	t_token	*prev;
	t_redir	*r;

	if (!cmd || !cmd->tokn)
		return (0);
	curr = cmd->tokn;
	prev = NULL;
	while (curr)
	{
		next = curr->next;
		if (is_redir(curr->raw))
		{
			if (!next)
			{
				ft_putstr_fd(ERR_RDI, STDERR_FILENO);
				return (1);
			}
			if (is_redir(next->raw))
			{
				ft_putstr_fd("minishell: syntax error near unexpected token `", STDERR_FILENO);
				ft_putstr_fd(next->raw, STDERR_FILENO);
				ft_putstr_fd("'\n", STDERR_FILENO);
				return (1);
			}
			r = mk_redir(next->raw, curr->raw, next);
			if (!r)
				return (1);
			add_redir(cmd, r);
			rm_redir_toks(&cmd->tokn, curr, prev);
			curr = prev;
			if (curr)
				curr = curr->next;
			else
				curr = cmd->tokn;
			continue ;
		}
		prev = curr;
		curr = next;
	}
	return (0);
}

static void	add_cmd_to_lst(t_cmd **lst, t_cmd *new)
{
	t_cmd	*curr;

	if (!*lst)
		*lst = new;
	else
	{
		curr = *lst;
		while (curr->next)
			curr = curr->next;
		curr->next = new;
	}
}

static int	count_tokens_until_pipe(t_token *start)
{
	int		count;
	t_token	*curr;

	count = 0;
	curr = start;
	while (curr && !is_pipe(curr->raw))
	{
		count++;
		curr = curr->next;
	}
	return (count);
}

static t_token	*skip_to_next_pipe(t_token *start)
{
	t_token	*curr;

	curr = start;
	while (curr && !is_pipe(curr->raw))
		curr = curr->next;
	if (curr && is_pipe(curr->raw))
		return (curr->next);
	return (NULL);
}

static t_cmd	*create_cmd_from_tokens(t_token *start, int count)
{
	t_cmd			*new_cmd;
	t_token			*curr;
	t_token			*new_tok;
	int				i;
	t_token_part	*part_curr;
	t_token_part	*new_part;
	t_token_part	*last_part;

	new_cmd = (t_cmd *)malloc(sizeof(t_cmd));
	if (!new_cmd)
		return (malloc_error(), NULL);
	ft_bzero(new_cmd, sizeof(t_cmd));
	curr = start;
	i = 0;
	while (i < count && curr)
	{
		new_tok = mk_tok_node(curr->raw, curr->is_squote, curr->is_dquote);
		if (!new_tok)
		{
			free_cmds(new_cmd);
			return (NULL);
		}
		if (curr->parts)
		{
			part_curr = curr->parts;
			last_part = NULL;
			while (part_curr)
			{
				new_part = (t_token_part *)malloc(sizeof(t_token_part));
				if (!new_part)
				{
					free_cmds(new_cmd);
					return (NULL);
				}
				new_part->content = ft_strdup(part_curr->content);
				if (!new_part->content)
				{
					free(new_part);
					free_cmds(new_cmd);
					return (NULL);
				}
				new_part->is_squote = part_curr->is_squote;
				new_part->is_dquote = part_curr->is_dquote;
				new_part->next = NULL;
				if (!new_tok->parts)
					new_tok->parts = new_part;
				else
					last_part->next = new_part;
				last_part = new_part;
				part_curr = part_curr->next;
			}
		}
		add_tok(new_cmd, new_tok);
		curr = curr->next;
		i++;
	}
	return (new_cmd);
}

static int	validate_pipe_syntax(t_token *tokens)
{
	t_token	*curr;
	int		first_token;

	if (!tokens)
		return (0);
	curr = tokens;
	first_token = 1;
	while (curr)
	{
		if (is_pipe(curr->raw))
		{
			if (first_token)
			{
				ft_putstr_fd("minishell: syntax error near unexpected token `|'\n", STDERR_FILENO);
				return (1);
			}
			if (curr->next && is_pipe(curr->next->raw))
			{
				ft_putstr_fd("minishell: syntax error near unexpected token `|'\n", STDERR_FILENO);
				return (1);
			}
			if (!curr->next)
			{
				ft_putstr_fd("minishell: syntax error: unexpected end after `|'\n", STDERR_FILENO);
				return (1);
			}
		}
		first_token = 0;
		curr = curr->next;
	}
	return (0);
}

static int	split_by_pipes(t_mini *mini)
{
	t_token	*curr;
	t_cmd	*cmd_list;
	t_cmd	*new_cmd;
	int		count;

	if (!mini->cmds || !mini->cmds->tokn)
		return (0);
	if (validate_pipe_syntax(mini->cmds->tokn))
		return (1);
	curr = mini->cmds->tokn;
	cmd_list = NULL;
	while (curr)
	{
		count = count_tokens_until_pipe(curr);
		if (count == 0)
		{
			curr = skip_to_next_pipe(curr);
			continue ;
		}
		new_cmd = create_cmd_from_tokens(curr, count);
		if (!new_cmd)
		{
			free_cmds(cmd_list);
			return (1);
		}
		add_cmd_to_lst(&cmd_list, new_cmd);
		curr = skip_to_next_pipe(curr);
	}
	free_cmds(mini->cmds);
	mini->cmds = cmd_list;
	return (0);
}

static int	finalize_cmds(t_mini *mini)
{
	t_cmd	*curr;

	curr = mini->cmds;
	while (curr)
	{
		if (curr->tokn)
		{
			curr->tokens = toks_to_arr(curr->tokn);
			if (!curr->tokens)
				return (1);
		}
		curr = curr->next;
	}
	return (0);
}

int	lexer(t_mini *mini)
{
	t_cmd	*curr;

	if (!mini || !mini->cmds)
		return (0);
	if (split_by_pipes(mini))
		return (1);
	curr = mini->cmds;
	while (curr)
	{
		if (proc_redirs(curr))
			return (1);
		curr = curr->next;
	}
	if (finalize_cmds(mini))
		return (1);
	return (0);
}
