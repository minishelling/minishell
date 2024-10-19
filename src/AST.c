#include "../include/minishell.h"

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




t_token *get_rid_of_first_parenthesis(t_token *start_token, t_token **middle, t_token **end_token)
{
	t_token *iterator;
	int		parentheses;
	
	//printf ("Reached get rid of parens\n");
	//printf ("start is %p and end is %p\n", start_token, (*end_token));
	
	//start_token = get_after_space_token(start_token);
	iterator = start_token;
	parentheses = 1;
	while (iterator->next && parentheses)
	{
		//printf ("parentheses  is %d\n", parentheses);
		//printf ("iterator->next is %s\n", iterator->next->str);
		if (iterator->next->id == PAR_OPEN)
			parentheses++;
			else if (iterator->next->id == PAR_CLOSE)
		{
			parentheses--;
			if (parentheses == 0)
			{
				// printf ("found close par\n");
				if (iterator->next == *end_token)
					*end_token = iterator;
				*middle = iterator;
				//printf ("*middle is %p\n", *middle);
				//printf ("in get rid start_token is %p, iterator->next is %p\n", start_token, iterator->next);
				start_token = remove_token_by_reference (start_token, start_token);
				start_token = remove_token_by_reference (start_token, iterator->next);
				//printf ("after getting rid of parens\n");
				//print_token (start_token);
				break;
			}
		}
		iterator = iterator->next;
	}
	return (start_token);
}


/**
 * find_last_log_op_token_nip - Find the last logical operator outside parentheses.
 *
 * This function iterates over the tokens from `token_head` to `end_token`, 
 * ignoring any logical operators within parentheses and returning the 
 * last logical operator (AND_OPR, OR_OPR, or PIPE) found outside of parentheses.
 *
 * The function checks for `AND_OPR` and `OR_OPR` first because they have 
 * higher precedence than `PIPE`. The `PIPE` operator is considered as a 
 * lower precedence logical operator and will only be returned if no higher 
 * precedence operator has been encountered.
 *
 * @token_head: The head of the token list.
 * @end_token: The token that marks the endpoint for scanning (can be NULL).
 *
 * Return: The token of the last logical operator found outside parentheses,
 *         or NULL if no such operator is found.
 */
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


/**
 * divide_token_list - Splits a token list into two parts based on a logical operator token.
 *
 * This function traverses the token list and finds the logical operator (`op_token`). It then 
 * splits the list into two parts: the left part contains all tokens up to the logical operator, 
 * and the right part starts from the token immediately after the logical operator.
 *
 * If the logical operator is found, the left part of the list is terminated, and the right part 
 * starts from the token after the logical operator.
 *
 * @param token_list: The head of the token list to be divided.
 * @param op_token: The logical operator token used as the splitting point.
 * @param left_head: A pointer to the head of the left token list (tokens before the operator).
 * @param right_head: A pointer to the head of the right token list (tokens after the operator).
 */
void divide_token_list(t_token *token_list, t_token *log_op_token, t_token **left_head, t_token **right_head)
{
    t_token *next_after_log_op_token;
	t_token *current;
	t_token *next_token;
    
    if (!token_list || !log_op_token)
    {
        *left_head = NULL;
        *right_head = NULL;
        return;
    }
	current = token_list;
    *left_head = token_list;
    while (current)
    {
        next_token = current->next;
        if (next_token == log_op_token)
            break;
        current = next_token;
    }
    if (current && current->next == log_op_token)
    {
        current->next = NULL;
        next_after_log_op_token = log_op_token->next;
        if (next_after_log_op_token)
            *right_head = next_after_log_op_token;
        else
            *right_head = NULL;
    }
}


t_tree *init_leaf_node(t_shell *shell, t_token *start_token, t_token *end_token)
{
	t_tree *leaf_node;
	
	leaf_node = (t_tree *)malloc(sizeof(t_tree));
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


t_tree *init_tree_node(t_shell *shell, t_token *op_token)
{
	t_tree *tree_node;
	
	if (!op_token) 
	{
		fprintf(stderr, "Error: op_token is NULL\n");
   		exit(EXIT_FAILURE);
	}
	tree_node = (t_tree *)malloc(sizeof(t_tree));
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
	t_token *left_head;
	t_token *right_head;
	t_tree *subtree;
	t_token *middle = NULL;
	// printf ("making tree\n");
	if (start_token == NULL || end_token == NULL)
		return NULL;
	
	// printf ("start_token is %s and end_token is %s\n", start_token->str, end_token->str);
	//if (start_token->id == PAR_OPEN && end_token->id == PAR_CLOSE && !contains_pipe_open_par(start_token, end_token))
	if (start_token->id == PAR_OPEN && end_token->id == PAR_CLOSE && get_matching_parenthesis(start_token) == end_token)
	// if (start_token->id == PAR_OPEN)
	{
		//printf("Removing parentheses: start = %p, end = %p\n", start_token, end_token);
		start_token = get_rid_of_first_parenthesis(start_token, &middle, &end_token);
		//printf ("got rid of parens\n");
		//printf ("start token is %p and end token is %p\n", start_token, end_token);
		// print_token(start_token);
		if (end_token)
		{
			if (start_token->id == PAR_OPEN && middle->id == PAR_CLOSE && get_matching_parenthesis(start_token) == middle)
			{
				//printf("arithmetic expantion\n");
				if(!safe_assign_str(&start_token->str, "((") || !safe_assign_str(&end_token->str, "))"))
					exit (FAILURE);  //protext better
				start_token->id = ARITH_EXPAN;
				end_token->id = ARITH_EXPAN;
				return init_leaf_node (shell, start_token, middle);
			}
			else	
				return make_tree(shell, start_token, end_token);
		}
	}
	// Find the last logical operator not in parentheses
	log_op_token = find_last_log_op_token_nip(start_token, end_token);
	// printf("Found log_op: %s\n", log_op_token ? log_op_token->str : "NULL");
	
	// If no logical operator is found, treat the range of tokens as a command
	if (!log_op_token)
	{
		// Create a leaf node that groups all tokens from start_token to end_token
		// printf("Creating CMD node from %s to %s\n", start_token->str, end_token->str);
		return init_leaf_node(shell, start_token, end_token);
	}

	// If a logical operator is found, create a subtree
	subtree = init_tree_node(shell, log_op_token);
	//printf("Creating tree node with log_op: %s\n", log_op_token->str);

	// Divide the token list into left and right parts
	divide_token_list(start_token, log_op_token, &left_head, &right_head);
	// Recursively create left and right subtrees
	//printf ("doing LEFT, left_head is %s\n", left_head->str);
	subtree->left = make_tree(shell, left_head, non_null_previous(start_token, log_op_token));
	//printf ("now right\n");
	//printf ("right head is %s and end_token is %s\n", right_head->str, end_token->str);
	subtree->right = make_tree(shell, right_head, end_token);

	return subtree;
}
