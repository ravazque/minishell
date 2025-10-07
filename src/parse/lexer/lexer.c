/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 17:06:13 by ravazque          #+#    #+#             */
/*   Updated: 2025/10/08 00:57:45 by ravazque         ###   ########.fr       */
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

static t_redir	*mk_redir(const char *tgt, const char *op)
{
	t_redir	*r;

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
		r->i_redir = 1;
	else if (ft_strcmp(op, "<<") == 0)
	{
		r->i_redir = 2;
		r->hd_expand = 1;
	}
	else if (ft_strcmp(op, ">") == 0)
		r->o_redir = 1;
	else if (ft_strcmp(op, ">>") == 0)
		r->o_redir = 2;
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

/* Continuación de lexer.c */

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
			r = mk_redir(next->raw, curr->raw);
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

static int	add_cmd_to_lst(t_cmd **lst, t_cmd *new)
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
	return (0);
}

static int	mk_cmd_range(char **toks, int start, int end, t_cmd **lst)
{
	t_cmd	*new;
	t_token	*tok_node;
	int		i;

	if (end <= start)
		return (0);
	new = (t_cmd *)malloc(sizeof(t_cmd));
	if (!new)
		return (malloc_error(), 1);
	ft_bzero(new, sizeof(t_cmd));
	i = start;
	while (i < end)
	{
		tok_node = mk_tok_node(toks[i], 0, 0);
		if (!tok_node)
		{
			free_cmds(new);
			return (1);
		}
		add_tok(new, tok_node);
		i++;
	}
	return (add_cmd_to_lst(lst, new));
}

static int	proc_pipe(char **toks, int *i, int *start, t_cmd **lst)
{
	if (mk_cmd_range(toks, *start, *i, lst))
		return (1);
	*start = *i + 1;
	return (0);
}

static int	split_pipes(t_mini *mini)
{
	char	**toks;
	t_cmd	*lst;
	int		i;
	int		start;

	if (!mini->cmds || !mini->cmds->tokens)
		return (0);
	toks = mini->cmds->tokens;
	mini->cmds->tokens = NULL;
	free_cmds(mini->cmds);
	mini->cmds = NULL;
	lst = NULL;
	start = 0;
	i = 0;
	while (toks[i])
	{
		if (is_pipe(toks[i]))
		{
			if (proc_pipe(toks, &i, &start, &lst))
			{
				free_dblptr(toks);
				free_cmds(lst);
				return (1);
			}
		}
		i++;
	}
	if (mk_cmd_range(toks, start, i, &lst))
	{
		free_dblptr(toks);
		free_cmds(lst);
		return (1);
	}
	mini->cmds = lst;
	free_dblptr(toks);
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
	if (split_pipes(mini))
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
