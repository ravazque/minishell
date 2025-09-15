/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 19:08:10 by ravazque          #+#    #+#             */
/*   Updated: 2025/09/15 17:11:39 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../src/auxlibft/include/libft.h"

# define ERR_C "Error: Argument is not -c.\n"

typedef struct s_cmd
{
	char	*cmd;
	char	**args;
	char	*infile;
	char	*outfile;
}			t_cmd;

typedef struct s_mini
{
	t_cmd	*cmds;
	char	*input;
	int		exit_sts;
}			t_mini;

int			ft_strcmp_ns(const char *s1, const char *s2);
bool		built_ins(t_mini mini);
void		non_interactive(void);

#endif
