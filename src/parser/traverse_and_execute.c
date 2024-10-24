#include "../../include/minishell.h"

int	handle_logical_operators(t_shell *shell, t_tree *tree_node, t_tree *parent_tree_node, int exit_code)
{
	if (parent_tree_node && parent_tree_node->type == T_AND_OPR \
	&& exit_code == 0)
	{
		if (tree_node->right)
			return (traverse_tree_and_execute \
			(shell, parent_tree_node->right, parent_tree_node, exit_code));
	}
	else if (parent_tree_node && parent_tree_node->type == T_OR_OPR \
	&& exit_code != 0)
	{
		if (tree_node->right)
			return (traverse_tree_and_execute \
			(shell, parent_tree_node->right, parent_tree_node, exit_code));
	}
	return (exit_code);
}

int	execute_command(t_shell *shell, t_tree *tree_node)
{
	expand(shell, tree_node->start_token, tree_node->end_token, shell->env_list);
	make_cmd(shell, &tree_node->cmd, tree_node->start_token, tree_node->end_token);
	if (open_redirections(shell, tree_node->cmd) == SUCCESS)
		return (executor(shell, tree_node->cmd));
	else
		return (FAILURE);
}

int	traverse_tree_and_execute(t_shell *shell, t_tree *tree_node, t_tree *parent_tree_node, int prev_exit_code)
{
	int	exit_code;

	exit_code = prev_exit_code;
	if (!tree_node)
		return (exit_code);
	if (tree_node->type == T_PIPE)
		return (handle_pipe_subtree(shell, tree_node));
	if (tree_node->left)
	{
		parent_tree_node = tree_node;
		exit_code = traverse_tree_and_execute \
		(shell, tree_node->left, parent_tree_node, exit_code);
	}
	if (tree_node->type == CMD)
		exit_code = execute_command(shell, tree_node);
	return (handle_logical_operators \
	(shell, tree_node, parent_tree_node, exit_code));
}
