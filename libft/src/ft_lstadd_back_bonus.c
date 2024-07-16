/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_lstadd_back_bonus.c                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: lprieri <lprieri@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/10/18 12:20:37 by lprieri       #+#    #+#                 */
/*   Updated: 2024/02/02 16:36:10 by lprieri       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

/*
* Function name: ft_lstadd_back
* Description: Adds the node ’new’ at the end of the list.
* External functs.: none
* Return value: none
* Parameters:
*	t_list **lst: The address of a pointer to the first link of a list.
*	t_list *new: The address of a pointer to the node to be added to the list.
*/

#include "../libft.h"

void	ft_lstadd_back(t_list **lst, t_list *new)
{
	t_list	*last_ptr;

	if (*lst == NULL)
	{
		*lst = new;
		return ;
	}
	last_ptr = ft_lstlast(*lst);
	if (new)
		last_ptr->next = new;
}

/*
* Test 1
*/

// int	main(void)
// {
// 	t_list	lst1;
// 	t_list	lst2;
// 	t_list	new;
// 	t_list	*lst_ptr;

// 	(&lst1)->next = &lst2;
// 	(&lst2)->next = NULL;
// 	(&new)->content = (char *) "New lst content";
// 	(&new)->next = NULL;
// 	lst_ptr = &lst1;
// 	ft_lstadd_back(&lst_ptr, &new);
// 	printf("%s\n", (char *)ft_lstlast(lst_ptr)->content);
// }

/*
* Test 2
*/

// int	main(void)
// {
// 	t_list	*lst1;
// 	t_list	new;

// 	lst1 = NULL;
// 	(&new)->content = (char *) "New lst content";
// 	(&new)->next = NULL;
// 	ft_lstadd_back(&lst1, &new);
// 	printf("%s\n", (char *)ft_lstlast(lst1)->content);
// }
