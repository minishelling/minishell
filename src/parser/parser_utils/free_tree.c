/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_tree.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfeuer <tfeuer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 13:25:59 by tfeuer            #+#    #+#             */
/*   Updated: 2024/10/31 13:26:00 by tfeuer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

void	free_tree(t_tree **node);

/**
 * @brief Recursively frees all nodes in a binary tree structure.
 *
 * This function performs a post-order traversal to free each node 
 * in the binary tree. It first frees the left and right subtrees 
 * and then the current node. If the node type is `CMD`, it uses 
 * `free_cmd` to release resources specific to command nodes.
 *
 * @param node Pointer to the root node of the tree to be freed.
 *             Sets the root pointer to `NULL` after freeing.
 */
void	free_tree(t_tree **node)
{
	if (*node == NULL)
		return ;
	free_tree(&(*node)->left);
	free_tree(&(*node)->right);
	if ((*node)->type == CMD)
		free_cmd(&(*node)->cmd);
	free(*node);
	*node = NULL;
}
