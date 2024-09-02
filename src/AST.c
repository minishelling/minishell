#include "../include/minishell.h"

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


t_token *find_last_log_op_token_nip(t_token *token_head)
{
	t_token *token_iterator;
	t_token *return_token;
	int parenthesis;

	return_token = NULL;
	token_iterator = token_head;
	parenthesis = 0;
	while (token_iterator)
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
		printf ("Current is %s\n", current->str);
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

// t_tree *init_leaf_node(t_token *start_token, t_token *end_token)
// {
//     t_tree *leaf_node;
	
//     leaf_node = (t_tree *)malloc(sizeof(t_tree));
//     if (!leaf_node) 
// 	{
//         perror("malloc");
//         exit(EXIT_FAILURE);
//     }

//     leaf_node->type = CMD;  // Indicate it's a command node
//     leaf_node->start_token = start_token;  // Assign start_token
//     leaf_node->end_token = end_token;      // Assign end_token
//     leaf_node->left = NULL;
//     leaf_node->right = NULL;

//     return leaf_node;
// }

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

    // Adjust end_token to stop before any closing parentheses
    while (end_token && end_token->id == PAR_CLOSE)
        end_token = NULL;

    leaf_node->end_token = end_token;      // Assign the adjusted end_token
    leaf_node->left = NULL;
    leaf_node->right = NULL;

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


// t_tree *make_tree(t_token *start_token)
// {
// 	if (!start_token)
// 		return NULL;

// 	// Find the last logical operator (root of the current subtree)
// 	t_token *log_op = find_last_log_op_token_nip(start_token);

// 	// Base case: No logical operator, return a leaf node
// 	   if (!log_op) 
// 	   {
// 			t_token *end_token = start_token;
// 			while (get_after_space_token(end_token))
// 				end_token = get_after_space_token(end_token);  // Find the last token in the command
// 			return init_leaf_node(start_token, end_token);  // Pass start and end tokens
//     	}

// 	// Create a new tree node for this logical operator
// 	t_tree *tree_node = init_tree_node(log_op);

// 	// Initialize left and right heads to hold the split lists
// 	t_token *left_head = NULL;
// 	t_token *right_head = NULL;

// 	// Divide the token list into left and right parts
// 	divide_token_list(start_token, log_op, &left_head, &right_head);
// 	printf (" log_op is %s, left_head is %s , right_head is %s\n",log_op->str, left_head->str, right_head->str);

// 	// Recursively build the left and right subtrees
// 	tree_node->left = make_tree(left_head);
// 	tree_node->right = make_tree(right_head);

// 	return tree_node;
// }

t_tree *handle_parentheses(t_token *start_token)
{
    if (start_token->id != PAR_OPEN)
    {
        fprintf(stderr, "Error: Expected an open parenthesis, but found %s\n", start_token->str);
        return NULL;
    }

    // Find the matching closing parenthesis
    t_token *closing_token = find_matching_paren(start_token);
    if (closing_token == NULL)
    {
        fprintf(stderr, "Syntax error: unmatched parentheses\n");
        exit(EXIT_FAILURE);
    }

    // Temporarily break the list to isolate the tokens inside the parentheses
    t_token *after_paren =  get_after_space_token(closing_token);
    closing_token->next = NULL;

    // Recursively build the subtree from the tokens inside the parentheses
    t_tree *subtree = make_tree(start_token->next);

    // Restore the connection and return the subtree
    closing_token->next = after_paren;
    return subtree;
}

t_tree *make_tree(t_token *start_token)
{
    if (!start_token)
        return NULL;

    // If the current token is an open parenthesis, delegate to handle_parentheses
    if (start_token->id == PAR_OPEN)
    {
        t_tree *subtree = handle_parentheses(start_token);
		printf ("reached here\n");
        t_token *after_paren = get_after_space_token(find_matching_paren(start_token));
		printf ("token after paren %s\n", after_paren->str);

        if (after_paren)
        {
            // Continue processing after the parentheses
            t_tree *log_op_tree = make_tree(after_paren);
            if (log_op_tree)
            {
                log_op_tree->left = subtree;
                return log_op_tree;
            }
        }

        return subtree; // Return the subtree if no tokens follow the parentheses
    }

    // Find the last logical operator (root of the current subtree)
    t_token *log_op = find_last_log_op_token_nip(start_token);

    // Base case: No logical operator, return a leaf node
    if (!log_op)
    {
        t_token *end_token = start_token;
        while (get_after_space_token(end_token) && end_token->id != PAR_CLOSE)
            end_token = get_after_space_token(end_token);  // Find the last token in the command
        
        return init_leaf_node(start_token, end_token);  // Pass start and end tokens
    }

    // Create a new tree node for this logical operator
    t_tree *tree_node = init_tree_node(log_op);

    // Initialize left and right heads to hold the split lists
    t_token *left_head = NULL;
    t_token *right_head = NULL;

    // Divide the token list into left and right parts
    divide_token_list(start_token, log_op, &left_head, &right_head);

    // Recursively build the left and right subtrees
    tree_node->left = make_tree(left_head);
    tree_node->right = make_tree(right_head);

    return tree_node;
}

