#include <stdio.h>
#include <sys/stat.h>
#include <dirent.h>
void do_stat(char*);

int main(int ac, char* argv[]) {
    const char* dirname = ".";
    DIR* dir_ptr;
    struct dirent* direntp;
    
    dir_ptr = opendir( dirname );
    if( dir_ptr == NULL )
        fprintf( stderr, "dirp: cannot open %s\n", dirname );
    else
    {
        printf("Entry Name\t\t\t Inode #\t T\n");
        while( ( direntp = readdir( dir_ptr ) ) != NULL )
            do_stat( direntp->d_name );
        
        closedir( dir_ptr );
    }

	return 0;
}
void do_stat(char* d_name) {
    struct stat info;
    struct stat* info_p = &info;
    int mode;
    
    if( stat( d_name, &info ) == -1 ) 
        perror( d_name );
    else {
		printf("%-20s%20lu\t ", d_name, info_p->st_ino);
		S_ISDIR (info_p->st_mode) ? printf("d\n") : S_ISREG (info_p->st_mode) ? printf("f\n") : 
		printf("o\n");
    }
}