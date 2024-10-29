#include "../../include/minishell.h"

size_t	get_token_list_size(t_token *list);
void	init_token_names(char **token_name);
void	print_tokens(t_token *list);
void	print_tree(t_tree *tree);
void	print_tree_nodes(t_tree *node, int level);

size_t	get_token_list_size(t_token *list)
{
	size_t	token_list_size;

	token_list_size = 0;
	if (!list)
		return (0);
	while (list)
	{
		list = list->next;
		token_list_size++;
	}
	return (token_list_size);
}

void	init_token_names(char **token_name)
{
	token_name[0] = "SPACE_CHAR";
	token_name[1] = "TAB_CHAR";
	token_name[2] = "NL";
	token_name[3] = "PIPE";
	token_name[4] = "AND_OPR";
	token_name[5] = "PAR_OPEN";
	token_name[6] = "PAR_CLOSE";
	token_name[7] = "LT";
	token_name[8] = "GT";
	token_name[9] = "SQUOTE";
	token_name[10] = "DQUOTE";
	token_name[11] = "ENV_VAR";
	token_name[12] = "WORD";
	token_name[13] = "OR_OPR";
	token_name[14] = "ARITH_EXPAN";
}

void	print_tokens(t_token *list)
{
	int			list_size;
	int			i;
	t_token		*cur_token;
	char		*token_name[15];

	init_token_names(token_name);
	list_size = get_token_list_size(list);
	i = list_size;
	cur_token = list;
	printf("\n"WHITE_TEXT MAGENTA_BACKGROUND"LIST OF TOKENS" \
	RESET_COLOR"\t[%d]\n", list_size);
	printf("--------------\n");
	while (i--)
	{
		if (cur_token && cur_token->str)
		{
			printf(MAGENTA_TEXT "TOKEN [%02d]      " RESET_COLOR \
			"id: %-12s [%02d]    str: |%s|\n\n", (list_size - i), \
			token_name[cur_token->id], cur_token->id, cur_token->str);

		}
		else
			printf("NULL\n");
		cur_token = cur_token->next;
	}
}

void	print_tree(t_tree *tree)
{
	printf("\n"WHITE_TEXT MAGENTA_BACKGROUND"THE TREE"RESET_COLOR"\n");
	printf("--------\n");
	print_tree_nodes(tree, 0);
	printf ("\n");
}

void	print_tree_nodes(t_tree *node, int level)
{
	int		i;
	char	*tree_node_name[4];

	tree_node_name[0] = "AND_OPR";
	tree_node_name[1] = "OR_OPR";
	tree_node_name[2] = "CMD";
	tree_node_name[3] = "PIPE";
	if (!node)
		return ;
	i = 0;
	while (i++ < level)
		printf("    ");
	if (node->type == CMD)
		printf(WHITE_TEXT MAGENTA_BACKGROUND "%s" RESET_COLOR \
		" start token:|%s|, end token:|%s|\n", tree_node_name[node->type],
		node->start_token->str, node->end_token->str);
	else if (node->type == T_AND_OPR || node->type == T_OR_OPR \
		|| node->type == T_PIPE)
		printf(MAGENTA_TEXT"%s"RESET_COLOR"\n", tree_node_name[node->type]);
	else
		printf ("NULL");
	print_tree_nodes(node->left, level + 1);
	print_tree_nodes(node->right, level + 1);
}
