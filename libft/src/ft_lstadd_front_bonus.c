/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_lstadd_front_bonus.c                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: lprieri <lprieri@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/10/18 10:55:51 by lprieri       #+#    #+#                 */
/*   Updated: 2024/02/02 16:36:10 by lprieri       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

/*
* Function name: ft_lstadd_front
* Description: Adds the node ’new’ at the beginning of the list.
* External functs.: none
* Return value: none
* Parameters:
*	t_list **lst: The address of a pointer to the first link of a list.
*	t_list *new: The address of a pointer to the node to be added to the list.
*/

#include "../libft.h"

void	ft_lstadd_front(t_list **lst, t_list *new)
{
	if (new != NULL)
	{
		(*new).next = *lst;
		*lst = new;
	}
}

/*
* Test 1
*/

// int	main(void)
// {
// 	t_list	lst1;
// 	t_list	lst2;
// 	t_list	*lst1_ptr;
// 	t_list	new;

// 	(&lst1)->content = (char *)"First original element";
// 	(&lst1)->next = &lst2;
// 	(&lst2)->content = (char *)"Second original element";
// 	(&lst2)->next = NULL;
// 	(&new)->content = (char *)"NEW ELEMENT";
// 	(&new)->next = NULL;
// 	lst1_ptr = &lst1;
// 	printf("%s\n", (char *)(&lst1)->content);
// 	ft_lstadd_front(&lst1_ptr, &new);
// 	printf("%s\n", (char *)lst1_ptr->content);
// }

/*
* Test 2
*/

// int	main(void)
// {
// 	t_list	*lst1;
// 	t_list	new;

// 	lst1 = NULL;
// 	(&new)->content = (char *)"NEW ELEMENT";
// 	ft_lstadd_front(&lst1, &new);
// 	printf("%s\n", (char *)lst1->content);
// }

/*
* Test 3
*/

// int	main(void)
// {
// 	t_list	*lst_ptr;
// 	t_list	lst1;
// 	t_list	*new;

// 	(&lst1)->content = (char *) "Content";
// 	(&lst1)->next = NULL;
// 	lst_ptr = &lst1;
// 	new = NULL;
// 	ft_lstadd_front(&lst_ptr, new);
// 	printf("%s\n", (char *)lst_ptr->content);
// }
