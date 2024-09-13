
#include "../../include/minishell.h"

int lisandro_execute(t_shell *shell, t_cmd *cmd_list) 
{
    int i;
    t_cmd *cmd = cmd_list;

    (void)shell;
    // Traverse the linked list of commands and print each command
    while (cmd != NULL) 
    {
        if (cmd->args)
            {
                printf("Executing command: ");
                i = 0;
                while (cmd->args[i] != NULL)
                {
                    if (i > 0) printf(" ");
                    printf("%s", cmd->args[i]);
                    i++;
                }
                printf("\n");
            }
        cmd = cmd->next;
    }
    return 1;
}

int ping_lisandro(t_shell *shell, t_tree *node, t_tree *parent_node) 
{
    int exit_code = 0;

    //printf ("   started ping Lisandro\n");
    if (node == NULL) 
        return exit_code;

    if (node->left != NULL)
    {   
        parent_node = node;
        exit_code = ping_lisandro(shell, node->left, node);  // Recursively handle left subtree
    }

    // If it's a command node, execute the commands
    if (node->type == CMD) 
    {
        //printf ("node is %s\n", node->cmd_list.args[0]);
        if (node->left)
            printf ("node->left is %s\n", node->left->cmd_list.args[0]);
        exit_code = lisandro_execute(shell, &(node->cmd_list));
        // printf ("exit code is %d\n", exit_code);
        // if (parent_node)
        //     printf ("parent_node_type is %d (and = 0, or = 1)\n", parent_node->type);
        // printf ("exit code is %d\n", exit_code);
        // if (parent_node->right)
        //     printf ("parent_node->right is %p\n", parent_node->right);
        // If successful (exit_code == 0) and parent is an AND operator, continue to the right
        if (parent_node && parent_node->type == T_AND_OPR && exit_code == 0 && parent_node->right != NULL) 
        {
          
            return ping_lisandro(shell, parent_node->right, node);  // Handle the right subtree
        }

        // If failed (exit_code != 0) and parent is an OR operator, continue to the right
        else if (parent_node && parent_node->type == T_OR_OPR && exit_code != 0 && parent_node->right != NULL) 
        {
            return ping_lisandro(shell, parent_node->right, node);  // Handle the right subtree
        }
    }

    // Finally, handle the right subtree (if not already handled by AND/OR conditions)
    // if (node->right != NULL) 
    // {
    //     exit_code = ping_lisandro(node->right, node);  // Recursively handle right subtree
    // }

    return exit_code;  // Return the final exit code
}
