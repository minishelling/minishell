#include "../../include/minishell.h"

void free_tree(t_tree *node)
{
    if (node == NULL)
        return;

    // Free left and right subtrees
    free_tree(node->left);
    free_tree(node->right);

    // Free the cmd_list
    //  if (node->type == CMD)
    //    free_cmd_list(node->cmd_list);

    // Finally, free the node itself
    free(node);
}
