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

t_token	*expand_env_var(t_token *token, t_env *env_list)
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
	printf ("in expand_env_var str is %s\n", env_value);
	
	split = ft_split (env_value, ' ');
	if (!split)
		return (NULL);
	while (split[i] != NULL)
	{
		expanded_token = create_expanded_tokens(split[i]);
		printf ("LIST OF EXPANDED TOKENS ");
		print_token(expanded_token);
		// if (expanded_token)
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
	size_t	len;
	
	len = ft_strlen(token->str);
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

char	*get_expanded_value(char *token_str, size_t pos, size_t *env_key_len, t_env *env_list)
{
	char	*env_value;
	char	*env_key;

	*env_key_len = 0;
	printf ("str[pos] is %s[%zu]\n", token_str, pos);
	set_pos_end_env_var(&token_str[pos], env_key_len, 0);
	printf ("     env_key_len is now %zu\n", *env_key_len);
	env_key = ft_substr(token_str, pos + 1, *env_key_len - 1);
	if (!env_key)
		return (NULL);
	//str_ret = env_var_get_env(env_var, env_list);
	env_value = env_get_value_from_key(env_list, env_key);
	free(env_key);
	return (env_value);
}

int	interpolate_env_var(t_token *token, int pos, t_env *env_list)
{
	char	*expanded_value;
	size_t	expanded_value_len;
	size_t	env_key_len;
	char	*new_token_str;

	if (!token || !token->str)
		return (ERROR);
	printf ("pos is %d\n", pos);
	expanded_value = get_expanded_value(token->str, pos, &env_key_len, env_list);
	printf ("shell expand is %s\n", expanded_value);
	if (!expanded_value)
		expanded_value_len = 0;
	else
		expanded_value_len = ft_strlen(expanded_value);
	new_token_str = ft_calloc(sizeof(char), \
			((ft_strlen(token->str) - env_key_len + expanded_value_len) + 1));
	printf ("ft_strlen(t_current->str) is %zu, env_key_len is %zu, expanded_value_len is %zu\n", ft_strlen(token->str), env_key_len, expanded_value_len);
	if (!new_token_str)
		return (ERROR);
	ft_strlcpy(new_token_str, token->str, pos + 1);
	printf ("1st new_token_str is |%s|\n", new_token_str);
	ft_strlcat(new_token_str, expanded_value, pos + expanded_value_len + 1);
	printf ("2nd new_token_str is |%s|\n", new_token_str);
	// echo "$PATH$TERM"
	ft_strlcat(new_token_str, &(token->str)[pos + env_key_len], \
			(ft_strlen(token->str) - env_key_len + expanded_value_len + 1));
	free(token->str);
	token->str = new_token_str;
	return (expanded_value_len);
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
		if ((token->str)[i] == '$')
		{
			printf ("Going to interpolate_e\n");
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

t_token	*expand(t_token *token_list_head, t_env *env_list)
{
	t_token	*new_token_list_head;
	t_token	*current_token;
	t_token	*expanded_tokens;

	new_token_list_head = NULL;
	current_token = token_list_head;
	while (current_token != NULL)
	{
		if (current_token->id == ENV_VAR)
		{
			//env_var_print_linked_list (env_list);
			//env_print_list (env_list);  // lisandro
			expanded_tokens = expand_env_var(current_token, env_list);
		
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
