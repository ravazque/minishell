/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 19:08:10 by ravazque          #+#    #+#             */
/*   Updated: 2025/09/18 17:19:15 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../src/auxlibft/include/libft.h"
# include "colors.h"

# define QPAD '\x01'
# define ERR_C "Error: Argument is not -c.\n"
# define ERR_QUO "Error: syntax error\n"

typedef struct s_cmd
{
	char			**args;
	struct s_cmd	*next;
}					t_cmd;

typedef struct s_mini
{
	t_cmd			*cmds;
	char			*input;
	char			*prompt;
	int				i;
	int				exit_sts;
}					t_mini;

// ====================================================== //

void	parse(t_mini *mini);
void	init_mini(t_mini *mini);
char	*ms_make_prompt(void);

// ====================================================== //

void	cleanup_mini(t_mini *mini);
void	free_cmds(t_cmd *cmds);
void	free_split(char **split);

// ====================================================== //

void	interactive_err(int argc, char *argv[]);
void	split_error(void);
void	malloc_error(void);

// ====================================================== //

bool	built_ins(t_mini *mini);
void	builtin_exit(t_mini *mini);
void	builtin_echo(t_mini mini);

// ====================================================== //

#endif
