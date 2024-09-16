#include "../../include/minishell.h"

void print_tree_with_cmds(t_tree *node, int level) 
{
    int i;
	int j;
	t_cmd *cmd;
    char *tree_node_name[3] = 
    {
        [0] = "AND_OPR",
        [1] = "OR_OPR",
        [2] = "CMD",
    };
	char *redir_name[4] = 
    {
        [0] = "INFILE",
        [1] = "HEREDOC",
        [2] = "OUTFILE",
		[3] = "APPEND",
    };

    if (!node)
		return;

	i = 0;
    // Print indentation for the current level
    while (i++ < level)
        printf("    ");
    if (node->type == CMD)
    {
        printf(MAGENTA_TEXT"%s "RESET_COLOR"|%s| |%s|\n", tree_node_name[node->type], node->start_token->str, node->end_token->str);
        
        cmd = (t_cmd *)node->cmd_list;
		while (cmd)
        {
            if (cmd->args)
            {
                i = 0;
                 while (i++ < level)
                    printf("    ");
                printf("    "GREY_BACKGROUND"Command"RESET_COLOR": ");
                j = 0;
                while (cmd->args[j] != NULL)
                {
                    if (j > 0) printf(" ");
                    printf("%s", cmd->args[j]);
                    j++;
                }
                printf("\n");
                //printf ("        "GREY_BACKGROUND"&cmd is"RESET_COLOR": %p\n", cmd);
            }

            // Print redirections if available
            if (cmd->redir)
            {
                t_redir *redir = cmd->redir;
                while (redir)
                {   
                    i = 0;
                    while (i++ < level)
                        printf("    ");
                    printf ("    "GREY_BACKGROUND"Redirection"RESET_COLOR": %s (%s)\n", redir->file, redir_name[redir->redir_id - 1]);
                    redir = redir->next;
                }
            }
            cmd = cmd->next;
        }
    }
    else if (node->type == T_AND_OPR || node->type == T_OR_OPR)
    {
        // Print logical operators
        printf(MAGENTA_TEXT"%s"RESET_COLOR"\n", tree_node_name[node->type]);
    }
    //printf ("        "GREY_BACKGROUND"&node is"RESET_COLOR": %p\n", node);
    // Print the left subtree
    print_tree_with_cmds(node->left, level + 1);

    // Print the right subtree
    print_tree_with_cmds(node->right, level + 1);
}

void free_redir_list(t_redir *redir)
{
    t_redir *temp_redir;

    while (redir != NULL)
    {
        temp_redir = redir;
        redir = redir->next;

        // Free the file associated with the redirection
        if (temp_redir->file)
            free(temp_redir->file);

        // Free the redir node itself
        free(temp_redir);
    }
}

void free_cmd_list(t_cmd *cmd_list)
{
    t_cmd *temp;

    while (cmd_list != NULL)
    {
        temp = cmd_list;
        cmd_list = cmd_list->next;

        // Free each argument in the args array
        if (temp->args)
        {
            int i = 0;
            while (temp->args[i] != NULL)
            {
                free(temp->args[i]);
                i++;
            }
            free(temp->args);
        }

        // Free the redirections (redir is a linked list)
        if (temp->redir)
        {
            free_redir_list(temp->redir);
        }
        // Free the command itself
        free(temp);
    }
}

void free_tree(t_tree *node)
{
    if (node == NULL)
        return;

    // Free left and right subtrees
    free_tree(node->left);
    free_tree(node->right);

    // Free the cmd_list
    if (node->type == CMD)
        free_cmd_list(node->cmd_list);

    // Finally, free the node itself
    free(node);
}
