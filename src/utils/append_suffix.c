/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   append_suffix.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: lprieri <lprieri@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/10/31 13:33:39 by lprieri       #+#    #+#                 */
/*   Updated: 2024/10/31 13:33:40 by lprieri       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/**
 * @brief Concatenates the 'suffix' string to the string 'str'.
 * 
 * If the string already contains the suffix, and the duplicate
 * option is false, it does not concatenate the suffix.
 * Otherwise it will attempt to concatenate the suffix into the string.
 * 
 * @param str The target string that needs the suffix attached.
 * @param suffix The suffix string.
 * @param duplicate If true, and the string already contains the suffix,
 * it will concatenate the suffix thus duplicating it into the string.
 * 
 * @return If the suffix is NULL, or the string already contained the suffix
 * and the duplicate option was set to false, or if the operation was successful,
 * it returns SUCCESS.
 * On error it prints an error message and returns an error code.
 */
t_ecode	append_suffix(char **str, char *suffix, bool duplicate)
{
	char	*new_str;
	size_t	suffix_len;
	size_t	str_len;

	if (!*str)
		return (NULL_STRING);
	if (!suffix)
		return (SUCCESS);
	suffix_len = ft_strlen(suffix);
	str_len = ft_strlen(*str);
	if (!ft_strncmp(&(*str)[str_len - suffix_len],
		suffix, suffix_len) && !duplicate)
		return (SUCCESS);
	new_str = ft_strdup(*str);
	if (!new_str)
		return (handle_perror("append_suffix"), MALLOC_ERROR);
	ft_free((void **) str);
	new_str = ft_strjoin_fs1(&new_str, suffix);
	if (!new_str)
		return (handle_perror("append_suffix"), MALLOC_ERROR);
	*str = ft_strdup(new_str);
	ft_free((void **) &new_str);
	if (!*str)
		return (handle_perror("append_suffix"), MALLOC_ERROR);
	return (SUCCESS);
}
