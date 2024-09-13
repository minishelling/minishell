#include "../../include/minishell.h"

void	env_print_list(t_env *head)
{
	printf ("Lisandro's\n");
	if (!head)
		return ;
	while(head != NULL)
	{
		ft_printf("Key-value: %s\n", head->keyvalue);
		ft_printf("Key: %s\n", head->key);
		ft_printf("Value: %s\n\n\n", head->value);
		head = head->next;
	}
}
