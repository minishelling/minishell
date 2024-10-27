#include "../../include/minishell.h"

t_tree	*init_leaf_node(t_shell *shell, t_token *start_token, \
	t_token *end_token)
{
	t_tree	*leaf_node;

	leaf_node = (t_tree *)ft_calloc(1, sizeof(t_tree));
	if (!leaf_node)
	{
		handle_parsing_err(shell, ERR_MEM);
		clean_nicely_and_exit(shell, EXIT_FAILURE);
	}
	leaf_node->type = CMD;
	leaf_node->start_token = start_token;
	while (end_token->id == SPACE_CHAR)
		end_token = non_null_previous (start_token, end_token);
	leaf_node->end_token = end_token;
	leaf_node->left = NULL;
	leaf_node->right = NULL;
	return (leaf_node);
}

static t_tree	*process_arith_expan(t_shell *shell, t_token *start_token, \
	t_token *end_token, t_token *middle)
{
	if (safe_assign_str(&(start_token->str), "((") != SUCCESS \
	|| safe_assign_str(&(end_token->str), "))") != SUCCESS)
	{
		handle_parsing_err(shell, ERR_MEM);
		clean_nicely_and_exit(shell, EXIT_FAILURE);
	}
	start_token->id = ARITH_EXPAN;
	end_token->id = ARITH_EXPAN;
	return (init_leaf_node(shell, start_token, middle));
}

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

t_token	*find_last_log_op_token_nip(t_token *token_head, t_token *end_token)
{
	t_token	*token_iterator;
	t_token	*return_token;
	int		parenthesis;

	return_token = NULL;
	parenthesis = 0;
	token_iterator = token_head;
	while (token_iterator && token_iterator != end_token)
	{
		if (token_iterator->id == PAR_OPEN)
			parenthesis++;
		else if (token_iterator->id == PAR_CLOSE)
			parenthesis--;
		else if (parenthesis == 0)
		{
			if (token_iterator->id == AND_OPR || token_iterator->id == OR_OPR)
				return_token = token_iterator;
			else if (token_iterator->id == PIPE)
				if (return_token == NULL)
					return_token = token_iterator;
		}
		token_iterator = token_iterator->next;
	}
	return (return_token);
}

t_tree	*init_tree_node(t_shell *shell, t_token *op_token)
{
	t_tree	*tree_node;

	if (!op_token)
	{
		handle_parsing_err(shell, ERR_MEM);
		clean_nicely_and_exit(shell, EXIT_FAILURE);
	}
	tree_node = (t_tree *)ft_calloc(1, sizeof(t_tree));
	if (!tree_node)
	{
		handle_parsing_err(shell, ERR_MEM);
		clean_nicely_and_exit(shell, EXIT_FAILURE);
	}
	if (op_token->id == AND_OPR)
		tree_node->type = T_AND_OPR;
	else if (op_token->id == OR_OPR)
		tree_node->type = T_OR_OPR;
	else if (op_token ->id == PIPE)
		tree_node->type = T_PIPE;
	tree_node->left = NULL;
	tree_node->right = NULL;
	return (tree_node);
}

t_tree	*handle_parentheses(t_shell *shell, t_token *start_token, \
	t_token *end_token)
{
	if (start_token->id == PAR_OPEN && end_token->id == PAR_CLOSE \
		&& get_matching_parenthesis(start_token) == end_token)
	{
		start_token = ignore_first_parenthesis(start_token, &end_token);
		if (end_token)
		{
			if (start_token->id == PAR_OPEN && end_token->id == PAR_CLOSE \
			&& get_matching_parenthesis(start_token) == end_token)
			{
				process_arith_expan(shell, start_token, end_token, end_token);
				return (init_leaf_node(shell, start_token, end_token));
			}
			else
				return (make_tree(shell, start_token, end_token));
		}
	}
	return (NULL);
}

t_tree	*handle_logical_operator(t_shell *shell, t_token *start_token, \
	t_token *end_token)
{
	t_token	*log_op_token;
	t_tree	*subtree;

	log_op_token = find_last_log_op_token_nip(start_token, end_token);
	if (!log_op_token)
		return (init_leaf_node(shell, start_token, end_token));
	subtree = init_tree_node(shell, log_op_token);
	subtree->left = make_tree(shell, start_token, \
	non_null_previous(start_token, log_op_token));
	subtree->right = make_tree(shell, log_op_token->next, end_token);
	return (subtree);
}

t_tree	*make_tree(t_shell *shell, t_token *start_token, t_token *end_token)
{
	t_tree	*subtree;

	if (start_token == NULL || end_token == NULL)
		return (NULL);
	subtree = handle_parentheses(shell, start_token, end_token);
	if (subtree)
		return (subtree);
	return (handle_logical_operator(shell, start_token, end_token));
}
