/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork_bomb_detector.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 00:30:00 by ravazque          #+#    #+#             */
/*   Updated: 2025/10/03 04:49:00 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static char	*g_fork_bomb_patterns[] = {
    ":()|:|:&};:",
    ":()|:|&};:",
    ":((){:|:&};:",
    "(){:|:&};:",
    NULL
};

static int	contains_pattern(const char *input, const char *pattern)
{
    int	i;
    int	j;
    int	match;

    if (!input || !pattern)
        return (0);
    i = 0;
    while (input[i])
    {
        j = 0;
        match = 1;
        while (pattern[j])
        {
            if (input[i + j] != pattern[j])
            {
                match = 0;
                break;
            }
            j++;
        }
        if (match && pattern[j] == '\0')
            return (1);
        i++;
    }
    return (0);
}

int	is_fork_bomb(const char *input)
{
    int	i;

    if (!input)
        return (0);
    i = 0;
    while (g_fork_bomb_patterns[i])
    {
        if (contains_pattern(input, g_fork_bomb_patterns[i]))
            return (1);
        i++;
    }
    return (0);
}
