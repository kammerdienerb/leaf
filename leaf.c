#include <SDL2/SDL.h>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <stdio.h>
#include <stdint.h>
#include <sys/time.h>

const int SCREEN_WIDTH = 2  * 640;
const int SCREEN_HEIGHT = 2  * 480;

typedef uint32_t u32;
typedef uint64_t u64;

#define FPS_TO_MS(fps)   ((1000) * (1.0 / (float)(fps)))
#define FPS_CAP_MS       ((u64)(FPS_TO_MS(30)))


#define TIME_ON(label)                                                 \
do {                                                                   \
    u32 _##label##_time;                                               \
    _##label##_time = SDL_GetTicks();

#define TIME_OFF(label)                                                \
    printf("[%s] %llums\n", #label, SDL_GetTicks() - _##label##_time); \
} while (0)



static u32 x, y;

int  init_font(FT_Library *ft_lib, FT_Face *ft_face);
void fini_font(FT_Library *ft_lib, FT_Face *ft_face);
int  init_video(SDL_Window **sdl_win, SDL_Renderer **sdl_ren, SDL_Texture **sdl_tex);
void fini_video(SDL_Window **sdl_win, SDL_Renderer **sdl_ren, SDL_Texture **sdl_tex);

void render_ch(int ch, FT_Face *ft_face, SDL_Renderer *sdl_ren, SDL_Texture *sdl_tex);

int main(int argc, char **args) {
    FT_Library    ft_lib;
    FT_Face       ft_face;
    SDL_Window   *sdl_win;
    SDL_Renderer *sdl_ren;
    SDL_Texture  *sdl_tex;
    SDL_Event     e;
    int           quit;
    u32           frame_start_ms, frame_elapsed_ms;
    u64           frame;

    TIME_ON(init_font) {
        if (!init_font(&ft_lib, &ft_face))             { return 1; }
    } TIME_OFF(init_font);

    TIME_ON(init_video) {
        if (!init_video(&sdl_win, &sdl_ren, &sdl_tex)) { return 1; }
    } TIME_OFF(init_video);

    quit  = 0;
    frame = 0;

    while (!quit) {
        frame_start_ms = SDL_GetTicks();

        SDL_RenderClear(sdl_ren);

        x = y = 100;
        const char *my_str = "leaf  abcdefghijklmnopqrstuvwxyz";
        for (int i = 0; i < strlen(my_str); i += 1) {
            render_ch(my_str[i], &ft_face, sdl_ren, sdl_tex);
        }

        x = 100;
        y = 200;
        const char *my_str2 = "      ABCDEFGHIJKLMNOPQRSTUVWXYZ";
        for (int i = 0; i < strlen(my_str2); i += 1) {
            render_ch(my_str2[i], &ft_face, sdl_ren, sdl_tex);
        }

        x = 100;
        y = 300;
        const char *my_str3 = "      if (my$thing.foo[0](is->cool);) {";
        for (int i = 0; i < strlen(my_str3); i += 1) {
            render_ch(my_str3[i], &ft_face, sdl_ren, sdl_tex);
        }

        SDL_RenderPresent(sdl_ren);

        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = 1;
            }
        }

        if (!quit) {
            frame_elapsed_ms = SDL_GetTicks() - frame_start_ms;

            if (frame_elapsed_ms < FPS_CAP_MS) {
                SDL_Delay(FPS_CAP_MS - frame_elapsed_ms);
            }

            frame += 1;
        }
    }

    fini_video(&sdl_win, &sdl_ren, &sdl_tex);
    fini_font(&ft_lib, &ft_face);

    return 0;
}

int init_font(FT_Library *ft_lib, FT_Face *ft_face) {
    int err;

    err = FT_Init_FreeType(ft_lib);

    if (err) {
        printf("font init error\n");
        return 0;
    }

/*     err = FT_New_Face(*ft_lib, "ProggyClean.ttf", 0, ft_face); */
/*     err = FT_New_Face(*ft_lib, "TolkienRegular.ttf", 0, ft_face); */
    err = FT_New_Face(*ft_lib, "luximb.ttf", 0, ft_face);

    if (err == FT_Err_Unknown_File_Format) {
        printf("font not a font error\n");
        return 0;
    } else if (err) {
        printf("font load error\n");
        return 0;
    }

    err = FT_Set_Char_Size(
                *ft_face, /* handle to face object           */
                0,        /* char_width in 1/64th of points  */
                16*64,    /* char_height in 1/64th of points */
                300,      /* horizontal device resolution    */
                300 );    /* vertical device resolution      */

    if (err) {
        printf("font size err\n");
        return 0;
    }

    return 1;
}

void fini_font(FT_Library *ft_lib, FT_Face *ft_face) {
    FT_Done_Face(*ft_face);
    FT_Done_FreeType(*ft_lib);
}

int init_video(SDL_Window **sdl_win, SDL_Renderer **sdl_ren, SDL_Texture **sdl_tex) {

    TIME_ON(sdl_init_video) {
        if (SDL_InitSubSystem(SDL_INIT_VIDEO) < 0) {
            printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
            return 0;
        }
    } TIME_OFF(sdl_init_video);


    TIME_ON(sdl_create_window) {
        *sdl_win = SDL_CreateWindow("leaf",
                                    SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                    SCREEN_WIDTH, SCREEN_HEIGHT,
                                    SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI);
        if (sdl_win == NULL) {
            printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
            return 0;
        }
    } TIME_OFF(sdl_create_window);


    TIME_ON(sdl_create_renderer) {
        *sdl_ren = SDL_CreateRenderer(*sdl_win, -1, SDL_RENDERER_ACCELERATED);
    } TIME_OFF(sdl_create_renderer);

    *sdl_tex = SDL_CreateTexture(*sdl_ren,
                                 SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC,
                                 SCREEN_WIDTH, SCREEN_HEIGHT);

    SDL_SetRenderDrawColor(*sdl_ren, 0, 0, 0, 200);
    SDL_SetWindowOpacity(*sdl_win, 0.9);

    return 1;
}

void fini_video(SDL_Window **sdl_win, SDL_Renderer **sdl_ren, SDL_Texture **sdl_tex) {
    SDL_DestroyTexture(*sdl_tex);
    SDL_DestroyRenderer(*sdl_ren);
    SDL_DestroyWindow(*sdl_win);
    SDL_Quit();
}


void render_ch(int ch, FT_Face *ft_face, SDL_Renderer *sdl_ren, SDL_Texture *sdl_tex) {
    int      err;
    FT_Bitmap b;
    FT_GlyphSlot  g;
    SDL_Texture *t;
    SDL_Rect r;

    FT_Load_Char(*ft_face, ch, FT_LOAD_RENDER);
    g = (*ft_face)->glyph;
    b = g->bitmap;

    t = SDL_CreateTexture(sdl_ren, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STATIC, b.width, b.rows);

    u32 *pixels = malloc(4 * b.width * b.rows);
    memset(pixels, 0, 4 * b.width * b.rows);
    for (int i = 0; i < b.width * b.rows; i += 1) {
        u32 r = b.buffer[i];
        pixels[i] |= r << 24;
        pixels[i] |= r << 16;
        pixels[i] |= r << 8;
        pixels[i] |= 0x000000FF;
    }

    r.x = 0;
    r.y = 0;
    r.w = b.width;
    r.h = b.rows;

    SDL_UpdateTexture(t, &r, pixels, 4 * b.width);

    r.x = x + g->bitmap_left;
    r.y = y - g->bitmap_top;

    x += g->advance.x >> 6;
    y += g->advance.y >> 6;

    SDL_RenderCopy(sdl_ren, t, NULL, &r);

    SDL_DestroyTexture(t);
}
