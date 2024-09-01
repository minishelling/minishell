#include "../include/minishell.h"

void print_tree(t_tree *node, int level) 
{
    if (!node)
        return;

    // Print indentation for the current level
    for (int i = 0; i < level; i++) 
    {
        printf("    ");
    }

    // Print the current node's value
   	
	if (!ft_strncmp(node->type, "CMD", 3))
    	printf("%s |%s| |%s|\n", node->type, node->start_token->str, node->end_token->str);	
	else if (!ft_strncmp(node->type, "T_AND_OPR", 9) || !ft_strncmp(node->type, "T_OR_OPR", 8)) 
    // If node->token is not NULL, print the token's value
		printf("%s\n", node->type);
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

t_tree *init_leaf_node(t_token *start_token, t_token *end_token)
{
    t_tree *leaf_node;
	
    leaf_node = (t_tree *)malloc(sizeof(t_tree));
    if (!leaf_node) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    leaf_node->type = "CMD";  // Indicate it's a command node
    leaf_node->start_token = start_token;  // Assign start_token
    leaf_node->end_token = end_token;      // Assign end_token
    leaf_node->left = NULL;
    leaf_node->right = NULL;

    return leaf_node;
}

t_tree *init_tree_node(t_token *op_token)
{
	t_tree *tree_node;

	tree_node = (t_tree *)malloc(sizeof(t_tree));
	if (!tree_node) {
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	if (op_token->id == AND_OPR)
		tree_node->type = "T_AND_OPR";
	else if (op_token->id == OR_OPR)
		tree_node->type = "T_OR_OPR";
	else
	{
		tree_node->type = "CMD";
	}
	tree_node->left = NULL;
	tree_node->right = NULL;

	return tree_node;
}


t_tree *make_tree(t_token *token_list)
{
	if (!token_list)
		return NULL;

	// Find the last logical operator (root of the current subtree)
	t_token *log_op = find_last_log_op_token_nip(token_list);

	// Base case: No logical operator, return a leaf node
	   if (!log_op) 
	   {
			t_token *end_token = token_list;
			while (end_token->next) 
				end_token = end_token->next;  // Find the last token in the command
			return init_leaf_node(token_list, end_token);  // Pass start and end tokens
    	}

	// Create a new tree node for this logical operator
	t_tree *tree_node = init_tree_node(log_op);

	// Initialize left and right heads to hold the split lists
	t_token *left_head = NULL;
	t_token *right_head = NULL;

	// Divide the token list into left and right parts
	divide_token_list(token_list, log_op, &left_head, &right_head);
	printf (" log_op is %s, left_head is %s , right_head is %s\n",log_op->str, left_head->str, right_head->str);

	// Recursively build the left and right subtrees
	tree_node->left = make_tree(left_head);
	tree_node->right = make_tree(right_head);

	return tree_node;
}
