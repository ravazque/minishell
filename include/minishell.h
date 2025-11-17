/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 19:08:10 by ravazque          #+#    #+#             */
/*   Updated: 2025/11/17 18:29:24 by ravazque         ###   ########.fr       */
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

# define ERR_HEREDOC "here-document delimited by end-of-file (wanted `"

# define INIT_ERR_VARS "failed to allocate local_vars\n"
# define INIT_ERR_ENV "minishell: fatal error: failed to copy environment\n"
# define INIT_ERR_HOME "minishell: fatal error: failed to allocate cd_home\n"
# define INIT_ERR_ARG_COPY "minishell: fatal error: failed to copy arguments\n"
# define INIT_ERR_ALLOC "minishell: fatal error: failed to alloc arguments\n"
# define INIT_ERR_MSHLVL "minishell: fatal error: failed to allocate MSHLVL\n"

# define ERR_FORKBOMB "FORK BOMB DETECTED!\n"
# define MSG_APOLOGY "You must apologize by typing: Sorry!\n"
# define MSG_CONFIRM "Now confirm by typing: I'm really sorry!\n"
# define MSG_FORGIVEN "\nApology accepted. Please be more careful.\n\n"
# define MSG_WRONG "That's not a proper apology. Try again.\n"

typedef struct s_heredoc_data
{
	int					pipe_fd[2];
	char				**lines;
}						t_heredoc_data;

typedef struct s_exec
{
	int					*pipe_fds;
	int					n_pipes;
	pid_t				*pids;
	int					n_cmds;
	int					stdin_backup;
	int					stdout_backup;
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
	int					is_assignment;
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
	char				**local_vars;
	int					exit_sts;
	int					apology_mode;
	char				*cd_home;
	t_cmd				*cmds;
}						t_mini;

typedef struct s_export
{
	char				***env;
	char				***local_vars;
}						t_export;

typedef struct s_heredoc
{
	char				*delimiter;
	t_mini				*mini;
	int					expand;
	char				***lines;
	int					stdin_backup;
}						t_heredoc;

typedef struct s_normi_ctx
{
	t_mini				*mini;
	int					exp;
	char				*bf;
	t_token_part		*tp;
	t_cmd				*cmd;
	int					in_tok;
}						t_normi_ctx;

// =[ Cleaner ]============================================================= //

void					free_dblptr(char **dblptr);
void					free_redirs(t_redir *r);
void					free_cmds(t_cmd *cmd);
void					cleanup_mini(t_mini *mini);
void					free_args(t_mini *mini);
void					free_token_parts(t_token_part *parts);
void					cleanup_exec(t_exec *exec);
void					free_heredoc_data(t_heredoc_data *data);
void					free_token_list(t_token *tokens);

// =[ Built Ins ]=========================================================== //

void					builtin_exit(t_mini *mini);
void					builtin_env(t_mini *mini, t_cmd *cmd);
void					builtin_pwd(t_mini *mini);
void					builtin_echo(t_cmd *cmd);
void					builtin_export(t_mini *mini);
void					builtin_cd(t_mini *mini);
void					builtin_unset(t_mini *mini);
int						is_builtin_cmd(const char *cmd);
bool					built_ins(t_mini *mini, t_cmd *cmd);

// =[ Built Ins ]=( cd )==================================================== //

char					*get_new_pwd(char *oldpwd, char *path, char *arg);
char					*get_parent_from_pwd(t_mini *mini);
char					*handle_home_dir(t_mini *mini);
char					*handle_oldpwd_dir(t_mini *mini);
char					*update_home_cache(t_mini *mini, char *current);
char					*get_home_cached(t_mini *mini);
void					print_chdir_error(char *path, char *arg);
int						needs_free(char *arg);

// =[ Built Ins ]=( utils )================================================= //

int						ft_argc(char **argv);
void					ft_setenv(char *name, char *value, char ***env);
char					*get_local_env(const char *name, char **env);
char					*get_var_value(const char *name, t_mini *mini);
void					set_local_var(char *name, char *value, t_mini *mini);
void					print_env(char **env);
char					*get_command_path(char *cmd, char **envp);
void					fork_and_exec(char **tokens, char **envp, t_mini *mini);
void					execute_env_command(t_mini *mini, t_cmd *cmd);

// =[ Built Ins ]=( export )================================================ //

char					**ft_sort_env(char **env);
char					**ft_split2(char *s, char c);
int						var_exists_with_value(char *name, char **env);
void					ft_putexport(char ***env);
void					ft_setexport(char *arg, t_export *ctx, int flag);
int						validate_export_arg(char *token);

// =[ Executor ]============================================================ //

void					executor(t_mini *mini);
int						init_exec(t_exec *exec, int n_cmds);
int						create_pipes(t_exec *exec);
void					close_pipes(t_exec *exec);
void					setup_pipe_fds(t_exec *exec, int cmd_idx);
int						wait_processes(t_exec *exec, t_mini *mini);
char					**build_exec_env(t_mini *mini);
int						is_directory(const char *path);
int						check_empty_command(t_mini *mini);
char					*ft_get_path(char *cmd, char **envp);
void					print_exec_error(char *cmd, int error_type, int is_path);					// insert a line break to pass norminette - [ 42 ]
void					handle_no_path(char **argv, char *cd_home);
void					ft_execve(char **argv, char **envp, char ***env_ptr, char *cd_home);		// insert a line break to pass norminette - [ 42 ]
void					execute_child_process(t_mini *mini, t_cmd *cmd, t_exec *exec, int idx);		// insert a line break to pass norminette - [ 42 ]
int						execute_single_command(t_mini *mini, t_cmd *cmd);
int						ft_lstsize(t_cmd *lst);
int						has_redirs(t_cmd *cmd);
int						count_args(char **tokens);
int						is_empty_cmd(t_cmd *cmd);
void					cleanup_child_fds(void);

// =[ Redirections ]======================================================== //

int						redirections(t_cmd *cmd);

// =[ Heredoc ]============================================================= //

int						heredocs(t_mini *mini);
int						process_heredoc(t_redir *redir, t_mini *mini);
int						collect_lines(char *delim, t_mini *mini, int expand, char ***lines);		// insert a line break to pass norminette - [ 42 ]
void					restore_stdin(int stdin_backup);
int						handle_eof(char *delimiter);
int						handle_line_error(t_heredoc *ctx);
int						handle_interrupt(t_heredoc *ctx);

// =[ Fork Bomb ]=========================================================== //

int						is_fork_bomb(const char *input);
void					fork_bomb(t_mini *mini);
void					print_step_message(int step);
void					print_wrong_answer(void);
void					correct_message(void);
void					fork_bomb_signal(t_mini *mini);

// =[ Expand ]============================================================== //

int						expander(t_mini *mini);
char					*exp_tok_parts(t_token *tok, t_mini *mini);
int						tok_has_quotes(t_token *tok);
int						exp_cmd_toks_with_split(t_cmd *cmd, t_mini *mini);
int						is_empty_str(const char *s);
char					*exp_str_part(const char *s, t_mini *mini, int exp);
char					*expand_var(const char *var, t_mini *mini);
int						is_valid_var_chr(char c);
char					*extract_var(const char *str, int start, int *end);
char					*str_cat(char *dst, const char *src);
char					*str_cat_chr(char *dst, char c);
int						exp_redirs(t_cmd *cmd, t_mini *mini);
int						count_words(const char *str);
void					free_split_result(char **result, int count);
int						needs_word_split(t_token *tok);
char					*extract_word(const char *str, int *start);
char					**split_by_spaces(const char *str);
int						count_expanded_tokens(t_token *tokens, t_mini *mini);
int						add_expanded_tokens(char **result, int *j, t_token *tok, t_mini *mini);		// insert a line break to pass norminette - [ 42 ]
char					*expand_part(t_token_part *curr, t_mini *mini);
char					*exp_parts_list(t_token *tok, t_mini *mini);
void					skip_spaces(const char *str, int *start);
int						get_word_len(const char *str, int start);
char					*alloc_and_copy(const char *str, int start, int len);
char					*process_tilde_expansion(int *i, t_mini *mini);
int						should_expand_tilde(const char *s, int i, int exp);
char					*process_char_exp(char *res, const char *s, int *i, t_normi_ctx *ctx);		// insert a line break to pass norminette - [ 42 ]

// =[ Lexer ]=============================================================== //

int						lexer(t_mini *mini);
int						tok_has_any_quotes(t_token *tok);
int						is_pipe(const char *str, t_token *tok);
int						is_redir(const char *str, t_token *tok);
int						validate_redir_target(t_token *tgt_tok);
t_redir					*mk_redir(const char *tgt, const char *op, t_token *tgt_tok);				// insert a line break to pass norminette - [ 42 ]
void					add_redir(t_cmd *cmd, t_redir *new);
int						proc_redirs(t_cmd *cmd);
t_token					*mk_tok_node(const char *raw, int sq, int dq);
void					add_tok(t_cmd *cmd, t_token *new);
char					**toks_to_arr(t_token *toks);
void					add_cmd_to_lst(t_cmd **lst, t_cmd *new);
int						count_tokens_until_pipe(t_token *start);
t_token					*skip_to_next_pipe(t_token *start);
int						validate_pipe_syntax(t_token *tokens);
t_cmd					*create_cmd_from_tokens(t_token *start, int count);
int						split_by_pipes(t_mini *mini);

// =[ Tokenizer ]=========================================================== //

int						tokenizer(t_mini **minip);
int						is_space(int c);
int						is_assignment(const char *str);
int						process_assignments(t_mini *mini);
void					mark_assignments(t_cmd *cmd);
t_token_part			*mk_tok_part(const char *c, int sq, int dq);
void					add_tok_part(t_token_part **list, t_token_part *new);
int						flush_part(char **bf, int sq, int dq, t_token_part **parts);				// insert a line break to pass norminette - [ 42 ]
char					*join_parts(t_token_part *parts);
t_token					*mk_tok_from_parts(t_token_part *parts);
int						append_char(char **dst, char c);
int						read_quoted(const char *in, size_t *i, char q, char **bf);					// insert a line break to pass norminette - [ 42 ]
int						is_operator_char(char c);
void					cleanup_tok_data(char **bf, t_token_part **p, t_cmd **cmd);					// insert a line break to pass norminette - [ 42 ]
int						add_tok_to_cmd(t_token *new, t_cmd *cmd);
int						finalize_tok(char **bf, t_token_part **tp, t_cmd *cmd);
int						handle_operator(const char *in, size_t *i, t_normi_ctx *ctx);				// insert a line break to pass norminette - [ 42 ]
int						handle_space(const char *in, size_t *i, t_normi_ctx *ctx);					// insert a line break to pass norminette - [ 42 ]
int						handle_quote(const char *in, size_t *i, t_normi_ctx *ctx);					// insert a line break to pass norminette - [ 42 ]
int						handle_regular(const char *in, size_t *i, t_normi_ctx *ctx);				// insert a line break to pass norminette - [ 42 ]

// =[ Parse ]=============================================================== //

void					parse(t_mini *mini);
void					cleanup_parse_error(t_mini *mini);
int						is_empty_or_whitespace(const char *str);
int						start_tokenizer(t_mini *mini);
int						start_lexer(t_mini *mini);
int						start_assignments(t_mini *mini);
int						start_expander(t_mini *mini);

// =[ Signals ]============================================================= //

extern volatile sig_atomic_t	g_signal_received;

void					ft_signal(t_mini *mini);
void					setup_interactive_signals(void);
void					setup_heredoc_signals(void);
void					restore_default_signals(void);
void					setup_execution_signals(void);
void					ignore_sigint_for_wait(void);

// =[ Prompt ]============================================================== //

char					*prompt(t_mini *mini);
char					*getcwd_or_pwd(t_mini mini);
int						is_git_repo(const char *path);
size_t					ft_strcspn(const char *s, const char *reject);
int						can_access_path(const char *path);
char					*get_short_path(const char *full_path, t_mini mini);
char					*get_hostname(void);
char					*get_username(t_mini mini);
char					*get_git_branch(const char *repo_path);
char					*build_user_host(t_mini mini);
char					*build_path_section(const char *pwd, t_mini mini);
char					*build_git_section(const char *branch);
char					*join_all_parts(char *user_host, char *path, char *git);

// =[ Aux ]================================================================= //

void					init_mini(t_mini *mini, int argc, char *argv[], char *envp[]);				// insert a line break to pass norminette - [ 42 ]
void					loop(t_mini *mini);
void					update_underscore(t_mini *mini);
void					setup_mshlvl(t_mini *mini);
void					malloc_error(void);
void					interactive_err(int argc, char *argv[]);
void					ensure_path(t_mini *mini);

// ========================================================================= //

#endif
