#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/version.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
#include <linux/i2c.h>
#include <linux/kernel.h>

#define DEVICE_NAME "ssd1306"
#define DEVICE_ADDRESS 0x3c
#define BUS_ID 1

static struct i2c_adapter * ssd1306_adapter = NULL;
static struct i2c_client * ssd1306_client = NULL;

static const struct i2c_device_id ssd1306_id_table[] = {
		{ DEVICE_NAME, 0 }, 
		{ }
};

MODULE_DEVICE_TABLE(i2c, ssd1306_id_table);

static struct i2c_driver ssd1306_driver = {
	.driver = {
		.name = DEVICE_NAME,
		.owner = THIS_MODULE
	},
  .id_table = ssd1306_id_table,
  // .probe = ssd1306_probe,
  // .remove = ssd1306_remove
};

static struct i2c_board_info ssd_1306_board_info = {
	I2C_BOARD_INFO(DEVICE_NAME, DEVICE_ADDRESS)
};

static int I2C_Read(unsigned char *out_buf, unsigned int len)
{
    int ret = i2c_master_recv(ssd1306_client, out_buf, len);
    
    return ret;
}

static int I2C_Write(unsigned char *buf, unsigned int len)
{
    /*
    ** Sending Start condition, Slave address with R/W bit, 
    ** ACK/NACK and Stop condtions will be handled internally.
    */ 
    int ret = i2c_master_send(ssd1306_client, buf, len);
    
    return ret;
}

static void SSD1306_Write(bool is_cmd, unsigned char data)
{
    unsigned char buf[2] = {0};
    int ret;
    
    if( is_cmd == true )
    {
        buf[0] = 0x00;
    }
    else
    {
        buf[0] = 0x40;
    }
    
    buf[1] = data;
    
    ret = I2C_Write(buf, 2);
}

static int __init ssd1306_init(void){
  int ret = -1;
  ssd1306_adapter = i2c_get_adapter(BUS_ID);

	if(ssd1306_adapter != NULL) {
		ssd1306_client = i2c_new_client_device(ssd1306_adapter, &ssd_1306_board_info);
		if(ssd1306_client != NULL) {
			if(i2c_add_driver(&ssd1306_driver) != -1) {
				ret = 0;
			}
			else
				printk("Can't add driver...\n");
		}
		i2c_put_adapter(ssd1306_adapter);
	}

  return ret;
  // return i2c_add_driver(&ssd1306_driver);
}
module_init(ssd1306_init);

static void __exit ssd1306_exit(void){
  i2c_del_driver(&ssd1306_driver);
}
module_exit(ssd1306_exit);


MODULE_AUTHOR("Dean Minnie");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("A driver for reading out a BMP280 temperature sensor");


