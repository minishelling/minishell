#include "../../include/minishell.h"

char	*copy_chars(char *token_str, char *expanded_str)
{
	size_t	len;
	char	*temp_str;
	char	*new_expanded_str;

	len = ft_strcspn(token_str, "\'\"$ ");
	temp_str = ft_substr(token_str, 0, len);
	if (!temp_str)
		return (NULL);
	new_expanded_str = ft_strjoin(expanded_str, temp_str);
	free(temp_str);
	if (!new_expanded_str)
		return (NULL);
	free(expanded_str);
	return (new_expanded_str);
}

char	*process_dquotes(t_shell *shell, char **str, char *expanded_str, t_env *env_list)
{
	char	temp[2];
	char	*new_expanded_str;

	while (**str && **str != '"')
	{
		if (**str == '$' && ft_strncmp(*str, "$\"", 2))
		{
			expanded_str = handle_env_var_sign(shell, str, expanded_str, env_list);
			if (!expanded_str)
				return (NULL);
		}
		else
		{
			temp[0] = **str;
			temp[1] = '\0';
			new_expanded_str = ft_strjoin(expanded_str, temp);
			free(expanded_str);
			if (!new_expanded_str)
				return (NULL);
			expanded_str = new_expanded_str;
			(*str)++;
		}
	}
		(*str)++;
	return (expanded_str);
}

char	*process_squotes(char **str, char *expanded_str)
{
	char	*end_quote;
	char	*temp_str;
	char	*new_expanded_str;

	end_quote = ft_strchr(*str, '\'');
	if (end_quote)
	{
		temp_str = ft_substr(*str, 0, end_quote - *str);
		if (!temp_str)
			return (NULL);
		new_expanded_str = ft_strjoin(expanded_str, temp_str);
		free(temp_str);
		if (!new_expanded_str)
			return (NULL);
		free(expanded_str);
		expanded_str = new_expanded_str;
		*str = end_quote + 1;
	}
	return (expanded_str);
}

char	*decide_expansion_type(t_shell *shell, char **str, char *expanded_str, t_env *env_list)
{
	if (**str == '\'')
	{
		(*str)++;
		expanded_str = process_squotes(str, expanded_str);
	}
	else if (**str == '\"')
	{
		(*str)++;
		expanded_str = process_dquotes(shell, str, expanded_str, env_list);
	}
	else if (**str == '$')
		expanded_str = handle_env_var_sign(shell, str, expanded_str, env_list);
	else
	{
		expanded_str = copy_chars(*str, expanded_str);
		*str += ft_strcspn(*str, "\'\"$ ");
	}
	return (expanded_str);
}

int	expand_token_str(t_shell *shell, t_token *token, t_env *env_list)
{
	char *original_str;
	char *expanded_str;

	expanded_str = ft_strdup("");
	if (!expanded_str)
		return (ERR_MEM);
	original_str = token->str;
	while (*original_str)
	{
		expanded_str = decide_expansion_type(shell, &original_str, expanded_str, env_list);
		if (!expanded_str)
			return (ERR_MEM);
	}
	safe_assign_str(&token->str, expanded_str);
	free(expanded_str);
	if (!token->str)
		return (ERR_MEM);
	return (PARSING_OK);
}


t_token	*expand(t_shell *shell, t_token *start_token, t_token *end_token, t_env *env_list)
{
	t_token *current_token;

	current_token = start_token;
	while (current_token)
	{
		expand_token_str(shell, current_token, env_list);
		if (current_token == end_token)
			break;
		current_token = current_token->next;
	}
	return (start_token);
}