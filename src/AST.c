#include "../include/minishell.h"


t_token *token_before(t_token *start_token, t_token *before_what)
{
	t_token		*return_token;

	return_token = start_token;
	while (return_token->next && return_token->next != before_what)
	{
		return_token = return_token->next;
		//printf ("return_token is |%s|\n", return_token->str);
	}
	printf("the \"token_before\" we are returning is: |%s|\n", return_token->str);
	return (return_token);
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
		{
		// If node->token is NULL, print "NULL"
			printf("NULL\n");
			exit(EXIT_FAILURE);
		}

	// Print the left subtree
	print_tree(node->left, level + 1);

	// Print the right subtree
	print_tree(node->right, level + 1);
}





t_token *get_rid_of_first_parenthesis(t_token *start_token, t_token **end_token)
{
	t_token *iterator;
	int		parentheses;
	
	printf ("Reached get rid of parens\n");
	printf ("start is %s and end is %s\n", start_token->str, (*end_token)->str);
	
	//start_token = get_after_space_token(start_token);
	iterator = start_token;
	parentheses = 1;
	while (iterator->next && parentheses !=0)
	//while (iterator && iterator->next && iterator->next != *end_token)
	{
		printf ("parentheses  is %d\n", parentheses);
		printf ("iterator->next is %s\n", iterator->next->str);
		if (iterator->next->id == PAR_OPEN)
		{
			printf ("found open par\n");
			parentheses++;
		}
			else if (iterator->next->id == PAR_CLOSE)
		{
			parentheses--;
			if (parentheses == 0)
			{
				printf ("found close par\n");
				if (iterator->next == *end_token)
					*end_token = iterator;
				printf ("start_token->next is %s, and iterator->id is %s\n", start_token->next->str, iterator->str);
				// if (start_token->next->id == PAR_OPEN && iterator->id == PAR_CLOSE)
				// {
				// 	printf("arithmetic expantion\n");
				// 	start_token->next->id = ARITH_EXPAN;
				// 	start_token->next->str = "((";
				// 	iterator->id = ARITH_EXPAN;
				// 	iterator->str = "))";
				// 	print_token(start_token);
				// }
				start_token = remove_token (start_token, start_token);
				start_token = remove_token (start_token, iterator->next);
				printf ("print token list after amend");
				print_token (start_token);
				break;
			}
		}
		iterator = iterator->next;
	}
	return (start_token);
}


t_token *find_last_log_op_token_nip(t_token *token_head, t_token *end_token)
{
	t_token *token_iterator;
	t_token *return_token;
	int parenthesis;

	return_token = NULL;
	token_iterator = token_head;
	parenthesis = 0;
	// if (token_head->id == PAR_OPEN && end_token->id == PAR_CLOSE)
	// 	token_head = get_rid_of_outer_most_parenthesis(token_head, &end_token);
	while (token_iterator && token_iterator != end_token)
	{
		//printf ("in find last op nip, token is %s and par is %d\n", token_iterator->str, parenthesis);
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
	leaf_node->type = CMD;
	leaf_node->start_token = start_token;
	while (end_token->id == SPACE_CHAR)
		end_token = token_before (start_token, end_token);
	leaf_node->end_token = end_token;
	leaf_node->left = NULL;
	leaf_node->right = NULL;
	//printf ("in init_leaf_node, it is:\n");
	//print_tree(leaf_node, 0);
	return leaf_node;
}


t_tree *init_tree_node(t_token *op_token)
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

t_tree *create_log_op_subtree(t_shell *shell, t_token *log_op, t_token *start_token, t_token *end_token)
{
	t_token *left_head;
	t_token *right_head;
	t_tree	*subtree;
	
	left_head = NULL;
	right_head = NULL;
	printf ("in create_log_op_subtree\n");
	subtree = init_tree_node(log_op);
	divide_token_list(start_token, log_op, &left_head, &right_head);
	printf ("LEFT CHILD:\n");
	printf ("in create sub tree left head is |%s|, token_before(start_token,log_op) is |%s|\n", left_head->str, token_before(start_token, log_op)->str);
	subtree->left = make_tree(shell, left_head, token_before(start_token, log_op));
	printf ("And now RIGHT CHILD:\n");
	printf ("in create sub tree right head is %s, end_token is %s\n", right_head->str, end_token->str);
	subtree->right = make_tree(shell, right_head, end_token);
	return (subtree);
}

t_tree *make_tree(t_shell *shell, t_token *start_token, t_token *end_token)
{
	t_token *log_op_token;
	t_token *left_head;
	t_token *right_head;
	t_tree *subtree;

	if (start_token == NULL || end_token == NULL)
		return NULL;

	if (start_token->id == PAR_OPEN && end_token->id == PAR_CLOSE)
	// if (start_token->id == PAR_OPEN)
	{
		printf("Removing parentheses: start = %s, end = %s\n", start_token->str, end_token->str);
		start_token = get_rid_of_first_parenthesis(start_token, &end_token);
		if (start_token->id == PAR_OPEN && end_token->id == PAR_CLOSE)
			{
				printf("arithmetic expantion\n");
				start_token->str = "((";
				end_token->str = "))";
				return init_leaf_node (start_token, end_token);
			}
		else	
		return make_tree(shell, start_token, end_token);
	}
	
	// Find the last logical operator not in parentheses
	log_op_token = find_last_log_op_token_nip(start_token, end_token);
	printf("Found log_op: %s\n", log_op_token ? log_op_token->str : "NULL");

	// If no logical operator is found, treat the range of tokens as a command
	if (!log_op_token)
	{
		// Create a leaf node that groups all tokens from start_token to end_token
		printf("Creating CMD node from %s to %s\n", start_token->str, end_token->str);
		return init_leaf_node(start_token, end_token);
	}

	// If a logical operator is found, create a subtree
	subtree = init_tree_node(log_op_token);
	printf("Creating tree node with log_op: %s\n", log_op_token->str);

	// Divide the token list into left and right parts
	divide_token_list(start_token, log_op_token, &left_head, &right_head);

	// Recursively create left and right subtrees
	subtree->left = make_tree(shell, left_head, token_before(start_token, log_op_token));
	subtree->right = make_tree(shell, right_head, end_token);

	return subtree;
}
