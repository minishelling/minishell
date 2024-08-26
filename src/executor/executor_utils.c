#include "../../include/minishell.h"

void	redirect_io(t_shell *shell, int io_fd, int io_target)
{
	if (dup2(io_fd, io_target) == -1)
		return ; //Error
	if (close(io_fd) == -1)
		return ; //Closing error. Is minishell still functional?
}

