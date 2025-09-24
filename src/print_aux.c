/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_aux.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 13:45:02 by ravazque          #+#    #+#             */
/*   Updated: 2025/09/24 13:45:51 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static void	print_redirection_info(t_redir *redirs)
{
	t_redir	*current;
	int		count;

	current = redirs;
	count = 1;
	while (current)
	{
		printf("    Redir[%d]:\n", count);
		printf("      - Target: \"%s\"\n", current->target);
		if (current->i_redir == 1)
			printf("      - Type: Input redirection (<)\n");
		else if (current->i_redir == 2)
			printf("      - Type: Heredoc (<<)\n");
		else if (current->o_redir == 1)
			printf("      - Type: Output redirection (>)\n");
		else if (current->o_redir == 2)
			printf("      - Type: Append redirection (>>)\n");
		printf("      - Heredoc expand: %s\n", current->hd_expand ? "Yes" : "No");
		current = current->next;
		count++;
	}
}

static void	print_tokens_info(char **tokens)
{
	int	i;

	if (!tokens)
	{
		printf("    No tokens available\n");
		return ;
	}
	i = 0;
	while (tokens[i])
	{
		printf("    Token[%d]: \"%s\"\n", i, tokens[i]);
		i++;
	}
	printf("    Total tokens: %d\n", i);
}

static void	print_command_stats(t_cmd *cmd, int cmd_num)
{
	int	token_count;

	printf("\n=== COMMAND %d ===\n", cmd_num);
	token_count = 0;
	if (cmd->tokens)
	{
		while (cmd->tokens[token_count])
			token_count++;
	}
	printf("  Command info:\n");
	printf("    - Token count: %d\n", token_count);
	printf("    - Has redirections: %s\n", cmd->redirs ? "Yes" : "No");
	printf("    - Has next command: %s\n", cmd->next ? "Yes" : "No");
	printf("  Tokens:\n");
	print_tokens_info(cmd->tokens);
	if (cmd->redirs)
	{
		printf("  Redirections:\n");
		print_redirection_info(cmd->redirs);
	}
	else
		printf("  Redirections: None\n");
	printf("\n");
}

void	print_tokens(t_mini *mini)
{
	t_cmd	*current;
	int		cmd_count;
	int		total_commands;

	if (!mini || !mini->cmds)
	{
		printf("minishell: No commands to display!\n");
		return ;
	}
	total_commands = 0;
	current = mini->cmds;
	while (current)
	{
		total_commands++;
		current = current->next;
	}
	current = mini->cmds;
	cmd_count = 1;
	while (current)
	{
		print_command_stats(current, cmd_count);
		current = current->next;
		cmd_count++;
	}
}
