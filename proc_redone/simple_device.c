/* Optimizations: 
1. Make methods static - methods cannot be seen outside of this file.
2. add __init before the proc init, and __exit before the exit. Both are macros that 
   improve efficiency. __init for example takes the function out of memory once used.

 */

#include <linux/init.h>   //initilization
#include <linux/module.h> //Lots of stuff
#include <linux/kernel.h>  //lots of stuff
#include <linux/fs.h>  //Contains useful linux data structures.
#include <linux/uaccess.h>  //Moves kernel code to user space. (Lets you print to user space)

#include <linux/cdev.h> //Allows you to reigster the device from code.
#include <linux/types.h> //Major and minor numbers (dev_t)

#include <linux/proc_fs.h>  //open_proc and close proc


#define BUFFER_SIZE 128
#define PROC_NAME "simple_device"


ssize_t proc_write(struct file *filep, const char *buffer, size_t len, loff_t *offset);
ssize_t proc_read(struct file *file, char *usr_buf,
size_t count, loff_t *pos);




static struct proc_dir_entry *proc_file;


static struct file_operations proc_ops = {
    .owner = THIS_MODULE,
    .read = proc_read,
    .write = proc_write
};



/* This function is called when the module is loaded. */
int proc_init(void)
{

//    proc_file -> mode = s_IFREG | S_IRUGO;                     //Also try 0660
    proc_file = proc_create(PROC_NAME, 00444, NULL, &proc_ops);     //MAX permissions power is this 00444
    
    return 0;
}
/* This function is called when the module is removed. */
void proc_exit(void)
{
    remove_proc_entry(PROC_NAME, NULL);
}



//Does not seem to know about this file?
ssize_t proc_read(struct file *file, char *usr_buf,
		  size_t count, loff_t *pos)
{
    int rv = 0;
    char buffer[BUFFER_SIZE];
    static int completed = 0;
    printk("READING!\n");
    
    
    
    if (completed) {
	completed = 0;
	return 0;
    }
    completed = 1;
    rv = sprintf(buffer, "Hello World\n");
// copies kernel space buffer to user space usr buf */
    copy_to_user(usr_buf, buffer, rv);
    return rv;
}

ssize_t proc_write(struct file *filep, const char *buffer, size_t len, loff_t *offset){


    //char buffer_in[128];
    //Sprintf is (src, dest, length);
    // sprintf(buffer_in,buffer, len);
    printk("WRITING lets goooooo\n");
    

    return -1;
}


module_init(proc_init);
module_exit(proc_exit);
MODULE_LICENSE("Dual BSD/GPL");
MODULE_DESCRIPTION("simple file read");
MODULE_AUTHOR("SGG");
