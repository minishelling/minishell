/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_lstmap_bonus.c                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: lprieri <lprieri@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/10/20 19:05:51 by lprieri       #+#    #+#                 */
/*   Updated: 2024/02/02 16:36:10 by lprieri       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

/*
* Function name: ft_lstmap
* Description: Iterates the list ’lst’ and applies the function ’f’
*	on the content of each node.
*	Creates a new list resulting of the successive applications
*	of the function ’f’. The ’del’ function is used to delete
*	the content of a node if needed.
* External functs.: malloc, free
* Return value: t_list: 
*	The new list.
*	NULL if the allocation fails.
* Parameters:
*	t_list *lst: The address of a pointer to a node.
*	void (*f)(void *): The address of the function used to
*	iterate on the list.
*	void (*del)(void*): The address of the function used to delete
*	the content of a node if needed.
*/

#include "../libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*new_lst;
	t_list	*current_node;
	t_list	*new_node;

	new_lst = NULL;
	current_node = lst;
	while (current_node)
	{
		new_node = ft_lstnew(f(current_node->content));
		if (!new_node)
		{
			ft_lstclear(&new_lst, del);
			return (NULL);
		}
		ft_lstadd_back(&new_lst, new_node);
		current_node = current_node->next;
	}
	return (new_lst);
}

/*
* Test 1
*/

// static void	*ft_f(void *content)
// {
// 	char	*str;

// 	str = (char *) malloc(10);
// 	if (str)
// 		ft_strlcpy(str, "Hello", 6);
// 	return (str);
// }

// static void	ft_del(void *content)
// {
// 	content = NULL;
// }

// static void	ft_lstdel(t_list *lst)
// {
// 	free(lst->content);
// 	free(lst);
// }

// static void	ft_printf(void *content)
// {
// 	printf("%s\n", (char *)content);
// 	content = NULL;
// }

// int	main(void)
// {
// 	t_list	*lst1;
// 	t_list	*lst2;
// 	t_list	*new;

// 	lst1 = (t_list *) malloc(sizeof(t_list));
// 	lst2 = (t_list *) malloc(sizeof(t_list));
// 	lst1->content = "Hello1";
// 	lst2->content = "Hello2";
// 	lst1->next = lst2;
// 	lst2->next = NULL;
// 	ft_lstiter(lst1, ft_printf);
// 	new = ft_lstmap(lst1, ft_f, ft_del);
// 	ft_lstiter(new, ft_printf);
// 	ft_lstdel(new->next);
// 	ft_lstdel(new);
// 	free(lst1);
// 	free(lst2);
// }
