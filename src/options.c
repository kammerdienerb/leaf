#include "internal.h"
#include "globals.h"
#include "options.h"

#define SIMPLE_CONFIG_IMPL
#include "simple_config.h"

struct options_t  _options;
struct options_t *options;

static int geq_zero_i(struct scfg *cfg, int val) {
    scfg_validate_set_err(cfg, "value must be greater than or equal to zero");
    return val >= 0;
}

static int between_zero_one_f(struct scfg *cfg, float val) {
    scfg_validate_set_err(cfg, "value must be in the range [0.0, 1.0]");
    return val >= 0.0 && val <= 1.0;
}

static void add_options(struct scfg *cfg) {
    scfg_add_bool(cfg, "quiet", &options->quiet);
    scfg_default_bool(cfg, "quiet", 0);

    scfg_add_int(cfg, "win_start_w", &options->win_start_w);
    scfg_default_int(cfg, "win_start_w", 1440);
    scfg_validate_int(cfg, "win_start_w", geq_zero_i);

    scfg_add_int(cfg, "win_start_h", &options->win_start_h);
    scfg_default_int(cfg, "win_start_h", 1080);
    scfg_validate_int(cfg, "win_start_h", geq_zero_i);

    scfg_add_string(cfg, "font", &options->font);
    scfg_require(cfg, "font");

    scfg_add_int(cfg, "font_size", &options->font_size);
    scfg_default_int(cfg, "font_size", 12);
    scfg_validate_int(cfg, "font_size", geq_zero_i);

    scfg_add_string(cfg, "bg_color", &options->bg_color);
    scfg_default_string(cfg, "bg_color", "000000");

    scfg_add_string(cfg, "fg_color", &options->fg_color);
    scfg_default_string(cfg, "fg_color", "FFFFFF");

    scfg_add_float(cfg, "opacity", &options->opacity);
    scfg_default_float(cfg, "opacity", 1.0);
    scfg_validate_float(cfg, "opacity", between_zero_one_f);
}

static void parse_options_fail(struct scfg *cfg, const char *path) {
    int scfg_err;

    scfg_err = scfg_parse(cfg, path);

    if (scfg_err) {
        ERR("config error: %s\n", scfg_err_msg(cfg));
    }
}

static int parse_options_info(struct scfg *cfg, const char *path) {
    int scfg_err;

    scfg_err = scfg_parse(cfg, path);

    if (scfg_err) {
        INFO("config error: %s\n", scfg_err_msg(cfg));
        return 0;
    }

    return 1;
}

static void load_options(const char *path) {
    struct scfg *cfg;

    cfg = scfg_make();
    add_options(cfg);
    parse_options_fail(cfg, path);
    scfg_free(cfg);

    clock_gettime(CLOCK_REALTIME_COARSE, &options->load_time);
}

static void reload_options(const char *path) {
    struct scfg *cfg;

    cfg = scfg_make();
    add_options(cfg);
    if (!parse_options_info(cfg, path)) {
        INFO("config errors encountered when reloading\n");
    }
    scfg_free(cfg);

    clock_gettime(CLOCK_REALTIME_COARSE, &options->load_time);
}

void init_options(const char *path) {
    options       = &_options;
    options->path = strdup(path);
    load_options(path);

    TIME("loaded options from config file '%s'\n", path);
}

int reload_options_if_modified(void) {
    struct stat s;

    if (stat(options->path, &s) == -1) {
        WARN("could not stat config file '%s'\n", options->path);
        return 0;
    }

    if (s.st_mtime > options->load_time.tv_sec) {
        free((void*)options->font);
        free((void*)options->bg_color);
        free((void*)options->fg_color);
        reload_options(options->path);

        return 1;
    }

    return 0;
}
