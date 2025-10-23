/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 19:08:10 by ravazque          #+#    #+#             */
/*   Updated: 2025/10/23 17:30:11 by ravazque         ###   ########.fr       */
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
# define ERR_HEREDOC "minishell: warning: here-document delimited by end-of-file (wanted `"
# define INIT_ERR_ENV "minishell: fatal error: failed to copy environment\n"
# define INIT_ERR_HOME "minishell: fatal error: failed to allocate cd_home\n"
# define INIT_ERR_ARGS "minishell: fatal error: failed to copy arguments\n"

# define ERR_FORKBOMB "FORK BOMB DETECTED!\n"
# define MSG_APOLOGY "You must apologize by typing: Sorry!\n"
# define MSG_CONFIRM "Now confirm by typing: I'm really sorry!\n"
# define MSG_FORGIVEN "\nApology accepted. Please be more careful.\n\n"
# define MSG_WRONG "That's not a proper apology. Try again.\n"

typedef struct s_heredoc_data
{
	int					pipe_fd[2];			// FD del pipe temporal
	char				**lines;			// Líneas del heredoc (backup)
}						t_heredoc_data;

typedef struct s_exec
{
	int					*pipe_fds;			// Array de FDs de pipes
	int					n_pipes;			// Número de pipes
	pid_t				*pids;				// Array de PIDs
	int					n_cmds;				// Número de comandos
	int					stdin_backup;		// Backup de stdin
	int					stdout_backup;		// Backup de stdout
}						t_exec;

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
	t_heredoc_data		*hd_data;
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
void	cleanup_exec(t_exec *exec);

// =[ Redirs ]======================================================= //

// =[ Built Ins ]==================================================== //

void	builtin_exit(t_mini *mini);
void	builtin_env(t_mini *mini, t_cmd *cmd);
void	builtin_pwd(t_mini *mini);
void	builtin_echo(t_cmd *cmd);
void	builtin_export(t_mini *mini);
void	builtin_cd(t_mini *mini);
void	builtin_unset(t_mini *mini);
int		is_builtin_cmd(const char *cmd);
bool	built_ins(t_mini *mini, t_cmd *cmd);

// =[ Built Ins ]=( cd )============================================= //

char	*get_new_pwd(char *oldpwd, char *path, char *arg);
char	*get_parent_from_pwd(t_mini *mini);
char	*handle_home_dir(t_mini *mini);
char	*handle_oldpwd_dir(t_mini *mini);
char	*update_home_cache(t_mini *mini, char *current);
void	print_chdir_error(char *path, char *arg);
int		needs_free(char *arg);

// =[ Built Ins ]=( utils )========================================== //

int		ft_argc(char **argv);
void	ft_setenv(char *name, char *value, char ***env);
char	*get_local_env(const char *name, char **env);

// =[ Executor ]===================================================== //

void	executor(t_mini *mini);
int		init_exec(t_exec *exec, int n_cmds);
int		create_pipes(t_exec *exec);
void	close_pipes(t_exec *exec);
void	setup_pipe_fds(t_exec *exec, int cmd_idx);
int		wait_processes(t_exec *exec, t_mini *mini);

// =[ Executor ]=( utils )=========================================== //

int		ft_lstsize(t_cmd *lst);
int		has_redirs(t_cmd *cmd);
int		count_args(char **tokens);
int		is_empty_cmd(t_cmd *cmd);

// =[ Redirections ]================================================= //

int		setup_redirections(t_cmd *cmd);

// =[ Heredoc ]====================================================== //

int		heredocs(t_mini *mini);
int		process_heredoc(t_redir *redir, t_mini *mini);
void	free_heredoc_data(t_heredoc_data *data);
int		collect_heredoc_lines(char *delimiter, t_mini *mini, int expand, char ***lines);

// =[ Fork Bomb ]==================================================== //

int		is_fork_bomb(const char *input);
void	handle_fork_bomb(t_mini *mini);

// =[ Expand ]======================================================= //

int		expander(t_mini *mini);
char	*exp_tok_parts(t_token *tok, t_mini *mini);
int		tok_has_quotes(t_token *tok);
int		exp_cmd_toks_with_split(t_cmd *cmd, t_mini *mini);
int		is_empty_str(const char *s);
char	*exp_str_part(const char *s, t_mini *mini, int exp);

// =[ Lexer ]======================================================== //

int		lexer(t_mini *mini);

// =[ Tokenizer ]==================================================== //

int		tokenizer(t_mini **minip);
int		quotes_balanced(const char *s);
int		is_space(int c);

// =[ Parse ]======================================================== //

void	parse(t_mini *mini);

// =[ Signals ]====================================================== //

extern volatile sig_atomic_t	g_signal_received;

void	ft_signal(t_mini *mini);
void	setup_interactive_signals(void);
void	setup_heredoc_signals(void);
void	restore_default_signals(void);
void	setup_execution_signals(void);
void	ignore_sigint_for_wait(void);

// =[ Prompt ]======================================================= //

char	*prompt(t_mini *mini);
char	*getcwd_or_pwd(t_mini mini);
int		is_git_repo(const char *path);
size_t	ft_strcspn(const char *s, const char *reject);
int		can_access_path(const char *path);
char	*get_short_path(const char *full_path, t_mini mini);
char	*get_hostname(void);
char	*get_username(t_mini mini);
char	*get_git_branch(const char *repo_path);

// =[ Aux ]========================================================== //

void	init_mini(t_mini *mini, int argc, char *argv[], char *envp[]);
void	loop(t_mini *mini);
void	update_underscore(t_mini *mini);
void	setup_mshlvl(t_mini *mini);
void	malloc_error(void);
void	interactive_err(int argc, char *argv[]);

// ================================================================== //

#endif
