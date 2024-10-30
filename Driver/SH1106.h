#ifndef SH1106_H
#define SH1106_H

#include <linux/delay.h>
#include <linux/i2c.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/fs.h>
#include <linux/ioctl.h>
#include <linux/uaccess.h>

#define SH1106_LCDWIDTH 128
#define SH1106_LCDHEIGHT 64

#define SH1106_SETCONTRAST 0x81
#define SH1106_DISPLAYALLON_RESUME 0xA4
#define SH1106_DISPLAYALLON 0xA5
#define SH1106_NORMALDISPLAY 0xA6
#define SH1106_INVERTDISPLAY 0xA7
#define SH1106_DISPLAYOFF 0xAE
#define SH1106_DISPLAYON 0xAF
#define SH1106_SETDISPLAYOFFSET 0xD3
#define SH1106_SETCOMPINS 0xDA
#define SH1106_SETVCOMDETECT 0xDB
#define SH1106_SETDISPLAYCLOCKDIV 0xD5
#define SH1106_SETPRECHARGE 0xD9
#define SH1106_SETMULTIPLEX 0xA8
#define SH1106_SETLOWCOLUMN 0x00
#define SH1106_SETHIGHCOLUMN 0x10
#define SH1106_SETSTARTLINE 0x40
#define SH1106_SETPAGEADDRESS 0xB0
#define SH1106_MEMORYMODE 0x20
#define SH1106_COLUMNADDR 0x21
#define SH1106_PAGEADDR 0x22
#define SH1106_COMSCANINC 0xC0
#define SH1106_COMSCANDEC 0xC8
#define SH1106_SEGREMAP 0xA0
#define SH1106_CHARGEPUMP 0x8D
#define SH1106_EXTERNALVCC 0x1
#define SH1106_SWITCHCAPVCC 0x2
#define SH1106_ACTIVATE_SCROLL 0x2F
#define SH1106_DEACTIVATE_SCROLL 0x2E
#define SH1106_SET_VERTICAL_SCROLL_AREA 0xA3
#define SH1106_RIGHT_HORIZONTAL_SCROLL 0x26
#define SH1106_LEFT_HORIZONTAL_SCROLL 0x27
#define SH1106_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL 0x29
#define SH1106_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL 0x2A

static void SH1106_Write_Buffer(void);
static int SH1106_File_Open(struct inode * device_file, struct file * instance);
static int SH1106_File_Close(struct inode * device_file, struct file * instance);
static ssize_t SH1106_File_Read(struct file *filep, char * bufferp, size_t len, loff_t *offset);
static long int SH1106_ioctl(struct file *filep, unsigned cmd, unsigned long arg);
static void flipBuffer(void);
static ssize_t SH1106_File_Write(struct file * filep, const char *bufferp, size_t len, loff_t *offset);
static int I2C_Write(uint8_t *buf, unsigned int len);
static int I2C_Read(uint8_t *out_buf, unsigned int len);
static void SH1106_Write(bool is_cmd, uint8_t data);
static int SH1106_DisplayInit(void);
static void SH1106_Write_Buffer(void);
static void SH1106_Fill(uint8_t data);
static int sh1106_probe(struct i2c_client *client);
static void sh1106_remove(struct i2c_client *client);
static int __init sh1106_driver_init(void);
static void __exit sh1106_driver_exit(void);

#endif // !SH1106_H
