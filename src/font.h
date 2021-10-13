#ifndef __FONT_H__
#define __FONT_H__

#include "internal.h"
#include "stb_truetype.h"

struct font_t {
    void           *file_data;
    u8             *bitmap;
    int             bm_w;
    int             bm_h;
    int             bm_xoff;
    int             bm_yoff;
    stbtt_fontinfo  stbtt;
    float           scale;
    int             w;
    int             h;
    u32             texture;
};

void init_font(void);
void fini_font(void);

#endif
