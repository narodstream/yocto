#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <asm/errno.h>
#include <linux/timer.h>
#include <linux/gpio.h>

MODULE_LICENSE( "GPL" );
MODULE_AUTHOR( "Vladimir Vorobyev <vlad_vorobyev@mail.ru>" );

static int device_open(struct inode *, struct file *);
static int device_release(struct inode *, struct file *);

#define SUCCESS 0
#define DEVICE_NAME "mygpio"

static int Major;

static struct timer_list my_timer;

static int value_cnt = 0;
static int i;
static int timer_period = 200;

static struct gpio leds_gpios[] = {
	{ 80, GPIOF_OUT_INIT_LOW, "PORTF00" },
	{ 81, GPIOF_OUT_INIT_LOW, "PORTF01" },
	{ 83, GPIOF_OUT_INIT_LOW, "PORTF03" },
	{ 84, GPIOF_OUT_INIT_LOW, "PORTF04" },
	{ 86, GPIOF_OUT_INIT_LOW, "PORTF06" },
	{ 87, GPIOF_OUT_INIT_LOW, "PORTF07" },
	{ 88, GPIOF_OUT_INIT_LOW, "PORTF08" },
	{ 89, GPIOF_OUT_INIT_LOW, "PORTF09" }
};

static struct file_operations fops = {
  .open = device_open,
  .release = device_release
};

static void my_timer_callback(struct timer_list  *timer)
{
	mod_timer(&my_timer, jiffies + msecs_to_jiffies(timer_period));
    printk(KERN_NOTICE "timer_callback called %d\n", value_cnt);
    gpio_set_value(leds_gpios[value_cnt].gpio, 1);
    value_cnt++;
    if(value_cnt > 7) {
		gpio_set_value(leds_gpios[value_cnt-2].gpio, 0);
		value_cnt=0;
	}
	else
	{
		gpio_set_value(leds_gpios[value_cnt-2].gpio, 0);
	    if(value_cnt == 1) {
			gpio_set_value(leds_gpios[7].gpio, 0);
		}
	}
}

static int __init mygpiomodule_init( void ) {
	printk(KERN_INFO "+> module mygpiomodule start!\n");
	   Major = register_chrdev(0, DEVICE_NAME, &fops);

	if (Major < 0) {
		printk ("Registering the character device failed with %d\n", Major);
		return Major;
	}
	
	printk("<1>I was assigned major number %d.  To talk to\n", Major);
	printk("<1>the driver, create a dev file with\n");
	printk("'mknod /dev/mygpio c %d 0'.\n", Major);	

	return 0;
}

static void __exit mygpiomodule_exit( void ) {
	unregister_chrdev(Major, DEVICE_NAME);
	printk(KERN_INFO "+> module mygpiomodule unloaded!\n" );
}

static int device_open(struct inode *inode, struct file *file)
{
    int ret;
	if ((!gpio_is_valid(80)) && (!gpio_is_valid(81)) && \
		(!gpio_is_valid(83)) && (!gpio_is_valid(84)) && \
		(!gpio_is_valid(86)) && (!gpio_is_valid(87)) && \
		(!gpio_is_valid(88)) && (!gpio_is_valid(89)))
	{
	  printk(KERN_ALERT " GPIO numbers not valid.\n");
	  return -1;
	}
	ret = gpio_request_array(leds_gpios, ARRAY_SIZE(leds_gpios));
	if (ret)
	{
	  printk(KERN_ALERT "Gpio request failed.\n");
	  return -2;
	}
	for(i=0;i<8;i++)
	{
		gpio_direction_output(leds_gpios[i].gpio, 0);
		gpio_set_value(leds_gpios[i].gpio, 0);
	}
	timer_setup( &my_timer, my_timer_callback, 0 );
	ret = mod_timer(&my_timer, jiffies + msecs_to_jiffies(200));
	if (ret)
		printk(KERN_ALERT "Error in mod_timer\n");
	return SUCCESS;
}

static int device_release(struct inode *inode, struct file *file)
{
	int ret;
	gpio_free_array(leds_gpios, ARRAY_SIZE(leds_gpios));
	ret = del_timer( &my_timer );
	if (ret)
	  printk(KERN_ALERT "The timer is still in use...\n");
	return 0;
}

module_init( mygpiomodule_init );
module_exit( mygpiomodule_exit );

