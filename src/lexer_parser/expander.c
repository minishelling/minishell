#include "../../include/minishell.h"

t_token	*free_node(t_token *t_node)
{
	t_token	*t_tmp;

	if (t_node == NULL)
		return (NULL);
	t_tmp = t_node->next;
	if (t_node->str != NULL)
		free(t_node->str);
	t_node->str = NULL;
	free(t_node);
	return (t_tmp);
}


void ft_free_split(char **s)
{
	int	i;

	i = 0;
	while (s[i])
	{
		free(s[i]);
		i++;
	}
	free(s);
}


t_token	*create_expanded_tokens(char *str)
{
	t_token	*expanded_token_list;
	t_token	*token;

	expanded_token_list = NULL;
	token = new_token();
	if (!token)
		return (NULL);
	token->id = WORD;
	token->str = str;
	expanded_token_list = token;
	token = new_token();
	if (!token)
		return (free_token_node(expanded_token_list), NULL);
	token->id = SPACE_CHAR;
	token->str = ft_strdup(" ");
	if (!(token->str))
		return (free_token_node(expanded_token_list),free_token_node(token), NULL);
	expanded_token_list->next = token;
	return (expanded_token_list);
}

t_token	*expand_shell_var(t_token *token, t_env *env_list)
{
	t_token	*return_tokens;
	t_token	*expanded_token;
	char	**split;
	size_t	i;

	//env_var_print_linked_list (env_list);
	// env_print_list (env_list);  // lisandro

	return_tokens = NULL;
	i = 0;

	char *env_value = env_get_value_from_key(env_list, (token->str + 1));
	printf ("in expand_shell_var str is %s\n", env_value);
	
	split = ft_split (env_value, ' ');
	if (!split)
		return (NULL);
	while (split[i] != NULL)
	{
		expanded_token = create_expanded_tokens(split[i]);
		printf ("LIST OF EXPANDED TOKENS ");
		list_token_print(expanded_token);
		// if (!t_node)
		// 	...
		add_token_in_back(&return_tokens, expanded_token);
		i++;
	}
	printf ("i is %zu\n", i);
	free(split);
	if (i == 0)
		return (new_token());
	free_last_token(return_tokens, free_node);
	return (return_tokens);
}

void remove_quotes(t_token *token)
{
	char	*str;
	size_t	len;
	
	len = ft_strlen(token->str);
	str = token->str;
	ft_memmove(token->str, token->str + 1, len);
	ft_memmove(&token->str[len - 2], &token->str[len - 1], 1);
}

bool	is_quote_unclosed(t_token *token)
{
	char	*str;
	size_t	len;
	
	len = ft_strlen(token->str);
	str = token->str;
	if (len == 1 || str[0] != str[len - 1])
	{
		printf ("str[0] is %c and quote is unclosed\n", str[0]);
		//error_msg 258, "unclosed quotes";
		return (true);
	}
	return (false);
}

char	*expander_get_shell_var(const char *str, const int pos, \
		size_t *len_shell_var, t_env *env_list)
{
	char	*str_ret;
	char	*shell_var;

	*len_shell_var = 0;
	token_id_shell_var(str + pos, len_shell_var, 0);
	shell_var = ft_substr(str, pos + 1, *len_shell_var - 1);
	if (!shell_var)
		return (NULL);
	//str_ret = env_var_get_env(shell_var, env_list);
	str_ret = env_get_value_from_key(env_list, shell_var);
	free(shell_var);
	return (str_ret);
}

int	interpolate_env_var(t_token *t_current, const int pos, \
		t_env *env_list)
{
	size_t	len_expanded_str;
	size_t	len_shell_var;
	char	*expanded_str;
	char	*new_token_str;

	if (!t_current || !t_current->str)
		return (ERROR);
	printf ("pos is %d\n", pos);
	expanded_str = expander_get_shell_var(t_current->str, pos, &len_shell_var, \
			env_list);
	printf ("shell expand is %s\n", expanded_str);
	if (!expanded_str)
		len_expanded_str = 0;
	else
		len_expanded_str = ft_strlen(expanded_str);
	new_token_str = ft_calloc(sizeof(char), \
			((ft_strlen(t_current->str) - len_shell_var + len_expanded_str) + 1));
	printf ("		ft_strlen(t_current->str) is %zu, len_shell_var is %zu, len expanded_str is %zu\n", ft_strlen(t_current->str), len_shell_var, len_expanded_str);
	if (!new_token_str)
		return (ERROR);
	ft_strlcpy(new_token_str, t_current->str, pos + 1);
	printf ("1st new_token_str is |%s|\n", new_token_str);
	ft_strlcat(new_token_str, expanded_str, pos + len_expanded_str + 1);
	printf ("2nd new_token_str is |%s|\n", new_token_str);
	// echo "$PATH $TERM"
	printf ("3rd new_token_str is |%s|\n", new_token_str);
	free(t_current->str);
	t_current->str = new_token_str;
	return (len_expanded_str);
}

t_token	*expand_quote(t_token *token, t_env *env_list)
{
	size_t	i;
	int		tmp;

	if (token->str == NULL || is_quote_unclosed(token))
		return (NULL);
	remove_quotes(token);
	if (token->id == SQUOTE)
	{
		token->id = WORD;
		return (copy_token(token));
	}
	i = 0;
	while ((token->str)[i])
	{
		printf ("reached here\n");
		if ((token->str)[i] == '$')
		{
			printf ("Going to expander_inject_var\n");
			tmp = interpolate_env_var(token, i, env_list);
			if (tmp < 0)
				return (NULL);
			i += (size_t) tmp;
		}
		else
			i++;
	}
	token->id = WORD;
	return (copy_token(token));
}

t_token	*expander(t_token *token_list_head, t_env *env_list)
{
	t_token	*new_token_list_head;
	t_token	*current_token;
	t_token	*expanded_tokens;

	new_token_list_head = NULL;
	current_token = token_list_head;
	while (current_token != NULL)
	{
		if (current_token->id == SHELL_VAR)
		{
			//env_var_print_linked_list (env_list);
			//env_print_list (env_list);  // lisandro
			expanded_tokens = expand_shell_var(current_token, env_list);
		
		}
		else if (current_token->id == DQUOTE || current_token->id == SQUOTE)
			expanded_tokens = expand_quote(current_token, env_list);
		else
			expanded_tokens = copy_token(current_token);
		if (!expanded_tokens)
			return (free_token_list(token_list_head, free_node), \
					free_token_list(new_token_list_head, free_node), \
					NULL); //error_print, 1, "expander: unable to expand")
		printf ("current_token is %s, expanded_token is %s\n", current_token->str, expanded_tokens->str);
		add_token_in_back(&new_token_list_head, expanded_tokens);
		current_token = current_token->next;
	}
	free_token_list(token_list_head, free_node);
	return (new_token_list_head);
}
