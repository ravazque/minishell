/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   colors.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 01:35:17 by ravazque          #+#    #+#             */
/*   Updated: 2025/09/16 01:43:04 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COLORS_H
# define COLORS_H

# define CLR_RESET "\033[0m"
# define CLR_BOLD "\033[1m"
# define CLR_RED "\033[31m"
# define CLR_GRN "\033[32m"
# define CLR_YEL "\033[33m"
# define CLR_BLU "\033[34m"
# define CLR_MAG "\033[35m"
# define CLR_CYN "\033[36m"
# define CLR_WHT "\033[37m"

# define RL_S "\001"
# define RL_E "\002"

# define RL_RST RL_S CLR_RESET RL_E
# define RL_BLD RL_S CLR_BOLD RL_E
# define RL_RED RL_S CLR_RED RL_E
# define RL_GRN RL_S CLR_GRN RL_E
# define RL_YEL RL_S CLR_YEL RL_E
# define RL_BLU RL_S CLR_BLU RL_E
# define RL_MAG RL_S CLR_MAG RL_E
# define RL_CYN RL_S CLR_CYN RL_E
# define RL_WHT RL_S CLR_WHT RL_E

#endif
