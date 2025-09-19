/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 19:08:10 by ravazque          #+#    #+#             */
/*   Updated: 2025/09/19 06:46:01 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../src/aux_libft/include/libft.h"
# include "colors.h"

# define ERR_C "Error: Argument is not -c.\n"
# define ERR_QUO "Error: syntax error\n"
# define ERR_TKN "Error: syntax error near unexpected token `"
# define ERR_C_ARG "Error: interactive mode only for now\n"

# define QPAD '\x01'

typedef struct s_token	t_token;
typedef struct s_redir	t_redir;
typedef struct s_cmd	t_cmd;
typedef struct s_mini	t_mini;

typedef enum e_tltype
{
	TK_WORD,
	TK_PIPE,
	TK_REDIR_IN,
	TK_REDIR_OUT,
	TK_SQUOT,
	TK_DQUOT,
	TK_HEREDOC
}						t_tktype;

typedef enum e_rtype
{
	R_IN,
	R_OUT,
	R_HEREDOC
}						t_rtype;

struct					s_token
{
	char				*raw;
	t_tktype				type;
	int					had_squote;
	int					had_dquote;
	struct s_token		*next;
};

struct					s_redir
{
	t_rtype				type;
	char				*target;
	int					hd_expand;
	struct s_redir		*next;
};

struct					s_cmd
{
	char				**tokens;
	t_redir				*redirs;
	struct s_cmd		*next;
};

struct					s_mini
{
	char				*prompt;
	char				*input;
	char				*pwd;
	char				**env;
	int					argc;
	char				**argv;
	int					exit_sts;
	t_cmd				*cmds;
};

// ====================================================== //

void		free_tokens(t_token *tk);
void		free_redirs(t_redir *r);
void		parse(t_mini *mini);
void		free_cmds(t_cmd *cmd);
bool		built_ins(t_mini *mini);
void		builtin_exit(t_mini *mini);
void		builtin_echo(t_mini mini);
void		builtin_pwd(t_mini *mini);
char		*build_prompt(t_mini *mini);
void		init_mini(t_mini *mini, int argc, char *argv[], char *envp[]);
void		cleanup_mini(t_mini *mini);
void		free_dblptr(char **split);
int			syntax_error(const char *near);
void		interactive_err(int argc, char *argv[]);
void		split_error(void);
void		malloc_error(void);
void		builtin_env(t_mini mini);
char		**ft_copy_dblptr(char **envp);
void		ft_signal(t_mini *mini);
char		*ms_substr(const char *s, size_t start, size_t len);


// ====================================================== //

#endif
