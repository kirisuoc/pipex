#include "pipex.h"

int	main(int argc, char **argv)
{
	printf("Opening...\n");
	int	fd = open("outfile", O_WRONLY | O_CREAT, 0644);
	if (fd == -1)
		return (2);
	printf("Open.\n");
	if (write(fd, "Holaaa\n", 7) == -1)
		return (1);

	printf("Written.\n");
	close(fd);
	printf("Closed.\n");
	return (0);
}
