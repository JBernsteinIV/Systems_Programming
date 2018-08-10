#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

#define MAX_SIZE 4096

int main(int ac, char* argv[]) {
	if (ac != 2) {
		printf("mh FILE\n");
		return 1;
	}
	int fd = open(argv[1], O_RDONLY);
	
	if (fd == -1) {
	    printf("Couldn't open file: No such file or directory\n");
	    return 1;
	}
	    
		char buf[MAX_SIZE];
		int bytes = read(fd, buf, MAX_SIZE);
		
		int i = 0, ctr = 0;
		for (; buf[i] != EOF; i++) {
		printf("%c",buf[i]);
		if (buf[i] == '\n')
		    ctr++;
		if (ctr >= 10)
		    break;
		}
		close(fd);
	
	return 0;
 }