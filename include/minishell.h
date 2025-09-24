/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 19:08:10 by ravazque          #+#    #+#             */
/*   Updated: 2025/09/24 04:44:59 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../src/aux_libft/include/libft.h"
# include "colors.h"

# define ERR_C "Error: Argument is not -c.\n"
# define ERR_QUO "Error: syntax error\n"
# define ERR_C_ARG "Error: interactive mode only for now\n"

typedef struct s_token	t_token;
typedef struct s_redir	t_redir;
typedef struct s_cmd	t_cmd;
typedef struct s_mini	t_mini;

struct					s_token
{
	char				*raw;
	int					is_squote;
	int					is_dquote;
	struct s_token		*next;
};

struct					s_redir
{
	char				*target;
	int					hd_expand;
	int					i_redir;
	int					o_redir;
	struct s_redir		*next;
};

struct					s_cmd
{
	t_redir				*redirs;
	t_token				*tokn;
	char				**tokens;
	struct s_cmd		*next;
};

struct					s_mini
{
	char				*prompt;
	char				*input;
	char				*pwd;
	int					argc;
	char				**argv;
	char				**env;
	int					exit_sts;
	t_cmd				*cmds;
};

// =[ Cleaner ]====================================================== //

void	free_dblptr(char **dblptr);
void	free_tokens(t_token *tk);
void	free_redirs(t_redir *r);
void	free_cmds(t_cmd *cmd);
void	cleanup_mini(t_mini *mini);

// =[ Built Ins ]==================================================== //

void	builtin_exit(t_mini *mini);
void	builtin_env(t_mini mini);
void	builtin_pwd(t_mini *mini);
void	builtin_cd(t_mini mini);
void	builtin_echo(t_mini mini);
bool	built_ins(t_mini *mini);

// =[ Parse ]======================================================== //

void	parse(t_mini *mini);

// =[ Tokenizer ]==================================================== //

int		tokenizer(t_mini **minip);
int		quotes_balanced(const char *s);
int		is_space(int c);
char	**dbpt_push(char ***dst, const char *s);

// =[ Lexer ]======================================================== //

// =[ Expand ]======================================================= //

// =[ Redirs ]======================================================= //

// =[ Error ]======================================================== //

// int		syntax_error(const char *near);
void	split_error(void);
void	malloc_error(void);
void	interactive_err(int argc, char *argv[]);

// =[ Aux ]========================================================== //

char	*build_prompt(t_mini *mini);
void	init_mini(t_mini *mini, int argc, char *argv[], char *envp[]);
char	**ft_copy_dblptr(char **envp);
void	ft_signal(t_mini *mini);

// ================================================================== //

#endif
