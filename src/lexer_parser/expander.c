#include "../../include/minishell.h"

t_token	*free_node(t_token *t_node)
{
	t_token	*t_tmp;

	t_tmp = t_node->next;
	if (t_node == NULL)
		return (NULL);
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


t_token	*expander_shell_var_spacer(char *str)
{
	t_token	*t_return;
	t_token	*t_node;

	t_return = NULL;
	t_node = list_token_new();
	if (!t_node)
		return (NULL);
	t_node->id = WORD;
	t_node->str = str;
	t_return = t_node;
	t_node = list_token_new();
	if (!t_node)
		return (list_token_free_node(t_return), NULL);
	t_node->id = SPACE_CHAR;
	t_node->str = ft_strdup(" ");
	if (!(t_node->str))
		return (list_token_free_node(t_return), \
				list_token_free_node(t_node), NULL);
	t_return->next = t_node;
	return (t_return);
}

t_token	*expander_shell_var(t_token *t_current, t_env_list *env_var_list)
{
	t_token	*t_return;
	t_token	*t_node;
	char	**split;
	size_t	i;

	t_return = NULL;
	i = 0;

	split = ft_split(env_var_get_env((t_current->str + 1), env_var_list), ' ');
	if (!split)
		return (NULL);
	
	while (split[i] != NULL)
	{
		t_node = expander_shell_var_spacer(split[i]);
		// if (!t_node)
		// 	...
		list_token_add_back(&t_return, t_node);
		i++;
	}
	free(split);
	if (i == 0)
		return (list_token_new());
	list_token_free_last(t_return, free_node);
	return (t_return);
}

bool	expander_remove_n_is_open(t_token *t_node)
{
	char			*str;
	const size_t	len = ft_strlen(t_node->str);

	str = t_node->str;
	if (len < 2 || str[0] != str[len - 1])
	{
		//error_msg 258, "unclosed quotes";
		return (true);
	}
	ft_memmove(str, str + 1, len);
	ft_memmove(&str[len - 2], &str[len - 1], 1);
	return (false);
}

char	*expander_get_shell_var(const char *str, const int pos, \
		size_t *len_sh_var, t_env_list *env_var_list)
{
	char	*str_ret;
	char	*sh_var;

	*len_sh_var = 0;
	token_id_shvar(str + pos, len_sh_var, 0);
	sh_var = ft_substr(str, pos + 1, *len_sh_var - 1);
	if (!sh_var)
		return (NULL);
	str_ret = env_var_get_env(sh_var, env_var_list);
	free(sh_var);
	return (str_ret);
}

int	expander_inject_var(t_token *t_current, const int pos, \
		t_env_list *env_var_list)
{
	size_t	len_sh_expand;
	size_t	len_sh_var;
	char	*sh_expand;
	char	*new_token_str;

	if (!t_current || !t_current->str)
		return (ERROR);
	sh_expand = expander_get_shell_var(t_current->str, pos, &len_sh_var, \
			env_var_list);
	if (!sh_expand)
		len_sh_expand = 0;
	else
		len_sh_expand = ft_strlen(sh_expand);
	new_token_str = ft_calloc(sizeof(char), \
			((ft_strlen(t_current->str) - len_sh_var + len_sh_expand) + 1));
	if (!new_token_str)
		return (ERROR);
	ft_strlcpy(new_token_str, t_current->str, pos + 1);
	ft_strlcat(new_token_str, sh_expand, pos + len_sh_expand + 1);
	ft_strlcat(new_token_str, &(t_current->str)[pos + len_sh_var], \
			(ft_strlen(t_current->str) - len_sh_var + len_sh_expand + 1));
	free(t_current->str);
	t_current->str = new_token_str;
	return (len_sh_expand);
}

t_token	*expander_quote(t_token *t_current, t_env_list *env_var_list)
{
	size_t	i;
	int		tmp;

	if (t_current->str == NULL || expander_remove_n_is_open(t_current))
		return (NULL);
	if (t_current->id == SQUOTE)
	{
		t_current->id = WORD;
		return (list_token_cpy_node(t_current));
	}
	i = 0;
	while ((t_current->str)[i])
	{
		if ((t_current->str)[i] == '$')
		{
			tmp = expander_inject_var(t_current, i, env_var_list);
			if (tmp < 0)
				return (NULL);
			i += (size_t) tmp;
		}
		else
			i++;
	}
	t_current->id = WORD;
	return (list_token_cpy_node(t_current));
}

t_token	*expander(t_token *t_input, t_env_list *env_var_list)
{
	t_token	*t_return;
	t_token	*t_current;
	t_token	*t_node;

	t_return = NULL;
	t_current = t_input;
	while (t_current != NULL)
	{
		if (t_current->id == SHELL_VAR)

			t_node = expander_shell_var(t_current, env_var_list);
		else if (t_current->id == DQUOTE || t_current->id == SQUOTE)
			t_node = expander_quote(t_current, env_var_list);
		else
			t_node = list_token_cpy_node(t_current);
		if (!t_node)
			return (list_token_free_list(t_input, free_node), \
					list_token_free_list(t_return, free_node), \
					NULL); //error_print, 1, "expander: unable to expand"),
		list_token_add_back(&t_return, t_node);
		t_current = t_current->next;
	}
	list_token_free_list(t_input, free_node);
	return (t_return);
}

//	SH_VAR appends to previous word token
//	SH_VAR adds in front of DQUOTE
/*
 *
 * EXPANSION:
 *
 * --==-- CASE [1] --==--
 *  VAR="b c d"
 *  ./print_arg a$VAR'ef'
 *
 *  1  ab
 *  2  c
 *  3  def
 *
 * --==-- CASE [2] --==--
 *  VAR=""
 *  ./print_arg a$VAR'ef'
 *  
 *  1  aef
 *
 * --==-- CASE [3] --==--
 *  A="b c d"
 *  B="e f g"
 *  ./print_arg a$A$B"h"
 *
 *  1  ab
 *  2  c
 *  3  de
 *  4  f
 *  5  gh
 */
