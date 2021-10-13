#ifndef __GLOBALS_H__
#define __GLOBALS_H__

#include "internal.h"

extern u64               start_ms; /* leaf.c    */
extern struct options_t *options;  /* options.c */
extern struct colors    *colors;   /* video.c   */
extern GLFWwindow       *window;   /* video.c   */
extern int               win_w;    /* video.c   */
extern int               win_h;    /* video.c   */
extern struct font_t      *font;   /* font.c    */
extern struct font_t     *bfont;   /* font.c    */
extern struct font_t     *ifont;   /* font.c    */
extern struct font_t    *bifont;   /* font.c    */

#endif
