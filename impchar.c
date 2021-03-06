/* 
 * chardev.c: Creates a read-only char device that says how many times 
 * you have read from the dev file 
 */ 

#include <linux/cdev.h> 
#include <linux/delay.h> 
#include <linux/device.h> 
#include <linux/fs.h> 
#include <linux/init.h> 
#include <linux/irq.h> 
#include <linux/kernel.h> 
#include <linux/module.h> 
#include <linux/poll.h> 


 
/*  Prototypes - this would normally go in a .h file */ 
static int device_open(struct inode *, struct file *); 
static int device_release(struct inode *, struct file *); 
static ssize_t device_read(struct file *, char __user *, size_t, loff_t *); 
static ssize_t dev_write(struct file *, const char *, size_t, loff_t *);               

#define BUF_LEN 80
#define SUCCESS 0 
#define DEVICE_NAME "impchar" /* Dev name as it appears in /proc/devices   */ 
#define BUF_LEN 80 /* Max length of the message from the device */ 
 
/* Global variables are declared as static, so are global within the file. */ 
static char   message[256] = {0}; 
static char   message2[256] = {0}; 
static short  size_of_message; 
static int major; /* major number assigned to our device driver */


enum { 
    CDEV_NOT_USED = 0, 
    CDEV_EXCLUSIVE_OPEN = 1, 
}; 
 
/* Is device open? Used to prevent multiple access to device */ 
static atomic_t already_open = ATOMIC_INIT(CDEV_NOT_USED); 
 
static char msg[BUF_LEN]; /* The msg the device will give when asked */ 
 
static struct class *cls; 
 
static struct file_operations impchar_fops = { 
    .read = device_read, 
    .write = dev_write, 
    .open = device_open, 
    .release = device_release, 
}; 
 
static int __init impchar_init(void) 
{ 
    major = register_chrdev(0, DEVICE_NAME, &impchar_fops); 
 
    if (major < 0) { 
        pr_alert("se registro impchar con el mayor Nro %d\n", major); 
        return major; 
    } 
 
    pr_info("asigne el Mayor Nro %d.\n", major); 
 
    cls = class_create(THIS_MODULE, DEVICE_NAME); 
    device_create(cls, NULL, MKDEV(major, 0), NULL, DEVICE_NAME); 
 
    pr_info("Device created on /dev/%s\n", DEVICE_NAME); 
 
    return SUCCESS; 
} 
 
static void __exit impchar_exit(void) 
{ 
    device_destroy(cls, MKDEV(major, 0)); 
    class_destroy(cls); 
 
    /* Unregister the device */ 
    unregister_chrdev(major, DEVICE_NAME); 
} 
 
/* Methods */ 
 
/* Called when a process tries to open the device file, like 
 * "sudo cat /dev/chardev" 
 */ 
static int device_open(struct inode *inode, struct file *file) 
{ 
    static int counter = 0; 
    static int salite=0;
    static int contador=0;
    char *p;
    static int contador2=0;
 
    if (atomic_cmpxchg(&already_open, CDEV_NOT_USED, CDEV_EXCLUSIVE_OPEN)) 
        return -EBUSY; 
 
    sprintf(msg, "Ya te dije %d veces Hello world!\n", counter++); 
    try_module_get(THIS_MODULE);
    p=message; 
    
    while(*p!='\0' && salite<20){
    /* lo paro en 20 porque no llega a funcionar y me tilda la maquina */ 
    salite++;
    p++;
    contador=contador+1;
    }
    printk(KERN_INFO "Este.. \"%s\" \n", message);
   
    while(contador>0){
    
    message2[contador2]=message[contador-1];
    
    contador2=contador2+1;
 
    contador=contador-1;
    
    }
    printk(KERN_INFO "alreves: \"%s\" \n", message2);
    
    
    return SUCCESS; 
} 
 
/* Called when a process closes the device file. */ 
static int device_release(struct inode *inode, struct file *file) 
{ 
    /* We're now ready for our next caller */ 
    atomic_set(&already_open, CDEV_NOT_USED); 
 
    /* Decrement the usage count, or else once you opened the file, you will 
     * never get rid of the module. 
     */ 
    module_put(THIS_MODULE); 
 
    return SUCCESS; 
} 
 
/* Called when a process, which already opened the dev file, attempts to 
 * read from it. 
 */ 
static ssize_t device_read(struct file *filp, /* see include/linux/fs.h   */ 
                           char __user *buffer, /* buffer to fill with data */ 
                           size_t length, /* length of the buffer     */ 
                           loff_t *offset) 
{ 
    /* Number of bytes actually written to the buffer */ 
    int bytes_read = 0; 
    const char *msg_ptr = msg; 
 
    if (!*(msg_ptr + *offset)) { /* we are at the end of message */ 
        *offset = 0; /* reset the offset */ 
        return 0; /* signify end of file */ 
    } 
 
    msg_ptr += *offset; 
 
    /* Actually put the data into the buffer */ 
    while (length && *msg_ptr) { 
        /* The buffer is in the user data segment, not the kernel 
         * segment so "*" assignment won't work.  We have to use 
         * put_user which copies data from the kernel data segment to 
         * the user data segment. 
         */ 
        put_user(*(msg_ptr++), buffer++); 
        length--; 
        bytes_read++; 
    } 
 
    *offset += bytes_read; 
 
    /* Most read functions return the number of bytes put into the buffer. */ 
    return bytes_read; 
} 
 


/* No pide que cuente letras, esta parte la puedo sacar. */ 
static ssize_t dev_write(struct file *filep, const char *buffer, size_t len, loff_t *offset){
   sprintf(message, "%s(%zu letras)", buffer, len);   // appending received string with its length
   size_of_message = strlen(message);                 // guardamos la longitud del mensaje
   printk(KERN_INFO "Recibimos %zu caracteres desde el usuario\n", len);

   return len;
}




 
module_init(impchar_init); 
module_exit(impchar_exit); 
 
MODULE_LICENSE("GPL");
