#ifndef __OPTIONS_H__
#define __OPTIONS_H__

struct options_t {
    struct timespec  load_time;
    const char      *path;

    int         quiet;
    int         win_start_w;
    int         win_start_h;
    const char *font;
    int         font_size;
    const char *bg_color;
    const char *fg_color;
    float       opacity;
};

void init_options(const char *path);
int reload_options_if_modified(void);

#endif
