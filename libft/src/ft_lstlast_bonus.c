/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_lstlast_bonus.c                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: lprieri <lprieri@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/10/18 12:02:07 by lprieri       #+#    #+#                 */
/*   Updated: 2024/02/02 16:36:10 by lprieri       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

/*
* Function name: ft_lstlast
* Description: Returns the last node of the list.
* External functs.: none
* Return value: Last node of the list
* Parameters:
*	t_list *lst: The beginning of the list.
*/

#include "../libft.h"

t_list	*ft_lstlast(t_list *lst)
{
	t_list	*current_ptr;
	t_list	*next_ptr;

	if (lst == NULL)
		return (NULL);
	current_ptr = lst;
	next_ptr = lst -> next;
	while (next_ptr != NULL)
	{
		current_ptr = next_ptr;
		next_ptr = next_ptr->next;
	}
	return (current_ptr);
}

/*
* Test 1
*/

// int	main(void)
// {
// 	t_list	lst1;
// 	t_list	lst2;

// 	(&lst1)->next = &lst2;
// 	(&lst2)->content = (char *)"2nd list content";
// 	(&lst2)->next = NULL;
// 	printf("%s\n", (char *) ft_lstlast(&lst1)->content);
// }

/*
* Test 2
*/

// int	main(void)
// {
// 	t_list	lst1;

// 	(&lst1)->next = NULL;
// 	(&lst1)->content = (char *)"1st list content";
// 	printf("%s\n", (char *) ft_lstlast(&lst1)->content);
// }
