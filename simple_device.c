#include <linux/init.h>   //initilization
#include <linux/module.h> //Lots of stuff
#include <linux/kernel.h>  //lots of stuff
#include <linux/fs.h>  //Contains useful linux data structures.
#include <linux/uaccess.h>  //Moves kernel code to user space. (Lets you print to user space)

#include <linux/cdev.h> //Allows you to reigster the device from code.
#include <linux/types.h> //Major and minor numbers (dev_t)


#define DEVICE_NAME "memory"    //This is for licensing


static int dev_open(struct inode*, struct file*);     //Functions from fs.h
static int dev_release(struct inode*, struct file*);
static ssize_t dev_read(struct file*, char*, size_t, loff_t*);
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



//static struct cdev dev= {

//   .module = THIS_MODULE; //THIS_MODULE is a macrto from module.h
//    .file_operations = fops;


//};







//file:

//inode:


static int major;

//Static methods remember changes
static int simple_init(void) {
    major = register_chrdev(0, DEVICE_NAME, &fops);       //This is old code.


//    alloc_chrdev_region //Dynamic allocation of major number
//    struct cdev *my_cdev = cdev_alloc(); //correct way (updated way) to register a device driver.
//    my_cdev->ops = &fops;

    if (major < 0) {

	 printk("load failed\n");
        return major;
    }

    printk("Module has been loaded: %d\n", major);
    return 0;
}

static void simple_exit(void) {
    unregister_chrdev(major, DEVICE_NAME);
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

static ssize_t dev_read(struct file *filep, char *buffer, size_t len, loff_t *offset) {

    int errors = 0;
    char *message = "reading...";       //send this to the user
    int message_len = strlen(message);

    errors = copy_to_user(buffer, message, message_len);

    return 0;
}

module_init(simple_init);
module_exit(simple_exit);

 
