#include "internal.h"
#include "globals.h"
#include "options.h"
#include "video.h"
#include "input.h"
#include "font.h"

u64 start_ms;
const char *config_path;

static void leaf_init(void);
static void leaf_fini(void);

int main(int argc, char **argv) {
    u64 frame;
    u32 frame_start_ms,
        frame_elapsed_ms;

    if (getenv("LEAF_PAUSE")) {
        INFO("Hit any key to continue once the instrument tool has been attached.\n");
        INFO("pid = %d\n", getpid());
        fflush(stdout);
        getchar();
    }

    config_path = "leaf.conf";

    if (argc > 1) {
        config_path = argv[1];
    }

    leaf_init();

    frame = 1;

    while (!window_closed()) {
        frame_start_ms = measure_time_now_ms();

        draw();
        handle_input();

        /* Sleep off rest of frame time. */
        frame_elapsed_ms = measure_time_now_ms() - frame_start_ms;
        if (frame_elapsed_ms < FPS_CAP_MS) {
            usleep(1000 * (FPS_CAP_MS - frame_elapsed_ms));
        }

        if (!(frame & 0x3f)) {
            if (reload_options_if_modified()) {
                load_colors_from_options();
            }
        }

        frame += 1;
    }

    leaf_fini();

    return 0;
}

static void leaf_init(void) {
    start_ms = measure_time_now_ms();

    init_options(config_path);
    init_font();
    init_video();
    create_font_textures();
    init_input();
}

static void leaf_fini(void) {
    fini_input();
    fini_video();
    fini_font();
}
