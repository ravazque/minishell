/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 18:26:42 by ravazque          #+#    #+#             */
/*   Updated: 2025/10/08 00:54:31 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/libft.h"

char	*ft_strjoin(const char *s1, const char *s2)
{
	size_t	len1;
	size_t	len2;
	size_t	i;
	char	*new_str;

	if (!s2)
		return (NULL);
	len1 = 0;
	if (s1)
		len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	new_str = (char *)malloc(len1 + len2 + 1);
	if (!new_str)
		return (NULL);
	i = 0;
	while (i < len1 && s1)
	{
		new_str[i] = s1[i];
		i++;
	}
	while (i < len1 + len2)
	{
		new_str[i] = s2[i - len1];
		i++;
	}
	new_str[i] = '\0';
	return (new_str);
}

char	*ft_strjoin3(char const *s1, char const *s2, char const *s3)
{
	char	*temp;
	char	*conc;

	if (!s1 && !s2 && !s3)
		return (ft_strdup(""));
	if (!s3)
	{
		if (!s1)
			return (ft_strdup(""));
		return (ft_strdup(s1));
	}
	if (!s1)
		s1 = "";
	if (!s2)
		s2 = "";
	temp = ft_strjoin(s1, s2);
	if (!temp)
		return (NULL);
	conc = ft_strjoin(temp, s3);
	free(temp);
	return (conc);
}
