/*
Authors:    Kyle & Andrew

Details:    Provides error checking before invoking the file operation read.
            The write call will then invoke the version defined in the device driver.
*/


#include <stdio.h>     //printf
#include <unistd.h>    //write and read

#include <sys/types.h>  //open
#include <sys/stat.h>
#include <fcntl.h>

#define BUFFER_SIZE 128

int main(){


    int fd;
    char buff[BUFFER_SIZE];
        
    fd = open("/proc/simple_device", O_RDONLY);
    
    if(fd == -1){
        printf("Could not open file\n");
    }
    else{
	
	if(read(fd, buff, BUFFER_SIZE) == -1){  
	    printf("read failed\n");
	}
	
	else{
	    printf("%s\n",buff); 
	}	
}
    close(fd);    
    
    return 0;
}
