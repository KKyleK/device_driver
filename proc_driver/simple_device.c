// email:               kkoiv395@mtroyal.ca
// Course:               COMP 3659
//
// Instructor:           Marc Schroeder
// Assignment:           2
// Due Date:             December 6th, 2021 
//
//
// Purpose:  This program is a device driver, which reimplements common file operations
//           to implement memory sharing through a system file which can be read and
//           written to.
//           
//
// Details:  This driver creates a file called simple_device in the /proc directory.
//           read/write/seek file calls to that file, are instead handled by functions
//           which handle an array of registers. There are six registers, each holding
//           128 bytes.
//
//  operations:  To run the device, run make, then lsmod simple_device.ko. You can then
//           go into /proc and read/write/seek the file directly, but it is recommended
//           to use the binaries supplied in user_code.
//
//           use rmmod simple_device when done with the device.
//
//
//   Known bugs:   running write inside of /proc will first require sudo bash.
//           after running sudo bash, write may still crash linux. (use user_code write
//           instead)


#include <linux/init.h>      //initilization
#include <linux/module.h>    //module init and exit, licensing. 

#include <linux/fs.h>        //Contains useful linux data structures.
#include <linux/uaccess.h>   //Moves kernel code to user space. (Lets you print to user space)

#include <linux/cdev.h>      //Allows you to reigster the device from code.
#include <linux/types.h>    //Major and minor numbers (dev_t)

#include <linux/proc_fs.h>  //open_proc and close proc

#include <linux/stat.h>    //For changing the file permissions with macros.


#define PROC_NAME "simple_device"
#define BUFFER_SIZE 128
#define NUM_REGISTERS 6


ssize_t proc_write(struct file *filep, const char *usr_buffer, size_t len, loff_t *offset);
ssize_t proc_read(struct file *file, char *usr_buf, size_t count, loff_t *pos);
loff_t proc_seek (struct file *filep, loff_t, int);
loff_t proc_seek (struct file *filep, loff_t offset, int type);


int location = 0;                           //current register.
char buffer[NUM_REGISTERS][128] = {0};      //the Buffers.
int buffer_fill[NUM_REGISTERS] = {0};      //How full each register is.


struct file_operations proc_ops = {
    .owner = THIS_MODULE,
    .read = proc_read,
    .write = proc_write,
    .llseek = proc_seek
};

/*---------------------------------------------------
Function name: proc_seek

Purpose: Implements the lseek function call.

Details: Uses the offset which is passed through the
         lseek function call to change the current register.

-----------------------------------------------------*/
loff_t proc_seek (struct file *filep, loff_t offset, int type){   


    if (offset <= NUM_REGISTERS-1 && offset >=0) {

	location = offset;
	printk("Changing current register to: %d\n",location);
	return location;    //Tells the user which register we are at.
	
    }
    else{
	printk("%d is not a valid register\n",(int)offset);
	return -1;   //tells user that the seek failed
    }  
}


/*---------------------------------------------------
Function name: proc_read

Purpose: Implements the read function call.

Details: Sends the user buffer the contents of the current 
         register in use.

-----------------------------------------------------*/
ssize_t proc_read(struct file *file, char *usr_buf,
		  size_t count, loff_t *pos)
{

    printk("READING!\n");
    
    if (copy_to_user(usr_buf, buffer[location], buffer_fill[location]) == -1){
	return -1;
	    }
    return buffer_fill[location];
}


/*---------------------------------------------------
Function name: proc_write

Purpose: Implements the write function call.

Details: Given the user buffer, fills the current register 
with the contents written to by the user.

-----------------------------------------------------*/
ssize_t proc_write(struct file *filep, const char *usr_buffer, size_t len, loff_t *offset){
    
    printk("WRITING!\n"); 
    buffer_fill[location] = 0;   
    
    if(copy_from_user(buffer[location], usr_buffer ,len) == -1){  //get the message.
	return -1;
    }
    buffer_fill[location] = len;
    
    printk("%s\n",buffer[location]);  
    
    return 0;    
}


/*---------------------------------------------------
Function name: proc_init

Details: creates the proc file entry when this device driver is loaded.

-----------------------------------------------------*/
int proc_init(void)
{
    proc_create(PROC_NAME, S_IWUSR | S_IRUGO, NULL, &proc_ops);    //read and write file permissions.
    
    return 0;
}


/*---------------------------------------------------
Function name: proc_exit

Details: deletes the proc file when this device driver is unloaded.

-----------------------------------------------------*/
void proc_exit(void)
{
    remove_proc_entry(PROC_NAME, NULL);
}


module_init(proc_init);
module_exit(proc_exit);

MODULE_DESCRIPTION("simple file read");
MODULE_AUTHOR("Kyle and Andrew");
MODULE_LICENSE("Dual BSD/GPL");  //Otherwise, kernel prints a warning that
                                 //code is not open source
