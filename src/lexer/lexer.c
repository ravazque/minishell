/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 17:06:13 by ravazque          #+#    #+#             */
/*   Updated: 2025/09/27 13:39:54 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	is_pipe(const char *str)
{
	if (!str)
		return (0);
	if (ft_strcmp(str, "|") == 0)
		return (1);
	return (0);
}

static int	is_redir_in(const char *str)
{
	if (!str)
		return (0);
	if (ft_strcmp(str, "<") == 0)
		return (1);
	return (0);
}

static int	is_redir_out(const char *str)
{
	if (!str)
		return (0);
	if (ft_strcmp(str, ">") == 0)
		return (1);
	return (0);
}

static int	is_redir_append(const char *str)
{
	if (!str)
		return (0);
	if (ft_strcmp(str, ">>") == 0)
		return (1);
	return (0);
}

static int	is_redir_heredoc(const char *str)
{
	if (!str)
		return (0);
	if (ft_strcmp(str, "<<") == 0)
		return (1);
	return (0);
}

static int	is_redirection(const char *str)
{
	if (is_redir_in(str))
		return (1);
	if (is_redir_out(str))
		return (1);
	if (is_redir_append(str))
		return (1);
	if (is_redir_heredoc(str))
		return (1);
	return (0);
}

static t_redir	*create_redir(const char *target, const char *redir_op)
{
	t_redir	*redir;

	redir = (t_redir *)malloc(sizeof(t_redir));
	if (!redir)
		return (NULL);
	ft_bzero(redir, sizeof(t_redir));
	redir->target = ft_strdup(target);
	if (!redir->target)
	{
		free(redir);
		return (NULL);
	}
	if (is_redir_in(redir_op))
		redir->i_redir = 1;
	else if (is_redir_heredoc(redir_op))
	{
		redir->i_redir = 2;
		redir->hd_expand = 1;
	}
	else if (is_redir_out(redir_op))
		redir->o_redir = 1;
	else if (is_redir_append(redir_op))
		redir->o_redir = 2;
	redir->next = NULL;
	return (redir);
}

static void	add_redir_to_cmd(t_cmd *cmd, t_redir *new_redir)
{
	t_redir	*current;

	if (!cmd->redirs)
	{
		cmd->redirs = new_redir;
		return ;
	}
	current = cmd->redirs;
	while (current->next)
		current = current->next;
	current->next = new_redir;
}

static t_token	*create_token_node(const char *raw, int is_squote, int is_dquote)
{
	t_token	*token;

	token = (t_token *)malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->raw = ft_strdup(raw);
	if (!token->raw)
	{
		free(token);
		return (NULL);
	}
	token->is_squote = is_squote;
	token->is_dquote = is_dquote;
	token->next = NULL;
	return (token);
}

static void	add_token_to_cmd(t_cmd *cmd, t_token *new_token)
{
	t_token	*current;

	if (!cmd->tokn)
	{
		cmd->tokn = new_token;
		return ;
	}
	current = cmd->tokn;
	while (current->next)
		current = current->next;
	current->next = new_token;
}

static char	**tokens_to_array(t_token *tokens)
{
	char	**array;
	t_token	*current;
	int		count;
	int		i;

	count = 0;
	current = tokens;
	while (current)
	{
		count++;
		current = current->next;
	}
	array = (char **)malloc(sizeof(char *) * (count + 1));
	if (!array)
		return (NULL);
	current = tokens;
	i = 0;
	while (current)
	{
		array[i] = ft_strdup(current->raw);
		if (!array[i])
		{
			free_dblptr(array);
			return (NULL);
		}
		current = current->next;
		i++;
	}
	array[count] = NULL;
	return (array);
}

static int	process_redirections(t_cmd *cmd)
{
	t_token	*current;
	t_token	*next;
	t_token	*prev;
	t_redir	*redir;

	if (!cmd || !cmd->tokn)
		return (0);
	current = cmd->tokn;
	prev = NULL;
	while (current)
	{
		next = current->next;
		if (is_redirection(current->raw))
		{
			if (!next)
			{
				ft_putstr_fd(ERR_RDI, STDERR_FILENO);
				return (1);
			}
			redir = create_redir(next->raw, current->raw);
			if (!redir)
			{
				malloc_error();
				return (1);
			}
			add_redir_to_cmd(cmd, redir);
			if (prev)
				prev->next = next->next;
			else
				cmd->tokn = next->next;
			free(current->raw);
			free(current);
			free(next->raw);
			free(next);
			current = prev;
			if (current)
				current = current->next;
			else
				current = cmd->tokn;
			continue ;
		}
		prev = current;
		current = next;
	}
	return (0);
}

static int	create_cmd_from_token_range(char **tokens, int start, int end, t_cmd **cmd_list)
{
	t_cmd	*new_cmd;
	t_token	*token_node;
	int		i;

	if (end <= start)
		return (0);
	new_cmd = (t_cmd *)malloc(sizeof(t_cmd));
	if (!new_cmd)
		return (1);
	ft_bzero(new_cmd, sizeof(t_cmd));
	i = start;
	while (i < end)
	{
		token_node = create_token_node(tokens[i], 0, 0);
		if (!token_node)
		{
			free_cmds(new_cmd);
			return (1);
		}
		add_token_to_cmd(new_cmd, token_node);
		i++;
	}
	if (!*cmd_list)
		*cmd_list = new_cmd;
	else
	{
		t_cmd *current = *cmd_list;
		while (current->next)
			current = current->next;
		current->next = new_cmd;
	}
	return (0);
}

static int	split_pipes(t_mini *mini)
{
	char	**tokens;
	t_cmd	*cmd_list;
	int		i;
	int		start;

	if (!mini->cmds || !mini->cmds->tokens)
		return (0);
	tokens = mini->cmds->tokens;
	mini->cmds->tokens = NULL;
	free_cmds(mini->cmds);
	mini->cmds = NULL;
	cmd_list = NULL;
	start = 0;
	i = 0;
	while (tokens[i])
	{
		if (is_pipe(tokens[i]))
		{
			if (create_cmd_from_token_range(tokens, start, i, &cmd_list))
			{
				free_dblptr(tokens);
				free_cmds(cmd_list);
				return (1);
			}
			start = i + 1;
		}
		i++;
	}
	if (create_cmd_from_token_range(tokens, start, i, &cmd_list))
	{
		free_dblptr(tokens);
		free_cmds(cmd_list);
		return (1);
	}
	mini->cmds = cmd_list;
	free_dblptr(tokens);
	return (0);
}

static int	finalize_commands(t_mini *mini)
{
	t_cmd	*current;

	current = mini->cmds;
	while (current)
	{
		if (current->tokn)
		{
			current->tokens = tokens_to_array(current->tokn);
			if (!current->tokens)
				return (1);
		}
		current = current->next;
	}
	return (0);
}

int	lexer(t_mini *mini)
{
	t_cmd	*current;

	if (!mini || !mini->cmds)
		return (0);
	if (split_pipes(mini))
		return (1);
	current = mini->cmds;
	while (current)
	{
		if (process_redirections(current))
			return (1);
		current = current->next;
	}
	if (finalize_commands(mini))
		return (1);
	return (0);
}
