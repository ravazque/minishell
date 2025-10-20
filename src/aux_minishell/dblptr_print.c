/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dblptr_print.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 00:00:00 by ravazque          #+#    #+#             */
/*   Updated: 2025/10/20 17:34:07 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void    print_dblptr(char **dblptr)
{
    int i;

    if (!dblptr)
    {
        printf("(null)\n");
        return ;
    }

    i = 0;
    while (dblptr[i])
    {
        printf("[%d]: %s\n", i, dblptr[i]);
        i++;
    }
}
