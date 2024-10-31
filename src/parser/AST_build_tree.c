#include "../../include/minishell.h"

t_tree			*make_tree(t_shell *shell, t_token *start_token, \
	t_token *end_token);
static t_tree	*handle_parens(t_shell *shell, t_token *start_token, \
	t_token *end_token);
static t_token	*find_last_log_op_token_nip(t_token *list, t_token *end_token);
t_tree			*init_leaf_node(t_shell *shell, t_token *start_token, \
	t_token *end_token);
static t_tree	*init_tree_node(t_shell *shell, t_token *op_token);

/**
 * @brief Functions for constructing an Abstract Syntax Tree (AST).
 *
 * Those functions in the file build an AST by recursively looking for 
 * the last logical operator (AND_OPR, OR_OPR, or PIPE) that is not 
 * enclosed in parentheses. If the entire expression is encapsulated 
 * in parentheses, it removes the outer parentheses and checks for 
 * arithmetic expansions that are not handled by this code.
 *
 * The code prioritizes finding the last AND_OPR or OR_OPR that is 
 * not in any parentheses, making this operator the parent of the 
 * expressions on the left and right (which can be other operators 
 * or commands). If no AND_OPR or OR_OPR that is not in parentheses 
 * is found, the last PIPE operator will be selected as the parent, 
 * ensuring that logical operations take precedence in the AST 
 * structure.
 *
 * If no logical operators are found, the function will create a 
 * leaf node representing a command.
 *
 * If a logical operator is found, the expression is divided into 
 * two parts: the left part (tokens before the logical operator) and 
 * the right part (tokens after the logical operator). The function 
 * is then called recursively to create subtrees for each of these 
 * parts.
 *
 * @param shell Pointer to the shell structure.
 * @param start_token Pointer to the first token in the sequence.
 * @param end_token Pointer to the last token in the sequence.
 * @return Pointer to a tree node representing the AST for the 
 * given token sequence, or NULL if no valid parentheses are found.
 */
t_tree	*make_tree(t_shell *shell, t_token *start_token, t_token *end_token)
{
	t_tree	*subtree;
	t_token	*log_op_token;

	if (start_token == NULL || end_token == NULL)
		return (NULL);
	subtree = handle_parens(shell, start_token, end_token);
	if (subtree)
		return (subtree);
	log_op_token = find_last_log_op_token_nip(start_token, end_token);
	if (!log_op_token)
		return (init_leaf_node(shell, start_token, end_token));
	subtree = init_tree_node(shell, log_op_token);
	subtree->left = make_tree(shell, start_token, \
	non_null_previous(start_token, log_op_token));
	subtree->right = make_tree(shell, log_op_token->next, end_token);
	return (subtree);
}

/**
 * @brief Handles the parsing of token sequences enclosed in parentheses.
 *
 * This function checks if the given token sequence is enclosed in matching 
 * parentheses. If so, it ignores the outer parentheses. If the entire 
 * expression is still encapsulated in parentheses after ignoring the 
 * outer ones, it assumes there are arithmetic expansions within them, 
 * which will create a special command that will not execute. In the case 
 * that the expression was enclosed by one pair of parentheses, it returns 
 * the expression without the parentheses so that the next recursion can 
 * look for logical operators. If the expression is not encapsulated, 
 * the function proceeds to look for logical operators.
 *
 * @param shell Pointer to the shell structure.
 * @param start_token Pointer to the first token in the sequence.
 * @param end_token Pointer to the last token in the sequence.
 * @return Pointer to a tree node representing the content within the 
 * parentheses, or NULL if the parentheses are not matching.
 */
static t_tree	*handle_parens(t_shell *shell, t_token *start_token, \
	t_token *end_token)
{
	if (start_token->id == PAR_OPEN && end_token->id == PAR_CLOSE \
		&& get_matching_paren(start_token) == end_token)
	{
		start_token = ignore_first_parens(start_token, &end_token);
		if (end_token)
		{
			if (start_token->id == PAR_OPEN && end_token->id == PAR_CLOSE \
			&& get_matching_paren(start_token) == end_token)
			{
				process_arith_expan(shell, start_token, end_token);
				return (init_leaf_node(shell, start_token, end_token));
			}
			else
				return (make_tree(shell, start_token, end_token));
		}
	}
	return (NULL);
}

/**
 * @brief Finds the last logical operator token in a list that is not 
 * enclosed in parentheses.
 *
 * This function iterates through a list of tokens to identify the last 
 * occurrence of a logical operator (AND_OPR or OR_OPR) that is not 
 * within any parentheses. If no AND_OPR or OR_OPR tokens are found, 
 * it then looks for the last PIPE token that is also not enclosed in 
 * parentheses. The function ensures that it correctly tracks the 
 * nesting of parentheses while searching for the appropriate token.
 *
 * @param list Pointer to the head of the token list to be searched.
 * @param end_token Pointer to the token at which to stop searching.
 * @return Pointer to the last found logical operator token (AND_OPR, 
 * OR_OPR, or PIPE) that is not within parentheses, or NULL if no 
 * suitable token is found.
 */
static t_token	*find_last_log_op_token_nip(t_token *list, t_token *end_token)
{
	t_token	*token_iterator;
	t_token	*return_token;
	int		parenthesis;

	return_token = NULL;
	parenthesis = 0;
	token_iterator = list;
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

/**
 * @brief Initializes a leaf node for a command in the Abstract Syntax Tree (AST).
 *
 * This function allocates memory for a new leaf node and initializes it 
 * with the specified command token information. The leaf node represents 
 * a command in the AST, storing the start and end tokens of the command. 
 * The left and right child pointers of the node are initialized to NULL, 
 * indicating that this is a leaf node with no further branches.
 *
 * @param shell Pointer to the shell context for error handling.
 * @param start_token Pointer to the token that marks the beginning of the command.
 * @param end_token Pointer to the token that marks the end of the command.
 * @return Pointer to the newly initialized leaf node, or exits if memory allocation fails.
 */
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
	leaf_node->end_token = end_token;
	leaf_node->left = NULL;
	leaf_node->right = NULL;
	return (leaf_node);
}

/**
 * @brief Initializes a tree node for an operator in the Abstract Syntax Tree (AST).
 *
 * This function allocates memory for a new tree node and initializes it 
 * with the specified operator token. The tree node represents a logical 
 * operator (AND, OR, or PIPE) in the AST, and its type is set based on 
 * the provided operator token. The left and right child pointers of the 
 * node are initialized to NULL, indicating that this node is a parent node 
 * with no children yet.
 *
 * @param shell Pointer to the shell context for error handling.
 * @param op_token Pointer to the token representing the operator.
 * @return Pointer to the newly initialized tree node, or exits if memory 
 * allocation fails or if the operator token is NULL.
 */
static t_tree	*init_tree_node(t_shell *shell, t_token *op_token)
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
