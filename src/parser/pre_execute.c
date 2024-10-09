
#include "../../include/minishell.h"

int pre_execute(t_shell *shell, t_tree *tree_node, t_tree *parent_tree_node, int prev_exit_code) 
{
    int exit_code = prev_exit_code; // Start with the previous exit code

    // fprintf (stderr, "   started pre_execute\n");
    // fprintf (stderr, "current node is %p\n", tree_node);

	if (tree_node == NULL)
		return exit_code;

	if (tree_node->type == T_PIPE)
	{
		// fprintf(stderr, "We're going in T_PIPE handling case\n");
		return (handle_pipe_subtree(shell, tree_node));
	}
	if (tree_node->left != NULL)
    {
        parent_tree_node = tree_node;
        exit_code = pre_execute(shell, tree_node->left, parent_tree_node, exit_code);
    }

    if (tree_node->type == CMD)
    {
        // t_cmd *cmd = (t_cmd *)tree_node->cmd_list;
        // fprintf (stderr, "cmd is %p\n", cmd);
        // fprintf(stderr, "Command args:\n");
        // int i = 0;
        // while (cmd->args && cmd->args[i] != NULL)
        // {
        //     fprintf(stderr, "Arg[%d] = %s\n", i, cmd->args[i]);
        //     i++;
        // }

        // fprintf(stderr, "STARTING MAKING EXPANSIONS\n");
        tree_node->start_token = expand(shell, tree_node->start_token, tree_node->end_token, shell->env_list);
        // fprintf(stderr, "FINISHED MAKING EXPANSIONS\n");
	    make_cmd(shell, tree_node->start_token, tree_node->end_token);
        tree_node->cmd = shell->cmd;
        // fprintf(stderr, "FINISHED MAKING CMDS\n");
        if (open_redirections(shell, tree_node->cmd) == SUCCESS)
            exit_code = executor(shell, tree_node->cmd);
        else
            exit_code = 1;
        //print_cmd(tree_node->cmd);
        // handle_redirs(shell, tree_node->cmd_list); //Still to implement.
        // t_cmd *cmd = (t_cmd *)tree_node->cmd_list;
        // fprintf (stderr, "cmd is %p\n", cmd);
        // fprintf(stderr, "Command args:\n");
        // int i = 0;
        // while (cmd->args && cmd->args[i] != NULL)
        // {
        //     fprintf(stderr, "Arg[%d] = %s\n", i, cmd->args[i]);
        //     i++;
        // }
        // print_cmd(tree_node->cmd_list);
        //print_tree_with_cmds(shell->tree, 0);
        
        
        // printf ("exit code is %d\n", exit_code);
    }
    if (parent_tree_node && parent_tree_node->type == T_AND_OPR && exit_code == 0) 
    {
		// fprintf (stderr, "performing the right side of AND\n");
		// if (tree_node->right)
		// {
		// 	if (tree_node->right->type == T_PIPE)
		// 		// return 1;
		// 		return (handle_pipe_subtree(shell, tree_node));
		// 	else
		// 		return pre_execute(shell, parent_tree_node->right, parent_tree_node, exit_code);
        //         // return pre_execute(shell, parent_tree_node->right, tree_node, exit_code);
		// }

		//Old version
		if (tree_node->right)
			return pre_execute(shell, parent_tree_node->right, parent_tree_node, exit_code);
	}
    else if (parent_tree_node && parent_tree_node->type == T_OR_OPR && exit_code != 0)
    {
        // fprintf (stderr, "performing the right side of OR");
        // if (tree_node->right)
        // {
		// 	if (tree_node->right->type == T_PIPE)
		// 		// return 1;
		// 		return (handle_pipe_subtree(shell, tree_node));
		// 	else
		// 		return pre_execute(shell, parent_tree_node->right, parent_tree_node, exit_code);
        //         // return pre_execute(shell, parent_tree_node->right, tree_node, exit_code);
		// }

		//Old version
		if (tree_node->right)
			return pre_execute(shell, parent_tree_node->right, parent_tree_node, exit_code);
	}
    return exit_code;
}
