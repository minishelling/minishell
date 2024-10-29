#include "../../include/minishell.h"

void	expand(t_shell *shell, t_token *start_token, t_token *end_token, \
		t_env *env_list);
int		expand_token_str(t_shell *shell, t_token *token, t_env *env_list);
void	process_squotes(char **str, char **expanded_str);
void	process_dquotes(t_shell *shell, char **str, char **expanded_str, \
	t_env *env_list);
void	copy_chars(char **str, char **expanded_str);

void	expand(t_shell *shell, t_token *start_token, t_token *end_token, \
		t_env *env_list)
{
	int		err_no;
	t_token	*current_token;

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

int	expand_token_str(t_shell *shell, t_token *token, t_env *env_list)
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

void	process_squotes(char **str, char **expanded_str)
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

void	process_dquotes(t_shell *shell, char **str, char **expanded_str, \
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

void	copy_chars(char **str, char **expanded_str)
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
