#ifndef SH1106_IOCTL_H
#define SH1106_IOCTL_H

struct screen_size{
  int width;
  int height;
};

#define GET_WIDTH _IOR('a', 'a', int *)
#define GET_HEIGHT _IOR('a', 'b', int *)
#define GET_SIZE _IOR('a', 'c', int *)
#define GET_BRIGHTNESS _IOR('a', 'd', int *)
#define SET_BRIGHTNESS _IOW('a', 'e', int *)

#endif // !SH1106_IOCTL_H
