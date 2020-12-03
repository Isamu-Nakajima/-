// SPDX-License-Identifier: GPL-3.0
/*
 * Copyright (C) 2020 Isamu Nakajima All rights reserved.
 */


#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>
#include <linux/io.h>
#include <linux/delay.h>

MODULE_AUTHOR("Ryuichi Ueda and Isamu Nakajima");
MODULE_DESCRIPTION("driver for LED control");
MODULE_LICENSE("GPL");
MODULE_VERSION("0.0.1");

static dev_t dev;
static struct cdev cdv;
static struct class *cls = NULL;
static volatile u32 *gpio_base = NULL;

static ssize_t led_write(struct file* filp, const char* buf, size_t count, loff_t* pos)
{
	int num, time = 500;
	char c;
	if(copy_from_user(&c,buf,sizeof(char)))
		return -EFAULT;

	printk(KERN_INFO "receive %c\n",c);
	

	if(c == '1'){
		for(num = 0; num < 16; num++){
			switch(num){
				case 2:
				case 6:
				case 9:
				case 12:
					msleep(time * 2);
					break;
				case 8:
				case 10:
				case 11:
				case 15:
					gpio_base[7] = 1 << 25;
					msleep(time * 3);
					gpio_base[10] = 1 << 25;
					msleep(time);
					break;
				default:
					gpio_base[7] = 1 << 25;
					msleep(time);
					gpio_base[10] = 1 << 25;
					msleep(time);
					break;
			}

		}
	}else if(c == '0'){
		gpio_base[10] = 1 << 25;
	}
	return 1;
}

static ssize_t sushi_read(struct file* filp, char* buf, size_t count, loff_t* pos)
{
	int size = 0;
	char sushi[] = {'s', 'u', 's', 'h', 'i'};
	if(copy_to_user(buf + size, (const char *)sushi, sizeof(sushi))){
		printk(KERN_INFO "sushi : copy_to_user failed\n");
		return -EFAULT;
	}
	size += sizeof(sushi);
	return size;
}

static struct file_operations led_fops = {
	.owner = THIS_MODULE,
	.write = led_write,
	.read = sushi_read
};

static int __init init_mod(void)
{
	int retval;
	retval = alloc_chrdev_region(&dev, 0, 1, "myled");
	if(retval < 0){
		printk(KERN_ERR "alloc_chrdev_region failed.\n");
		return retval;
	}
	printk(KERN_INFO "%s is loaded. major:%d\n",__FILE__,MAJOR(dev));
	
	cdev_init(&cdv, &led_fops);
	retval = cdev_add(&cdv, dev, 1);
	if(retval < 0){
		printk(KERN_ERR "cdev_add failed. major:%d, minor:%d\n",MAJOR(dev),MINOR(dev));
		return retval;
	}
	
	cls = class_create(THIS_MODULE,"myled");
	if(IS_ERR(cls)){
		printk(KERN_ERR "class_create failed.");
		return PTR_ERR(cls);
	}
	device_create(cls, NULL, dev, NULL, "myled%d",MINOR(dev));
	
	gpio_base = ioremap_nocache(0xfe200000, 0xA0);
	
	const u32 led = 25;
	const u32 index = led/10;
	const u32 shift = (led%10)*3;
	const u32 mask = ~(0x7 << shift);
	gpio_base[index] = (gpio_base[index] & mask) | (0x1 << shift);

	return 0;
}

static void __exit cleanup_mod(void)
{
	cdev_del(&cdv);
	device_destroy(cls, dev);
	class_destroy(cls);
	unregister_chrdev_region(dev, 1);
	printk(KERN_INFO "%s is unloaded.major:%d\n",__FILE__,MAJOR(dev));
}

module_init(init_mod);
module_exit(cleanup_mod);
