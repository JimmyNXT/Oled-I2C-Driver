#include "SH1106.h"

#include <linux/delay.h>
#include <linux/i2c.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/slab.h>


#define I2C_BUS_AVAILABLE (1)          
#define SLAVE_DEVICE_NAME ("SH1106_DISPLAY") 
#define SH1106_SLAVE_ADDR (0x3C)      
static struct i2c_adapter *sh1106_adapter = NULL;
static struct i2c_client *sh1106_client = NULL;

static int I2C_Write(unsigned char *buf, unsigned int len) {
  return i2c_master_send(sh1106_client, buf, len);
}

static int I2C_Read(unsigned char *out_buf, unsigned int len) {
  return i2c_master_recv(sh1106_client, out_buf, len);
}

static void SH1106_Write(bool is_cmd, unsigned char data) {
  unsigned char buf[3] = {0};

  if (is_cmd == true)
    buf[0] = 0x00;
  else
    buf[0] = 0x40;
  buf[1] = data;

  I2C_Write(buf, 2);
}

static int SH1106_DisplayInit(void) {
  msleep(100);

  SH1106_Write(true, SH1106_DISPLAYOFF);
  SH1106_Write(true, SH1106_SETDISPLAYCLOCKDIV);
  SH1106_Write(true, 0x80);
  SH1106_Write(true, SH1106_SETMULTIPLEX);
  SH1106_Write(true, 0x3F);
  SH1106_Write(true, SH1106_SETDISPLAYOFFSET);
  SH1106_Write(true, 0x00);
  SH1106_Write(true, SH1106_SETSTARTLINE | 0x00);
  SH1106_Write(true, SH1106_CHARGEPUMP);
  SH1106_Write(true, 0x14);
  SH1106_Write(true, SH1106_MEMORYMODE);
  SH1106_Write(true, 0x00);
  SH1106_Write(true, SH1106_SEGREMAP | 0x1);
  SH1106_Write(true, SH1106_COMSCANDEC);
  SH1106_Write(true, SH1106_SETCOMPINS);
  SH1106_Write(true, 0x12);
  SH1106_Write(true, SH1106_SETCONTRAST);
  SH1106_Write(true, 0xCF);
  SH1106_Write(true, SH1106_SETPRECHARGE);
  SH1106_Write(true, 0xF1);
  SH1106_Write(true, SH1106_SETVCOMDETECT);
  SH1106_Write(true, 0x40);
  SH1106_Write(true, SH1106_DISPLAYALLON_RESUME);
  SH1106_Write(true, SH1106_NORMALDISPLAY);
  SH1106_Write(true, SH1106_DISPLAYON);

  return 0;
}

static void SH1106_Fill(unsigned char data) {
  SH1106_Write(true, SH1106_SETLOWCOLUMN | 0x0);  
  SH1106_Write(true, SH1106_SETHIGHCOLUMN | 0x0); 
  SH1106_Write(true, SH1106_SETSTARTLINE | 0x0);  

  int m_row = 0;
  int m_col = 2;

  static uint8_t buffer[SH1106_LCDWIDTH + 1] = {};
  buffer[0] = 0x40;

  for (int i = 0; i < SH1106_LCDHEIGHT / 8; i++) {
    SH1106_Write(true, 0xB0 + i + m_row);
    SH1106_Write(true, m_col & 0xf);
    SH1106_Write(true, 0x10 | (m_col >> 4));

    for (int j = 1; j < SH1106_LCDWIDTH + 1; j++) {
      buffer[j] = data;
    }
    I2C_Write(buffer, SH1106_LCDWIDTH + 1);
  }
}

static int sh1106_probe(struct i2c_client *client) {
  SH1106_DisplayInit();

  SH1106_Fill(0xFF);
  pr_info("OLED Probed!!!\n");

  return 0;
}

static void sh1106_remove(struct i2c_client *client) {
  SH1106_Fill(0x00);
  pr_info("OLED Removed!!!\n");
}

static const struct i2c_device_id sh1106_id[] = {{SLAVE_DEVICE_NAME, 0}, {}};
MODULE_DEVICE_TABLE(i2c, sh1106_id);

static struct i2c_driver sh1106_driver = {
    .driver =
        {
            .name = SLAVE_DEVICE_NAME,
            .owner = THIS_MODULE,
        },
    .probe = sh1106_probe,
    .remove = sh1106_remove,
    .id_table = sh1106_id,
};

static struct i2c_board_info oled_i2c_board_info = {
    I2C_BOARD_INFO(SLAVE_DEVICE_NAME, SH1106_SLAVE_ADDR)};

static int __init sh1106_driver_init(void) {
  int ret = -1;
  sh1106_adapter = i2c_get_adapter(I2C_BUS_AVAILABLE);

  if (sh1106_adapter != NULL) {
    sh1106_client =
        i2c_new_client_device(sh1106_adapter, &oled_i2c_board_info);

    if (sh1106_client != NULL) {
      i2c_add_driver(&sh1106_driver);
      ret = 0;
    }

    i2c_put_adapter(sh1106_adapter);
  }

  pr_info("Driver Added!!!\n");
  return ret;
}
/*
** Module Exit function
*/
static void __exit sh1106_driver_exit(void) {
  i2c_unregister_device(sh1106_client);
  i2c_del_driver(&sh1106_driver);
  pr_info("Driver Removed!!!\n");
}

module_init(sh1106_driver_init);
module_exit(sh1106_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("EmbeTronicX <embetronicx@gmail.com>");
MODULE_DESCRIPTION("Simple I2C driver explanation (SSD_1306 OLED Display Interface)");
MODULE_VERSION("1.34");
