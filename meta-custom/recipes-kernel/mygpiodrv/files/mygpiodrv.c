#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/ioctl.h>
#include <linux/uaccess.h>
#include <linux/delay.h>
#include <asm/errno.h>
#include <linux/gpio.h>
#include "dev.h"

MODULE_LICENSE( "GPL" );
MODULE_AUTHOR( "Vladimir Vorobyev <vlad_vorobyev@mail.ru>" );

static int device_open(struct inode *, struct file *);
static int device_release(struct inode *, struct file *);
static ssize_t device_read(struct file *file, /* см. include/linux/fs.h   */
            char __user * buffer,             /* буфер для сообщения */
            size_t length,                    /* размер буфера       */
            loff_t * offset);
static ssize_t
device_write(struct file *file,
            const char __user * buffer, size_t length, loff_t * offset);
static long device_ioctl( struct file *f, unsigned int cmd, unsigned long arg);

static struct gpio leds_gpios[] = {
	{ 80, GPIOF_OUT_INIT_LOW, "PORTF00" },
	{ 81, GPIOF_OUT_INIT_LOW, "PORTF01" },
	{ 84, GPIOF_OUT_INIT_LOW, "PORTF04" },
	{ 86, GPIOF_OUT_INIT_LOW, "PORTF06" },
	{ 87, GPIOF_OUT_INIT_LOW, "PORTF07" },
	{ 88, GPIOF_OUT_INIT_LOW, "PORTF08" },
	{ 89, GPIOF_OUT_INIT_LOW, "PORTF09" }
};

enum hd44780_pin {
	PIN_CTRL_E,
	PIN_CTRL_RS,
	PIN_CTRL_RW,
	PIN_DATA4,
	PIN_DATA5,
	PIN_DATA6,
	PIN_DATA7,
	PIN_NUM
};

#define SUCCESS 0
#define DEVICE_NAME "mygpio"
#define e_set() gpio_set_value(leds_gpios[PIN_CTRL_E].gpio, 1)  //установка линии Е в 1
#define e_reset() gpio_set_value(leds_gpios[PIN_CTRL_E].gpio, 0)  //установка линии Е в 0
#define rs_set() gpio_set_value(leds_gpios[PIN_CTRL_RS].gpio, 1)  //установка линии Е в 1
#define rs_reset() gpio_set_value(leds_gpios[PIN_CTRL_RS].gpio, 0)  //установка линии Е в 0
#define setwrite() gpio_set_value(leds_gpios[PIN_CTRL_RW].gpio, 0) //установка линии RW в 0
#define setread() gpio_set_value(leds_gpios[PIN_CTRL_RW].gpio, 1) //установка линии RW в 1
#define d4_set() gpio_set_value(leds_gpios[PIN_DATA4].gpio, 1) //установка линии D4 в 1
#define d4_reset() gpio_set_value(leds_gpios[PIN_DATA4].gpio, 0) //установка линии D4 в 0
#define d5_set() gpio_set_value(leds_gpios[PIN_DATA5].gpio, 1) //установка линии D5 в 1
#define d5_reset() gpio_set_value(leds_gpios[PIN_DATA5].gpio, 0) //установка линии D5 в 0
#define d6_set() gpio_set_value(leds_gpios[PIN_DATA6].gpio, 1) //установка линии D6 в 1
#define d6_reset() gpio_set_value(leds_gpios[PIN_DATA6].gpio, 0) //установка линии D6 в 0
#define d7_set() gpio_set_value(leds_gpios[PIN_DATA7].gpio, 1) //установка линии D7 в 1
#define d7_reset() gpio_set_value(leds_gpios[PIN_DATA7].gpio, 0) //установка линии D7 в 0

#define MAJOR_NUM 202

static int i;
static uint32_t CUR_POS;

#define BUF_LEN 80
static char Message[BUF_LEN];

#define IOCTL_LCD_CLEAR _IOWR(MAJOR_NUM, 0, int)
#define IOCTL_LCD_SET_POS _IOWR(MAJOR_NUM, 1, int)
#define IOCTL_GET_NTH_BYTE _IOWR(MAJOR_NUM, 2, int)

static struct file_operations fops = {
	.read = device_read,
	.write = device_write,
	.unlocked_ioctl = device_ioctl,
	.open = device_open,
	.release = device_release,
};

void sendhalfbyte(uint8_t dt)
{
	e_set(); //включаем линию E
	udelay(1);
	if(((dt >> 3)&0x01)==1) {d7_set();} else {d7_reset();}
    if(((dt >> 2)&0x01)==1) {d6_set();} else {d6_reset();}
	if(((dt >> 1)&0x01)==1) {d5_set();} else {d5_reset();}
    if((dt&0x01)==1) {d4_set();} else {d4_reset();}
	udelay(1);
	e_reset(); //выключаем линию E
	udelay(1);
}

void sendbyte(uint8_t c, uint8_t mode)
{
	uint8_t hc=0;
	if(mode==0) rs_reset();
	else rs_set();
	hc=c>>4;
	sendhalfbyte(hc);
	sendhalfbyte(c);
}

static void LCD_Clear(void)
{
	sendbyte(0x01,0);
	mdelay(2);
}

void LCD_SetPos(uint8_t x,uint8_t y)
{
	switch(y)
	{
		case 0:
			CUR_POS=x|0x80;
			break;
		case 1:
			CUR_POS=(0x40+x)|0x80;
			break;
		case 2:
			CUR_POS=(0x14+x)|0x80;
			break;
		case 3:
			CUR_POS=(0x54+x)|0x80;
			break;
	}
	sendbyte(CUR_POS,0);
}

static void LCD_String(char*st)
{
	uint8_t i=0;
	while(st[i]!=0)
	{
		sendbyte(st[i],1);
		i++;
		udelay(10);
	}
}

static void hd44780_init(void)
{
	mdelay(50);
	setwrite();//запись
	mdelay(100);
	sendhalfbyte(0x03);
	mdelay(4);
	udelay(500);
	sendhalfbyte(0x03);
	mdelay(4);
	udelay(500);
	sendhalfbyte(0x03);
	mdelay(4);
	udelay(500);
	sendhalfbyte(0x02);
	sendbyte(0x28,0); //режим 4 бит, 2 линии (для нашего большого дисплея это 4 линии, шрифт 5х8
	sendbyte(0x08,0); //дисплей пока выключаем
	udelay(1000);
	sendbyte(0x01,0); //уберем мусор
	udelay(2000);
	sendbyte(0x06,0); // пишем вправо
	udelay(1000);
	sendbyte(0x0C,0); //дисплей включаем (D=1), курсоры никакие не нужны
	sendbyte(0x02,0); //курсор на место
	sendbyte(0X80,0); //SET POS LINE 0
	udelay(2000);
}

static int __init mygpiomodule_init( void ) {
	printk(KERN_INFO "+> module mygpiomodule start!\n");
	   register_chrdev(MAJOR_NUM, DEVICE_NAME, &fops);

	printk("<1>the driver, create a dev file with\n");
	printk("'mknod /dev/mygpio c %d 0'.\n", MAJOR_NUM);	

	return 0;
}

static void __exit mygpiomodule_exit( void ) {
	unregister_chrdev(MAJOR_NUM, DEVICE_NAME);
	
	printk(KERN_INFO "+> module mygpiomodule unloaded!\n" );
}

static ssize_t device_read(struct file *file, /* см. include/linux/fs.h   */
            char __user * buffer,             /* буфер для сообщения */
            size_t length,                    /* размер буфера       */
            loff_t * offset)
{
	return 0;
}

static ssize_t
device_write(struct file *file,
            const char __user * buffer, size_t length, loff_t * offset)
{
	for(i=0; i<length && i<BUF_LEN; i++)
		get_user(Message[i], buffer+i);
	LCD_String(Message);
	printk(KERN_INFO "+> mygpio write: \"%s\"\n", Message);
	return i;	
}

static long device_ioctl( struct file *f, 
                      unsigned int ioctl_num, unsigned long ioctl_param ) {
	switch(ioctl_num) {
	  case IOCTL_LCD_CLEAR:
		LCD_Clear();
 	 	printk(KERN_INFO "+> hd44780 is cleared!\n" );
		return 4;
		break;
	  case IOCTL_LCD_SET_POS:
		LCD_SetPos(ioctl_param & 0x000000FF, \
			(ioctl_param >> 8) & 0x000000FF);
		return 5;
		break;
  	  case IOCTL_GET_NTH_BYTE:
		hd44780_init();
		mdelay(200);
 	 	printk(KERN_INFO "+> hd44780 is initialised!\n" );
		return 6;
		break;
	  default: 
		 return -ENOTTY;
	}
	return 0;
}

static int device_open(struct inode *inode, struct file *file)
{
	int ret;
	if ((!gpio_is_valid(80)) && (!gpio_is_valid(81)) && \
		(!gpio_is_valid(84)) && (!gpio_is_valid(86)) && \
		(!gpio_is_valid(87)) && (!gpio_is_valid(88)) && \
		(!gpio_is_valid(89)))
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
	for(i=0;i<7;i++)
	{
		gpio_direction_output(leds_gpios[i].gpio, 0);
		gpio_set_value(leds_gpios[i].gpio, 0);
	}
	
	mdelay(200);
	printk(KERN_INFO "+> mygpio is opened!\n" );
	return SUCCESS;
}

static int device_release(struct inode *inode, struct file *file)
{
	gpio_free_array(leds_gpios, ARRAY_SIZE(leds_gpios));
	printk(KERN_INFO "+> mygpio is closed!\n" );
	return 0;
}

module_init( mygpiomodule_init );
module_exit( mygpiomodule_exit );

