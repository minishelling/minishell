#include "../../include/minishell.h"

size_t ft_strcspn(const char *str, const char *reject) 
{
    size_t count;

	count = 0;
    // Iterate through the string str
    while (str[count] != '\0') 
	{
        // Check if the current character is in the `reject` string
        if (strchr(reject, str[count])) 
            break; // Found a character to reject, exit the loop
        count++; // Increment count if the character is not in `reject`
    }
    return (count); // Return the length of the initial segment
}

char *expand_variable(char **str, char *expanded_str, t_env *env_list)
{
	char	*var_end;
	char	*var_name;
	char	*env_value;
    char	*var_start; // Variable starts at the current '$'
    char	*temp_str;
    
	var_start = *str;
    // Move past the '$' and find the end of the variable name
    var_end = var_start + 1; 
    while (ft_isalnum(*var_end) || *var_end == '_')
        var_end++; // Find the end of the variable name

    // Extract the variable name
    var_name = ft_substr(var_start + 1, 0, var_end - var_start - 1);
    
    // Get the environment variable's value
    env_value = get_env_value_from_key(env_list, var_name); 
    free(var_name); // Free the variable name
    // Add the environment variable's value to expanded_str
    if (env_value)
    {
        temp_str = ft_strjoin(expanded_str, env_value);
        free(expanded_str);
        expanded_str = temp_str;
    }
    // Update *str to point to the character after the variable
    *str = var_end;
    return (expanded_str);
}


char *process_double_quotes(char **str, char *expanded_str, t_env *env_list)
{
	char temp[2];
	char *new_expanded_str;
    while (**str && **str != '"') // While not at the end of the string or another quote
    {
        if (**str == '$') // Found a variable
        {
            expanded_str = expand_variable(str, expanded_str, env_list); // Pass 'str' (char**)
        }
        else
        {
            // Handle normal characters inside quotes
            temp[0] = **str; // Copy the current character
            temp[1] = '\0';  // Null-terminate the temporary string, create a temporary string of one char
            new_expanded_str = ft_strjoin(expanded_str, temp);
            free(expanded_str);
            expanded_str = new_expanded_str;
            (*str)++; // Move to next character
        }
    }
    if (**str == '"') (*str)++; // Move past the closing quote
    return expanded_str;
}


char *process_single_quotes(char **str, char *expanded_str)
{
	char	*end_quote;
	char	*temp_str;
    char	*new_expanded_str;
	
    end_quote = ft_strchr(*str, '\''); // Find closing quote
    if (end_quote)
    {
        temp_str = ft_substr(*str, 0, end_quote - *str);
        new_expanded_str = ft_strjoin(expanded_str, temp_str);
        free(temp_str);
        free(expanded_str);
        expanded_str = new_expanded_str;
        *str = end_quote + 1; // Move past the closing quote
    }
    return expanded_str;
}

char *copy_chars(char *token_str, char *expanded_str)
{
	size_t	len;
	char	*temp_str;
	char	*new_expanded_str;
	
    len = ft_strcspn(token_str, "\'\"$ "); // Get the length until a special character
   	temp_str = ft_substr(token_str, 0, len); // Extract that part of the string
    new_expanded_str = ft_strjoin(expanded_str, temp_str); // Join with expanded_str
    free(temp_str); // Free the temporary string
    free(expanded_str); // Free the old expanded string
    return (new_expanded_str); // Return the new expanded string
}

void expand_str(t_token *token, t_env *env_list)
{
    char *expanded_str = ft_strdup(""); // Start with an empty string to concatenate to

    while (*token->str) // While there is something to process in the token string
    {
        if (*token->str == '\'') // Found a single quote
        {
			//printf ("found single quotes\n");
            token->str++; // Move past the opening quote
            expanded_str = process_single_quotes(&(token->str), expanded_str);
        }
        else if (*token->str == '\"') // Found a double quote
        {
			//printf ("found double quotes\n");
            token->str++; // Move past the opening quote
            expanded_str = process_double_quotes(&(token->str), expanded_str, env_list);
        }
        else if (*token->str == '$') // Found a dollar sign for variable expansion
        {
			//printf ("found $\n");
            expanded_str = expand_variable(&(token->str), expanded_str, env_list); // Pass address of token->str
        }
        else
        {
			//printf ("didnt find, copying\n");
            expanded_str = copy_chars(token->str, expanded_str); // Pass token->str to copy_chars
            token->str += ft_strcspn(token->str, "\'\"$ "); // Move token->str forward by the length we copied
        }
    }

    token->str = expanded_str; // Assign the new expanded string to token->str
    //printf("The expanded str is %s\n", expanded_str);
}

t_token	*expand(t_token *start_token, t_token *end_token, t_env *env_list)
{
	t_token	*current_token;
	
	//printf ("begining of expand\n");
	//print_token(start_token);
	current_token = start_token;
	while (current_token)
	{
		//printf("current token is %s\n", current_token->str);
		expand_str(current_token, env_list);

		if (current_token == end_token)
			break;
		current_token = current_token->next;
}
	//free_token_list(token_list_head);
	//printf("new token list\n");
	//print_token (start_token);
	return (start_token);
}
