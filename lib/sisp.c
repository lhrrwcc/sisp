#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>
#include "sisp.h"

#define FILE_LENGTH sizeof(s)

int 
write_m (int fd)
{
	int i;
	static int NELEMS;
	NELEMS =  sizeof(s) / sizeof(s[0]);
	lseek (fd, FILE_LENGTH+1, SEEK_SET);
	lseek (fd, 0, SEEK_SET);
	for( i = 0; i < NELEMS; i++) 
		if (write(fd, s[i], strlen(s[i])) == -1)
			return -1;
	close(fd);
	return 0;
}
