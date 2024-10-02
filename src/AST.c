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
	// printf("the \"token_before\" we are returning is: |%s|\n", return_token->str);
	return (return_token);
}


void print_tree_verbose(t_tree *node, int level)
{
	int i;
	char	*tree_node_name[4] = 
	{
		[0] = "AND_OPR",
		[1] = "OR_OPR",
		[2] = "CMD",
		[3] = "PIPE"
	};
	if (!node)
	{
		return;
	}
	i = 0;
	while (i < level)
	{
		printf("    ");
		i++;
	}
	
	if (node->type == CMD)
		printf(GREY_BACKGROUND"%s"RESET_COLOR" |%s| |%s|  %p\n", tree_node_name[node->type], node->start_token->str, node->end_token->str, node);
	else if (node->type == T_AND_OPR || node->type == T_OR_OPR || node->type == T_PIPE)
		printf(MAGENTA_TEXT"%s"RESET_COLOR" %p\n", tree_node_name[node->type], node);
	else
		{
			printf("NULL\n");
			exit(EXIT_FAILURE);
		}

	// Print the left subtree
	print_tree_verbose(node->left, level + 1);

	// Print the right subtree
	print_tree_verbose(node->right, level + 1);
}


void print_tree(t_tree *node, int level) 
{
	int i;
	char	*tree_node_name[4] = 
	{
		[0] = "AND_OPR",
		[1] = "OR_OPR",
		[2] = "CMD",
		[3] = "PIPE"
	};
	if (!node)
	{
		return;
	}
	i = 0;
	while (i < level)
	{
		fprintf(stderr, "    ");
		i++;
	}
	
	if (node->type == CMD)
		fprintf(stderr, GREY_BACKGROUND"%s"RESET_COLOR" |%s| |%s|\n", tree_node_name[node->type], node->start_token->str, node->end_token->str);
	else if (node->type == T_AND_OPR || node->type == T_OR_OPR || node->type == T_PIPE)
		fprintf(stderr, MAGENTA_TEXT"%s"RESET_COLOR"\n", tree_node_name[node->type]);
	else
		{
			fprintf(stderr, "NULL\n");
			exit(EXIT_FAILURE);
		}

	// Print the left subtree
	print_tree(node->left, level + 1);

	// Print the right subtree
	print_tree(node->right, level + 1);
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
	//while (iterator && iterator->next && iterator->next != *end_token)
	{
		//printf ("parentheses  is %d\n", parentheses);
		//printf ("iterator->next is %s\n", iterator->next->str);
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
				if (iterator->next == *end_token)
					*end_token = iterator;
				*middle = iterator;
				//printf ("*middle is %p\n", *middle);
				//printf ("in get rid start_token is %p, iterator->next is %p\n", start_token, iterator->next);
				start_token = remove_token (start_token, start_token);
				start_token = remove_token (start_token, iterator->next);
				//printf ("after getting rid of parens\n");
				//print_token (start_token);
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
    t_token *return_token = NULL;
    int parenthesis = 0;
	t_token *temp = token_head;
	while (temp)
	{
		//printf ("token_iterator->str is %s\n", temp->str);
		if (temp == end_token)
			break;
		temp = temp->next;
	}
	
    token_iterator = token_head;

    while (token_iterator && token_iterator != end_token)
    {
        // Adjust parentheses depth
        if (token_iterator->id == PAR_OPEN)
        {
            parenthesis++;
        }
        else if (token_iterator->id == PAR_CLOSE)
        {
            parenthesis--;
        }
        else if (parenthesis == 0)
        {	
			//printf ("token_iterator->str is %s\n", token_iterator->str);
            // Check for AND_OPR and OR_OPR first (higher precedence)
            if (token_iterator->id == AND_OPR || token_iterator->id == OR_OPR)
            {
                return_token = token_iterator; // Set return token to AND/OR
            }
            // Check for PIPE only if there was no higher precedence operator set
            else if (token_iterator->id == PIPE)
            {
                // Set pipe as the last logical operator, but only if no AND/OR has been found
                if (return_token == NULL)
                {
                    return_token = token_iterator;
                }
            }
        }
        token_iterator = token_iterator->next;
    }
    return return_token; // Return the last found logical operator
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
		if (op_token->next)
		{
			//printf ("beginning of right is %s\n", op_token->next->str);
			*right_head = op_token ->next;
		}
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
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	(void)shell;
	leaf_node->type = CMD;
	leaf_node->start_token = start_token;
	while (end_token->id == SPACE_CHAR)
		end_token = token_before (start_token, end_token);
	leaf_node->end_token = end_token;
	
	//shell->token = expand(start_token, end_token, shell->env_list);

	// leaf_node->cmd_list = make_cmd(shell, start_token, end_token);
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
	else if (op_token ->id == PIPE)
		tree_node->type = T_PIPE;
	tree_node->left = NULL;
	tree_node->right = NULL;
	return tree_node;
}



// bool contains_pipe_open_par(t_token *start_token, t_token *end_token) 
// {
//     t_token *token_iterator = start_token;

//     while (token_iterator)
// 	{
//         if (token_iterator == end_token)
// 			break;

//         if (token_iterator->id == PIPE && token_iterator->next && token_iterator->next->id == PAR_OPEN)
//             return (true);
//         token_iterator = token_iterator->next;
//     }
//     return (false);
// }


t_tree *make_tree(t_shell *shell, t_token *start_token, t_token *end_token)
{
	t_token *log_op_token;
	t_token *left_head;
	t_token *right_head;
	t_tree *subtree;
	t_token *middle = NULL;
	//printf ("making tree\n");
	if (start_token == NULL || end_token == NULL)
		return NULL;
	
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
				start_token->str = "((";
				start_token->id = ARITH_EXPAN;
				end_token->str = "))";
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
	subtree = init_tree_node(log_op_token);
	//printf("Creating tree node with log_op: %s\n", log_op_token->str);

	// Divide the token list into left and right parts
	divide_token_list(start_token, log_op_token, &left_head, &right_head);
	// Recursively create left and right subtrees
	//printf ("doing LEFT, left_head is %s\n", left_head->str);
	subtree->left = make_tree(shell, left_head, token_before(start_token, log_op_token));
	//printf ("now right\n");
	subtree->right = make_tree(shell, right_head, end_token);

	return subtree;
}
