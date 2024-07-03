/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_lstiter_bonus.c                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: lprieri <lprieri@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/10/20 14:23:36 by lprieri       #+#    #+#                 */
/*   Updated: 2024/02/02 16:36:10 by lprieri       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

/*
* Function name: ft_lstiter
* Description: Iterates the list ’lst’ and applies the function
*	’f’ on the content of each node.
* External functs.: none
* Return value: none
* Parameters:
*	t_list *lst: The address of a pointer to a node.
*	void (*f)(void *):The address of the function
*	used to iterate on the list.
*/

#include "../libft.h"

void	ft_lstiter(t_list *lst, void (*f)(void *))
{
	t_list	*current_ptr;
	t_list	*next_ptr;

	if (lst == NULL)
		return ;
	current_ptr = lst;
	next_ptr = lst->next;
	while (current_ptr != NULL)
	{
		next_ptr = current_ptr->next;
		f(current_ptr->content);
		current_ptr = next_ptr;
	}
}

/*
*	Test 1
*/

// static void	ft_del(void *content)
// {
// 	content = NULL;
// }

// static void	ft_f(void *content)
// {
// 	printf("%s\n", (char *) content);
// }

// int	main(void)
// {
// 	t_list	*lst1;
// 	t_list	*lst2;

// 	lst1 = (t_list *)malloc (sizeof(t_list));
// 	lst2 = (t_list *)malloc (sizeof(t_list));
// 	lst1->content = "This is Content1";
// 	lst2->content = "This is Content2";
// 	lst1->next = lst2;
// 	lst2->next = NULL;
// 	ft_lstiter(lst1, ft_f);
// 	ft_lstclear(&lst1, ft_del);
// }
