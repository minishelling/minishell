/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   traverse_and_execute.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfeuer <tfeuer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 13:27:02 by tfeuer            #+#    #+#             */
/*   Updated: 2024/11/04 13:29:07 by tfeuer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int			traverse_tree_and_execute(t_shell *shell, t_tree *tree_node, \
	t_tree *parent_tree_node, int prev_exit_code);
static int	execute_command(t_shell *shell, t_tree *tree_node);
static int	handle_logical_operators(t_shell *shell, t_tree *tree_node, \
	t_tree *parent_tree_node, int exit_code);

/**
 * @brief Traverses the AST and executes commands based on tree nodes.
 *
 * This function recursively traverses the AST, executing commands and 
 * handling logical operators. It usually starts from the leftmost downward 
 * node unless the parent node is a PIPE, in which case the function 
 * traverses down that subtree. It evaluates the exit code of previously 
 * executed commands to decide whether to execute the right child of the 
 * AND_OPR or OR_OPR parent nodes.
 *
 * A successful execution of the left child with an AND_OPR parent, for 
 * example, will trigger execution of the right child. Conversely, for an 
 * OR_OPR parent, if the exit code is non-zero from the left child, the 
 * right child will be executed.
 *
 * @param shell Pointer to the shell structure.
 * @param tree_node Pointer to the current tree node being executed.
 * @param parent_tree_node Pointer to the parent tree node for context.
 * @param prev_exit_code The exit code from the previous command executed.
 * @return The updated exit code after executing the command(s).
 */
int	traverse_tree_and_execute(t_shell *shell, t_tree *tree_node, \
	t_tree *parent_tree_node, int prev_exit_code)
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

/**
 * @brief Executes a command represented by the given tree node.
 *
 * This function expands the tokens for the command, constructs the 
 * command structure, and handles any necessary redirections. The 
 * command will only be executed if:
 * 1. The token expansion succeeds.
 * 2. The command parsing (construction) is successful.
 * 3. The handling of redirections is successful.
 *
 * If all conditions are met, the command is executed, and the exit 
 * code is returned. If any of the steps fail, the function returns 
 * FAILURE without executing the command.
 *
 * @param shell Pointer to the shell structure.
 * @param tree_node Pointer to the tree node representing the command 
 *                  to be executed.
 * @return The exit code of the executed command, or FAILURE if the 
 *         command could not be executed.
 */
static int	execute_command(t_shell *shell, t_tree *tree_node)
{
	expand(shell, tree_node->start_token, tree_node->end_token, \
		shell->env_list);
	make_cmd(shell, &tree_node->cmd, tree_node->start_token, \
		tree_node->end_token);
	if (open_redirections(shell, tree_node->cmd) == SUCCESS)
	{
		if (tree_node->cmd && shell->debug_mode == ON)
			print_cmd(tree_node->cmd);
		return (executor(shell, tree_node->cmd));
	}
	else
		return (FAILURE);
}

/**
 * @brief Handles logical operators in the AST based on the exit code.
 *
 * This function checks the type of the parent tree node (AND_OPR or OR_OPR)
 * and evaluates the exit code of the previously executed command to decide
 * whether to execute the right child of the parent node.
 *
 * - If the parent is an AND_OPR and the exit code is 0 (successful), 
 *   the right child will be executed.
 * - If the parent is an AND_OPR and the exit code is non-zero (failure), 
 *   the right child will not be executed.
 * - If the parent is an OR_OPR and the exit code is non-zero (failure), 
 *   the right child will be executed.
 * - If the parent is an OR_OPR and the exit code is 0 (successful), 
 *   the right child will not be executed.
 *
 * @param shell Pointer to the shell structure.
 * @param tree_node Pointer to the current tree node being processed.
 * @param parent_tree_node Pointer to the parent tree node for context.
 * @param exit_code The exit code from the previously executed command.
 * @return The updated exit code after handling the logical operators.
 */
static int	handle_logical_operators(t_shell *shell, t_tree *tree_node, \
	t_tree *parent_tree_node, int exit_code)
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
