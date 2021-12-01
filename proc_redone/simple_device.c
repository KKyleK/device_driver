#include <linux/init.h>   //initilization
#include <linux/module.h> //Lots of stuff
#include <linux/kernel.h>  //lots of stuff
#include <linux/fs.h>  //Contains useful linux data structures.
#include <linux/uaccess.h>  //Moves kernel code to user space. (Lets you print to user space)

#include <linux/cdev.h> //Allows you to reigster the device from code.
#include <linux/types.h> //Major and minor numbers (dev_t)

#include <linux/proc_fs.h>


#define BUFFER_SIZE 128
#define PROC_NAME "simple_device"

ssize_t proc_read(struct file *file, char *usr_buf,
size_t count, loff_t *pos);

static struct file_operations proc_ops = {
    .owner = THIS_MODULE,
    .read = proc_read,
    


    
};


static int major;



/* This function is called when the module is loaded. */
int proc_init(void)
{
    major = register_chrdev(0, PROC_NAME, &proc_ops);   //otional
    
    if (major < 0) {
	
	printk("load failed\n");
        return major;
    }    
/* creates the /proc/hello entry */
    proc_create(PROC_NAME, major, NULL, &proc_ops);
    return 0;
}
/* This function is called when the module is removed. */
void proc_exit(void)
{
/* removes the /proc/hello entry */
    remove_proc_entry(PROC_NAME, NULL);
}



//was __user
ssize_t proc_read(struct file *file, char *usr_buf,
		  size_t count, loff_t *pos)
{
    int rv = 0;
    char buffer[BUFFER_SIZE];
    static int completed = 0;
    if (completed) {
	completed = 0;
	return 0;
    }
    completed = 1;
    rv = sprintf(buffer, "Hello World\n");
/* copies kernel space buffer to user space usr buf */
    copy_to_user(usr_buf, buffer, rv);
    return rv;
}

module_init(proc_init);
module_exit(proc_exit);
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Hello Module");
MODULE_AUTHOR("SGG");
