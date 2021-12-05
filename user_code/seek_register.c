#include <unistd.h>
#include <stdio.h>    //printf

#include <sys/types.h>   //file reposition and open
#include <sys/stat.h>
#include <fcntl.h>


int main(int argc, char *argv[])
{
    //small bug: 35 counts as 3... 
    if(argc !=2 || ((((int)argv[1][0])-48) > 5) || (((int)argv[1][0])-48) < 0) { 

	printf("incorrect usage: run with a desired register number. (0-5)\n");
    }

    else{
	
	char reg_int = ((int)argv[1][0]) -48;
//	printf("%d\n",reg_int);

	int fd;
	
	fd = open("/proc/simple_device", O_RDWR);    //This is gonna need to be something else...

	if(fd == -1){

	    printf("Could not open file\n");
	}
	
	else{
	    printf("changing register to: %d\n",reg_int);
	    lseek(fd,reg_int,SEEK_SET);   //Currently, third parameter will not actually do anything, but pass it anyways.
	    close(fd);
	}
    }
    
    return 0;
}
