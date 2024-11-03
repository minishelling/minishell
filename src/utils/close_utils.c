/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   close_utils.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: lprieri <lprieri@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/10/31 13:33:42 by lprieri       #+#    #+#                 */
/*   Updated: 2024/10/31 13:33:43 by lprieri       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/**
 * @brief Closes all the file descriptors in the current process.
 * 
 * It opens the file descriptors of the current process in /proc/
 * as a directory stream. Then the readdir function is used
 * to iterate through each file in the directory.
 * It converts the file name to an integer, and leaving aside
 * the standard file descriptors, it closes everything smaller than 1024,
 * which is the default file descriptor limit for a process.
 * If the close function encounters an error it prints an error code.
 * At the end the directory stream is closed, and if it fails
 * it also prints an error message using errno.
 * 
 * @param void
 * 
 * @return void
 */
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
				handle_perror("close_all_fds_in_process");
		}
		current_entry = readdir(dir);
	}
	if (closedir(dir))
		handle_perror("close_all_fds_in_process");
}

/**
 * @brief Closes the active file descriptors in the command's structure.
 * 
 * The active file descriptors can be just two: the latest input,
 * and the latest output. And these file descriptors have to be
 * different than the STDIN and STDOUT respectively, because
 * otherwise they are simply holding the value they were initialized to.
 * 
 * @param cmd A pointer to the command's structure.
 * 
 * @return void
 */
void	close_all_fds_in_cmd(t_cmd *cmd)
{
	if (!cmd)
		return ;
	if (cmd->latest_in != STDIN_FILENO && cmd->latest_in != ERROR)
	{
		if (close(cmd->latest_in) == ERROR)
			handle_perror("close_all_fds_in_cmd");
	}
	if (cmd->latest_out != STDOUT_FILENO && cmd->latest_out != ERROR)
	{
		if (close(cmd->latest_out) == ERROR)
			handle_perror("close_all_fds_in_cmd");
	}
}
