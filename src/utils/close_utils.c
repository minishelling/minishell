#include "../../include/minishell.h"

void	close_all_fds_in_process(void)
{
	DIR				*dir;
	struct dirent	*current_entry;
	int				fd;

	dir = opendir("/proc/self/fd");
	if (!dir)
		return ;
	current_entry = readdir(dir);
	while (current_entry != NULL)
	{
		fd = ft_atoi(current_entry->d_name);
		if (fd > STDERR_FILENO && fd < 1024)
		{
			if (close(fd) == -1)
				perror("close");
		}
		current_entry = readdir(dir);
	}
	closedir(dir);
}

void	close_all_fds_in_cmd(t_cmd *cmd)
{
	if (!cmd)
		return ;
	if (cmd->latest_in != STDIN_FILENO)
	{
		if (close(cmd->latest_in) == -1)
			perror("close");
	}
	if (cmd->latest_out != STDOUT_FILENO)
	{
		if (close(cmd->latest_out) == -1)
			perror("close");
	}
}
