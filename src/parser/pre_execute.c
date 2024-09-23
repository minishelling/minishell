
#include "../../include/minishell.h"

int execute(t_shell *shell, t_tree *tree_node, t_tree *parent_tree_node, int prev_exit_code) 
{
    int exit_code = prev_exit_code; // Start with the previous exit code

    // printf ("   started execute\n");
    // printf ("current node is %p\n", tree_node);

    if (tree_node == NULL) 
        return exit_code;

    if (tree_node->left != NULL)
    {
        parent_tree_node = tree_node;
        exit_code = execute(shell, tree_node->left, parent_tree_node, exit_code);
    }

    if (tree_node->type == CMD)
    {
        // t_cmd *cmd = (t_cmd *)tree_node->cmd_list;
        // printf ("cmd is %p\n", cmd);
        // printf("Command args:\n");
        // int i = 0;
        // while (cmd->args && cmd->args[i] != NULL)
        // {
        //     printf("Arg[%d] = %s\n", i, cmd->args[i]);
        //     i++;
        // }

        tree_node->start_token = expand(tree_node->start_token, tree_node->end_token, shell->env_list);
	    tree_node->cmd_list = make_cmd(shell, tree_node->start_token, tree_node->end_token);
        //print_tree_with_cmds(shell->tree, 0);
        exit_code = executor(shell, tree_node->cmd_list);
        // printf ("exit code is %d\n", exit_code);
    }
    if (parent_tree_node && parent_tree_node->type == T_AND_OPR && exit_code == 0) 
    {
        // printf ("performing the right side of AND\n");
        if (tree_node->right)
            return execute(shell, parent_tree_node->right, tree_node, exit_code);
    }
    else if (parent_tree_node && parent_tree_node->type == T_OR_OPR && exit_code != 0) 
    {
        // printf ("performing the right side of OR");
        if (tree_node->right)
            return execute(shell, parent_tree_node->right, tree_node, exit_code);
    }

    return exit_code;
}

