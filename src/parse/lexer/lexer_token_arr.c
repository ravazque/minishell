/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_token_arr.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 00:00:00 by ravazque          #+#    #+#             */
/*   Updated: 2025/11/11 00:00:00 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

static int	count_non_assignments(t_token *toks)
{
	t_token	*curr;
	int		cnt;

	cnt = 0;
	curr = toks;
	while (curr)
	{
		if (!curr->is_assignment)
			cnt++;
		curr = curr->next;
	}
	return (cnt);
}

static void	free_arr_on_error(char **arr, int i)
{
	while (i > 0)
	{
		i--;
		free(arr[i]);
	}
	free(arr);
}

static int	copy_token_strings(char **arr, t_token *toks)
{
	t_token	*curr;
	int		i;

	curr = toks;
	i = 0;
	while (curr)
	{
		if (!curr->is_assignment)
		{
			arr[i] = ft_strdup(curr->raw);
			if (!arr[i])
			{
				free_arr_on_error(arr, i);
				return (malloc_error(), -1);
			}
			i++;
		}
		curr = curr->next;
	}
	return (i);
}

char	**toks_to_arr(t_token *toks)
{
	char	**arr;
	int		cnt;
	int		result;

	cnt = count_non_assignments(toks);
	arr = (char **)malloc(sizeof(char *) * (cnt + 1));
	if (!arr)
		return (malloc_error(), NULL);
	result = copy_token_strings(arr, toks);
	if (result == -1)
		return (NULL);
	arr[cnt] = NULL;
	return (arr);
}
