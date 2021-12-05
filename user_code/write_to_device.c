//MUST BE RUN WITH SUDO ON
#include <string.h>   //concat strings

#include <stdio.h>   //printf
#include <unistd.h>   //write and read

#include <sys/types.h>  //open
#include <sys/stat.h>
#include <fcntl.h>



int main(int argc, char *argv[]){

    int fd;
    char buff[128] = {0};

    int fill = 0;
    for(fill = 1; fill <argc; fill++){   //dont want the command that was added...

	strcat(buff,argv[fill]);
	if(fill<argc){
	    	strcat(buff," ");
    }
    }

    
    fd = open("/proc/simple_device", O_RDWR);   //CHANGE THIS LATER (the permission can be write only)
    
    if(fd == -1){
        printf("Could not open file\n");
    }
    else{
	
    	if(write(fd, buff, 128) == -1){   //Try writing.... -> Should output the result into dmesg
	    printf("write failed\n");
	}
	
	else{
	    printf("Writing...\n"); 
	}	
    }
    
    close(fd);    
    
    return 0;
}
