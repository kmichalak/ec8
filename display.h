#ifndef DISPLAY_H
#define DISPLAY_H

#include <inttypes.h>

typedef struct Display Display;

struct Display {
    uint64_t screen[32];    // 64x32 pixels
    void (*put_pixels)(Display *display, unsigned char *sprite, unsigned short sprite_size, unsigned char x, unsigned char y);

};

#endif