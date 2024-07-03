/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_lstclear_bonus.c                                :+:    :+:            */
/*                                                     +:+                    */
/*   By: lprieri <lprieri@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/10/19 12:14:16 by lprieri       #+#    #+#                 */
/*   Updated: 2024/02/02 16:36:10 by lprieri       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

/*
* Function name: ft_lstclear
* Description: Deletes and frees the given node and every successor of that node,
*	using the function ’del’ and free(3).
*	Finally, the pointer to the list must be set to NULL.
* External functs.: free
* Return value: none
* Parameters:
*	t_list *lst: The address of a pointer to a node.
*	void (*del)(void*): The address of the function used to delete
the content.
*/

#include "../libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void*))
{
	t_list	*current_ptr;
	t_list	*next_ptr;

	if (*lst == NULL)
		return ;
	current_ptr = *lst;
	next_ptr = (*lst)->next;
	if (lst != NULL)
	{
		while (current_ptr != NULL)
		{
			next_ptr = current_ptr->next;
			ft_lstdelone(current_ptr, del);
			current_ptr = next_ptr;
		}
		*lst = NULL;
	}
}

/*
* Test 1
*/

// static void	ft_del(void *content)
// {
// 	content = NULL;
// }

// int	main(void)
// {
// 	t_list	*lst1;
// 	t_list	*lst2;

// 	lst1 = (t_list *) malloc (sizeof(t_list));
// 	lst2 = (t_list *) malloc (sizeof(t_list));
// 	lst1->content = (char *)"Content";
// 	lst1->next = lst2;
// 	lst2->content = (char *)"Content2";
// 	lst2->next = NULL;

// 	if (lst1 == NULL)
// 		printf("Couldn't print lst2 contents because lst1 is NULL\n");
// 	else
// 	{
// 		printf("Lst2 contents: %s\n", (char *)lst2->content);
// 		printf("lst1 pointer: %p\n", lst1);
// 		printf("lst2 pointer: %p\n", lst2);
// 	}

// 	ft_lstclear(&lst1, ft_del);

// 	if (lst1 == NULL)
// 	{
// 		printf("Lst1 is now null, and lst2 contents are inaccesible.\n");
// 		printf("But lst2 pointer is still there.\n");
// 		printf("lst1 pointer: %p\n", lst1);
// 		printf("lst2 pointer: %p\n", lst2);
// 	}
// 	else
// 		printf("%s\n", (char *)lst2->content);
// }
