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


t_token	*expand_shell_var_spacer(char *str)
{
	t_token	*return_token;
	t_token	*token;

	return_token = NULL;
	token = new_token();
	if (!token)
		return (NULL);
	token->id = WORD;
	token->str = str;
	return_token = token;
	token = new_token();
	if (!token)
		return (free_token_node(return_token), NULL);
	token->id = SPACE_CHAR;
	token->str = ft_strdup(" ");
	if (!(token->str))
		return (free_token_node(return_token), \
				free_token_node(token), NULL);
	return_token->next = token;
	return (return_token);
}

t_token	*expand_shell_var(t_token *token, t_env *env_list)
{
	t_token	*return_token;
	t_token	*space_token;
	char	**split;
	size_t	i;

	//env_var_print_linked_list (env_list);
	// env_print_list (env_list);  // lisandro

	return_token = NULL;
	i = 0;

	char *str = env_get_value_from_key(env_list, (token->str + 1));
	printf ("in expand_shell_var str is %s\n", str);
	
	split = ft_split (str, ' ');
	if (!split)
		return (NULL);
	while (split[i] != NULL)
	{
		
		space_token = expand_shell_var_spacer(split[i]);
		// if (!t_node)
		// 	...
		add_token_in_back(&return_token, space_token);
		i++;
	}
	
	free(split);
	if (i == 0)
		return (new_token());
	free_last_token(return_token, free_node);
	return (return_token);
}

bool	remove_quotes_is_unclosed(t_token *token)
{
	char	*str;
	size_t	len;
	
	len = ft_strlen(token->str);
	str = token->str;
	if (len == 1 || str[0] != str[len - 1])
	{
		printf ("len is %zu and str[0] is %c\n", len, str[0]);
		//error_msg 258, "unclosed quotes";
		return (true);
	}
	// ft_memmove(str, str + 1, len);
	// ft_memmove(&str[len - 2], &str[len - 1], 1);
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

int	expander_inject_var(t_token *t_current, const int pos, \
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
	printf ("len_shell_expand is %zu\n", len_expanded_str);
	new_token_str = ft_calloc(sizeof(char), \
			((ft_strlen(t_current->str) - len_shell_var + len_expanded_str) + 1));
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

	if (token->str == NULL || remove_quotes_is_unclosed(token))
		return (NULL);
	ft_memmove(token->str, token->str + 1, len);
	ft_memmove(&token->str[len - 2], &token->str[len - 1], 1);
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
			tmp = expander_inject_var(token, i, env_list);
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
	t_token	*expanded_token;

	new_token_list_head = NULL;
	current_token = token_list_head;
	while (current_token != NULL)
	{
		if (current_token->id == SHELL_VAR)
		{
			//env_var_print_linked_list (env_list);
			//env_print_list (env_list);  // lisandro
			expanded_token = expand_shell_var(current_token, env_list);
		
		}
		else if (current_token->id == DQUOTE || current_token->id == SQUOTE)
			expanded_token = expand_quote(current_token, env_list);
		else
			expanded_token = copy_token(current_token);
		if (!expanded_token)
			return (free_token_list(token_list_head, free_node), \
					free_token_list(new_token_list_head, free_node), \
					NULL); //error_print, 1, "expander: unable to expand")
		printf ("current_token is %s, expanded_token is %s\n", current_token->str, expanded_token->str);
		add_token_in_back(&new_token_list_head, expanded_token);
		current_token = current_token->next;
	}
	free_token_list(token_list_head, free_node);
	return (new_token_list_head);
}
