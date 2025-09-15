/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 17:15:38 by ravazque          #+#    #+#             */
/*   Updated: 2025/09/15 21:40:47 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// static int	count_args(char **line)
// {
// 	int	count;

// 	count = 0;
// 	while (line[count] != NULL)
// 		count++;
// 	return (count);
// }

static void double_mark(t_mini *mini, int *start_pos)
{
	char	*phase;
	int		i;
	
	i = start_pos;
	while(mini->input[i] != '"')
		i++;
	phase = malloc(sizeof(char) * i);
	while(start_pos < i)
	{
		phase[*start_pos] = mini->input[*start_pos];
		start_pos++;
	}
	mini->cmds->args = ft_strjoin(mini->cmds->args, phase);
}

static void *simple_mark(t_mini *mini, int *start_pos)
{
	char	*phase;
	int		i;
	
	i = start_pos;
	while(mini->input[i] != '\'')
		i++;
	phase = malloc(sizeof(char) * i);
	while(start_pos < i)
	{
		phase[*start_pos] = mini->input[*start_pos];
		start_pos++;
	}
	mini->cmds->args = ft_strjoin(mini->cmds->args, phase);
}

void	parse(t_mini *mini)
{
	int		pos_check;

	if (!mini->cmds)
	{
		mini->cmds = malloc(sizeof(t_cmd));
		if (!mini->cmds)
		{
			malloc_error();
			return ;
		}
	}
	pos_check = 0;
	while(pos_check < ft_strlen(mini->input))
	{
		if(mini->input[pos_check] == '"')
			simple_mark(mini, &pos_check + 1);
		if(mini->input[pos_check] == '\'')
			double_mark(mini, &pos_check + 1);
		pos_check++;
	}
}
