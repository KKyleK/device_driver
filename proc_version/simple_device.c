#include <linux/init.h>   //initilization
#include <linux/module.h> //Lots of stuff
#include <linux/kernel.h>  //lots of stuff
#include <linux/fs.h>  //Contains useful linux data structures.
#include <linux/uaccess.h>  //Moves kernel code to user space. (Lets you print to user space)

#include <linux/cdev.h> //Allows you to reigster the device from code.
#include <linux/types.h> //Major and minor numbers (dev_t)

#include <linux/proc_fs.h>



#define DEVICE_NAME "simple"    //This is for licensing
#define BUFFER_SIZE 20


static int dev_open(struct inode*, struct file*);     //Functions from fs.h
static int dev_release(struct inode*, struct file*);
//static ssize_t dev_read(struct file*, char*, size_t, loff_t*);
static ssize_t dev_read(struct file *file, char *usr_buf,
		 size_t count, loff_t *pos);



static ssize_t dev_write(struct file*, const char*, size_t, loff_t*);


//First important data strucute.
static struct file_operations fops = {   //fops by convention

    //. is a valid way to set a veriable in c. Here, open, read, write, and release
    //Are all inside of the file_operations structre specified in fs.h

    .open = dev_open,                  //structure contains pointers to these functions
    .read = dev_read,
    .write = dev_write,
    .release = dev_release,
};


static int major;


static int simple_init(void) {


    major = register_chrdev(0, DEVICE_NAME, &fops);   //otional

    if (major < 0) {

	 printk("load failed\n");
        return major;
    }

    proc_create(DEVICE_NAME,10,NULL, &fops);       //creates the "device" inside of the proc directory.
    
    printk("Module has been loaded: %d\n", major);

    
    return 0;
}


static void simple_exit(void) {
    //unregister_chrdev(major, DEVICE_NAME);   //optional
    remove_proc_entry(DEVICE_NAME,NULL);
    
    printk("module has been unloaded\n");
}

static int dev_open(struct inode *inodep, struct file *filep) {
   printk("device opened\n");
   return 0;
}

static ssize_t dev_write(struct file *filep, const char *buffer,
                         size_t len, loff_t *offset) {

   
   printk("device is read only\n");
   return 0;
}


static int dev_release(struct inode *inodep, struct file *filep) {
   printk("device closed\n");
   return 0;
}



//static ssize_t dev_read(struct file *filep, char *buffer, size_t len, loff_t *offset) {

static ssize_t dev_read(struct file *file, char *usr_buf,
		  size_t count, loff_t *pos){

    int rv = 0;
    char buffer[BUFFER_SIZE];
    static int completed = 0;
    if (completed) {
	completed = 0;
	return 0;
    }
    //  completed = 1;
    rv = sprintf(buffer, "Hello World\n");    //Puts Hello world into the buffer. rv is if it worked
/* copies kernel space buffer to user space usr buf */
    copy_to_user(usr_buf, buffer, rv);
    return rv;    //returns this amount to the user.



module_init(simple_init);
module_exit(simple_exit);

 
