#ifndef __VIDEO_H__
#define __VIDEO_H__

typedef union {
    struct {
        u8 r, g, b;
    };
    u32 _u32;
} color_t;

struct colors {
    color_t bg;
    color_t fg;

    color_t _red;
    color_t _green;
    color_t _blue;
};

typedef struct {
    int x, y, w, h;
} rect_t;


#define CC2F(c) (((u8)(c)) / 255.0f)


void init_video(void);
void fini_video(void);

void create_font_textures(void);
void load_colors_from_options(void);
color_t hex_string_to_color(const char *str);
int     window_closed(void);
void    draw(void);

#endif
