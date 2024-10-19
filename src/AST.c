#include "../include/minishell.h"


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



static t_tree *process_arithmetic_expansion(t_shell *shell, t_token *start_token, t_token *end_token, t_token *middle)
{
	if (!safe_assign_str(&start_token->str, "((") || !safe_assign_str(&end_token->str, "))"))
	{

		exit(FAILURE);  // pay attention!!!
	}
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




// t_token *get_rid_of_first_parenthesis(t_token *start_token, t_token **middle, t_token **end_token)
// {
// 	t_token *iterator;
// 	int		parentheses;

// 	//printf ("Reached get rid of parens\n");
// 	//printf ("start is %p and end is %p\n", start_token, (*end_token));

// 	//start_token = get_after_space_token(start_token);
// 	iterator = start_token;
// 	parentheses = 1;
// 	while (iterator->next && parentheses)
// 	{
// 		//printf ("parentheses  is %d\n", parentheses);
// 		//printf ("iterator->next is %s\n", iterator->next->str);
// 		if (iterator->next->id == PAR_OPEN)
// 			parentheses++;
// 			else if (iterator->next->id == PAR_CLOSE)
// 		{
// 			parentheses--;
// 			if (parentheses == 0)
// 			{
// 				// printf ("found close par\n");
// 				if (iterator->next == *end_token)
// 					*end_token = iterator;
// 				*middle = iterator;
// 				//printf ("*middle is %p\n", *middle);
// 				//printf ("in get rid start_token is %p, iterator->next is %p\n", start_token, iterator->next);
// 				start_token = remove_token_by_reference (start_token, start_token);
// 				start_token = remove_token_by_reference (start_token, iterator->next);
// 				//printf ("after getting rid of parens\n");
// 				//print_token (start_token);
// 				break;
// 			}
// 		}
// 		iterator = iterator->next;
// 	}
// 	return (start_token);
// }


// t_token *get_rid_of_first_parenthesis(t_shell *shell, t_token *start_token, t_token **middle, t_token **end_token)
// {
//     t_token *iterator;
//     int     parentheses;


//     iterator = start_token;
//     parentheses = 1;

//     // Handle case where the first token is an opening parenthesis
//     if (iterator->id == PAR_OPEN)
//     {
//         start_token = iterator->next;
//         free_token(&iterator);
//         iterator = start_token;
// 		shell->token = start_token;
//     }

//     while (iterator && iterator->next && parentheses)
//     {
//         if (iterator->next->id == PAR_OPEN)
//             parentheses++;
//         else if (iterator->next->id == PAR_CLOSE)
//         {
//             parentheses--;
//             if (parentheses == 0)
//             {
//                 if (iterator->next == *end_token)
//                     *end_token = iterator;
//                 *middle = iterator;

//                 // Remove the closing parenthesis
//                 t_token *to_remove = iterator->next;
//                 iterator->next = to_remove->next;
//                 free_token(&to_remove);

//                 break;
//             }
//         }
//         iterator = iterator->next;
//     }
//     return start_token;
// }

t_token *get_rid_of_first_parenthesis(t_shell *shell, t_token *start_token, t_token **middle, t_token **end_token)
{
	t_token *to_remove;
	t_token *matching_parenthesis;

	printf("in get rid, shell->token is %p\n", shell->token);
	if (start_token == shell->token)
	{
		shell->token = start_token->next;
		printf("in get rid, updated shell->token to %p\n", shell->token);
	}
	to_remove = start_token;
	start_token = start_token->next;
	free_token(&to_remove);
	// Get the matching closing parenthesis
	matching_parenthesis = get_matching_parenthesis(start_token);

	// If we found a matching parenthesis
	if (matching_parenthesis)
	{
		// Set middle to the token immediately after the closing parenthesis
		*middle = matching_parenthesis->next;

		// Update end_token to link to the next token after the closing parenthesis
		if (matching_parenthesis == *end_token)
		{
			printf ("it is\n");
			*end_token = non_null_previous(start_token, matching_parenthesis); // Link end_token to the next token
		}
		// Remove the closing parenthesis
		to_remove = matching_parenthesis;
		non_null_previous(start_token, matching_parenthesis)->next = to_remove->next; // Link to the next token after the closing parenthesis
		printf("removing closing %p\n", to_remove);
		free_token(&to_remove);
	}
	return start_token; // Return the new start_token
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



// void divide_token_list(t_shell *shell, t_token *token_list, t_token *log_op_token, t_token **left_head, t_token **right_head)
// {
//     t_token *next_after_log_op_token;
// 	t_token *current;
// 	t_token *next_token;

// 	printf("in divide\n");
// 	print_token(shell->token);
//     if (!token_list || !log_op_token)
//     {
//         *left_head = NULL;
//         *right_head = NULL;
//         return;
//     }
// 	current = token_list;
//     *left_head = token_list;
//     while (current)
//     {
//         next_token = current->next;
//         if (next_token == log_op_token)
//             break;
//         current = next_token;
//     }
//     if (current && current->next == log_op_token)
//     {
//         current->next = NULL;
//         next_after_log_op_token = log_op_token->next;
//         if (next_after_log_op_token)
//             *right_head = next_after_log_op_token;
//         else
//             *right_head = NULL;
//     }
// 	printf("in divide\n");
// 	print_token(shell->token);
// }

// void divide_token_list(t_shell *shell, t_token *token_list, t_token *log_op_token, t_token **left_head, t_token **right_head)
// {
//     t_token *current;

//     printf("in divide\n");
//     print_token(shell->token);

//     if (!token_list || !log_op_token)
//     {
//         *left_head = NULL;
//         *right_head = NULL;
//         return;
//     }

//     current = token_list;
//     *left_head = token_list;  // Left part starts from the beginning

//     // Traverse the list to find the logical operator token
//     while (current && current != log_op_token)
//     {
//         current = current->next;
//     }

//     // If logical operator token is found, assign right_head to the token after log_op_token
//     if (current == log_op_token)
//     {
//         *right_head = log_op_token->next;
//     }
//     else
//     {
//         // If log_op_token is not found, set right_head to NULL
//         *right_head = NULL;
//     }

//     printf("in divide\n");
//     print_token(shell->token);
// }





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
	//t_token *left_head;
	//t_token *right_head;
	t_tree *subtree;
	t_token *middle = NULL;

	if (start_token == NULL || end_token == NULL)
		return NULL;
	printf ("making tree\n");
	// printf ("start_token is %s and end_token is %s\n", start_token->str, end_token->str);
	if (start_token->id == PAR_OPEN && end_token->id == PAR_CLOSE && get_matching_parenthesis(start_token) == end_token)
	{
		printf("Removing parentheses: start = %p, end = %p\n", start_token, end_token);
		start_token = get_rid_of_first_parenthesis(shell, start_token, &middle, &end_token);
		printf ("got rid of parens\n");
		printf ("start token is %p and end token is %p\n", start_token, end_token);
		if (end_token)
		{
			if (start_token->id == PAR_OPEN && middle->id == PAR_CLOSE && get_matching_parenthesis(start_token) == middle)
			{
				process_arithmetic_expansion(shell, start_token, end_token, middle);
				return (init_leaf_node (shell, start_token, middle));
			}
			else
				return (make_tree(shell, start_token, end_token));
		}
	}
	log_op_token = find_last_log_op_token_nip(start_token, end_token);
	printf("Found log_op: %s\n", log_op_token ? log_op_token->str : "NULL");
	if (!log_op_token)
	{

		printf("Creating CMD node from %s to %s\n", start_token->str, end_token->str);
		return (init_leaf_node(shell, start_token, end_token));
	}
	subtree = init_tree_node(shell, log_op_token);
	printf("Creating tree node with log_op: %s\n", log_op_token->str);
	//divide_token_list(shell, start_token, log_op_token, &left_head, &right_head);

	printf ("doing LEFT\n");
	printf ("start %s , end %s\n", start_token->str, non_null_previous(start_token ,log_op_token)->str);
	//subtree->left = make_tree(shell, left_head, non_null_previous(start_token, log_op_token));
	subtree->left = make_tree(shell, start_token, non_null_previous(start_token ,log_op_token));
	printf ("now right\n");
	printf ("start %s , end %s\n", log_op_token->next->str, end_token->str);
	//printf ("right head is %s and end_token is %s\n", right_head->str, end_token->str);
	//subtree->right = make_tree(shell, right_head, end_token);
	subtree->right = make_tree(shell, log_op_token->next, end_token);
	return subtree;
}
