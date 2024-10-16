#include "../../include/minishell.h"

/**
 * @brief Frees the memory allocated for a binary tree and its commands.
 * 
 * This function recursively frees the nodes of a binary tree. If a node
 * contains a command (node->type == CMD), it will also free the command
 * using `free_cmd`. After freeing the tree and its contents, it sets the
 * pointer to NULL to avoid dangling pointers.
 * 
 * @param node A double pointer to the root of the tree. After freeing, the
 * node will be set to NULL.
 */
void free_tree(t_tree **node)
{
	if (*node == NULL)
		return;
	free_tree(&(*node)->left);
	free_tree(&(*node)->right);
	if ((*node)->type == CMD)
		free_cmd(&(*node)->cmd);
	free(*node);
	*node = NULL;
}
