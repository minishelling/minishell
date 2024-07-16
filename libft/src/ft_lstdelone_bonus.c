/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_lstdelone_bonus.c                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: lprieri <lprieri@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/10/18 14:33:50 by lprieri       #+#    #+#                 */
/*   Updated: 2024/02/02 16:36:10 by lprieri       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

/*
* Function name: ft_lstdelone
* Description: Takes as a parameter a node and frees the memory
*	of the node’s content using the function ’del’ given as a parameter
*	and free the node. The memory of ’next’ must not be freed.
* External functs.: free
* Return value: none
* Parameters:
*	t_list *lst: The node to free.
*	void (*del)(void*): The address of the function used to delete
the content.
*/

#include "../libft.h"

void	ft_lstdelone(t_list *lst, void (*del)(void*))
{
	if (lst != NULL)
	{
		del((lst->content));
		free(lst);
	}
}

/*
* Test 1
*/

/*
* If I use free in this function I get a free(): invalid pointer error.
* Therefore I simply set the content to NULL, to have it do something.
*/

// static void	ft_del(void *content)
// {
// 	content = NULL;
// }

// int	main(void)
// {
// 	t_list	*lst1;
// 	t_list	lst2;

// 	lst1 = (t_list *) malloc (sizeof(t_list));
// 	lst1->content = (char *)"Content";
// 	lst1->next = &lst2;
// 	(&lst2)->content = (char *)"Content2";
// 	(&lst2)->next = NULL;
// 	ft_lstdelone(lst1, ft_del);
// 	printf("%s\n", (char *)(lst2)->content);
// }
