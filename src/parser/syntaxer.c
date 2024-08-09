#include "../../include/minishell.h"

typedef bool	(*t_syntax_func) (t_token *t_prev, t_token *t_cur, \
		t_env *env_list);

t_token	*skip_whitespace(t_token *list)
{
	if (list == NULL)
		return (NULL);
	if (list->id == SPACE_CHAR || list->id == TAB_CHAR || list->id == NL)
		return (list->next);
	return (list);
}


bool is_par_closed(t_token *token_list_head) {
	
	t_token *current_token;
	int paren_count;

	if (!token_list_head) {
		return 1;
	}
	paren_count = 0;
	current_token = token_list_head;
	while (current_token) 
	{
		printf("current token is %s\n", current_token->str);
		if (current_token->id == PAR_OPEN) 
		{
			paren_count++;
		} 
		else if (current_token->id == PAR_CLOSE) 
		{
			if (paren_count == 0) 
				return 0;
			paren_count--;
		}
		current_token = current_token->next;
	}
	return (paren_count == 0);
}

t_token	*syntax(t_token *token_list_head, t_env *env_list)
{
	t_token				*previous_token;
	t_token				*current_token;

	bool par_closed = is_par_closed(token_list_head);
	printf ("par_closed is %d\n", par_closed);
	if (!par_closed)
		{
			printf ("Returning 1\n");
			return (token_list_head);
		}
	t_syntax_func	func[14] = {
	[0] = NULL,
	[1] = NULL,
	[2] = NULL,
	[3] = syntax_id_pipe,
	[4] = syntax_id_ampersand,
	[5] = syntax_id_semicol,
	[6] = syntax_id_parentheses_open,
	[7] = syntax_id_parentheses_open,
	[8] = syntax_id_redir,
	[9] = syntax_id_redir,
	[10] = syntax_id_misc,
	[11] = syntax_id_misc,
	[12] = syntax_id_misc,
	[13] = syntax_id_misc,
	};

	previous_token = NULL;
	current_token = skip_whitespace(token_list_head);
	while (current_token != NULL)
	{
		printf ("in syntax, t_current->id is %d\n", current_token->id);
		if (func[current_token->id](previous_token, current_token, env_list))
			return (current_token);
		previous_token = current_token;
		current_token = get_after_space_token(current_token);
	}
	return (NULL);
}

