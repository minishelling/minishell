#include "../../include/minishell.h"

void				expand(t_shell *shell, t_token *start_token, \
	t_token *end_token, t_env *env_list);
static t_ecode_p	expand_token_str(t_shell *shell, t_token *token, \
	t_env *env_list);
static void			process_squotes(char **str, char **expanded_str);
static void			process_dquotes(t_shell *shell, char **str, \
	char **expanded_str, t_env *env_list);
static void			copy_chars(char **str, char **expanded_str);

/**
 * @brief Expands the tokens between start and end tokens.
 *
 * This function traverses the token list from the specified start token 
 * to the end token, expanding each token's string representation 
 * according to shell expansion rules. It handles errors during 
 * expansion and cleans up resources on failure.
 *
 * @param shell Pointer to the shell structure containing relevant data.
 * @param start_token Pointer to the token where expansion starts.
 * @param end_token Pointer to the token where expansion ends.
 * @param env_list Pointer to the environment variable list for expansion.
 */
void	expand(t_shell *shell, t_token *start_token, t_token *end_token, \
		t_env *env_list)
{
	t_ecode_p	err_no;
	t_token		*current_token;

	current_token = start_token;
	while (current_token)
	{
		err_no = expand_token_str(shell, current_token, env_list);
		if (err_no)
		{
			handle_parsing_err(shell, err_no);
			clean_nicely_and_exit(shell, EXIT_FAILURE);
		}
		if (current_token == end_token)
			break ;
		current_token = current_token->next;
	}
}

/**
 * @brief Expands the string representation of a token.
 *
 * This function processes the string of a given token and performs shell 
 * expansion according to the rules for single quotes, double quotes, 
 * and variable signs ('$'). The expanded string replaces the original 
 * string of the token. Memory management is handled to avoid leaks.
 *
 * @param shell Pointer to the shell structure containing relevant data.
 * @param token Pointer to the token whose string is to be expanded.
 * @param env_list Pointer to the environment variable list for variable expansions.
 * @return The result of the expansion operation, indicating success or failure.
 *         Possible return values:
 *         - PARSING_OK on success.
 *         - ERR_EXPAND if an error occurs during expansion.
 */
static t_ecode_p	expand_token_str(t_shell *shell, t_token *token, \
	t_env *env_list)
{
	char	*original_str;
	char	*expanded_str;

	expanded_str = ft_strdup("");
	if (!expanded_str)
		return (ERR_EXPAND);
	original_str = token->str;
	while (*original_str)
	{
		if (*original_str == '\'')
			process_squotes(&original_str, &expanded_str);
		else if (*original_str == '\"')
			process_dquotes(shell, &original_str, &expanded_str, env_list);
		else if (*original_str == '$')
			handle_var_sign(shell, &original_str, &expanded_str, env_list);
		else
			copy_chars(&original_str, &expanded_str);
		if (!expanded_str)
			return (ERR_EXPAND);
	}
	safe_assign_str(&token->str, expanded_str);
	ft_free((void **) &expanded_str);
	if (!token->str)
		return (ERR_EXPAND);
	return (PARSING_OK);
}

/**
 * @brief Processes a single-quoted section in the input string.
 *
 * This function identifies and processes characters within single quotes 
 * in the input string. It extracts the substring enclosed by the quotes, 
 * appending it to the expanded string. The original input pointer is 
 * updated to point to the character immediately following the closing 
 * quote. Note that within single quotes, the string is taken literally, 
 * meaning no variable expansion occurs.
 *
 * @param str Pointer to the input string, which will be modified to 
 *            skip the processed single-quoted section.
 * @param expanded_str Pointer to the string that accumulates the expanded 
 *                     result, which will be updated with the newly added 
 *                     substring.
 */
static void	process_squotes(char **str, char **expanded_str)
{
	char	*end_quote;
	char	*temp_str;

	(*str)++;
	end_quote = ft_strchr(*str, '\'');
	if (end_quote)
	{
		temp_str = ft_substr(*str, 0, end_quote - *str);
		if (!temp_str)
			ft_free((void **) expanded_str);
		*expanded_str = ft_strjoin_fs1(expanded_str, temp_str);
		free(temp_str);
		*str = end_quote + 1;
	}
}

/**
 * @brief Processes double quotes in a string and expands variables if 
 * applicable.
 *
 * This function traverses a string that is contained within double quotes.
 * It checks for dollar signs (`$`) that indicate variable expansion. 
 * If a dollar sign is followed by something, that something will be given a 
 * chance to expand in handle_var_sign function. However, if a standalone 
 * dollar sign (`$`) appears, indicating that there is no variable name to 
 * expand, it will not trigger variable expansion. In this case, the dollar 
 * sign is copied literally to the expanded string.
 *
 * @param shell Pointer to the shell structure containing the environment 
 * list.
 * @param str Pointer to the current position in the string being processed.
 * @param expanded_str Pointer to the string where expanded results are 
 * stored.
 * @param env_list Pointer to the environment variable list.
 */
static void	process_dquotes(t_shell *shell, char **str, char **expanded_str, \
	t_env *env_list)
{
	char	temp[2];

	(*str)++;
	while (**str && **str != '"')
	{
		if (**str == '$' && (*(*str + 1) != '"' && *(*str + 1) != ' '))
			handle_var_sign(shell, str, expanded_str, env_list);
		else
		{
			temp[0] = **str;
			temp[1] = '\0';
			*expanded_str = ft_strjoin_fs1(expanded_str, temp);
			(*str)++;
		}
	}
	(*str)++;
}

/**
 * @brief Copies characters from the input string until a special character is found.
 *
 * This function scans the input string for characters up to the first 
 * occurrence of a single quote (`'`), double quote (`"`), or dollar sign 
 * (`$`). It then extracts this substring and appends it to the expanded 
 * string. The original input pointer is advanced to skip over the copied 
 * characters.
 *
 * @param str Pointer to the input string being processed. This pointer 
 *            will be updated to reflect the position after the copied 
 *            characters.
 * @param expanded_str Pointer to the string that accumulates the expanded 
 *                     result. This string will be updated with the newly 
 *                     added substring.
 */
static void	copy_chars(char **str, char **expanded_str)
{
	size_t	len;
	char	*temp_str;

	len = ft_strcspn(*str, "\'\"$");
	temp_str = ft_substr(*str, 0, len);
	if (!temp_str)
		ft_free((void **)expanded_str);
	*expanded_str = ft_strjoin_fs1(expanded_str, temp_str);
	free(temp_str);
	*str += ft_strcspn(*str, "\'\"$");
}
