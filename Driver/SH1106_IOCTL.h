#ifndef SH1106_IOCTL_H
#define SH1106_IOCTL_H

#define BLACK 0
#define WHITE 1
#define INVERT 2

struct screen_size {
  int width;
  int height;
};

struct set_pixel {
  int x;
  int y;
  int colour;
};

#define GET_WIDTH _IOR('a', 'a', int *)
#define GET_HEIGHT _IOR('a', 'b', int *)
#define GET_SIZE _IOR('a', 'c', int *)
#define GET_CONTRAST _IOR('a', 'd', int *)
#define SET_CONTRAST _IOW('a', 'e', int *)
#define SET_PIXEL _IOW('a', 'f', struct set_pixel *)

#endif // !SH1106_IOCTL_H
