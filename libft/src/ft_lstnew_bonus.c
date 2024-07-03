/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_lstnew_bonus.c                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: lprieri <lprieri@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/10/16 16:44:57 by lprieri       #+#    #+#                 */
/*   Updated: 2024/02/02 16:36:10 by lprieri       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

/*
* Function name: ft_lstnew
* Description: Allocates (with malloc(3)) and returns a new node.
*	The member variable ’content’ is initialized with
*	the value of the parameter ’content’.
*	The variable ’next’ is initialized to NULL.
* External functs.: malloc
* Return value: The new node
* Parameters:
*	content: The content to create the node with.
*/

#include "../libft.h"

t_list	*ft_lstnew(void *content)
{
	t_list	*new_list;

	new_list = (t_list *) malloc (sizeof(t_list));
	if (new_list == NULL)
		return (NULL);
	(*new_list).content = content;
	new_list->next = NULL;
	return (new_list);
}

/*
* Test 1
*/

// int	main(void)
// {
// 	char	*str;
// 	t_list	*new_list;

// 	str = "Content";
// 	new_list = ft_lstnew(str);
// 	printf("%s\n", (char *)new_list->content);
// 	free(new_list);
// }
