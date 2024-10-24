#include "../../include/minishell.h"

size_t	cmd_size(t_cmd *t_list)
{
	size_t	ret;

	ret = 0;
	if (t_list == NULL)
		return (0);
	while (t_list != NULL)
	{
		t_list = t_list->next;
		ret++;
	}
	return (ret);
}

size_t	token_size(t_token *t_list)
{
	size_t	ret;

	ret = 0;
	if (t_list == NULL)
		return (0);
	while (t_list)
	{
		t_list = t_list->next;
		ret++;
	}
	return (ret);
}

void	print_env(t_env *env_list)
{
	while (env_list)
	{
		printf("%s", env_list->key);
		printf("=");
		printf("%s\n", env_list->value);
		env_list = env_list->next;
	}
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
	token_name[7] = "GT";
	token_name[8] = "LT";
	token_name[9] = "SQUOTE";
	token_name[10] = "DQUOTE";
	token_name[11] = "ENV_VAR";
	token_name[12] = "WORD";
	token_name[13] = "OR_OPR";
	token_name[14] = "ARITH_EXPAN";
}

void	print_token(t_token *head)
{
	int			size;
	t_token		*ptr;
	int			con;
	char		*token_name[15];

	init_token_names(token_name);
	con = token_size(head);
	ptr = head;
	size = token_size(ptr);
	printf("\n"WHITE_TEXT MAGENTA_BACKGROUND"LIST OF TOKENS" \
	RESET_COLOR"\t[%d]\n", con);
	printf("--------------------\n");
	while (size--)
	{
		if (ptr && ptr->str)
		{
			printf(MAGENTA_TEXT"%pTOKEN [%02d]"RESET_COLOR"\t \
			id: %s [%d]\n\t\tstr: |%s|\tstr address: %p\n", ptr, \
			(con - size), token_name[ptr->id], ptr->id, ptr->str, ptr->str);
		}
		else
			printf("NULL\n");
		ptr = ptr->next;
	}
}

void	print_cmd_args(char **arg)
{
	size_t	i;

	i = 0;
	fprintf(stderr, MAGENTA_TEXT WHITE_BACKGROUND"ARGUMENTS" RESET_COLOR"\n");
	while (arg[i] != NULL)
	{
		fprintf(stderr, "arg [%02zu]:\t|%s| arg adress: %p\n", \
		i, arg[i], &arg[i]);
		i++;
	}
}

void	print_redir(t_redir *redir_list_head)
{
	t_redir	*current_redir;
	char	*redir_name[5];

	redir_name[0] = "REDIR";
	redir_name[1] = "INPUT";
	redir_name[2] = "HERE_DOC";
	redir_name[3] = "OUTPUT";
	redir_name[4] = "APPEND";
	if (!redir_list_head)
	{
		fprintf(stderr, "\n"MAGENTA_TEXT WHITE_BACKGROUND \
		"NO REDIRECTIONS"RESET_COLOR"\n");
		return ;
	}
	current_redir = redir_list_head;
	fprintf(stderr, "\n"MAGENTA_TEXT WHITE_BACKGROUND \
	"REDIRECTIONS"RESET_COLOR"\n");
	while (current_redir != NULL)
	{
		fprintf(stderr, "TYPE [%d]\t%s\n", current_redir->redir_id, \
		redir_name[current_redir->redir_id]);
		fprintf(stderr, "FILE\t\t%s\n", current_redir->file);
		current_redir = current_redir->next;
	}
}

void	print_cmd(t_cmd *cmd)
{
	fprintf(stderr, "\n"WHITE_TEXT MAGENTA_BACKGROUND"CMD"RESET_COLOR"\n");
	fprintf(stderr, "---------------------\n");
	if (cmd)
	{
		print_cmd_args((char **)cmd->args);
		print_redir(cmd->redir);
		fprintf (stderr, "latest_in is %d and latest_out %d\n", \
		cmd->latest_in, cmd->latest_out);
		cmd = cmd->next;
		fprintf(stderr, "______________________\n\n");
	}
}

void	print_tree_verbose(t_tree *node, int level)
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
		printf(WHITE_TEXT MAGENTA_BACKGROUND"%s"RESET_COLOR" start token:|%s|, \
		end token:|%s|, tree address: %p\n", tree_node_name[node->type], \
		node->start_token->str, node->end_token->str, node);
	else if (node->type == T_AND_OPR || node->type == T_OR_OPR \
		|| node->type == T_PIPE)
		printf(MAGENTA_TEXT"%s"RESET_COLOR" %p\n", \
		tree_node_name[node->type], node);
	else
		printf ("NULL");
	print_tree_verbose(node->left, level + 1);
	print_tree_verbose(node->right, level + 1);
}
