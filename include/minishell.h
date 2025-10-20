/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 19:08:10 by ravazque          #+#    #+#             */
/*   Updated: 2025/10/20 12:57:44 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../src/aux_libft/include/libft.h"
# include "colors.h"

# define ERR_C "minishell: error: Argument is not -c.\n"
# define ERR_QUO "minishell: error: syntax error\n"
# define ERR_C_ARG "minishell: error: -c: option requires an argument\n"
# define ERR_VAR "minishell: expansion error\n"
# define ERR_RDI "minishell: syntax error near unexpected token `newline'\n"
# define ERR_OLDPWD "minishell: cd: OLDPWD not set\n"
# define ERR_HOME "minishell: cd: HOME not set\n"

# define ERR_FORKBOMB "FORK BOMB DETECTED!\n"
# define MSG_APOLOGY "You must apologize by typing: Sorry!\n"
# define MSG_CONFIRM "Now confirm by typing: I'm really sorry!\n"
# define MSG_FORGIVEN "\nApology accepted. Please be more careful.\n\n"
# define MSG_WRONG "That's not a proper apology. Try again.\n"

typedef enum e_token_type
{
	TOKEN_WORD = 0,
	TOKEN_PIPE,
	TOKEN_REDIR_IN,
	TOKEN_REDIR_OUT,
	TOKEN_REDIR_APPEND,
	TOKEN_REDIR_HEREDOC
}						t_token_type;

typedef struct s_token_part
{
	char				*content;
	int					is_squote;
	int					is_dquote;
	struct s_token_part	*next;
}						t_token_part;

typedef struct s_token
{
	char				*raw;
	int					is_squote;
	int					is_dquote;
	t_token_part		*parts;
	struct s_token		*next;
}						t_token;

typedef struct s_redir
{
	char				*target;
	int					hd_expand;
	int					in_redir;
	int					out_redir;
	int					fd;
	struct s_redir		*next;
}						t_redir;

typedef struct s_cmd
{
	t_redir				*redirs;
	t_token				*tokn;
	char				**tokens;
	struct s_cmd		*next;
}						t_cmd;

typedef struct s_mini
{
	char				*prompt;
	char				*input;
	char				*pwd;
	int					argc;
	char				**argv;
	char				**env;
	int					exit_sts;
	int					apology_mode;	
	char				*cd_home;	
	t_cmd				*cmds;
}						t_mini;

// =[ Cleaner ]====================================================== //

void	free_dblptr(char **dblptr);
void	free_redirs(t_redir *r);
void	free_cmds(t_cmd *cmd);
void	cleanup_mini(t_mini *mini);
void	free_args(t_mini *mini);
void	free_token_parts(t_token_part *parts);

// =[ Built Ins ]==================================================== //

void	builtin_exit(t_mini *mini);
void	builtin_env(t_mini *mini);
void	builtin_pwd(t_mini *mini);
void	builtin_cd(t_mini *mini);
void	builtin_echo(t_mini *mini);
void	builtin_export(t_mini *mini);
void	builtin_unset(t_mini *mini);
bool	built_ins(t_mini *mini);

int		ft_argc(char **argv);
void	ft_setenv(char *name, char *value, char ***env);
int		ft_envlen(char **env);
char	*get_local_env(const char *name, char **env);
void	update_underscore(t_mini *mini, t_cmd *cmd);

// =[ Executor ]===================================================== //

int		ft_lstsize(t_cmd *lst);
int		has_redirs(t_cmd *cmd);
int		count_args(char **tokens);
int		is_empty_cmd(t_cmd *cmd);

void	executor(t_mini *mini);

// =[ Parse ]======================================================== //

void	parse(t_mini *mini);

// =[ Tokenizer ]==================================================== //

int		tokenizer(t_mini **minip);
int		quotes_balanced(const char *s);
int		is_space(int c);
char	**dbpt_push(char ***dst, const char *s);

// =[ Lexer ]======================================================== //

int		lexer(t_mini *mini);

// =[ Expand ]======================================================= //

int		expander(t_mini *mini);
char	*exp_tok_parts(t_token *tok, t_mini *mini);
int		tok_has_quotes(t_token *tok);
int		exp_cmd_toks_with_split(t_cmd *cmd, t_mini *mini);
int		is_empty_str(const char *s);

// =[ Redirs ]======================================================= //

// =[ Error ]======================================================== //

void	malloc_error(void);
void	interactive_err(int argc, char *argv[]);

// =[ Signals ]====================================================== //

void	ft_signal(t_mini *mini);
void	setup_interactive_signals(void);
void	restore_default_signals(void);
void	setup_execution_signals(void);

// =[ Propmt ]======================================================= //

char	*prompt(t_mini *mini);
char	*getcwd_or_pwd(void);
int		is_git_repo(const char *path);

// =[ Fork Bomb ]==================================================== //

int		is_fork_bomb(const char *input);
void	handle_fork_bomb(t_mini *mini);

// =[ Prompt ]======================================================= //

size_t	ft_strcspn(const char *s, const char *reject);
int		can_access_path(const char *path);
char	*get_short_path(const char *full_path);
char	*get_hostname(void);
char	*get_username(void);
char	*get_git_branch(const char *repo_path);

// =[ Aux ]========================================================== //

void	init_mini(t_mini *mini, int argc, char *argv[], char *envp[]);
void	loop(t_mini *mini);

// ================================================================== //

#endif
