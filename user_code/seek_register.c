/*
Authors:    Kyle & Andrew

Details:    Provides error checking before invoking the file operation lseek.
            This call then goes to the llseek function implemeted by the 
            simple_device device driver, which changes the register.

*/


#include <unistd.h>      //printf
#include <stdio.h>    

#include <sys/types.h>   //file reposition and open
#include <sys/stat.h>
#include <fcntl.h>


#define ASCII_OFFSET '0'
#define NUM_REGISTERS 6


int main(int argc, char *argv[])
{
    //inputted number is found at arg[1] (2 arguments)
    if(argc !=2 || ((((int)argv[1][0])-ASCII_OFFSET) > NUM_REGISTERS-1) ||
       (((int)argv[1][0])-ASCII_OFFSET) < 0) { 

	printf("incorrect usage: run with a desired register number. (0-5)\n");
    }

    else{
	
	char reg_int = ((int)argv[1][0]) -ASCII_OFFSET;
	int fd;
	
	fd = open("/proc/simple_device", O_RDONLY);    //read only to bypass sudo usage.

	if(fd == -1){

	    printf("Could not open file\n");
	}
	
	else{
	    printf("changing register to: %d\n",reg_int);
	    
	    if(lseek(fd,reg_int,SEEK_SET) == -1){   //Currently, third parameter will not actually do anything,
		printf("seek failed\n");
	    }

	    close(fd);
	}
    }
    
    return 0;
}
