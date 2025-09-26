// /* ************************************************************************** */
// /*                                                                            */
// /*                                                        :::      ::::::::   */
// /*   print_aux.c                                        :+:      :+:    :+:   */
// /*                                                    +:+ +:+         +:+     */
// /*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
// /*                                                +#+#+#+#+#+   +#+           */
// /*   Created: 2025/09/24 13:45:02 by ravazque          #+#    #+#             */
// /*   Updated: 2025/09/24 16:19:04 by ravazque         ###   ########.fr       */
// /*                                                                            */
// /* ************************************************************************** */

#include "../include/minishell.h"

void	print_tokens(t_mini *mini)
{
	int	i;

	i = 0;
	while (mini->cmds->tokens[i] != NULL)
	{
		printf("args[%d] = ( %s )\n", i, mini->cmds->tokens[i]);
		i++;
	}
	// (void)mini;
	// ft_putstr_fd("Error: command not found", STDERR_FILENO);
}

// static void	print_separator(const char *title)
// {
// 	int	i;
// 	int	len;
// 	int	padding;

// 	len = ft_strlen(title);
// 	padding = (60 - len - 2) / 2;
// 	printf("\n");
// 	i = 0;
// 	while (i < 60)
// 	{
// 		printf("=");
// 		i++;
// 	}
// 	printf("\n");
// 	i = 0;
// 	while (i < padding)
// 	{
// 		printf(" ");
// 		i++;
// 	}
// 	printf("[%s]", title);
// 	i = 0;
// 	while (i < (60 - len - 2 - padding))
// 	{
// 		printf(" ");
// 		i++;
// 	}
// 	printf("\n");
// 	i = 0;
// 	while (i < 60)
// 	{
// 		printf("=");
// 		i++;
// 	}
// 	printf("\n");
// }

// static void	print_input_analysis(t_mini *mini)
// {
// 	int		i;
// 	char	c;

// 	print_separator("INPUT ANALYSIS");
// 	printf("Original input: \"%s\"\n", mini->input);
// 	printf("Input length: %d\n", (int)ft_strlen(mini->input));
// 	printf("Character by character analysis:\n");
// 	i = 0;
// 	while (mini->input[i])
// 	{
// 		c = mini->input[i];
// 		if (c == '\'')
// 			printf("  [%d]: '%c' (SINGLE QUOTE)\n", i, c);
// 		else if (c == '"')
// 			printf("  [%d]: '%c' (DOUBLE QUOTE)\n", i, c);
// 		else if (c == '$')
// 			printf("  [%d]: '%c' (DOLLAR SIGN)\n", i, c);
// 		else if (c == ' ')
// 			printf("  [%d]: ' ' (SPACE)\n", i);
// 		else if (c == '\t')
// 			printf("  [%d]: '\\t' (TAB)\n", i);
// 		else
// 			printf("  [%d]: '%c'\n", i, c);
// 		i++;
// 	}
// }

// static void	print_raw_tokens_array(char **tokens)
// {
// 	int	i;

// 	print_separator("RAW TOKENS ARRAY");
// 	if (!tokens)
// 	{
// 		printf("tokens array is NULL\n");
// 		return ;
// 	}
// 	i = 0;
// 	while (tokens[i])
// 	{
// 		printf("  tokens[%d] = \"%s\"\n", i, tokens[i]);
// 		i++;
// 	}
// 	printf("Total tokens in array: %d\n", i);
// }

// static void	print_token_linked_list(t_token *tokn)
// {
// 	t_token	*current;
// 	int		count;

// 	print_separator("TOKEN LINKED LIST");
// 	if (!tokn)
// 	{
// 		printf("Token linked list is NULL\n");
// 		return ;
// 	}
// 	current = tokn;
// 	count = 0;
// 	while (current)
// 	{
// 		printf("  Token[%d]:\n", count);
// 		printf("    raw: \"%s\"\n", current->raw);
// 		printf("    is_squote: %s\n", current->is_squote ? "YES" : "NO");
// 		printf("    is_dquote: %s\n", current->is_dquote ? "YES" : "NO");
// 		printf("    next: %s\n", current->next ? "exists" : "NULL");
// 		printf("    ------------\n");
// 		current = current->next;
// 		count++;
// 	}
// 	printf("Total tokens in linked list: %d\n", count);
// }

// static void	print_redirections_detailed(t_redir *redirs)
// {
// 	t_redir	*current;
// 	int		count;

// 	print_separator("REDIRECTIONS ANALYSIS");
// 	if (!redirs)
// 	{
// 		printf("No redirections found\n");
// 		return ;
// 	}
// 	current = redirs;
// 	count = 0;
// 	while (current)
// 	{
// 		printf("  Redirection[%d]:\n", count);
// 		printf("    target: \"%s\"\n", current->target);
// 		printf("    i_redir: %d ", current->i_redir);
// 		if (current->i_redir == 0)
// 			printf("(no input redirection)\n");
// 		else if (current->i_redir == 1)
// 			printf("(< input redirection)\n");
// 		else if (current->i_redir == 2)
// 			printf("(<< heredoc)\n");
// 		else
// 			printf("(unknown input type)\n");
// 		printf("    o_redir: %d ", current->o_redir);
// 		if (current->o_redir == 0)
// 			printf("(no output redirection)\n");
// 		else if (current->o_redir == 1)
// 			printf("(> output redirection)\n");
// 		else if (current->o_redir == 2)
// 			printf("(>> append redirection)\n");
// 		else
// 			printf("(unknown output type)\n");
// 		printf("    hd_expand: %s\n", current->hd_expand ? "YES" : "NO");
// 		printf("    next: %s\n", current->next ? "exists" : "NULL");
// 		printf("    ------------\n");
// 		current = current->next;
// 		count++;
// 	}
// }

// static void	print_command_detailed(t_cmd *cmd, int cmd_num)
// {
// 	char	title[50];

// 	sprintf(title, "COMMAND %d DETAILED ANALYSIS", cmd_num);
// 	print_separator(title);
	
// 	printf("Command structure address: %p\n", (void*)cmd);
// 	printf("Has tokens array: %s\n", cmd->tokens ? "YES" : "NO");
// 	printf("Has token linked list: %s\n", cmd->tokn ? "YES" : "NO");
// 	printf("Has redirections: %s\n", cmd->redirs ? "YES" : "NO");
// 	printf("Has next command: %s\n", cmd->next ? "YES" : "NO");
	
// 	if (cmd->tokens)
// 		print_raw_tokens_array(cmd->tokens);
	
// 	if (cmd->tokn)
// 		print_token_linked_list(cmd->tokn);
	
// 	if (cmd->redirs)
// 		print_redirections_detailed(cmd->redirs);
// }

// static void	print_pipeline_overview(t_mini *mini)
// {
// 	t_cmd	*current;
// 	int		total_commands;

// 	print_separator("PIPELINE OVERVIEW");
// 	if (!mini || !mini->cmds)
// 	{
// 		printf("No commands in pipeline\n");
// 		return ;
// 	}
	
// 	total_commands = 0;
// 	current = mini->cmds;
// 	while (current)
// 	{
// 		total_commands++;
// 		current = current->next;
// 	}
	
// 	printf("Total commands in pipeline: %d\n", total_commands);
// 	printf("Exit status: %d\n", mini->exit_sts);
// 	printf("Mini structure address: %p\n", (void*)mini);
// 	printf("First command address: %p\n", (void*)mini->cmds);
// }

// static void	print_expansion_debug(t_mini *mini)
// {
// 	t_cmd	*current;
// 	t_token	*token;
// 	int		cmd_count;
// 	int		token_count;

// 	print_separator("EXPANSION DEBUG INFO");
// 	current = mini->cmds;
// 	cmd_count = 1;
// 	while (current)
// 	{
// 		printf("Command %d expansion analysis:\n", cmd_count);
// 		if (current->tokn)
// 		{
// 			token = current->tokn;
// 			token_count = 0;
// 			while (token)
// 			{
// 				printf("  Token[%d] BEFORE expansion:\n", token_count);
// 				printf("    raw: \"%s\"\n", token->raw);
// 				printf("    is_squote: %s\n", token->is_squote ? "YES" : "NO");
// 				printf("    is_dquote: %s\n", token->is_dquote ? "YES" : "NO");
// 				printf("    should_expand: %s\n", !token->is_squote ? "YES" : "NO");
// 				token = token->next;
// 				token_count++;
// 			}
// 		}
// 		if (current->tokens)
// 		{
// 			printf("  Tokens array AFTER expansion:\n");
// 			token_count = 0;
// 			while (current->tokens[token_count])
// 			{
// 				printf("    tokens[%d]: \"%s\"\n", token_count, current->tokens[token_count]);
// 				token_count++;
// 			}
// 		}
// 		current = current->next;
// 		cmd_count++;
// 	}
// }

// void	print_tokens(t_mini *mini)
// {
// 	t_cmd	*current;
// 	int		cmd_count;

// 	if (!mini)
// 	{
// 		printf("ERROR: mini is NULL\n");
// 		return ;
// 	}
	
// 	print_input_analysis(mini);
// 	print_pipeline_overview(mini);
	
// 	if (!mini->cmds)
// 	{
// 		printf("No commands to display!\n");
// 		return ;
// 	}
	
// 	print_expansion_debug(mini);
	
// 	current = mini->cmds;
// 	cmd_count = 1;
// 	while (current)
// 	{
// 		print_command_detailed(current, cmd_count);
// 		current = current->next;
// 		cmd_count++;
// 	}
	
// 	print_separator("END OF DETAILED ANALYSIS");
// }
