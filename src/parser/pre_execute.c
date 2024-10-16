
#include "../../include/minishell.h"

int pre_execute(t_shell *shell, t_tree *tree_node, t_tree *parent_tree_node, int prev_exit_code) 
{
	int exit_code;
	exit_code = prev_exit_code;
	fprintf (stderr, "   started pre_execute\n");
	fprintf (stderr, "current node is %p\n", tree_node);
	if (tree_node == NULL)
		return exit_code;
	if (tree_node->type == T_PIPE)
		return (handle_pipe_subtree(shell, tree_node));
	if (tree_node->left != NULL)
	{
		parent_tree_node = tree_node;
		exit_code = pre_execute(shell, tree_node->left, parent_tree_node, exit_code);
	}
	if (tree_node->type == CMD)
	{
		// fprintf(stderr, "STARTING MAKING EXPANSIONS\n");
		tree_node->start_token = expand(shell, tree_node->start_token, tree_node->end_token, shell->env_list);
		// fprintf(stderr, "FINISHED MAKING EXPANSIONS\n");
		make_cmd(shell, tree_node->start_token);   //what shall we do if make_cmd fails? 
		tree_node->cmd = shell->cmd;
		// fprintf(stderr, "FINISHED MAKING CMDS\n");
		if (open_redirections(shell, tree_node->cmd) == SUCCESS)
			exit_code = executor(shell, tree_node->cmd);
		else
			exit_code = 1;

		print_cmd(tree_node->cmd);
		printf ("exit code is %d\n", exit_code);
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
	return (exit_code);
}
