#include "internal.h"
#include "globals.h"
#include "options.h"
#include "font.h"
#include "video.h"
#define GL_LITE_IMPLEMENTATION
#include "gl_lite.h"

struct colors  _colors;
struct colors *colors;
GLFWwindow    *window;
int            win_w;
int            win_h;
int            fb_w;
int            fb_h;

GLuint         shader_prg;
GLuint         frag_shader;
const char    *frag_shader_src =
"#version 330 core                             \n"
"                                              \n"
"out     vec4      color;                      \n"
"in      vec3      in_color;                   \n"
"in      vec2      tex_coords;                 \n"
"uniform sampler2D tex;                        \n"
"                                              \n"
"void main() {                                 \n"
"    vec4 foo = texture(tex, tex_coords);      \n"
"    color = vec4(foo.r, foo.r, foo.r, foo.r); \n"
"}";

static void framebuffer_size_change_handler(GLFWwindow *win, int w, int h) {
    fb_w = w;
    fb_h = h;
    glViewport(0, 0, fb_w, fb_h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0f, win_w, win_h, 0.0f, -1.0f, 1.0f);
    glMatrixMode(GL_MODELVIEW);
    draw();
    INFO("framebuffer: w = %d, h = %d\n", fb_w, fb_h);
}

static void window_size_change_handler(GLFWwindow *win, int w, int h) {
    win_w = w;
    win_h = h;
    INFO("window:      w = %d, h = %d\n", win_w, win_h);
}

color_t hex_string_to_color(const char *str) {
    u32     x;
    color_t color;

    sscanf(str, "%x", &x);
    color.r = (x >> 16) & 0x0000FF;
    color.g = (x >> 8)  & 0x0000FF;
    color.b = (x)       & 0x0000FF;

    return color;
}

void load_colors_from_options(void) {
    colors->fg = hex_string_to_color(options->fg_color);
    colors->bg = hex_string_to_color(options->bg_color);
}

static void make_shader(GLuint which, GLuint *id, const char *src) {
    GLint  result;
    int    len;
    char  *buff;

    *id = glCreateShader(which);
    glShaderSource(*id, 1, &src, NULL);
    glCompileShader(*id);

    glGetShaderiv(*id, GL_COMPILE_STATUS, &result);
    glGetShaderiv(*id, GL_INFO_LOG_LENGTH, &len);

    if (len > 0) {
        buff = malloc(len + 1);
        glGetShaderInfoLog(*id, len, NULL, buff);
        ERR("gl shader error: %s\n", buff);
        free(buff);
    }
}

static void make_shader_prg(void) {
    GLint  result;
    int    len;
    char  *buff;

    shader_prg = glCreateProgram();
    glAttachShader(shader_prg, frag_shader);
    glLinkProgram(shader_prg);

    glGetProgramiv(shader_prg, GL_LINK_STATUS, &result);
    glGetProgramiv(shader_prg, GL_INFO_LOG_LENGTH, &len);

    if (len > 0) {
        buff = malloc(len + 1);
        glGetProgramInfoLog(shader_prg, len, NULL, buff);
        ERR("gl program error: %s\n", buff);
        free(buff);
    }
}

void init_video(void) {
    colors = &_colors;

    colors->_red._u32   = 0xFF0000;
    colors->_green._u32 = 0x00FF00;
    colors->_blue._u32  = 0x0000FF;

    load_colors_from_options();
    win_w = options->win_start_w;
    win_h = options->win_start_h;

    if (!gl_lite_init()) {
        ERR("could not initialize gl_lite\n");
    }

    if (!glfwInit()) {
        ERR("could not initialize glfw\n");
    }

    TIME("initialized glfw\n");

    glfwWindowHint(GLFW_RESIZABLE, 1);
    glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, 1);
    glfwWindowHintString(GLFW_X11_CLASS_NAME, "leaf");
    glfwWindowHintString(GLFW_WAYLAND_APP_ID, "leaf");

    window = glfwCreateWindow(win_w, win_h, "leaf", NULL, NULL);

    if (!window) {
        ERR("could not create the window\n");
    }

    TIME("created window\n");

    glfwMakeContextCurrent(window);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_change_handler);
    glfwSetWindowSizeCallback(window, window_size_change_handler);

    glfwGetFramebufferSize(window, &fb_w, &fb_h);
    glfwGetWindowSize(window, &win_w, &win_h);
    INFO("window:      w = %d, h = %d\n", win_w, win_h);
    INFO("framebuffer: w = %d, h = %d\n", fb_w, fb_h);

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glViewport(0, 0, fb_w, fb_h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0f, win_w, win_h, 0.0f, -1.0f, 1.0f);
    glMatrixMode(GL_MODELVIEW);

    make_shader(GL_FRAGMENT_SHADER, &frag_shader, frag_shader_src);
    make_shader_prg();
    glUseProgram(shader_prg);

    TIME("video initialized\n");
}

void fini_video(void) {
    glfwTerminate();
}

void create_font_textures(void) {
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glGenTextures(1, &font->texture);
    glBindTexture(GL_TEXTURE_2D, font->texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RED,
        font->bm_w,
        font->bm_h,
        0,
        GL_RED,
        GL_UNSIGNED_BYTE,
        font->bitmap);

    glUniform1i(glGetUniformLocation(shader_prg, "tex"), 0);
    glActiveTexture(GL_TEXTURE0);
}

int window_closed(void) {
    return glfwWindowShouldClose(window);
}

static void draw_char(int ch, rect_t *r) {
    glBegin(GL_QUADS);
        glTexCoord2i(0, 0); glVertex2f(r->x,        r->y);
        glTexCoord2i(0, 1); glVertex2f(r->x,        r->y + r->h);
        glTexCoord2i(1, 1); glVertex2f(r->x + r->w, r->y + r->h);
        glTexCoord2i(1, 0); glVertex2f(r->x + r->w, r->y);
    glEnd();
}

#if 0
static void draw_rect(rect_t *r, color_t color) {
    glBegin(GL_QUADS);
        glColor3f(CC2F(color.r),
                  CC2F(color.g),
                  CC2F(color.b));

        glVertex2f(r->x,        r->y);
        glVertex2f(r->x,        r->y + r->h);
        glVertex2f(r->x + r->w, r->y + r->h);
        glVertex2f(r->x + r->w, r->y);
    glEnd();
}
#endif

void draw(void) {
    rect_t r;

    glClearColor(CC2F(colors->bg.r),
                 CC2F(colors->bg.g),
                 CC2F(colors->bg.b),
                 options->opacity);

    glClear(GL_COLOR_BUFFER_BIT);

    r.x = r.y = 0;
    r.w = r.h = options->font_size * 2;
    draw_char('a', &r);

    glfwSwapBuffers(window);
}
