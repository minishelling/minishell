#include "../../include/minishell.h"

t_token	*get_matching_paren(t_token *start_token);
t_token	*ignore_first_parens(t_token *start_token, t_token **end_token);
t_tree	*process_arith_expan(t_shell *shell, t_token *start_token, \
	t_token *end_token);

t_token	*get_matching_paren(t_token *start_token)
{
	t_token	*cur_token;
	int		parens;

	cur_token = start_token;
	parens = 1;
	while (cur_token->next && parens)
	{
		if (cur_token->next->id == PAR_OPEN)
			parens++;
		else if (cur_token->next->id == PAR_CLOSE)
		{
			parens--;
			if (parens == 0)
				return (cur_token->next);
		}
		cur_token = cur_token->next;
	}
	return (NULL);
}

t_token	*ignore_first_parens(t_token *start_token, t_token **end_token)
{
	t_token	*matching_paren;

	matching_paren = get_matching_paren(start_token);
	start_token = start_token->next;
	if (matching_paren)
		*end_token = non_null_previous(start_token, matching_paren);
	return (start_token);
}

t_tree	*process_arith_expan(t_shell *shell, t_token *start_token, \
	t_token *end_token)
{
	if (safe_assign_str(&(start_token->str), "((") != SUCCESS \
	|| safe_assign_str(&(end_token->str), "))") != SUCCESS)
	{
		handle_parsing_err(shell, ERR_MEM);
		clean_nicely_and_exit(shell, EXIT_FAILURE);
	}
	start_token->id = ARITH_EXPAN;
	end_token->id = ARITH_EXPAN;
	return (init_leaf_node(shell, start_token, end_token));
}
