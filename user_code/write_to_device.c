/*
Authors:    Kyle & Andrew

Details:    Provides error checking before invoking the file operation write.
            The write call will then invoke the version defined in the device driver.

	    if the text entered is longer than 128 characters, the input will be automatically shortened.
*/


#include <string.h>       //concat strings

#include <stdio.h>        //printf
#include <unistd.h>       //write and read

#include <sys/types.h>    //open
#include <sys/stat.h>
#include <fcntl.h>

#define BUFFER_SIZE 128


int main(int argc, char *argv[]){

    int fd;
    char buff[BUFFER_SIZE] = {0};

    int fill = 0;
    for(fill = 1; fill <argc; fill++){  
	                                
	strcat(buff,argv[fill]);
	if(fill<argc){
	    strcat(buff," ");           //adds a space inbetween words
	  }
    }

    
    fd = open("/proc/simple_device", O_WRONLY);
    
    if(fd == -1){
        printf("Could not open file\n");
    }
    else{
	
    	if(write(fd, buff, BUFFER_SIZE) == -1){
	    printf("write failed\n");
	}
	
	else{
	    printf("Writing...\n"); 
	}	
    }
    
    close(fd);    
    
    return 0;
}
