#include "../../include/minishell.h"

/**
 * @brief Calculates the length of the initial segment of `str` that 
 *        consists entirely of characters not found in `reject`.
 *
 * This function iterates through the string `str` and counts the number
 * of characters until a character from the `reject` string is found. 
 *
 * @param str The string to be scanned.
 * @param reject The string containing the characters to reject.
 * @return The number of characters in the initial segment of `str` 
 *         that are not in `reject`.
 */
size_t ft_strcspn(const char *str, const char *reject) 
{
	size_t count;

	count = 0;
	while (str[count] != '\0')
	{
		if (strchr(reject, str[count]))
			break;
		count++;
	}
	return (count);
}

/**
 * @brief Expands a variable in the given string and appends its value 
 *        to the expanded string.
 *
 * This function searches for a variable starting with '$' in the input 
 * string, retrieves its value from the environment variable list, 
 * and appends it to the expanded string.
 *
 * @param str A pointer to the string containing the variable.
 * @param expanded_str The string to which the variable's value will be appended.
 * @param env_list A pointer to the list of environment variables.
 * @return A pointer to the newly expanded string.
 */
char *expand_variable(char **str, char *expanded_str, t_env *env_list)
{
	char *var_end;
	char *var_name;
	char *env_value;
	char *var_start;
	char *temp_str;
	
	if (ft_strncmp(*str, "$", 1) == 0 && ft_strlen(*str) == 1)
	{
		expanded_str = ft_strdup ("$");
		(*str)++;
	}
	else if (ft_strncmp(*str, "$?", 2) == 0 && ft_strlen(*str) == 2)
	{
		expanded_str = ft_itoa(g_exitcode);
		(*str) += 2;
	}
	else
	{
		var_start = *str;
		var_end = var_start + 1; 
		while (ft_isalnum(*var_end) || *var_end == '_')
			var_end++;
		var_name = ft_substr(var_start + 1, 0, var_end - var_start - 1);  //check
		env_value = get_env_value_from_key(env_list, var_name); 
		free(var_name); 
		if (env_value)
		{
			temp_str = ft_strjoin(expanded_str, env_value);  //check
			free(expanded_str);
			expanded_str = temp_str;
		}
		*str = var_end;
	}
	return (expanded_str);
}

/**
 * @brief Processes a string enclosed in double quotes, expanding variables
 *        and handling normal characters.
 *
 * This function iterates through the string, expanding any variables found 
 * while appending normal characters to the expanded string. It stops 
 * processing when it encounters the closing double quote.
 *
 * @param str A pointer to the string containing the double-quoted section.
 * @param expanded_str The string to which expanded variables and characters
 *        will be appended.
 * @param env_list A pointer to the list of environment variables.
 * @return A pointer to the newly expanded string.
 */
char *process_double_quotes(char **str, char *expanded_str, t_env *env_list)
{
	char temp[2];
	char *new_expanded_str;

	while (**str && **str != '"')
	{
		if (**str == '$')
			expanded_str = expand_variable(str, expanded_str, env_list);
		else
		{
			temp[0] = **str;
			temp[1] = '\0';
			new_expanded_str = ft_strjoin(expanded_str, temp);
			free(expanded_str);
			expanded_str = new_expanded_str;
			(*str)++;
		}
	}
	if (**str == '"') (*str)++;
	return expanded_str;
}

/**
 * @brief Processes a string enclosed in single quotes, appending the content
 *        to the expanded string without expanding variables.
 *
 * This function finds the closing single quote in the input string and 
 * appends the content within the quotes to the expanded string. 
 * Variables inside single quotes are treated as literal strings and are not 
 * expanded.
 *
 * @param str A pointer to the string containing the single-quoted section.
 * @param expanded_str The string to which the content of the single-quoted 
 *        section will be appended.
 * @return A pointer to the newly expanded string.
 */
char *process_single_quotes(char **str, char *expanded_str)
{
	char *end_quote;
	char *temp_str;
	char *new_expanded_str;

	end_quote = ft_strchr(*str, '\'');
	if (end_quote)
	{
		temp_str = ft_substr(*str, 0, end_quote - *str);
		new_expanded_str = ft_strjoin(expanded_str, temp_str);
		free(temp_str);
		free(expanded_str);
		expanded_str = new_expanded_str;
		*str = end_quote + 1;
	}
	return expanded_str;
}

/**
 * @brief Copies a segment of characters from the token string until a special 
 *        character is encountered and appends it to the expanded string.
 *
 * This function extracts characters from the `token_str` until it encounters 
 * one of the special characters: single quote, double quote, dollar sign, or 
 * space. The extracted segment is then joined with the existing 
 * `expanded_str`.
 *
 * @param token_str The string from which characters are copied.
 * @param expanded_str The string to which the copied characters will be 
 *        appended.
 * @return A pointer to the newly expanded string.
 */
char *copy_chars(char *token_str, char *expanded_str)
{
	size_t len;
	char *temp_str;
	char *new_expanded_str;

	len = ft_strcspn(token_str, "\'\"$ ");
	temp_str = ft_substr(token_str, 0, len);   //check
	new_expanded_str = ft_strjoin(expanded_str, temp_str);  //check
	free(temp_str);
	free(expanded_str);
	
	//check 
	return (new_expanded_str);
}

/**
 * @brief Expands the string in the given token by processing single quotes, 
 *        double quotes, and environment variables.
 *
 * This function modifies the `token->str` field to contain the expanded 
 * string based on the characters and special symbols encountered in it. 
 * It handles single quotes, double quotes, and variable expansion, 
 * concatenating the results into a single string.
 *
 * @param token A pointer to the token containing the string to be expanded.
 * @param env_list A pointer to the environment variable list for variable 
 *        expansion.
 */
void expand_str(t_token *token, t_env *env_list)
{
	char *expanded_str = ft_strdup("");

	while (*(token->str))
	{
		if (*(token->str) == '\'')
		{
			token->str++;
			expanded_str = process_single_quotes(&(token->str), expanded_str);
		}
		else if (*(token->str) == '\"')
		{
			token->str++;
			expanded_str = process_double_quotes(&(token->str), expanded_str, env_list);
		}
		else if (*(token->str) == '$')
			expanded_str = expand_variable(&(token->str), expanded_str, env_list);
		else
		{
			expanded_str = copy_chars(token->str, expanded_str);
			token->str += ft_strcspn(token->str, "\'\"$ ");
		}
	}
	token->str = expanded_str;
}

/**
 * @brief Expands a list of tokens from the starting token to the ending token.
 *
 * This function iterates through the tokens starting from `start_token`
 * and applies the `expand_str` function to each token until it reaches
 * the `end_token`. The `expand_str` function processes each token's string 
 * to handle variable expansion and quotes.
 *
 * @param start_token A pointer to the first token in the list to be expanded.
 * @param end_token A pointer to the last token in the list to be expanded.
 * @param env_list A pointer to the environment variable list for variable 
 *        expansion.
 * @return A pointer to the starting token after expansion.
 */
t_token *expand(t_token *start_token, t_token *end_token, t_env *env_list)
{
	t_token *current_token;

	current_token = start_token;
	while (current_token)
	{
		expand_str(current_token, env_list);

		if (current_token == end_token)
			break;
		current_token = current_token->next;
	}
	return (start_token);
}

