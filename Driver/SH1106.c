#include "SH1106.h"
#include "SH1106_IOCTL.h"

#define I2C_BUS_AVAILABLE (1)
#define DEVICE_NAME ("SH1106_DISPLAY")
#define CLASSNAME "SH1106"
#define SH1106_ADDR (0x3C)
static struct i2c_adapter *sh1106_adapter = NULL;
static struct i2c_client *sh1106_client = NULL;
static uint8_t buffer[(SH1106_LCDWIDTH * SH1106_LCDHEIGHT) / 8];
static int majourNumber;
static struct device *SH1106_File_Device = NULL;
static struct class *SH1106_File_Class = NULL;

static int xyToIndex(int x, int y, int w) { return x + (y * w); }

static int SH1106_File_Open(struct inode *device_file, struct file *instance) {
  printk("Driver file opened");
  return 0;
}

static int SH1106_File_Close(struct inode *device_file, struct file *instance) {
  printk("Driver file opened");
  return 0;
}

static ssize_t SH1106_File_Read(struct file *filep, char *bufferp, size_t len,
                                loff_t *offset) {
  return 0;
}

static void flipBuffer(void) {
  bool bool_buffer[8 * 8];
  bool temp_bool_buffer[8 * 8];

  for (int i = 0; i < (8 * 8); i++) {
    bool_buffer[i] = false;
    temp_bool_buffer[i] = false;
  }

  int index = 0;
  uint8_t mask = 0x01;

  for (int i = 0; i < (SH1106_LCDWIDTH * SH1106_LCDHEIGHT) / 8; i = i + 8) {
    index = 0;

    for (int j = 0; j < 8; j++) {
      uint8_t c = buffer[i + j];

      for (int k = 0; k < 8; k++) {
        uint8_t temp_c = c & mask;
        if (temp_c == 0x01) {
          bool_buffer[index] = true;
        }
        c = c >> 1;
        index++;
      }
    }

    for (int x = 0; x < 8; x++) {
      for (int y = 0; y < 8; y++) {
        temp_bool_buffer[xyToIndex(x, y, 8)] = bool_buffer[xyToIndex(x, y, 8)];
      }
    }

    index = 0;

    for (int x = 0; x < 8; x++) {
      uint8_t c = 0x00;

      for (int y = 0; y < 8; y++) {
        c = c << 1;
        if (temp_bool_buffer[xyToIndex(x, y, 8)]) {
          c = c | mask;
        }
      }
      buffer[i + x] = c;
    }
  }
}

static ssize_t SH1106_File_Write(struct file *filep, const char *bufferp,
                                 size_t len, loff_t *offset) {
  printk("data recieved");

  if (len < (SH1106_LCDWIDTH * SH1106_LCDHEIGHT) / 8) {
    printk("Buffer written is of oncorrect length. %d %d", (int)len,
           (SH1106_LCDWIDTH * SH1106_LCDHEIGHT) / 8);
    return 0;
  }

  copy_from_user(buffer, bufferp, (SH1106_LCDWIDTH * SH1106_LCDHEIGHT) / 8);

  // flipBuffer();

  SH1106_Write_Buffer();

  return 0;
}

static long int SH1106_ioctl(struct file *filep, unsigned cmd,
                             unsigned long arg) {
  switch (cmd) {
  case GET_WIDTH:
    int32_t tempScreenWidth = SH1106_LCDWIDTH;
    if (copy_to_user((int *)arg, &tempScreenWidth, sizeof(tempScreenWidth))) {
      printk("Failed to copy screen width to user.");
    } else {
      printk("Copied screen width of %d to user.", tempScreenWidth);
    }
    break;
  case GET_HEIGHT:
    int32_t tempScreenHeight = SH1106_LCDHEIGHT;
    if (copy_to_user((int *)arg, &tempScreenHeight, sizeof(tempScreenHeight))) {
      printk("Failed to copy screen height to user.");
    } else {
      printk("Copied screen height of %d to user.", tempScreenHeight);
    }
    break;
  case GET_SIZE:
    struct screen_size tempScreenSize;
    tempScreenSize.height = SH1106_LCDHEIGHT;
    tempScreenSize.width = SH1106_LCDWIDTH;
    if (copy_to_user((struct screen_size *)arg, &tempScreenSize,
                     sizeof(tempScreenSize))) {
      printk("Failed to copy screen size to user.");
    } else {
      printk("Copied screen size to user.");
    }
    break;
  case GET_BRIGHTNESS:
    int32_t tempScreenBrightness = 0;
    if (copy_to_user((int *)arg, &tempScreenBrightness,
                     sizeof(tempScreenBrightness))) {
      printk("Failed to copy screen brightness to user.");
    } else {
      printk("Copied screen brightness of %d to user.", tempScreenBrightness);
    }
    break;
  case SET_BRIGHTNESS:
    break;
  case SET_PIXEL:
      int x, y, colour = 0;
      // set_pixel pix_val;

      if ((x < 0) || (x >= SH1106_LCDWIDTH) || (y < 0) || (y >= SH1106_LCDHEIGHT))
        return -1;
      // x is which column
      switch (colour) {
      case WHITE:
        buffer[x + (y / 8) * SH1106_LCDWIDTH] |= (1 << (y & 7));
        break;
      case BLACK:
        buffer[x + (y / 8) * SH1106_LCDWIDTH] &= ~(1 << (y & 7));
        break;
      case INVERT:
        buffer[x + (y / 8) * SH1106_LCDWIDTH] ^= (1 << (y & 7));
        break;
      }
    break;
  }

  return 0;
}

static struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = SH1106_File_Open,
    .release = SH1106_File_Close,
    .read = SH1106_File_Read,
    .write = SH1106_File_Write,
    .unlocked_ioctl = SH1106_ioctl,
};

static int I2C_Write(uint8_t *buf, unsigned int len) {
  return i2c_master_send(sh1106_client, buf, len);
}

static int I2C_Read(uint8_t *out_buf, unsigned int len) {
  return i2c_master_recv(sh1106_client, out_buf, len);
}

static void SH1106_Write(bool is_cmd, uint8_t data) {
  uint8_t buf[3] = {0};

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

static void SH1106_Write_Buffer(void) {
  SH1106_Write(true, SH1106_SETLOWCOLUMN | 0x0);
  SH1106_Write(true, SH1106_SETHIGHCOLUMN | 0x0);
  SH1106_Write(true, SH1106_SETSTARTLINE | 0x0);

  // int m_row = 0;
  int m_col = 2;
  int counter = 0;

  static uint8_t out_buffer[SH1106_LCDWIDTH + 1] = {};
  out_buffer[0] = 0x40;

  for (int i = 0; i < SH1106_LCDHEIGHT / 8; i++) {
    SH1106_Write(true, 0xB0 + i);
    SH1106_Write(true, SH1106_SETLOWCOLUMN | (m_col & 0xf));
    SH1106_Write(true, SH1106_SETHIGHCOLUMN | (m_col >> 4));

    for (int j = 1; j < SH1106_LCDWIDTH + 1; j++) {
      out_buffer[j] = buffer[counter];
      counter = counter + 1;
    }
    I2C_Write(out_buffer, SH1106_LCDWIDTH + 1);
  }
}

static void SH1106_Fill(uint8_t data) {
  for (int i = 0; i < SH1106_LCDWIDTH * SH1106_LCDHEIGHT / 8; i++) {
    buffer[i] = data;
  }
  SH1106_Write_Buffer();
}

static int sh1106_probe(struct i2c_client *client) {
  SH1106_DisplayInit();

  SH1106_Fill(0xFF);
  printk("OLED Probed!!!\n");

  return 0;
}

static void sh1106_remove(struct i2c_client *client) {
  SH1106_Fill(0x00);
  printk("OLED Removed!!!\n");
}

static const struct i2c_device_id sh1106_id[] = {{DEVICE_NAME, 0}, {}};
MODULE_DEVICE_TABLE(i2c, sh1106_id);

static struct i2c_driver sh1106_driver = {
    .driver =
        {
            .name = DEVICE_NAME,
            .owner = THIS_MODULE,
        },
    .probe = sh1106_probe,
    .remove = sh1106_remove,
    .id_table = sh1106_id,
};

static struct i2c_board_info oled_i2c_board_info = {
    I2C_BOARD_INFO(DEVICE_NAME, SH1106_ADDR)};

static int __init sh1106_driver_init(void) {
  printk("Initializing SH1106 Oled Driver. \n");

  majourNumber = register_chrdev(0, DEVICE_NAME, &fops);
  if (majourNumber < 0) {
    printk("Failed to register SH1106 Char Device Majout number.\n");
    return majourNumber;
  }
  printk("Successfully registered SH1106 Char Device Majour number.\n");

  SH1106_File_Class = class_create(CLASSNAME);
  if (IS_ERR(SH1106_File_Class)) {
    printk("Failed to register SH1106 Char Device Class.\n");
    unregister_chrdev(majourNumber, DEVICE_NAME);
    return PTR_ERR(SH1106_File_Class);
  }
  printk("Successfully registered SH1106 Char Device Class.\n");

  SH1106_File_Device = device_create(SH1106_File_Class, NULL,
                                     MKDEV(majourNumber, 0), NULL, DEVICE_NAME);

  if (IS_ERR(SH1106_File_Device)) {
    printk("Failed to register SH1106 Char Device.\n");
    class_destroy(SH1106_File_Class);
    unregister_chrdev(majourNumber, DEVICE_NAME);
    return PTR_ERR(SH1106_File_Class);
  }
  printk("Successfully registered SH1106 Char Device.\n");

  int ret = -1;
  sh1106_adapter = i2c_get_adapter(I2C_BUS_AVAILABLE);

  if (sh1106_adapter != NULL) {
    sh1106_client = i2c_new_client_device(sh1106_adapter, &oled_i2c_board_info);

    if (sh1106_client != NULL) {
      i2c_add_driver(&sh1106_driver);
      ret = 0;
    }

    i2c_put_adapter(sh1106_adapter);
  }

  printk("SH1106 Driver successfully loaded.\n");
  return ret;
}
/*
** Module Exit function
*/
static void __exit sh1106_driver_exit(void) {
  device_destroy(SH1106_File_Class, MKDEV(majourNumber, 0));
  class_unregister(SH1106_File_Class);
  class_destroy(SH1106_File_Class);
  unregister_chrdev(majourNumber, DEVICE_NAME);

  i2c_unregister_device(sh1106_client);
  i2c_del_driver(&sh1106_driver);

  printk("SH1106 Driver successfully removed.\n");
}

module_init(sh1106_driver_init);
module_exit(sh1106_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Dean Minnie");
MODULE_DESCRIPTION("SH1106 OLED Driver");
