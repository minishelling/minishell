#include "../../../include/builtins.h"

bool	is_dir_prefix_valid(char *directory)
{
	if (!directory || directory[0] == '/' || directory[0] == '.'
		|| (directory[0] == '.' && directory[1] == '.'))
		return (false);
	return (true);
}