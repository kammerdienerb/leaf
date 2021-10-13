#include "internal.h"
#include "globals.h"
#include "font.h"
#include "options.h"

#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"


struct font_t _font;
struct font_t _bfont;
struct font_t _ifont;
struct font_t _bifont;

struct font_t *font;
struct font_t *bfont;
struct font_t *ifont;
struct font_t *bifont;

#define BUFF_FILL_NO_ERR   (0)
#define BUFF_FILL_ERR_NOF  (1)
#define BUFF_FILL_ERR_DIR  (2)
#define BUFF_FILL_ERR_PER  (3)
#define BUFF_FILL_ERR_UNK  (4)

static FILE * checked_open_file(const char *path, int *file_size) {
    FILE        *f;
    struct stat  fs;
    int          fd;
    int          status;

    status = BUFF_FILL_NO_ERR;
    errno  = 0;
    f      = fopen(path, "r");
    if (f) {
        fd = fileno(f);

        errno = 0;
        if (fstat(fd, &fs) != 0) {
            errno = 0;
            status = BUFF_FILL_ERR_NOF;
            goto out;
        } else if (S_ISDIR(fs.st_mode)) {
            errno = 0;
            status = BUFF_FILL_ERR_DIR;
            goto out;
        }

        *file_size = fs.st_size;
    }

    if (errno) {
        if (errno == ENOENT) {
            status = BUFF_FILL_ERR_NOF;
        } else if (errno == EISDIR) {
            status = BUFF_FILL_ERR_DIR;
        } else if (errno == EACCES) {
            status = BUFF_FILL_ERR_PER;
        } else {
            status = BUFF_FILL_ERR_UNK;
        }

        errno = 0;
    }

out:
    switch (status) {
        case BUFF_FILL_ERR_NOF:
            ERR("file '%s' does not exist\n", path);
        case BUFF_FILL_ERR_DIR:
            ERR("file '%s' is a directory\n", path);
        case BUFF_FILL_ERR_PER:
            ERR("file '%s': permission to read was denied\n", path);
        case BUFF_FILL_ERR_UNK:
            ERR("file '%s': unknown error\n", path);
    }

    return f;
}

static void * mmap_font_file(const char *path) {
    FILE *f;
    int   file_size;
    int   fd;
    void *buff;
    void *file_data;

    f  = checked_open_file(path, &file_size);
    fd = fileno(f);

    buff      = malloc(file_size);
    file_data = mmap(NULL, file_size, PROT_READ, MAP_SHARED, fd, 0);

    if (file_data == MAP_FAILED) {
        ERR("mmap for file '%s' failed\n", path);
    }

    memcpy(buff, file_data, file_size);

    munmap(file_data, file_size);
    fclose(f);

    return buff;
}

static void load_font(struct font_t *font, const char *font_name) {
    int success;

    if (font->file_data) {
        free(font->file_data);
    }
    font->file_data = mmap_font_file(font_name);
    success = stbtt_InitFont(&font->stbtt, font->file_data, 0);
    if (!success) {
        ERR("could not load font data for '%s'\n", font_name);
    }
    font->scale = stbtt_ScaleForPixelHeight(&font->stbtt, options->font_size);
    stbtt_GetFontVMetrics(&font->stbtt, &font->h, NULL, NULL);
    INFO("h     = %d\n", font->h);
    INFO("scale = %f\n", font->scale);
    font->bitmap = stbtt_GetCodepointBitmap(
                        &font->stbtt,
                        font->scale, font->scale,
                        'a',
                        &font->bm_w, &font->bm_h,
                        &font->bm_xoff, &font->bm_yoff);
}

void init_font(void) {
    font   = &_font;
    bfont  = &_ifont;
    ifont  = &_bfont;
    bifont = &_bifont;

    load_font(font, options->font);
    TIME("fonts initialized\n");
}

void fini_font(void) {
}
