#include "../../../../include/minishell.h"

t_ecode_p	advance_pos_quote(char *str, size_t *pos, t_token_id *token_id);
t_ecode_p	advance_pos_env_var(char *str, size_t *pos, t_token_id *token_id);

/**
 * @brief Advances the position through quoted strings.
 *
 * This function skips the initial quote and continues until it finds
 * the matching closing quote, advancing the position in the process.
 *
 * @param str The input string.
 * @param pos A pointer to the current position in the string.
 * @param token_id The current token ID being processed.
 * @return Returns a status indicating success.
 */
t_ecode_p	advance_pos_quote(char *str, size_t *pos, t_token_id *token_id)
{
	(*pos)++;
	while (str[*pos] && *token_id != get_token_id(str[*pos]))
		(*pos)++;
	if (str[*pos])
		(*pos)++;
	return (PARSING_OK);
}

/**
 * @brief Advances the position through environment variable tokens.
 *
 * This function handles the special case of environment variables, including
 * the `$?` status and checking for valid variable names.
 *
 * @param str The input string.
 * @param pos A pointer to the current position in the string.
 * @param token_id The current token ID being processed (unused).
 * @return Returns a status indicating success or an error code.
 */
t_ecode_p	advance_pos_env_var(char *str, size_t *pos, t_token_id *token_id)
{
	(void) token_id;
	(*pos)++;
	if (str[*pos] == '?')
	{
		(*pos)++;
		return (PARSING_OK);
	}
	if (str[*pos] == '\0')
	{
		*token_id = WORD;
		return (PARSING_OK);
	}
	if (str[*pos] == '(')
		return (ERR_CMD_SUBSTIT);
	if ((!ft_isalpha(str[*pos]) && str[*pos] != '_'))
		return (PARSING_OK);
	(*pos)++;
	while (str[*pos] && (ft_isalnum(str[*pos]) || str[*pos] == '_'))
		(*pos)++;
	return (PARSING_OK);
}
