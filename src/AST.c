#include "../include/minishell.h"


t_token *token_before(t_token *start_token, t_token *before_what)
{
	t_token		*return_token;

	return_token = start_token;
	while (return_token->next && return_token->next != before_what)
		return_token = return_token->next;
	if (return_token->id == SPACE_CHAR
		&& get_after_space_token(return_token) == before_what)
	{
		return_token = token_before(start_token, return_token);
	}
	printf("token_before we are returning is: %s\n", return_token->str);
	return (return_token);
}

t_token *find_matching_paren(t_token *start_token)
{
	int parenthesis = 0;
	t_token *current = start_token;

	while (current)
	{
		if (current->id == PAR_OPEN)
			parenthesis++;
		else if (current->id == PAR_CLOSE)
			parenthesis--;

		if (parenthesis == 0)
			return current;

		current = current->next;
	}

	return NULL; // No matching parenthesis found
}


void print_tree(t_tree *node, int level) 
{
	int i;
	char	*tree_node_name[3] = 
	{
		[0] = "AND_OPR",
		[1] = "OR_OPR",
		[2] = "CMD",
	};
    if (!node)
	{
        return;
	}
	i = 0;
    // Print indentation for the current level
    while (i < level)
    {
        printf("    ");
		i++;
    }
	
    // Print the current node's value
	if (node->type == CMD)
    	printf("%s |%s| |%s|\n", tree_node_name[node->type], node->start_token->str, node->end_token->str);
	else if (node->type == T_AND_OPR || node->type == T_OR_OPR)
    // If node->token is not NULL, print the token's value
		printf("%s\n",  tree_node_name[node->type]);
	else 
    // If node->token is NULL, print "NULL"
    	printf("NULL\n");

    // Print the left subtree
    print_tree(node->left, level + 1);

    // Print the right subtree
    print_tree(node->right, level + 1);
}

bool is_leaf(t_tree *node) {
	return (node->left == NULL && node->right == NULL);
}

// t_token *get_rid_of_inner_most_parenthesis(t_token *start_token, t_token **end_token)
// {
// 	t_token *iterator;
// 	t_token *inner_most_open_paren;

// 	iterator = start_token;
// 	inner_most_open_paren = NULL;
// 	while (iterator)
// 	{
// 		if (iterator->id == PAR_OPEN)
// 		{
// 			inner_most_open_paren = iterator;
// 			printf("Found token\n");
// 			iterator = iterator->next;
// 		}
// 		else if (iterator->id == PAR_CLOSE)
// 		{
// 			*end_token = token_before(inner_most_open_paren, iterator);
// 			//iterator-> next = NULL; // should be done temporarily
// 			printf ("found closing, and breaking now\n");
// 			break ;
// 		}
// 		else
// 			iterator = iterator->next;
// 	}
// 	printf("We're printing in inner_most_parenthesis\n");
// 	printf ("inner most_opern_paren is %s\n", inner_most_open_paren->str);
// 	return (get_after_space_token( inner_most_open_paren));
// }

// t_token *get_rid_of_parenthesis(t_token *start_token, t_token **end_token)
// {
	
// }

t_token *get_rid_of_outer_most_parenthesis(t_token *start_token, t_token **end_token)
{
	t_token *iterator;
	int		parentheses;
	
	printf ("Reached get rid of parens\n");
	printf ("start is %s and end is %s\n", start_token->str, (*end_token)->str);
	start_token = get_after_space_token(start_token);
	iterator = start_token;
	parentheses = 0;
	//while (iterator && iterator->next && iterator->next->id != PAR_CLOSE)
	while (iterator && iterator->next && iterator->next != *end_token)
	{
		printf ("parentheses  is %d\n", parentheses);
		printf ("iterator is %s\n", iterator->str);
		if (iterator->next->id == PAR_OPEN)
		{
			printf ("found open par\n");
			parentheses++;
		}
		iterator = iterator->next;
	}
	if (iterator->next->id != PAR_CLOSE)
	{
		printf("in get_rid_of... didn't find a closing paren\n");
		return (NULL); //Syntax error??
	}
	//printf ("parentheses is %d\n", parentheses);
	while (iterator && iterator->next && parentheses != 0)
	{
		
		if (iterator->next->id == PAR_CLOSE)
			parentheses--;
		if (parentheses == 0)
			break ;
		iterator = iterator->next;
	}
	printf("Parentheses in get_rid_of_outer...: %i\n", parentheses);
	if (parentheses == 0)
	{
		if (iterator->id == SPACE_CHAR)
			*end_token = token_before(start_token, iterator->next);
		else
		{
			printf ("	the toke before is not a space\n");
			printf("Iterator is: %s\n", iterator->str);
			*end_token = iterator;
			// *end_token = token_before(start_token, iterator);
			// *end_token = token_before(start_token, iterator->next);
		}
		return (start_token);
	}
	printf("in get_rid_of... didnt close all parens\n");
	return (NULL); //Syntax error again
}


t_token *find_last_log_op_token_nip(t_token *token_head, t_token *end_token)
{
	t_token *token_iterator;
	t_token *return_token;
	int parenthesis;

	return_token = NULL;
	token_iterator = token_head;
	parenthesis = 0;
	if (token_head->id == PAR_OPEN && end_token->id == PAR_CLOSE)
		token_head = get_rid_of_outer_most_parenthesis(token_head, &end_token);
	while (token_iterator && token_iterator != end_token)
	{
		if (token_iterator->id == PAR_OPEN)
			parenthesis++;
		else if (token_iterator->id == PAR_CLOSE)
			parenthesis--;
		else if ((token_iterator->id == AND_OPR || token_iterator->id == OR_OPR)
				&& parenthesis == 0)
			return_token = token_iterator;
		token_iterator= token_iterator -> next;
	}
	return (return_token);
}

void divide_token_list(t_token *token_list, t_token *op_token, t_token **left_head, t_token **right_head)
{
	t_token *current = token_list;

	// Initialize the left head to the start of the list
	*left_head = token_list;

	// Traverse the list to find the logical operator
	while (current && get_after_space_token(current) != op_token) 
	{
		current = get_after_space_token(current);
		//printf ("Current is %s\n", current->str);
	}

	// If we found the logical operator, split the list
	if (current && get_after_space_token(current) == op_token) 
	{
		// Terminate the left list
		
		current->next = NULL;
		// Set the right head to the token after the logical operator
		*right_head = get_after_space_token(op_token);
	}
}

t_tree *init_leaf_node(t_token *start_token, t_token *end_token)
{
	t_tree *leaf_node;
	
	leaf_node = (t_tree *)malloc(sizeof(t_tree));
	if (!leaf_node)
	{
		perror("malloc");
		exit(EXIT_FAILURE);
	}
   
	leaf_node->type = CMD;  // Indicate it's a command node
	
	leaf_node->start_token = start_token;  // Assign start_token
   
	leaf_node->end_token = end_token;      // Assign the adjusted end_token
	leaf_node->left = NULL;
	leaf_node->right = NULL;
	printf ("in init_leaf_node, it is:\n");
	print_tree(leaf_node, 0);
	return leaf_node;
}


t_tree *init_tree_node(t_token *op_token)
{
	t_tree *tree_node;

	tree_node = (t_tree *)malloc(sizeof(t_tree));
	if (!tree_node) 
	{
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	if (op_token->id == AND_OPR)
		tree_node->type = T_AND_OPR;
	else if (op_token->id == OR_OPR)
		tree_node->type = T_OR_OPR;
	tree_node->left = NULL;
	tree_node->right = NULL;

	return tree_node;
}

t_tree *handle_parentheses(t_token *start_token, t_token *end_token)
{
	t_tree *subtree;

	subtree = NULL;
	
	if (!start_token)
	{
		printf("Unfortunately start_token disappointed us with a NULL value\n");
		return (NULL);
	}
	start_token = get_rid_of_outer_most_parenthesis(start_token, &end_token);
	// start_token = sever_parenthesis();
	printf("start_token: %s, end_token: %s\n",start_token->str, end_token->str);
	// print_token(start_token);
	subtree = make_tree(start_token, end_token);
	printf("In handle_parenthesis, printing the subtree\n");
	print_tree(subtree, 0);
	// Temporarily break the list to isolate the tokens inside the parentheses
	//printf ("closing token is %s\n", closing_token->str);
	// t_token *after_paren =  get_after_space_token(closing_token);
	// closing_token->next = NULL;

	// printf ("start_token is %s and start_token->next is %s\n", start_token->str, start_token->next->str);
	// Recursively build the subtree from the tokens inside the parentheses
	// if (!(start_token->next->id == PAR_OPEN))
	// 	subtree = make_tree(start_token->next, end_token);
	// else
	// {
	// 	if (subtree == NULL)
	// 		printf("We're sorry, we're returning null\n");
	// }

	// Restore the connection and return the subtree
	// closing_token->next = after_paren;
	return subtree;
}


t_tree *make_tree(t_token *start_token, t_token *end_token)
{
	// t_token	*end_token = NULL;
	if (!start_token)
		return NULL;
	 printf ("reached make_tree\n");
	// Find the last logical operator (root of the current subtree)
	if (start_token->id == PAR_OPEN && end_token->id == PAR_CLOSE)
		start_token = get_rid_of_outer_most_parenthesis(start_token, &end_token);
	printf ("After get rid of in make tree: start token is %s and end_token %s\n", start_token->str, end_token->str); // this is not correct
	t_token *log_op = find_last_log_op_token_nip(start_token, end_token);
	if (log_op)
		printf ("LOG_OP IS %s\n", log_op->str);
	// Base case: No logical operator, return a leaf node
	printf ("before !log_op: start token is %s and end_token %s\n", start_token->str, end_token->str);
	if (!log_op)
	{
		return init_leaf_node(start_token, end_token);  
	}
	// Create a new tree node for this logical operator
	t_tree *tree_node = init_tree_node(log_op);

	t_token *left_head = NULL;
	t_token *right_head = NULL;

	// Divide the token list into left and right parts
	divide_token_list(start_token, log_op, &left_head, &right_head);

	// Recursively build the left and right subtrees
	printf ("LEFT CHILD:\n");
	printf ("in make tree left head is %s, token_before(start_token,log_op) is %s\n", left_head->str, token_before(start_token, log_op)->str);
	tree_node->left = make_tree(left_head, token_before(start_token, log_op));
	printf ("And now RIGHT CHILD:\n");
	printf ("in make tree right head is %s, end_token is %s\n", right_head->str, end_token->str);
	tree_node->right = make_tree(right_head, end_token);

	return tree_node;
}

