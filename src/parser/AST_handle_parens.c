#include "../../include/minishell.h"

t_token	*get_matching_parenthesis(t_token *start_token);
t_token	*ignore_first_parenthesis(t_token *start_token, t_token **end_token);
t_tree	*process_arith_expan(t_shell *shell, t_token *start_token, \
	t_token *end_token);
	
t_token	*get_matching_parenthesis(t_token *start_token)
{
	t_token	*iterator;
	int		parentheses;

	iterator = start_token;
	parentheses = 1;
	while (iterator->next && parentheses)
	{
		if (iterator->next->id == PAR_OPEN)
			parentheses++;
		else if (iterator->next->id == PAR_CLOSE)
		{
			parentheses--;
			if (parentheses == 0)
				return (iterator->next);
		}
		iterator = iterator->next;
	}
	return (NULL);
}

t_token	*ignore_first_parenthesis(t_token *start_token, t_token **end_token)
{
	t_token	*matching_parenthesis;

	matching_parenthesis = get_matching_parenthesis(start_token);
	start_token = start_token->next;
	if (matching_parenthesis)
		*end_token = non_null_previous(start_token, matching_parenthesis);
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
