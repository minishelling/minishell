#include "../../../include/minishell.h"

/**
 * @brief Advances the position past spaces or words.
 * 
 * This function increments the position within the string as long as the 
 * current character matches the token type.
 * 
 * @param str The input string being processed.
 * @param pos Pointer to the current position in the string.
 * @param token_id Pointer to the current token identifier.
 */
void	advance_pos_space_or_word(char *str, size_t *pos, t_token_id *token_id)
{
	while (str[*pos] && *token_id == get_token_id(str[*pos]))
		(*pos)++;
}

/**
 * @brief Advances the position past a pipe (|) or double pipe (||).
 * 
 * This function increments the position within the string while counting up to
 * two consecutive pipe characters (|).
 * 
 * @param str The input string being processed.
 * @param pos Pointer to the current position in the string.
 * @param token_id Unused parameter.
 */
void	advance_pos_pipe(char *str, size_t *pos, t_token_id *token_id)
{
	int	pipe_count;

	(void)token_id;
	pipe_count = 0;
	while (pipe_count < 2 && str[*pos] && str[*pos] == '|')
	{
		(*pos)++;
		pipe_count++;
	}
}

/**
 * @brief Advances the position past an AND operator (&&).
 * 
 * This function increments the position within the string if it encounters the
 * logical AND operator (&&) or a single ampersand (&).
 * 
 * @param str The input string being processed.
 * @param pos Pointer to the current position in the string.
 * @param token_id Unused parameter.
 */
void	advance_pos_and_operator(char *str, size_t *pos, t_token_id *token_id)
{
	(void) token_id;
	if (str[*pos] == '&' && str[*pos + 1] && str[*pos +1] == '&')
		(*pos) += 2;
	else
		(*pos)++;
}

/**
 * @brief Advances the position past a parenthesis.
 * 
 * This function increments the position within the string when it encounters 
 * an opening or closing parenthesis.
 * 
 * @param str The input string being processed.
 * @param pos Pointer to the current position in the string.
 * @param token_id Unused parameter.
 */
void	advance_pos_parens(char *str, size_t *pos, t_token_id *token_id)
{
	char	c;

	(void) token_id;
	c = str[*pos];
	if (str[*pos] == c)
		(*pos)++;
}

/**
 * @brief Advances the position past a redirection operator (< or >).
 * 
 * This function increments the position within the string while counting up to
 * two consecutive redirection characters (< or >).
 * 
 * @param str The input string being processed.
 * @param pos Pointer to the current position in the string.
 * @param token_id Unused parameter.
 */
void	advance_pos_redir(char *str, size_t *pos, t_token_id *token_id)
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
}

/**
 * @brief Advances the position to the end of a quoted string.
 * 
 * This function increments the position within the string until it encounters 
 * the matching closing quote, as determined by the token_id.
 * 
 * @param str The input string being processed.
 * @param pos Pointer to the current position in the string.
 * @param token_id Pointer to the current token identifier.
 */
void	advance_pos_quote(char *str, size_t *pos, t_token_id *token_id)
{
	(*pos)++;
	while (str[*pos] && *token_id != get_token_id(str[*pos]))
		(*pos)++;
	if (str[*pos])
		(*pos)++;
}

/**
 * @brief Advances the position past an environment variable ($VAR).
 * 
 * This function increments the position within the string, handling special 
 * cases like `$?` and checking for valid environment variable names.
 * 
 * @param str The input string being processed.
 * @param pos Pointer to the current position in the string.
 * @param token_id Pointer to the current token identifier.
 */
void	advance_pos_env_var(char *str, size_t *pos, t_token_id *token_id)
{
	(void) token_id;
	(*pos)++;
	if (str[*pos] == '?')
	{
		(*pos)++;
		return ;
	}
	if (str[*pos] == '\0')
	{
		*token_id = WORD;
		return ;
	}
	if ((str[*pos] && !ft_isalpha(str[*pos]) && str[*pos] != '_'))
		return ;
	(*pos)++;
	while (str[*pos] && (ft_isalnum(str[*pos]) || str[*pos] == '_'))
		(*pos)++;
}
