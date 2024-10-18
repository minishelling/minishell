#include "../../include/minishell.h"

/**
 * @brief Compares the length of two strings and returns the larger length.
 * 
 * @param s1 String 1.
 * @param s2 String 2.
 * 
 * @return The length of s1 if it is greater or equal to the length of s2.
 * The length of s2 if it is greater than the length of s1.
 */
size_t	max_len(char *s1, char *s2)
{
	size_t	s1_len;
	size_t	s2_len;

	s1_len = ft_strlen(s1);
	s2_len = ft_strlen(s2);
	if (s1_len >= s2_len)
		return (s1_len);
	return (s2_len);
}
