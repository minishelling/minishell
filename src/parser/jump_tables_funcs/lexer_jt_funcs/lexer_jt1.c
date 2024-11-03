/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_jt1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfeuer <tfeuer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 13:25:01 by tfeuer            #+#    #+#             */
/*   Updated: 2024/10/31 13:25:06 by tfeuer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../include/minishell.h"

t_ecode_p	advance_pos_space_or_word(char *str, size_t *pos, \
	t_token_id *token_id);
t_ecode_p	advance_pos_pipe(char *str, size_t *pos, t_token_id *token_id);
t_ecode_p	advance_pos_and_operator(char *str, size_t *pos, \
	t_token_id *token_id);
t_ecode_p	advance_pos_parens(char *str, size_t *pos, t_token_id *token_id);
t_ecode_p	advance_pos_redir(char *str, size_t *pos, t_token_id *token_id);

/**
 * @brief Advances the position through space or word tokens.
 *
 * This function iterates through the input string until it encounters a
 * character that does not match the current token type.
 *
 * @param str The input string.
 * @param pos A pointer to the current position in the string.
 * @param token_id The current token ID being processed.
 * @return Returns a status indicating success.
 */
t_ecode_p	advance_pos_space_or_word(char *str, size_t *pos, \
	t_token_id *token_id)
{
	while (str[*pos] && *token_id == get_token_id(str[*pos]))
		(*pos)++;
	return (PARSING_OK);
}

/**
 * @brief Advances the position through pipe tokens.
 *
 * This function counts and advances the position for up to two pipe tokens.
 *
 * @param str The input string.
 * @param pos A pointer to the current position in the string.
 * @param token_id The current token ID being processed (unused).
 * @return Returns a status indicating success.
 */
t_ecode_p	advance_pos_pipe(char *str, size_t *pos, t_token_id *token_id)
{
	t_ecode_p	pipe_count;

	(void)token_id;
	pipe_count = 0;
	while (pipe_count < 2 && str[*pos] && str[*pos] == '|')
	{
		(*pos)++;
		pipe_count++;
	}
	return (PARSING_OK);
}

/**
 * @brief Advances the position through AND operator tokens.
 *
 * Checks for the "&&" operator and advances the position accordingly.
 *
 * @param str The input string.
 * @param pos A pointer to the current position in the string.
 * @param token_id The current token ID being processed (unused).
 * @return Returns a status indicating success.
 */
t_ecode_p	advance_pos_and_operator(char *str, size_t *pos, \
	t_token_id *token_id)
{
	(void) token_id;
	if (str[*pos] == '&' && str[*pos + 1] && str[*pos +1] == '&')
		(*pos) += 2;
	else
		(*pos)++;
	return (PARSING_OK);
}

/**
 * @brief Advances the position through parentheses.
 *
 * Checks for opening or closing parentheses and advances the position.
 *
 * @param str The input string.
 * @param pos A pointer to the current position in the string.
 * @param token_id The current token ID being processed (unused).
 * @return Returns a status indicating success.
 */
t_ecode_p	advance_pos_parens(char *str, size_t *pos, t_token_id *token_id)
{
	char	c;

	(void) token_id;
	c = str[*pos];
	if (str[*pos] == c)
		(*pos)++;
	return (PARSING_OK);
}

/**
 * @brief Advances the position through redirection tokens.
 *
 * Counts and advances the position for up to two redirection tokens.
 *
 * @param str The input string.
 * @param pos A pointer to the current position in the string.
 * @param token_id The current token ID being processed (unused).
 * @return Returns a status indicating success.
 */
t_ecode_p	advance_pos_redir(char *str, size_t *pos, t_token_id *token_id)
{
	int		redir_count;
	char	c;

	(void)token_id;
	c = str[*pos];
	redir_count = 0;
	while (redir_count < 2 && str[*pos] && str[*pos] == c)
	{
		(*pos)++;
		redir_count++;
	}
	return (PARSING_OK);
}
