//MUST BE RUN WITH SUDO ON


#include <stdio.h>   //printf
#include <unistd.h>   //write and read

#include <sys/types.h>  //open
#include <sys/stat.h>
#include <fcntl.h>






int main(){


    int fd;
    char buff[128];
    
    
    fd = open("/proc/simple_device", O_RDWR);
    
    if(fd == -1){
        printf("\nCould not open file\n");
    }
    else{
	
	if(read(fd, buff, 128) == -1){   //not sure what to make this 128 thing...
	    printf("read failed\n");
	}
	
	else{
	    printf("%s\n",buff); 
	}	
}
    close(fd);    
    
    return 0;
}
