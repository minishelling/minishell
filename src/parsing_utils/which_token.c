#include "../include/minishell.h"

/**
 * @brief Check if the token is a double quote.
 *
 * This function determines if the given token is classified as a
 * double quote based on its token ID.
 *
 * @param token A pointer to the token to check.
 * @return true If the token is a double quote.
 * @return false If the token is not a double quote.
 */
bool is_dquote(t_token *token)
{
	return (token && token->id == DQUOTE);
}

/**
 * @brief Check if the token is a single quote.
 *
 * This function determines if the given token is classified as a
 * single quote based on its token ID.
 *
 * @param token A pointer to the token to check.
 * @return true If the token is a single quote.
 * @return false If the token is not a single quote.
 */
bool is_squote(t_token *token)
{
	return (token && token->id == SQUOTE);
}

/**
 * @brief Check if the token is a word.
 *
 * This function determines if the given token is classified as a word
 * based on its token ID.
 *
 * @param token A pointer to the token to check.
 * @return true If the token is a word.
 * @return false If the token is not a word.
 */
bool is_word(t_token *token)
{
	return (token && token->id == WORD);
}

/**
 * @brief Check if the token is an environment variable.
 *
 * This function determines if the given token is classified as an
 * environment variable based on its token ID.
 *
 * @param token A pointer to the token to check.
 * @return true If the token is an environment variable.
 * @return false If the token is not an environment variable.
 */
bool is_env_var(t_token *token)
{
	return (token && token->id == ENV_VAR);
}