#include "../include/minishell.h"


t_tree *init_leaf_node(t_shell *shell, t_token *start_token, t_token *end_token)
{
	t_tree *leaf_node;

	leaf_node = (t_tree*)ft_calloc(1, sizeof(t_tree));
	//leaf_node = (t_tree *)malloc(sizeof(t_tree));
	if (!leaf_node)
	{
		handle_parsing_err(shell, ERR_MEM, NULL);
		//free token_list and tree, with exit
	}
	leaf_node->type = CMD;
	leaf_node->start_token = start_token;
	while (end_token->id == SPACE_CHAR)
		end_token = non_null_previous (start_token, end_token);
	leaf_node->end_token = end_token;
	leaf_node->left = NULL;
	leaf_node->right = NULL;
	//printf ("in init_leaf_node, it is:\n");
	//print_tree(leaf_node, 0);
	return (leaf_node);
}


static t_tree *process_arithmetic_expansion(t_shell *shell, t_token *start_token, t_token *end_token, t_token *middle)
{
	// print_token(shell->token);
	// printf("start token in arith is %s %p\n", start_token->str, start_token);
	// printf("end token in arith is %s %p\n", end_token->str, end_token);
	if (safe_assign_str(&(start_token->str), "((") != SUCCESS || 
		safe_assign_str(&(end_token->str), "))") != SUCCESS)
			exit (EXIT_FAILURE);  //    !!!pay attention
	start_token->id = ARITH_EXPAN;
	end_token->id = ARITH_EXPAN;
	return (init_leaf_node(shell, start_token, middle));
}


t_token *get_matching_parenthesis(t_token *start_token)
{
	t_token *iterator;
	int		parentheses;

	iterator = start_token;
	parentheses = 1;
	while (iterator->next && parentheses)
	{
		if (iterator->next->id == PAR_OPEN)
		{
			//printf ("found open par\n");
			parentheses++;
		}
			else if (iterator->next->id == PAR_CLOSE)
		{
			parentheses--;
			if (parentheses == 0)
			{
				// printf ("found close par\n");
				return (iterator->next);
			}
		}
		iterator = iterator->next;
	}
	return (NULL);
}

/**
 * @brief Ignores the outermost opening parenthesis and attempts to find its matching closing parenthesis in a token list.
 * 
 * The `start_token` is always an opening parenthesis (`(`), and it is called only when the `end_token` is a closing parenthesis (`)`).
 * However, the `end_token` may not always be the matching closing parenthesis for the initial opening parenthesis.
 * 
 * The function proceeds by identifying the matching closing parenthesis (`)`), 
 * skipping over the `start_token` (opening parenthesis), and adjusting token pointers accordingly. 
 * 
 * - The `start_token` is updated to the token immediately following the outermost opening parenthesis (`start_token->next`).
 * - The `middle` pointer is set to the token immediately after the matching closing parenthesis.
 * - If the `end_token` corresponds to the matching closing parenthesis, it is updated to point to the token just before the closing parenthesis.
 * - If the `end_token` does not match the outermost closing parenthesis, the `end_token` remains unchanged.
 * The parentheses are not freed in this version of the function.
 * 
 * @param shell        Pointer to the shell structure containing the token list.
 * @param start_token  Pointer to the token where the process starts (always an opening parenthesis `(`).
 * @param middle       Pointer to the token immediately after the matching closing parenthesis, or updated to the end token if the `end_token` is the closing parenthesis.
 * @param end_token    Pointer to the last token in the current group of tokens. It is updated if the matching closing parenthesis is the `end_token`.
 * 
 * @return t_token*    Returns the new `start_token`, which is the token immediately after the outermost opening parenthesis.
 * 
 */
t_token *ignore_first_parenthesis(t_shell *shell, t_token *start_token, t_token **middle, t_token **end_token)
{

	t_token *matching_parenthesis;

	if (start_token == shell->token)
		shell->token = start_token->next;
	matching_parenthesis = get_matching_parenthesis(start_token);
	start_token = start_token->next;
	if (matching_parenthesis)
		*middle = non_null_previous(start_token, matching_parenthesis);
	if (matching_parenthesis == *end_token)
		*end_token = *middle;
	return (start_token);
}


t_token *find_last_log_op_token_nip(t_token *token_head, t_token *end_token)
{
	t_token *token_iterator;
	t_token *return_token;
	int parenthesis;

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
	return return_token;
}

t_tree *init_tree_node(t_shell *shell, t_token *op_token)
{
	t_tree *tree_node;

	if (!op_token)
	{
		fprintf(stderr, "Error: op_token is NULL\n");
   		exit(EXIT_FAILURE);
	}
	//tree_node = (t_tree *)malloc(sizeof(t_tree));
	tree_node = (t_tree*)ft_calloc(1, sizeof(t_tree));
	if (!tree_node)
	{
		handle_parsing_err(shell, ERR_MEM, NULL);
		//free token_list and tree, with exit
	}
	if (op_token->id == AND_OPR)
		tree_node->type = T_AND_OPR;
	else if (op_token->id == OR_OPR)
		tree_node->type = T_OR_OPR;
	else if (op_token ->id == PIPE)
		tree_node->type = T_PIPE;
	tree_node->left = NULL;
	tree_node->right = NULL;
	return tree_node;
}

t_tree *make_tree(t_shell *shell, t_token *start_token, t_token *end_token)
{
	t_token *log_op_token;
	//t_token *left_head;
	//t_token *right_head;
	t_tree *subtree;
	t_token *middle = NULL;

	if (start_token == NULL || end_token == NULL)
		return NULL;
	//printf ("making tree\n");
	// printf ("start_token is %s and end_token is %s\n", start_token->str, end_token->str);
	if (start_token->id == PAR_OPEN && end_token->id == PAR_CLOSE && get_matching_parenthesis(start_token) == end_token)
	{
		//printf("Removing parentheses: start = %p, end = %p\n", start_token, end_token);
		//printf("Removing parentheses: start = %s, end = %s\n", start_token->str, end_token->str);
		start_token = ignore_first_parenthesis(shell, start_token, &middle, &end_token);
		//printf ("got rid of parens\n");
		//printf ("start token is %p middle is %p and end token is %p\n", start_token, middle, end_token);

		if (end_token)
		{
			if (start_token->id == PAR_OPEN && middle->id == PAR_CLOSE && get_matching_parenthesis(start_token) == middle)
			{
				//printf ("arith_expan\n");
				process_arithmetic_expansion(shell, start_token, end_token, middle);
				return (init_leaf_node (shell, start_token, middle));
			}
			else
				return (make_tree(shell, start_token, end_token));
		}
	}
	log_op_token = find_last_log_op_token_nip(start_token, end_token);
	//printf("Found log_op: %s\n", log_op_token ? log_op_token->str : "NULL");
	if (!log_op_token)
		return (init_leaf_node(shell, start_token, end_token));
	subtree = init_tree_node(shell, log_op_token);
	//printf("Creating tree node with log_op: %s\n", log_op_token->str);
	//divide_token_list(shell, start_token, log_op_token, &left_head, &right_head);

	//printf ("doing LEFT\n");
	//printf ("start %s , end %s\n", start_token->str, non_null_previous(start_token ,log_op_token)->str);
	//subtree->left = make_tree(shell, left_head, non_null_previous(start_token, log_op_token));
	subtree->left = make_tree(shell, start_token, non_null_previous(start_token ,log_op_token));
	//print_token(start_token);
	//printf ("now RIGHT\n");
	//printf ("start %s , end %s\n", log_op_token->next->str, end_token->str);
	//printf ("right head is %s and end_token is %s\n", right_head->str, end_token->str);
	//subtree->right = make_tree(shell, right_head, end_token);
	subtree->right = make_tree(shell, log_op_token->next, end_token);
	return subtree;
}
