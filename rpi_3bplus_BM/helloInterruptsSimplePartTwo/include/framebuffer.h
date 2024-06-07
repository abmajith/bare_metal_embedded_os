#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H


// just setting the frame buffer, it will be used after we set up the hdmi or DSI display driver module

#define FRAMEBUFFER_WIDTH  1024
#define FRAMEBUFFER_HEIGHT 768
#define FRAMEBUFFER_DEPTH  32

int framebuffer_init(void);

#endif // FRAMEBUFFER_H
