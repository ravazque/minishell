/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 19:08:10 by ravazque          #+#    #+#             */
/*   Updated: 2025/09/15 20:39:52 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../src/auxlibft/include/libft.h"
# include <readline/readline.h>
# include <readline/history.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <stdbool.h>

# define ERR_C "Error: Argument is not -c.\n"

typedef struct s_cmd
{
    char    **args;
}           t_cmd;

typedef struct s_mini
{
    t_cmd   *cmds;
    char    *input;
    int     i;
    int     exit_sts;
}           t_mini;

bool    built_ins(t_mini mini);
int     ft_strcmp_ns(const char *s1, const char *s2);
void    non_interactive(void);
void    parse(t_mini *mini);
void    free_cmds(t_cmd *cmds);
void	free_split(char **split);
void	cleanup_mini(t_mini *mini);
void	init_mini(t_mini *mini);
void	malloc_error(void);
void	split_error(void);

#endif
