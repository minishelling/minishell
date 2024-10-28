#include "../../include/minishell.h"

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

void	print_cmd_redir(t_redir *redir_list_head)
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
		print_cmd_redir(cmd->redir);
		fprintf (stderr, "latest_in is %d and latest_out %d\n", \
		cmd->latest_in, cmd->latest_out);
		cmd = cmd->next;
		fprintf(stderr, "______________________\n\n");
	}
}
