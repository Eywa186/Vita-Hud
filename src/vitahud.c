#include <psp2/kernel/modulemgr.h>
#include <psp2/kernel/threadmgr.h>
#include <psp2/ctrl.h>
#include <psp2/power.h>
#include <psp2/rtc.h>
#include <psp2/display.h>
#include <psp2/appmgr.h>
#include <psp2/io/fcntl.h>
#include <psp2/io/stat.h>
#include <taihen.h>

#define CONFIG_DIR  "ur0:data/VitaHUD"
#define CONFIG_PATH "ur0:data/VitaHUD/config.txt"

#define MENU_COMBO (SCE_CTRL_LTRIGGER | SCE_CTRL_RTRIGGER | SCE_CTRL_START)

#define POS_BOTTOM_RIGHT 0
#define POS_BOTTOM_LEFT  1
#define POS_TOP_RIGHT    2
#define POS_TOP_LEFT     3
#define POS_COUNT        4

#define SIZE_MICRO   0
#define SIZE_NORMAL  1
#define SIZE_LARGE   2
#define SIZE_COUNT   3

#define LAYOUT_SINGLE  0
#define LAYOUT_COMPACT 1
#define LAYOUT_STACKED 2
#define LAYOUT_ICONS   3
#define LAYOUT_COUNT   4

#define COLOR_AUTO       0
#define COLOR_WHITE      1
#define COLOR_GREEN      2
#define COLOR_YELLOW     3
#define COLOR_RED        4
#define COLOR_CYAN       5
#define COLOR_MAGENTA    6
#define COLOR_ORANGE     7
#define COLOR_BLUE       8
#define COLOR_PINK       9
#define COLOR_LIME       10
#define COLOR_SILVER     11
#define COLOR_BLACK      12
#define COLOR_PURPLE     13
#define COLOR_VIOLET     14
#define COLOR_GOLD       15
#define COLOR_AMBER      16
#define COLOR_MINT       17
#define COLOR_AQUA       18
#define COLOR_SKY        19
#define COLOR_NAVY       20
#define COLOR_MAROON     21
#define COLOR_CRIMSON    22
#define COLOR_ROSE       23
#define COLOR_LAVENDER   24
#define COLOR_TURQUOISE  25
#define COLOR_EMERALD    26
#define COLOR_FOREST     27
#define COLOR_SLATE      28
#define COLOR_GRAY       29
#define COLOR_DARK_GRAY  30
#define COLOR_COUNT      31

#define BG_TRANSPARENT 0
#define BG_BLACK       1
#define BG_GRAY        2
#define BG_NAVY        3
#define BG_GREEN       4
#define BG_PURPLE      5
#define BG_TEAL        6
#define BG_RED         7
#define BG_WHITE       8
#define BG_CYAN        9
#define BG_MAGENTA     10
#define BG_BLUE        11
#define BG_GOLD        12
#define BG_MAROON      13
#define BG_FOREST      14
#define BG_SLATE       15
#define BG_COUNT       16

#define LANG_EN 0
#define LANG_ES 1
#define LANG_FR 2
#define LANG_DE 3
#define LANG_IT 4
#define LANG_PT 5
#define LANG_NL 6
#define LANG_ID 7
#define LANG_TR 8
#define LANG_PL 9
#define LANG_COUNT 10

#define FONT_DEFAULT  0
#define FONT_BOLD     1
#define FONT_THIN     2
#define FONT_WIDE     3
#define FONT_TALL     4
#define FONT_COMPACT  5
#define FONT_DOUBLE   6
#define FONT_BLOCK    7
#define FONT_SHADOW   8
#define FONT_SOFT     9
#define FONT_SHARP    10
#define FONT_RETRO    11
#define FONT_PSP      12
#define FONT_VITA     13
#define FONT_MINI     14
#define FONT_SQUARE   15
#define FONT_ROUNDED  16
#define FONT_DIGITAL  17
#define FONT_ARCADE   18
#define FONT_CLEAN    19
#define FONT_COUNT    20

#define TOGGLE_SELECT 0
#define TOGGLE_UP     1
#define TOGGLE_DOWN   2
#define TOGGLE_LEFT   3
#define TOGGLE_RIGHT  4
#define TOGGLE_COUNT  5

#define AUTO_HIDE_OFF 0
#define AUTO_HIDE_3S  1
#define AUTO_HIDE_5S  2
#define AUTO_HIDE_10S 3
#define AUTO_HIDE_COUNT 4

#define THEME_DEFAULT   0
#define THEME_VITASHELL 1
#define THEME_PSP       2
#define THEME_MATRIX    3
#define THEME_NEON      4
#define THEME_RETRO     5
#define THEME_MINIMAL   6
#define THEME_COUNT     7

#define HUD_THEME_DEFAULT 0
#define HUD_THEME_STEALTH 1
#define HUD_THEME_NEON    2
#define HUD_THEME_MATRIX  3
#define HUD_THEME_GOLD    4
#define HUD_THEME_ICE     5
#define HUD_THEME_BLOOD   6
#define HUD_THEME_CLEAN   7
#define HUD_THEME_COUNT   8

#define PROFILE_1 0
#define PROFILE_2 1
#define PROFILE_3 2
#define PROFILE_COUNT 3

/*
 * MENU ORDER ONLY.
 * Render/glitch hook path is intentionally untouched.
 * CPU/BUS/GPU/APP/RAM/IP are now directly under CLOCK for faster toggling.
 * Theme/color and profile groups are exposed as clear menu sections.
 */
#define ITEM_HUD          0
#define ITEM_LAYOUT       1
#define ITEM_POSITION     2
#define ITEM_X_OFFSET     3
#define ITEM_Y_OFFSET     4
#define ITEM_SIZE         5
#define ITEM_FONT         6
#define ITEM_FPS          7
#define ITEM_BATTERY      8
#define ITEM_TIME         9
#define ITEM_CPU_HUD      10
#define ITEM_BUS_HUD      11
#define ITEM_GPU_HUD      12
#define ITEM_APP_ID_HUD   13
#define ITEM_RAM_HUD      14
#define ITEM_IP_HUD       15
#define ITEM_CHARGING     16
#define ITEM_TIMEMODE     17
#define ITEM_THEME_MENU   18
#define ITEM_THEME        19
#define ITEM_HUD_THEME    20
#define ITEM_HUD_TEXT     21
#define ITEM_HUD_SHADOW   22
#define ITEM_HUD_ICON     23
#define ITEM_CLOCK_ICON   24
#define ITEM_HUD_BOX      25
#define ITEM_HUD_BOX_BG   26
#define ITEM_MENU_TEXT    27
#define ITEM_MENU_SELECT  28
#define ITEM_MENU_BORDER  29
#define ITEM_MENUBG       30
#define ITEM_TOP_BAR      31
#define ITEM_PROFILE_MENU 32
#define ITEM_PROFILE      33
#define ITEM_SAVE_PROFILE 34
#define ITEM_LOAD_PROFILE 35
#define ITEM_LANGUAGE     36
#define ITEM_AUTO_HIDE    37
#define ITEM_TOGGLE       38
#define ITEM_RESET        39
#define ITEM_COUNT        40

static int hud_enabled = 1;
static int menu_open = 0;
static int menu_index = 0;
static int menu_scroll = 0;

static int hud_position = POS_BOTTOM_RIGHT;
static int hud_layout = LAYOUT_SINGLE;
static int hud_x_offset = 8;
static int hud_y_offset = 8;
static int hud_size = SIZE_NORMAL;
static int font_style = FONT_DEFAULT;

static int show_fps = 1;
static int show_battery = 1;
static int show_time = 1;
static int show_charging = 0;
static int show_cpu = 0;
static int show_bus = 0;
static int show_gpu = 0;
static int show_app_id = 0;
static int show_ram = 0;
static int show_ip = 0;
static int use_24h_time = 0;

static int hud_text_color = COLOR_WHITE;
static int hud_shadow_color = COLOR_BLACK;
static int hud_icon_color = COLOR_AUTO;
static int clock_icon_color = COLOR_AUTO;
static int menu_text_color = COLOR_WHITE;
static int menu_select_color = COLOR_YELLOW;
static int menu_border_color = COLOR_WHITE;
static int menu_bg_color = BG_BLACK;
static int top_menu_bar_color = COLOR_BLACK;
static int hud_box_enabled = 0;
static int hud_box_bg_color = BG_BLACK;

static int hud_language = LANG_EN;
static int auto_hide_mode = AUTO_HIDE_OFF;
static int toggle_combo_mode = TOGGLE_SELECT;
static int theme_id = THEME_DEFAULT;
static int hud_theme_id = HUD_THEME_DEFAULT;
static int profile_id = PROFILE_1;

static int temporary_show_frames = 0;
static int save_message_frames = 0;
static int reset_message_frames = 0;

static int last_toggle_down = 0;
static int last_menu_down = 0;
static unsigned int last_buttons = 0;
static int hold_up_frames = 0;
static int hold_down_frames = 0;

#define MENU_PAGE_MAIN    0
#define MENU_PAGE_PROFILE 1
#define MENU_PAGE_THEME   2

static int menu_page = MENU_PAGE_MAIN;
static int menu_nav_dir = 0;
static SceUInt64 menu_nav_hold_start_tick = 0;
static SceUInt64 menu_nav_next_repeat_tick = 0;
static int menu_lr_dir = 0;
static SceUInt64 menu_lr_hold_start_tick = 0;
static SceUInt64 menu_lr_next_repeat_tick = 0;
static int menu_lr_step = 4;

static unsigned int frame_count = 0;
static unsigned int fps_value = 0;
static SceUInt64 last_tick = 0;

/* VITABatteryPlus-style display hook. Draw into the frame buffer being submitted, not a separate polling thread. */
static SceUID g_display_hook_uid = -1;
static tai_hook_ref_t g_display_hook;

/*
 * PSVshellPlus-style stability model:
 * Do NOT call system APIs inside draw_hud().
 * Update cached strings on a timer, then draw cached text only.
 */
#define SYSTEM_CACHE_UPDATE_USEC 500000

static SceUInt64 last_system_cache_tick = 0;

static char cached_cpu_text[16] = "CPU --M";
static char cached_bus_text[16] = "BUS --M";
static char cached_gpu_text[16] = "GPU --M";
static char cached_app_id_text[24] = "APP UNKNOWN";
static char cached_ram_text[16] = "RAM OFF";
static char cached_ip_text[20] = "IP OFF";

static int g_screen_w = 960;
static int g_screen_h = 544;
static int last_hud_clear_x = -1;
static int last_hud_clear_y = -1;
static int last_hud_clear_w = 0;
static int last_hud_clear_h = 0;

/*
 * IMPORTANT FIX:
 * The previous version delayed after VBlank and used IMMEDIATE.
 * That makes some menus worse because VitaHUD draws late/into unstable redraws.
 *
 * This version:
 * - NO delay in hud_thread
 * - uses SCE_DISPLAY_SETBUF_NEXTFRAME
 * - clips every pixel write so nothing writes outside the active framebuffer
 */

static void put_2digits(char *out, int value) {
    out[0] = '0' + ((value / 10) % 10);
    out[1] = '0' + (value % 10);
}

static int append_text(char *out, int pos, const char *text) {
    while (*text) out[pos++] = *text++;
    return pos;
}

static void copy_cstr(char *dst, int dst_size, const char *src) {
    int i = 0;

    if (!dst || dst_size <= 0) {
        return;
    }

    if (!src) {
        dst[0] = '\0';
        return;
    }

    while (src[i] && i < dst_size - 1) {
        dst[i] = src[i];
        i++;
    }

    dst[i] = '\0';
}

static int append_int(char *out, int pos, int value) {
    char temp[16];
    int count = 0;
    int i;

    if (value < 0) {
        out[pos++] = '-';
        value = -value;
    }

    if (value == 0) {
        out[pos++] = '0';
        return pos;
    }

    while (value > 0 && count < 15) {
        temp[count++] = '0' + (value % 10);
        value /= 10;
    }

    for (i = count - 1; i >= 0; i--) {
        out[pos++] = temp[i];
    }

    return pos;
}

static int append_2digit_number(char *out, int pos, int value) {
    if (value < 0) value = 0;
    if (value > 99) value = 99;

    out[pos++] = '0' + ((value / 10) % 10);
    out[pos++] = '0' + (value % 10);

    return pos;
}

static int append_battery_number(char *out, int pos, int value) {
    if (value < 0) value = 0;
    if (value > 100) value = 100;

    if (value == 100) {
        out[pos++] = '1';
        out[pos++] = '0';
        out[pos++] = '0';
    } else {
        out[pos++] = '0' + ((value / 10) % 10);
        out[pos++] = '0' + (value % 10);
    }

    return pos;
}

static int key_match(const char *src, const char *key) {
    while (*key) {
        if (*src != *key) return 0;
        src++;
        key++;
    }

    return *src == '=';
}

static int parse_int_value(const char *src) {
    int sign = 1;
    int value = 0;

    if (*src == '-') {
        sign = -1;
        src++;
    }

    while (*src >= '0' && *src <= '9') {
        value = value * 10 + (*src - '0');
        src++;
    }

    return value * sign;
}

static int get_config_int(const char *buf, const char *key, int default_value) {
    const char *p = buf;

    while (*p) {
        if (key_match(p, key)) {
            while (*p && *p != '=') p++;

            if (*p == '=') {
                p++;
                return parse_int_value(p);
            }
        }

        while (*p && *p != '\n') p++;

        if (*p == '\n') p++;
    }

    return default_value;
}

static void reset_defaults(void) {
    hud_enabled = 1;
    menu_open = 0;
    menu_index = 0;
    menu_scroll = 0;
    menu_page = MENU_PAGE_MAIN;
    menu_nav_dir = 0;
    menu_nav_hold_start_tick = 0;
    menu_nav_next_repeat_tick = 0;
    menu_lr_dir = 0;
    menu_lr_hold_start_tick = 0;
    menu_lr_next_repeat_tick = 0;
    menu_lr_step = 4;

    hud_position = POS_BOTTOM_RIGHT;
    hud_layout = LAYOUT_SINGLE;
    hud_x_offset = 8;
    hud_y_offset = 8;
    hud_size = SIZE_NORMAL;
    font_style = FONT_DEFAULT;

    show_fps = 1;
    show_battery = 1;
    show_time = 1;
    show_charging = 0;
    show_cpu = 0;
    show_bus = 0;
    show_gpu = 0;
    show_app_id = 0;
    show_ram = 0;
    show_ip = 0;
    use_24h_time = 0;

    hud_text_color = COLOR_WHITE;
    hud_shadow_color = COLOR_BLACK;
    hud_icon_color = COLOR_AUTO;
    clock_icon_color = COLOR_AUTO;
    menu_text_color = COLOR_WHITE;
    menu_select_color = COLOR_YELLOW;
    menu_border_color = COLOR_WHITE;
    menu_bg_color = BG_BLACK;
    top_menu_bar_color = COLOR_BLACK;
    hud_box_enabled = 0;
    hud_box_bg_color = BG_BLACK;

    hud_language = LANG_EN;
    auto_hide_mode = AUTO_HIDE_OFF;
    toggle_combo_mode = TOGGLE_SELECT;
    theme_id = THEME_DEFAULT;
    hud_theme_id = HUD_THEME_DEFAULT;
    profile_id = PROFILE_1;

    temporary_show_frames = 0;
    reset_message_frames = 180;
}

static void clamp_settings(void) {
    if (hud_enabled < 0 || hud_enabled > 1) hud_enabled = 1;
    if (hud_position < 0 || hud_position >= POS_COUNT) hud_position = POS_BOTTOM_RIGHT;
    if (hud_layout < 0 || hud_layout >= LAYOUT_COUNT) hud_layout = LAYOUT_SINGLE;
    if (hud_x_offset < 0) hud_x_offset = 0;
    if (hud_x_offset > 960) hud_x_offset = 960;
    if (hud_y_offset < 0) hud_y_offset = 0;
    if (hud_y_offset > 544) hud_y_offset = 544;
    if (hud_size < 0 || hud_size >= SIZE_COUNT) hud_size = SIZE_NORMAL;
    if (font_style < 0 || font_style >= FONT_COUNT) font_style = FONT_DEFAULT;

    if (show_fps < 0 || show_fps > 1) show_fps = 1;
    if (show_battery < 0 || show_battery > 1) show_battery = 1;
    if (show_time < 0 || show_time > 1) show_time = 1;
    if (show_charging < 0 || show_charging > 1) show_charging = 0;
    if (show_cpu < 0 || show_cpu > 1) show_cpu = 0;
    if (show_bus < 0 || show_bus > 1) show_bus = 0;
    if (show_gpu < 0 || show_gpu > 1) show_gpu = 0;
    if (show_app_id < 0 || show_app_id > 1) show_app_id = 0;
    if (show_ram < 0 || show_ram > 1) show_ram = 0;
    if (show_ip < 0 || show_ip > 1) show_ip = 0;
    if (use_24h_time < 0 || use_24h_time > 1) use_24h_time = 0;

    if (hud_text_color < 0 || hud_text_color >= COLOR_COUNT) hud_text_color = COLOR_WHITE;
    if (hud_shadow_color < 0 || hud_shadow_color >= COLOR_COUNT) hud_shadow_color = COLOR_BLACK;
    if (hud_icon_color < 0 || hud_icon_color >= COLOR_COUNT) hud_icon_color = COLOR_AUTO;
    if (clock_icon_color < 0 || clock_icon_color >= COLOR_COUNT) clock_icon_color = COLOR_AUTO;
    if (menu_text_color < 0 || menu_text_color >= COLOR_COUNT) menu_text_color = COLOR_WHITE;
    if (menu_select_color < 0 || menu_select_color >= COLOR_COUNT) menu_select_color = COLOR_YELLOW;
    if (menu_border_color < 0 || menu_border_color >= COLOR_COUNT) menu_border_color = COLOR_WHITE;
    if (menu_bg_color < 0 || menu_bg_color >= BG_COUNT) menu_bg_color = BG_BLACK;
    if (top_menu_bar_color < 1 || top_menu_bar_color >= COLOR_COUNT) top_menu_bar_color = COLOR_BLACK;
    if (hud_box_enabled < 0 || hud_box_enabled > 1) hud_box_enabled = 0;
    if (hud_box_bg_color < 0 || hud_box_bg_color >= BG_COUNT) hud_box_bg_color = BG_BLACK;

    if (hud_language < 0 || hud_language >= LANG_COUNT) hud_language = LANG_EN;
    if (auto_hide_mode < 0 || auto_hide_mode >= AUTO_HIDE_COUNT) auto_hide_mode = AUTO_HIDE_OFF;
    if (toggle_combo_mode < 0 || toggle_combo_mode >= TOGGLE_COUNT) toggle_combo_mode = TOGGLE_SELECT;
    if (theme_id < 0 || theme_id >= THEME_COUNT) theme_id = THEME_DEFAULT;
    if (hud_theme_id < 0 || hud_theme_id >= HUD_THEME_COUNT) hud_theme_id = HUD_THEME_DEFAULT;
    if (profile_id < 0 || profile_id >= PROFILE_COUNT) profile_id = PROFILE_1;
}

static void write_config_line(SceUID fd, const char *key, int value) {
    char line[64];
    int pos = 0;

    pos = append_text(line, pos, key);
    line[pos++] = '=';
    pos = append_int(line, pos, value);
    line[pos++] = '\n';

    sceIoWrite(fd, line, pos);
}

static void save_settings_to_fd(SceUID fd) {
    write_config_line(fd, "hud_enabled", hud_enabled);
    write_config_line(fd, "layout", hud_layout);
    write_config_line(fd, "position", hud_position);
    write_config_line(fd, "x_offset", hud_x_offset);
    write_config_line(fd, "y_offset", hud_y_offset);
    write_config_line(fd, "size", hud_size);
    write_config_line(fd, "font", font_style);

    write_config_line(fd, "show_fps", show_fps);
    write_config_line(fd, "show_battery", show_battery);
    write_config_line(fd, "show_time", show_time);
    write_config_line(fd, "show_charging", show_charging);
    write_config_line(fd, "show_cpu", show_cpu);
    write_config_line(fd, "show_bus", show_bus);
    write_config_line(fd, "show_gpu", show_gpu);
    write_config_line(fd, "show_app_id", show_app_id);
    write_config_line(fd, "show_ram", show_ram);
    write_config_line(fd, "show_ip", show_ip);
    write_config_line(fd, "time_24h", use_24h_time);

    write_config_line(fd, "hud_text_color", hud_text_color);
    write_config_line(fd, "hud_shadow_color", hud_shadow_color);
    write_config_line(fd, "hud_icon_color", hud_icon_color);
    write_config_line(fd, "clock_icon_color", clock_icon_color);
    write_config_line(fd, "menu_text_color", menu_text_color);
    write_config_line(fd, "menu_select_color", menu_select_color);
    write_config_line(fd, "menu_border_color", menu_border_color);
    write_config_line(fd, "menu_bg", menu_bg_color);
    write_config_line(fd, "top_menu_bar", top_menu_bar_color);
    write_config_line(fd, "hud_box", hud_box_enabled);
    write_config_line(fd, "hud_box_bg", hud_box_bg_color);

    write_config_line(fd, "language", hud_language);
    write_config_line(fd, "auto_hide", auto_hide_mode);
    write_config_line(fd, "toggle_combo", toggle_combo_mode);
    write_config_line(fd, "theme", theme_id);
    write_config_line(fd, "hud_theme", hud_theme_id);
    write_config_line(fd, "profile", profile_id);
}

static void load_settings_from_buffer(char *buf) {
    hud_enabled = get_config_int(buf, "hud_enabled", hud_enabled);
    hud_layout = get_config_int(buf, "layout", hud_layout);
    hud_position = get_config_int(buf, "position", hud_position);
    hud_x_offset = get_config_int(buf, "x_offset", hud_x_offset);
    hud_y_offset = get_config_int(buf, "y_offset", hud_y_offset);
    hud_size = get_config_int(buf, "size", hud_size);
    font_style = get_config_int(buf, "font", font_style);

    show_fps = get_config_int(buf, "show_fps", show_fps);
    show_battery = get_config_int(buf, "show_battery", show_battery);
    show_time = get_config_int(buf, "show_time", show_time);
    show_charging = get_config_int(buf, "show_charging", show_charging);
    show_cpu = get_config_int(buf, "show_cpu", show_cpu);
    show_bus = get_config_int(buf, "show_bus", show_bus);
    show_gpu = get_config_int(buf, "show_gpu", show_gpu);
    show_app_id = get_config_int(buf, "show_app_id", show_app_id);
    show_ram = get_config_int(buf, "show_ram", show_ram);
    show_ip = get_config_int(buf, "show_ip", show_ip);
    use_24h_time = get_config_int(buf, "time_24h", use_24h_time);

    hud_text_color = get_config_int(buf, "hud_text_color", hud_text_color);
    hud_shadow_color = get_config_int(buf, "hud_shadow_color", hud_shadow_color);
    hud_icon_color = get_config_int(buf, "hud_icon_color", hud_icon_color);
    clock_icon_color = get_config_int(buf, "clock_icon_color", clock_icon_color);
    menu_text_color = get_config_int(buf, "menu_text_color", menu_text_color);
    menu_select_color = get_config_int(buf, "menu_select_color", menu_select_color);
    menu_border_color = get_config_int(buf, "menu_border_color", menu_border_color);
    menu_bg_color = get_config_int(buf, "menu_bg", menu_bg_color);
    top_menu_bar_color = get_config_int(buf, "top_menu_bar", top_menu_bar_color);
    hud_box_enabled = get_config_int(buf, "hud_box", hud_box_enabled);
    hud_box_bg_color = get_config_int(buf, "hud_box_bg", hud_box_bg_color);

    hud_language = get_config_int(buf, "language", hud_language);
    auto_hide_mode = get_config_int(buf, "auto_hide", auto_hide_mode);
    toggle_combo_mode = get_config_int(buf, "toggle_combo", toggle_combo_mode);
    theme_id = get_config_int(buf, "theme", theme_id);
    hud_theme_id = get_config_int(buf, "hud_theme", hud_theme_id);
    profile_id = get_config_int(buf, "profile", profile_id);

    clamp_settings();
}

static int save_config_path(const char *path) {
    SceUID fd;

    sceIoMkdir(CONFIG_DIR, 0777);

    fd = sceIoOpen(path, SCE_O_WRONLY | SCE_O_CREAT | SCE_O_TRUNC, 0777);

    if (fd < 0) {
        save_message_frames = 180;
        return fd;
    }

    save_settings_to_fd(fd);
    sceIoClose(fd);

    save_message_frames = 180;
    return 0;
}

static int load_config_path(const char *path) {
    SceUID fd;
    char buf[4096];
    int read_size;

    fd = sceIoOpen(path, SCE_O_RDONLY, 0);

    if (fd < 0) {
        return fd;
    }

    read_size = sceIoRead(fd, buf, sizeof(buf) - 1);
    sceIoClose(fd);

    if (read_size <= 0) {
        return -1;
    }

    buf[read_size] = '\0';
    load_settings_from_buffer(buf);

    return 0;
}

static void profile_path(char *out) {
    int pos = 0;

    pos = append_text(out, pos, CONFIG_DIR);
    out[pos++] = '/';
    pos = append_text(out, pos, "profile");
    pos = append_int(out, pos, profile_id + 1);
    pos = append_text(out, pos, ".txt");
    out[pos] = '\0';
}

static int save_config(void) {
    return save_config_path(CONFIG_PATH);
}

static void load_config(void) {
    load_config_path(CONFIG_PATH);
}

static int save_profile(void) {
    char path[64];

    /*
     * Profiles are now the main save system.
     * This saves the FULL VitaHUD setup:
     * HUD, layout, offsets, font, colors, menu background,
     * language, auto-hide, toggle combo, theme, and profile id.
     */
    profile_path(path);
    return save_config_path(path);
}

static int load_profile(void) {
    char path[64];
    int result;

    /*
     * Do NOT call apply_theme() here.
     * The profile already saves exact HUD/menu colors and menu background.
     * Calling apply_theme() after loading would overwrite saved custom colors.
     */
    profile_path(path);
    result = load_config_path(path);

    if (result >= 0) {
        save_message_frames = 180;
    }

    return result;
}

static void apply_theme(void) {
    switch (theme_id) {
        case THEME_VITASHELL:
            hud_text_color = COLOR_WHITE;
            hud_shadow_color = COLOR_BLACK;
            hud_icon_color = COLOR_AUTO;
            menu_text_color = COLOR_WHITE;
            menu_select_color = COLOR_CYAN;
            menu_border_color = COLOR_CYAN;
            menu_bg_color = BG_BLACK;
            top_menu_bar_color = COLOR_BLACK;
            hud_box_bg_color = BG_BLACK;
            break;

        case THEME_PSP:
            hud_text_color = COLOR_GREEN;
            hud_shadow_color = COLOR_BLACK;
            hud_icon_color = COLOR_AUTO;
            menu_text_color = COLOR_GREEN;
            menu_select_color = COLOR_YELLOW;
            menu_border_color = COLOR_GREEN;
            menu_bg_color = BG_BLACK;
            top_menu_bar_color = COLOR_BLACK;
            hud_box_bg_color = BG_BLACK;
            break;

        case THEME_MATRIX:
            hud_text_color = COLOR_LIME;
            hud_shadow_color = COLOR_BLACK;
            hud_icon_color = COLOR_GREEN;
            menu_text_color = COLOR_LIME;
            menu_select_color = COLOR_WHITE;
            menu_border_color = COLOR_LIME;
            menu_bg_color = BG_BLACK;
            top_menu_bar_color = COLOR_BLACK;
            hud_box_bg_color = BG_BLACK;
            break;

        case THEME_NEON:
            hud_text_color = COLOR_CYAN;
            hud_shadow_color = COLOR_BLACK;
            hud_icon_color = COLOR_MAGENTA;
            menu_text_color = COLOR_CYAN;
            menu_select_color = COLOR_MAGENTA;
            menu_border_color = COLOR_CYAN;
            menu_bg_color = BG_NAVY;
            top_menu_bar_color = COLOR_PURPLE;
            hud_box_bg_color = BG_NAVY;
            break;

        case THEME_RETRO:
            hud_text_color = COLOR_ORANGE;
            hud_shadow_color = COLOR_BLACK;
            hud_icon_color = COLOR_YELLOW;
            menu_text_color = COLOR_ORANGE;
            menu_select_color = COLOR_YELLOW;
            menu_border_color = COLOR_ORANGE;
            menu_bg_color = BG_PURPLE;
            top_menu_bar_color = COLOR_MAROON;
            hud_box_bg_color = BG_PURPLE;
            break;

        case THEME_MINIMAL:
            hud_text_color = COLOR_WHITE;
            hud_shadow_color = COLOR_BLACK;
            hud_icon_color = COLOR_AUTO;
            menu_text_color = COLOR_WHITE;
            menu_select_color = COLOR_WHITE;
            menu_border_color = COLOR_WHITE;
            menu_bg_color = BG_TRANSPARENT;
            top_menu_bar_color = COLOR_BLACK;
            hud_box_bg_color = BG_TRANSPARENT;
            break;

        case THEME_DEFAULT:
        default:
            hud_text_color = COLOR_WHITE;
            hud_shadow_color = COLOR_BLACK;
            hud_icon_color = COLOR_AUTO;
            menu_text_color = COLOR_WHITE;
            menu_select_color = COLOR_YELLOW;
            menu_border_color = COLOR_WHITE;
            menu_bg_color = BG_BLACK;
            top_menu_bar_color = COLOR_BLACK;
            hud_box_bg_color = BG_BLACK;
            break;
    }
}

static unsigned int get_toggle_combo(void) {
    switch (toggle_combo_mode) {
        case TOGGLE_UP:
            return SCE_CTRL_LTRIGGER | SCE_CTRL_RTRIGGER | SCE_CTRL_UP;

        case TOGGLE_DOWN:
            return SCE_CTRL_LTRIGGER | SCE_CTRL_RTRIGGER | SCE_CTRL_DOWN;

        case TOGGLE_LEFT:
            return SCE_CTRL_LTRIGGER | SCE_CTRL_RTRIGGER | SCE_CTRL_LEFT;

        case TOGGLE_RIGHT:
            return SCE_CTRL_LTRIGGER | SCE_CTRL_RTRIGGER | SCE_CTRL_RIGHT;

        case TOGGLE_SELECT:
        default:
            return SCE_CTRL_LTRIGGER | SCE_CTRL_RTRIGGER | SCE_CTRL_SELECT;
    }
}

static void update_fps(void) {
    SceRtcTick tick;
    sceRtcGetCurrentTick(&tick);

    if (last_tick == 0) {
        last_tick = tick.tick;
        return;
    }

    frame_count++;

    if (tick.tick - last_tick >= 1000000) {
        fps_value = frame_count;
        frame_count = 0;
        last_tick = tick.tick;
    }
}

static void build_fps_text(char *out) {
    int pos = 0;

    if (hud_layout == LAYOUT_COMPACT || hud_layout == LAYOUT_ICONS) {
        pos = append_2digit_number(out, pos, (int)fps_value);
        pos = append_text(out, pos, "FPS");
    } else {
        pos = append_text(out, pos, "FPS ");
        pos = append_2digit_number(out, pos, (int)fps_value);
    }

    out[pos] = '\0';
}

static void build_battery_text(char *out, int battery) {
    int pos = 0;

    pos = append_battery_number(out, pos, battery);

    if (hud_layout != LAYOUT_ICONS) {
        out[pos++] = '%';
    }

    out[pos] = '\0';
}

static void build_charging_text(char *out) {
    int pos = 0;

    if (scePowerIsBatteryCharging()) {
        pos = append_text(out, pos, "CHG");
    } else {
        pos = append_text(out, pos, "BAT");
    }

    out[pos] = '\0';
}

static void build_cpu_text(char *out) {
    copy_cstr(out, 16, cached_cpu_text);
}

static void build_bus_text(char *out) {
    copy_cstr(out, 16, cached_bus_text);
}

static void build_gpu_text(char *out) {
    copy_cstr(out, 16, cached_gpu_text);
}

static int copy_title_id_to_output(char *out, int pos, const char *title_id) {
    int i = 0;

    while (title_id[i] && i < 12) {
        out[pos++] = title_id[i++];
    }

    return pos;
}

static void build_app_id_live_text(char *out) {
    int pos = 0;
    char title_id[32];
    int result;
    int pid;
    int i;

    for (i = 0; i < 32; i++) {
        title_id[i] = 0;
    }

    pid = sceKernelGetProcessId();

    /*
     * Safer APP ID fallback:
     * First try title ID by process ID.
     * If that fails, try PARAM.SFO titleid.
     * No RAM calls. No NetCtl calls.
     */
    result = sceAppMgrGetNameById(pid, title_id);

    if (result < 0 || title_id[0] == 0) {
        for (i = 0; i < 32; i++) {
            title_id[i] = 0;
        }

        result = sceAppMgrAppParamGetString(pid, 12, title_id, sizeof(title_id));
    }

    pos = append_text(out, pos, "APP ");

    if (result < 0 || title_id[0] == 0) {
        pos = append_text(out, pos, "UNKNOWN");
    } else {
        pos = copy_title_id_to_output(out, pos, title_id);
    }

    out[pos] = '\0';
}

static void build_app_id_text(char *out) {
    copy_cstr(out, 24, cached_app_id_text);
}

static void build_ram_text(char *out) {
    copy_cstr(out, 16, cached_ram_text);
}

static void build_ip_text(char *out) {
    copy_cstr(out, 20, cached_ip_text);
}

static void build_mhz_cache_text(char *out, int out_size, const char *label, int mhz) {
    int pos = 0;

    if (mhz < 0) {
        mhz = 0;
    }

    pos = append_text(out, pos, label);
    pos = append_text(out, pos, " ");
    pos = append_int(out, pos, mhz);
    pos = append_text(out, pos, "M");
    out[pos] = '\0';
}

static void update_system_cache(void) {
    SceRtcTick tick;
    SceUInt64 now;
    char temp_app[24];

    sceRtcGetCurrentTick(&tick);
    now = tick.tick;

    if (last_system_cache_tick != 0 && now - last_system_cache_tick < SYSTEM_CACHE_UPDATE_USEC) {
        return;
    }

    last_system_cache_tick = now;

    /*
     * Cache only.
     * These calls are no longer inside draw_hud().
     * This matches the safer PSVshellPlus idea: update system data on a timer,
     * then draw cached values.
     */
    build_mhz_cache_text(cached_cpu_text, sizeof(cached_cpu_text), "CPU", scePowerGetArmClockFrequency());
    build_mhz_cache_text(cached_bus_text, sizeof(cached_bus_text), "BUS", scePowerGetBusClockFrequency());
    build_mhz_cache_text(cached_gpu_text, sizeof(cached_gpu_text), "GPU", scePowerGetGpuClockFrequency());

    build_app_id_live_text(temp_app);
    copy_cstr(cached_app_id_text, sizeof(cached_app_id_text), temp_app);

    /*
     * RAM/IP intentionally remain placeholders for now.
     * Next step: port PSVshellPlus-style kernel RAM provider.
     */
    copy_cstr(cached_ram_text, sizeof(cached_ram_text), "RAM OFF");
    copy_cstr(cached_ip_text, sizeof(cached_ip_text), "IP OFF");
}

static void build_time_text(char *out) {
    SceDateTime time;
    int pos = 0;
    int hour;
    int is_pm = 0;

    sceRtcGetCurrentClockLocalTime(&time);

    if (use_24h_time || hud_layout == LAYOUT_COMPACT || hud_layout == LAYOUT_ICONS) {
        put_2digits(&out[pos], time.hour);
        pos += 2;

        out[pos++] = ':';

        put_2digits(&out[pos], time.minute);
        pos += 2;

        out[pos] = '\0';
        return;
    }

    hour = time.hour;

    if (hour >= 12) {
        is_pm = 1;
    }

    hour = hour % 12;

    if (hour == 0) {
        hour = 12;
    }

    put_2digits(&out[pos], hour);
    pos += 2;

    out[pos++] = ':';

    put_2digits(&out[pos], time.minute);
    pos += 2;

    out[pos++] = ' ';
    out[pos++] = is_pm ? 'P' : 'A';
    out[pos++] = 'M';
    out[pos] = '\0';
}

/*
 * Vita A8B8G8R8 values.
 * Yellow must be 0xFF00FFFF on this framebuffer.
 */
static unsigned int color_value(int color_id, unsigned int fallback) {
    switch (color_id) {
        case COLOR_WHITE:     return 0xFFFFFFFF;
        case COLOR_GREEN:     return 0xFF00FF00;
        case COLOR_YELLOW:    return 0xFF00FFFF;
        case COLOR_RED:       return 0xFF0000FF;
        case COLOR_CYAN:      return 0xFFFFFF00;
        case COLOR_MAGENTA:   return 0xFFFF00FF;
        case COLOR_ORANGE:    return 0xFF0080FF;
        case COLOR_BLUE:      return 0xFFFF0000;
        case COLOR_PINK:      return 0xFFFF80FF;
        case COLOR_LIME:      return 0xFF80FF00;
        case COLOR_SILVER:    return 0xFFC0C0C0;
        case COLOR_BLACK:     return 0xFF000000;
        case COLOR_PURPLE:    return 0xFF800080;
        case COLOR_VIOLET:    return 0xFFFF40A0;
        case COLOR_GOLD:      return 0xFF00D7FF;
        case COLOR_AMBER:     return 0xFF00BFFF;
        case COLOR_MINT:      return 0xFFAAFF99;
        case COLOR_AQUA:      return 0xFFFFD070;
        case COLOR_SKY:       return 0xFFFFB060;
        case COLOR_NAVY:      return 0xFF800000;
        case COLOR_MAROON:    return 0xFF000080;
        case COLOR_CRIMSON:   return 0xFF3C14DC;
        case COLOR_ROSE:      return 0xFF8080FF;
        case COLOR_LAVENDER:  return 0xFFFFA0E6;
        case COLOR_TURQUOISE: return 0xFFD0E040;
        case COLOR_EMERALD:   return 0xFF50C878;
        case COLOR_FOREST:    return 0xFF228B22;
        case COLOR_SLATE:     return 0xFF908070;
        case COLOR_GRAY:      return 0xFF808080;
        case COLOR_DARK_GRAY: return 0xFF303030;
        case COLOR_AUTO:
        default:              return fallback;
    }
}

static unsigned int get_menu_bg_for(int bg_id) {
    switch (bg_id) {
        case BG_GRAY:        return 0xCC404040;
        case BG_NAVY:        return 0xCC401020;
        case BG_GREEN:       return 0xCC203010;
        case BG_PURPLE:      return 0xCC401030;
        case BG_TEAL:        return 0xCC383810;
        case BG_RED:         return 0xCC000040;
        case BG_WHITE:       return 0xCCD0D0D0;
        case BG_CYAN:        return 0xCC604000;
        case BG_MAGENTA:     return 0xCC600060;
        case BG_BLUE:        return 0xCC703000;
        case BG_GOLD:        return 0xCC006080;
        case BG_MAROON:      return 0xCC000020;
        case BG_FOREST:      return 0xCC103010;
        case BG_SLATE:       return 0xCC302820;
        case BG_BLACK:       return 0xCC000000;
        case BG_TRANSPARENT:
        default:             return 0x00000000;
    }
}

static unsigned int get_menu_bg(void) {
    return get_menu_bg_for(menu_bg_color);
}

static unsigned int get_battery_color(int battery) {
    if (battery >= 60) {
        return 0xFF00FF00;
    }

    if (battery >= 21) {
        return 0xFF00FFFF;
    }

    return 0xFF0000FF;
}

static void get_hud_metrics(int *scale, int *gap_small, int *gap_big) {
    switch (hud_size) {
        case SIZE_MICRO:
            *scale = 1;
            *gap_small = 1;
            *gap_big = 2;
            break;

        case SIZE_NORMAL:
            *scale = 2;
            *gap_small = 2;
            *gap_big = 5;
            break;

        case SIZE_LARGE:
        default:
            *scale = 3;
            *gap_small = 2;
            *gap_big = 7;
            break;
    }
}

static int font_extra_spacing(void) {
    switch (font_style) {
        case FONT_WIDE:
        case FONT_PSP:
        case FONT_ARCADE:
            return 2;

        case FONT_COMPACT:
        case FONT_MINI:
            return -1;

        default:
            return 0;
    }
}

static int font_shadow_style(void) {
    switch (font_style) {
        case FONT_THIN:
        case FONT_CLEAN:
            return 0;

        case FONT_SOFT:
            return 2;

        case FONT_SHADOW:
        case FONT_RETRO:
        case FONT_PSP:
        case FONT_VITA:
        default:
            return 1;
    }
}

static int font_bold_style(void) {
    switch (font_style) {
        case FONT_BOLD:
        case FONT_BLOCK:
        case FONT_SQUARE:
        case FONT_ARCADE:
            return 1;

        case FONT_DOUBLE:
            return 2;

        default:
            return 0;
    }
}

static unsigned char font5x7(char c, int row) {
    switch (c) {
        case '0': { static const unsigned char g[7] = {0x0E,0x11,0x13,0x15,0x19,0x11,0x0E}; return g[row]; }
        case '1': { static const unsigned char g[7] = {0x04,0x0C,0x04,0x04,0x04,0x04,0x0E}; return g[row]; }
        case '2': { static const unsigned char g[7] = {0x0E,0x11,0x01,0x02,0x04,0x08,0x1F}; return g[row]; }
        case '3': { static const unsigned char g[7] = {0x1E,0x01,0x01,0x0E,0x01,0x01,0x1E}; return g[row]; }
        case '4': { static const unsigned char g[7] = {0x02,0x06,0x0A,0x12,0x1F,0x02,0x02}; return g[row]; }
        case '5': { static const unsigned char g[7] = {0x1F,0x10,0x10,0x1E,0x01,0x01,0x1E}; return g[row]; }
        case '6': { static const unsigned char g[7] = {0x0E,0x10,0x10,0x1E,0x11,0x11,0x0E}; return g[row]; }
        case '7': { static const unsigned char g[7] = {0x1F,0x01,0x02,0x04,0x08,0x08,0x08}; return g[row]; }
        case '8': { static const unsigned char g[7] = {0x0E,0x11,0x11,0x0E,0x11,0x11,0x0E}; return g[row]; }
        case '9': { static const unsigned char g[7] = {0x0E,0x11,0x11,0x0F,0x01,0x01,0x0E}; return g[row]; }

        case 'A': { static const unsigned char g[7] = {0x0E,0x11,0x11,0x1F,0x11,0x11,0x11}; return g[row]; }
        case 'B': { static const unsigned char g[7] = {0x1E,0x11,0x11,0x1E,0x11,0x11,0x1E}; return g[row]; }
        case 'C': { static const unsigned char g[7] = {0x0E,0x11,0x10,0x10,0x10,0x11,0x0E}; return g[row]; }
        case 'D': { static const unsigned char g[7] = {0x1E,0x11,0x11,0x11,0x11,0x11,0x1E}; return g[row]; }
        case 'E': { static const unsigned char g[7] = {0x1F,0x10,0x10,0x1E,0x10,0x10,0x1F}; return g[row]; }
        case 'F': { static const unsigned char g[7] = {0x1F,0x10,0x10,0x1E,0x10,0x10,0x10}; return g[row]; }
        case 'G': { static const unsigned char g[7] = {0x0E,0x11,0x10,0x17,0x11,0x11,0x0F}; return g[row]; }
        case 'H': { static const unsigned char g[7] = {0x11,0x11,0x11,0x1F,0x11,0x11,0x11}; return g[row]; }
        case 'I': { static const unsigned char g[7] = {0x0E,0x04,0x04,0x04,0x04,0x04,0x0E}; return g[row]; }
        case 'J': { static const unsigned char g[7] = {0x01,0x01,0x01,0x01,0x11,0x11,0x0E}; return g[row]; }
        case 'K': { static const unsigned char g[7] = {0x11,0x12,0x14,0x18,0x14,0x12,0x11}; return g[row]; }
        case 'L': { static const unsigned char g[7] = {0x10,0x10,0x10,0x10,0x10,0x10,0x1F}; return g[row]; }
        case 'M': { static const unsigned char g[7] = {0x11,0x1B,0x15,0x15,0x11,0x11,0x11}; return g[row]; }
        case 'N': { static const unsigned char g[7] = {0x11,0x19,0x15,0x13,0x11,0x11,0x11}; return g[row]; }
        case 'O': { static const unsigned char g[7] = {0x0E,0x11,0x11,0x11,0x11,0x11,0x0E}; return g[row]; }
        case 'P': { static const unsigned char g[7] = {0x1E,0x11,0x11,0x1E,0x10,0x10,0x10}; return g[row]; }
        case 'Q': { static const unsigned char g[7] = {0x0E,0x11,0x11,0x11,0x15,0x12,0x0D}; return g[row]; }
        case 'R': { static const unsigned char g[7] = {0x1E,0x11,0x11,0x1E,0x14,0x12,0x11}; return g[row]; }
        case 'S': { static const unsigned char g[7] = {0x0F,0x10,0x10,0x0E,0x01,0x01,0x1E}; return g[row]; }
        case 'T': { static const unsigned char g[7] = {0x1F,0x04,0x04,0x04,0x04,0x04,0x04}; return g[row]; }
        case 'U': { static const unsigned char g[7] = {0x11,0x11,0x11,0x11,0x11,0x11,0x0E}; return g[row]; }
        case 'V': { static const unsigned char g[7] = {0x11,0x11,0x11,0x11,0x0A,0x0A,0x04}; return g[row]; }
        case 'W': { static const unsigned char g[7] = {0x11,0x11,0x11,0x15,0x15,0x1B,0x11}; return g[row]; }
        case 'X': { static const unsigned char g[7] = {0x11,0x0A,0x04,0x04,0x04,0x0A,0x11}; return g[row]; }
        case 'Y': { static const unsigned char g[7] = {0x11,0x0A,0x04,0x04,0x04,0x04,0x04}; return g[row]; }
        case 'Z': { static const unsigned char g[7] = {0x1F,0x01,0x02,0x04,0x08,0x10,0x1F}; return g[row]; }

        case ':': { static const unsigned char g[7] = {0x00,0x04,0x04,0x00,0x04,0x04,0x00}; return g[row]; }
        case '%': { static const unsigned char g[7] = {0x19,0x19,0x02,0x04,0x08,0x13,0x13}; return g[row]; }
        case '-': { static const unsigned char g[7] = {0x00,0x00,0x00,0x1F,0x00,0x00,0x00}; return g[row]; }
        case '<': { static const unsigned char g[7] = {0x02,0x04,0x08,0x10,0x08,0x04,0x02}; return g[row]; }
        case '>': { static const unsigned char g[7] = {0x08,0x04,0x02,0x01,0x02,0x04,0x08}; return g[row]; }
        case '/': { static const unsigned char g[7] = {0x01,0x02,0x02,0x04,0x08,0x08,0x10}; return g[row]; }
        case '+': { static const unsigned char g[7] = {0x00,0x04,0x04,0x1F,0x04,0x04,0x00}; return g[row]; }
        case ' ': return 0x00;

        default:
            return 0x00;
    }
}

static void draw_rect(unsigned int *pixels, int pitch, int x, int y, int w, int h, unsigned int color) {
    int yy;
    int xx;
    int x2;
    int y2;

    if (w <= 0 || h <= 0) {
        return;
    }

    x2 = x + w;
    y2 = y + h;

    if (x < 0) x = 0;
    if (y < 0) y = 0;
    if (x2 > g_screen_w) x2 = g_screen_w;
    if (y2 > g_screen_h) y2 = g_screen_h;

    if (x >= x2 || y >= y2) {
        return;
    }

    for (yy = y; yy < y2; yy++) {
        for (xx = x; xx < x2; xx++) {
            pixels[yy * pitch + xx] = color;
        }
    }
}

static void draw_char(unsigned int *pixels, int pitch, int x, int y, char c, unsigned int color, int scale) {
    int row;
    int col;
    int sy;
    int sx;
    int px;
    int py;
    int bold = font_bold_style();

    for (row = 0; row < 7; row++) {
        unsigned char bits = font5x7(c, row);

        for (col = 0; col < 5; col++) {
            if (bits & (1 << (4 - col))) {
                for (sy = 0; sy < scale; sy++) {
                    for (sx = 0; sx < scale; sx++) {
                        px = x + col * scale + sx;
                        py = y + row * scale + sy;

                        if (px >= 0 && px < g_screen_w && py >= 0 && py < g_screen_h) {
                            pixels[py * pitch + px] = color;
                        }

                        if (bold >= 1) {
                            px = x + col * scale + sx + 1;
                            py = y + row * scale + sy;

                            if (px >= 0 && px < g_screen_w && py >= 0 && py < g_screen_h) {
                                pixels[py * pitch + px] = color;
                            }
                        }

                        if (bold >= 2) {
                            px = x + col * scale + sx;
                            py = y + row * scale + sy + 1;

                            if (px >= 0 && px < g_screen_w && py >= 0 && py < g_screen_h) {
                                pixels[py * pitch + px] = color;
                            }
                        }
                    }
                }
            }
        }
    }
}

static int text_width(const char *text, int scale) {
    int count = 0;
    int spacing = 6 * scale + font_extra_spacing();

    if (spacing < 3) spacing = 3;

    while (text[count]) count++;

    return count * spacing;
}

static void draw_text(unsigned int *pixels, int pitch, int x, int y, const char *text, unsigned int color, int scale) {
    int cursor_x = x;
    int spacing = 6 * scale + font_extra_spacing();

    if (spacing < 3) spacing = 3;

    while (*text) {
        draw_char(pixels, pitch, cursor_x, y, *text, color, scale);
        cursor_x += spacing;
        text++;
    }
}

static void draw_text_shadow(unsigned int *pixels, int pitch, int x, int y, const char *text, unsigned int color, int scale) {
    int shadow = font_shadow_style();
    unsigned int shadow_color = color_value(hud_shadow_color, 0xFF000000);

    if (shadow == 1) {
        draw_text(pixels, pitch, x + 1, y + 1, text, shadow_color, scale);
    } else if (shadow == 2) {
        draw_text(pixels, pitch, x + 1, y, text, shadow_color, scale);
        draw_text(pixels, pitch, x, y + 1, text, shadow_color, scale);
    }

    draw_text(pixels, pitch, x, y, text, color, scale);
}

static void draw_battery_icon(unsigned int *pixels, int pitch, int x, int y, int battery, int scale) {
    unsigned int white = color_value(hud_icon_color, 0xFFFFFFFF);
    unsigned int fill = hud_icon_color == COLOR_AUTO ? get_battery_color(battery) : color_value(hud_icon_color, get_battery_color(battery));

    int body_w = 13 * scale;
    int body_h = 7 * scale;
    int cap_w = 2 * scale;
    int cap_h = 3 * scale;
    int border = scale;

    int inner_x;
    int inner_y;
    int inner_w;
    int inner_h;
    int fill_w;

    if (battery < 0) battery = 0;
    if (battery > 100) battery = 100;

    draw_rect(pixels, pitch, x, y, body_w, border, white);
    draw_rect(pixels, pitch, x, y + body_h - border, body_w, border, white);
    draw_rect(pixels, pitch, x, y, border, body_h, white);
    draw_rect(pixels, pitch, x + body_w - border, y, border, body_h, white);
    draw_rect(pixels, pitch, x + body_w, y + (2 * scale), cap_w, cap_h, white);

    inner_x = x + border;
    inner_y = y + border;
    inner_w = body_w - (border * 2);
    inner_h = body_h - (border * 2);
    fill_w = (inner_w * battery) / 100;

    if (fill_w > 0) {
        draw_rect(pixels, pitch, inner_x, inner_y, fill_w, inner_h, fill);
    }
}

static void draw_clock_icon(unsigned int *pixels, int pitch, int x, int y, int scale) {
    unsigned int white = color_value(clock_icon_color, color_value(hud_icon_color, 0xFFFFFFFF));
    int s = scale;

    if (s < 1) s = 1;

    draw_rect(pixels, pitch, x + (2 * s), y, 3 * s, s, white);
    draw_rect(pixels, pitch, x + s, y + s, s, s, white);
    draw_rect(pixels, pitch, x + (5 * s), y + s, s, s, white);
    draw_rect(pixels, pitch, x, y + (2 * s), s, s, white);
    draw_rect(pixels, pitch, x + (6 * s), y + (2 * s), s, s, white);
    draw_rect(pixels, pitch, x, y + (3 * s), s, s, white);
    draw_rect(pixels, pitch, x + (3 * s), y + (3 * s), s, s, white);
    draw_rect(pixels, pitch, x + (4 * s), y + (3 * s), s, s, white);
    draw_rect(pixels, pitch, x + (6 * s), y + (3 * s), s, s, white);
    draw_rect(pixels, pitch, x, y + (4 * s), s, s, white);
    draw_rect(pixels, pitch, x + (3 * s), y + (4 * s), s, s, white);
    draw_rect(pixels, pitch, x + (6 * s), y + (4 * s), s, s, white);
    draw_rect(pixels, pitch, x + s, y + (5 * s), s, s, white);
    draw_rect(pixels, pitch, x + (5 * s), y + (5 * s), s, s, white);
    draw_rect(pixels, pitch, x + (2 * s), y + (6 * s), 3 * s, s, white);
}

static const char *word_open(void) {
    switch (hud_language) {
        case LANG_ES: return "ABRIR";
        case LANG_FR: return "OUVRIR";
        case LANG_DE: return "OFFNEN";
        case LANG_IT: return "APRI";
        case LANG_PT: return "ABRIR";
        case LANG_NL: return "OPEN";
        case LANG_ID: return "BUKA";
        case LANG_TR: return "AC";
        case LANG_PL: return "OTWORZ";
        default: return "OPEN";
    }
}

static const char *word_press_x(void) {
    switch (hud_language) {
        case LANG_ES: return "PULSA X";
        case LANG_FR: return "X VALIDER";
        case LANG_DE: return "X DRUCKEN";
        case LANG_IT: return "PREMI X";
        case LANG_PT: return "APERTE X";
        case LANG_NL: return "DRUK X";
        case LANG_ID: return "TEKAN X";
        case LANG_TR: return "X BAS";
        case LANG_PL: return "NACISNIJ X";
        default: return "PRESS X";
    }
}

static const char *word_saved(void) {
    switch (hud_language) {
        case LANG_ES: return "GUARDADO";
        case LANG_FR: return "SAUVE";
        case LANG_DE: return "GESPEICHERT";
        case LANG_IT: return "SALVATO";
        case LANG_PT: return "SALVO";
        case LANG_NL: return "OPGESLAGEN";
        case LANG_ID: return "DISIMPAN";
        case LANG_TR: return "KAYDEDILDI";
        case LANG_PL: return "ZAPISANO";
        default: return "SAVED";
    }
}

static const char *word_loaded(void) {
    switch (hud_language) {
        case LANG_ES: return "CARGADO";
        case LANG_FR: return "CHARGE";
        case LANG_DE: return "GELADEN";
        case LANG_IT: return "CARICATO";
        case LANG_PT: return "CARREGADO";
        case LANG_NL: return "GELADEN";
        case LANG_ID: return "DIMUAT";
        case LANG_TR: return "YUKLENDI";
        case LANG_PL: return "WCZYTANO";
        default: return "LOADED";
    }
}

static const char *onoff_name(int value) {
    switch (hud_language) {
        case LANG_ES: return value ? "SI" : "NO";
        case LANG_FR: return value ? "OUI" : "NON";
        case LANG_DE: return value ? "EIN" : "AUS";
        case LANG_IT: return value ? "SI" : "NO";
        case LANG_PT: return value ? "SIM" : "NAO";
        case LANG_NL: return value ? "AAN" : "UIT";
        case LANG_ID: return value ? "NYALA" : "MATI";
        case LANG_TR: return value ? "ACIK" : "KAPALI";
        case LANG_PL: return value ? "WL" : "WYL";
        default: return value ? "ON" : "OFF";
    }
}

static const char *position_name(void) {
    switch (hud_position) {
        case POS_BOTTOM_LEFT:  return "BOTTOM LEFT";
        case POS_TOP_RIGHT:    return "TOP RIGHT";
        case POS_TOP_LEFT:     return "TOP LEFT";
        case POS_BOTTOM_RIGHT:
        default:               return "BOTTOM RIGHT";
    }
}

static const char *layout_name(void) {
    switch (hud_layout) {
        case LAYOUT_COMPACT: return "COMPACT";
        case LAYOUT_STACKED: return "STACKED";
        case LAYOUT_ICONS:   return "ICONS";
        case LAYOUT_SINGLE:
        default:             return "SINGLE";
    }
}

static const char *size_name(void) {
    switch (hud_size) {
        case SIZE_MICRO:  return "MICRO";
        case SIZE_NORMAL: return "NORMAL";
        case SIZE_LARGE:
        default:          return "LARGE";
    }
}

static const char *color_name_generic(int color_id) {
    switch (color_id) {
        case COLOR_AUTO:      return "AUTO";
        case COLOR_WHITE:     return "WHITE";
        case COLOR_GREEN:     return "GREEN";
        case COLOR_YELLOW:    return "YELLOW";
        case COLOR_RED:       return "RED";
        case COLOR_CYAN:      return "CYAN";
        case COLOR_MAGENTA:   return "MAGENTA";
        case COLOR_ORANGE:    return "ORANGE";
        case COLOR_BLUE:      return "BLUE";
        case COLOR_PINK:      return "PINK";
        case COLOR_LIME:      return "LIME";
        case COLOR_SILVER:    return "SILVER";
        case COLOR_BLACK:     return "BLACK";
        case COLOR_PURPLE:    return "PURPLE";
        case COLOR_VIOLET:    return "VIOLET";
        case COLOR_GOLD:      return "GOLD";
        case COLOR_AMBER:     return "AMBER";
        case COLOR_MINT:      return "MINT";
        case COLOR_AQUA:      return "AQUA";
        case COLOR_SKY:       return "SKY";
        case COLOR_NAVY:      return "NAVY";
        case COLOR_MAROON:    return "MAROON";
        case COLOR_CRIMSON:   return "CRIMSON";
        case COLOR_ROSE:      return "ROSE";
        case COLOR_LAVENDER:  return "LAVENDER";
        case COLOR_TURQUOISE: return "TURQUOISE";
        case COLOR_EMERALD:   return "EMERALD";
        case COLOR_FOREST:    return "FOREST";
        case COLOR_SLATE:     return "SLATE";
        case COLOR_GRAY:      return "GRAY";
        case COLOR_DARK_GRAY: return "DARK GRAY";
        default:              return "WHITE";
    }
}

static const char *menu_bg_name_for(int bg_id) {
    switch (bg_id) {
        case BG_TRANSPARENT: return hud_language == LANG_ES ? "TRANSPARENTE" : "TRANSPARENT";
        case BG_GRAY:        return hud_language == LANG_ES ? "GRIS" : "GRAY";
        case BG_NAVY:        return hud_language == LANG_ES ? "AZUL OSCURO" : "NAVY";
        case BG_GREEN:       return hud_language == LANG_ES ? "VERDE" : "GREEN";
        case BG_PURPLE:      return hud_language == LANG_ES ? "MORADO" : "PURPLE";
        case BG_TEAL:        return "TEAL";
        case BG_RED:         return hud_language == LANG_ES ? "ROJO" : "RED";
        case BG_WHITE:       return hud_language == LANG_ES ? "BLANCO" : "WHITE";
        case BG_CYAN:        return "CYAN";
        case BG_MAGENTA:     return "MAGENTA";
        case BG_BLUE:        return "BLUE";
        case BG_GOLD:        return "GOLD";
        case BG_MAROON:      return "MAROON";
        case BG_FOREST:      return "FOREST";
        case BG_SLATE:       return "SLATE";
        case BG_BLACK:
        default:             return hud_language == LANG_ES ? "NEGRO" : "BLACK";
    }
}

static const char *menu_bg_name(void) {
    return menu_bg_name_for(menu_bg_color);
}

/* original menu_bg_name switch removed */
static const char *menu_bg_name_old_unused(void) {
    switch (menu_bg_color) {
        case BG_TRANSPARENT: return "TRANSPARENT";
        case BG_GRAY:        return "GRAY";
        case BG_NAVY:        return "NAVY";
        case BG_GREEN:       return "GREEN";
        case BG_PURPLE:      return "PURPLE";
        case BG_TEAL:        return "TEAL";
        case BG_RED:         return "RED";
        case BG_WHITE:       return "WHITE";
        case BG_BLACK:
        default:             return "BLACK";
    }
}

static const char *font_name(void) {
    switch (font_style) {
        case FONT_BOLD:    return "BOLD";
        case FONT_THIN:    return "THIN";
        case FONT_WIDE:    return "WIDE";
        case FONT_TALL:    return "TALL";
        case FONT_COMPACT: return "COMPACT";
        case FONT_DOUBLE:  return "DOUBLE";
        case FONT_BLOCK:   return "BLOCK";
        case FONT_SHADOW:  return "SHADOW";
        case FONT_SOFT:    return "SOFT";
        case FONT_SHARP:   return "SHARP";
        case FONT_RETRO:   return "RETRO";
        case FONT_PSP:     return "PSP";
        case FONT_VITA:    return "VITA";
        case FONT_MINI:    return "MINI";
        case FONT_SQUARE:  return "SQUARE";
        case FONT_ROUNDED: return "ROUNDED";
        case FONT_DIGITAL: return "DIGITAL";
        case FONT_ARCADE:  return "ARCADE";
        case FONT_CLEAN:   return "CLEAN";
        case FONT_DEFAULT:
        default:           return "DEFAULT";
    }
}

static const char *time_mode_name(void) {
    return use_24h_time ? "24H" : "12H";
}

static const char *language_name(void) {
    switch (hud_language) {
        case LANG_ES: return "ESPANOL";
        case LANG_FR: return "FRANCAIS";
        case LANG_DE: return "DEUTSCH";
        case LANG_IT: return "ITALIANO";
        case LANG_PT: return "PORTUGUES";
        case LANG_NL: return "NEDERLANDS";
        case LANG_ID: return "INDONESIA";
        case LANG_TR: return "TURKCE";
        case LANG_PL: return "POLSKI";
        case LANG_EN:
        default:      return "ENGLISH";
    }
}

static const char *auto_hide_name(void) {
    switch (auto_hide_mode) {
        case AUTO_HIDE_3S:  return "3 SEC";
        case AUTO_HIDE_5S:  return "5 SEC";
        case AUTO_HIDE_10S: return "10 SEC";
        case AUTO_HIDE_OFF:
        default:            return "OFF";
    }
}

static const char *theme_name(void) {
    switch (theme_id) {
        case THEME_VITASHELL: return "VITASHELL";
        case THEME_PSP:       return "PSP";
        case THEME_MATRIX:    return "MATRIX";
        case THEME_NEON:      return "NEON";
        case THEME_RETRO:     return "RETRO";
        case THEME_MINIMAL:   return "MINIMAL";
        case THEME_DEFAULT:
        default:              return "DEFAULT";
    }
}

static const char *hud_theme_name(void) {
    switch (hud_theme_id) {
        case HUD_THEME_STEALTH: return "STEALTH";
        case HUD_THEME_NEON:    return "NEON GLASS";
        case HUD_THEME_MATRIX:  return "MATRIX";
        case HUD_THEME_GOLD:    return "GOLD PRO";
        case HUD_THEME_ICE:     return "ICE BLUE";
        case HUD_THEME_BLOOD:   return "BLOOD RED";
        case HUD_THEME_CLEAN:   return "CLEAN WHITE";
        case HUD_THEME_DEFAULT:
        default:                return "DEFAULT";
    }
}

static void apply_hud_theme(void) {
    switch (hud_theme_id) {
        case HUD_THEME_STEALTH:
            hud_text_color = COLOR_WHITE;
            hud_shadow_color = COLOR_BLACK;
            hud_icon_color = COLOR_GREEN;
            clock_icon_color = COLOR_WHITE;
            hud_box_enabled = 1;
            hud_box_bg_color = BG_BLACK;
            hud_layout = LAYOUT_SINGLE;
            break;

        case HUD_THEME_NEON:
            hud_text_color = COLOR_CYAN;
            hud_shadow_color = COLOR_BLACK;
            hud_icon_color = COLOR_MAGENTA;
            clock_icon_color = COLOR_CYAN;
            hud_box_enabled = 1;
            hud_box_bg_color = BG_PURPLE;
            hud_layout = LAYOUT_SINGLE;
            break;

        case HUD_THEME_MATRIX:
            hud_text_color = COLOR_LIME;
            hud_shadow_color = COLOR_BLACK;
            hud_icon_color = COLOR_GREEN;
            clock_icon_color = COLOR_LIME;
            hud_box_enabled = 1;
            hud_box_bg_color = BG_BLACK;
            hud_layout = LAYOUT_COMPACT;
            break;

        case HUD_THEME_GOLD:
            hud_text_color = COLOR_GOLD;
            hud_shadow_color = COLOR_BLACK;
            hud_icon_color = COLOR_GOLD;
            clock_icon_color = COLOR_AMBER;
            hud_box_enabled = 1;
            hud_box_bg_color = BG_BLACK;
            hud_layout = LAYOUT_SINGLE;
            break;

        case HUD_THEME_ICE:
            hud_text_color = COLOR_SKY;
            hud_shadow_color = COLOR_NAVY;
            hud_icon_color = COLOR_AQUA;
            clock_icon_color = COLOR_SKY;
            hud_box_enabled = 1;
            hud_box_bg_color = BG_NAVY;
            hud_layout = LAYOUT_SINGLE;
            break;

        case HUD_THEME_BLOOD:
            hud_text_color = COLOR_RED;
            hud_shadow_color = COLOR_BLACK;
            hud_icon_color = COLOR_CRIMSON;
            clock_icon_color = COLOR_RED;
            hud_box_enabled = 1;
            hud_box_bg_color = BG_MAROON;
            hud_layout = LAYOUT_SINGLE;
            break;

        case HUD_THEME_CLEAN:
            hud_text_color = COLOR_WHITE;
            hud_shadow_color = COLOR_BLACK;
            hud_icon_color = COLOR_AUTO;
            clock_icon_color = COLOR_WHITE;
            hud_box_enabled = 0;
            hud_box_bg_color = BG_TRANSPARENT;
            hud_layout = LAYOUT_SINGLE;
            break;

        case HUD_THEME_DEFAULT:
        default:
            hud_text_color = COLOR_WHITE;
            hud_shadow_color = COLOR_BLACK;
            hud_icon_color = COLOR_AUTO;
            clock_icon_color = COLOR_AUTO;
            hud_layout = LAYOUT_SINGLE;
            break;
    }
}

static const char *profile_name(void) {
    switch (profile_id) {
        case PROFILE_2: return "2";
        case PROFILE_3: return "3";
        case PROFILE_1:
        default:        return "1";
    }
}

static const char *toggle_name(void) {
    switch (toggle_combo_mode) {
        case TOGGLE_UP:    return "L+R+UP";
        case TOGGLE_DOWN:  return "L+R+DOWN";
        case TOGGLE_LEFT:  return "L+R+LEFT";
        case TOGGLE_RIGHT: return "L+R+RIGHT";
        case TOGGLE_SELECT:
        default:           return "L+R+SELECT";
    }
}

static const char *tr_menu_title(void) {
    return "VITAHUD ULTIMATE";
}

static const char *tr_footer(void) {
    switch (hud_language) {
        case LANG_ES: return "ARR/ABAJO NAVEGAR  IZQ/DER CAMBIAR  X ABRIR  O VOLVER";
        case LANG_FR: return "HAUT/BAS NAVIGUER  GAUCHE/DROITE CHANGER  X OUVRIR  O RETOUR";
        case LANG_DE: return "HOCH/RUNTER NAVIGIEREN  LINKS/RECHTS ANDERN  X OFFNEN  O ZURUCK";
        case LANG_IT: return "SU/GIU NAVIGA  SINISTRA/DESTRA CAMBIA  X APRI  O INDIETRO";
        case LANG_PT: return "CIMA/BAIXO NAVEGAR  ESQ/DIR MUDAR  X ABRIR  O VOLTAR";
        case LANG_NL: return "OMHOOG/OMLAAG NAVIGEER  LINKS/RECHTS WIJZIG  X OPEN  O TERUG";
        case LANG_ID: return "ATAS/BAWAH NAVIGASI  KIRI/KANAN UBAH  X BUKA  O KEMBALI";
        case LANG_TR: return "YUKARI/ASAGI GEZ  SOL/SAG DEGISTIR  X AC  O GERI";
        case LANG_PL: return "GORA/DOL NAWIGUJ  LEWO/PRAWO ZMIEN  X OTWORZ  O WROC";
        case LANG_EN:
        default:      return "UP/DOWN NAVIGATE  LEFT/RIGHT CHANGE  X OPEN/SELECT  O BACK/CLOSE";
    }
}

static int current_menu_count(void) {
    switch (menu_page) {
        case MENU_PAGE_PROFILE:
            return 3;

        case MENU_PAGE_THEME:
            return 13;

        case MENU_PAGE_MAIN:
        default:
            return 24;
    }
}

static int current_menu_item_at(int index) {
    static const int main_items[24] = {
        ITEM_HUD,
        ITEM_LAYOUT,
        ITEM_POSITION,
        ITEM_X_OFFSET,
        ITEM_Y_OFFSET,
        ITEM_SIZE,
        ITEM_FONT,
        ITEM_FPS,
        ITEM_BATTERY,
        ITEM_TIME,
        ITEM_CPU_HUD,
        ITEM_BUS_HUD,
        ITEM_GPU_HUD,
        ITEM_APP_ID_HUD,
        ITEM_RAM_HUD,
        ITEM_IP_HUD,
        ITEM_CHARGING,
        ITEM_TIMEMODE,
        ITEM_PROFILE_MENU,
        ITEM_THEME_MENU,
        ITEM_LANGUAGE,
        ITEM_AUTO_HIDE,
        ITEM_TOGGLE,
        ITEM_RESET
    };

    static const int profile_items[3] = {
        ITEM_PROFILE,
        ITEM_SAVE_PROFILE,
        ITEM_LOAD_PROFILE
    };

    static const int theme_items[13] = {
        ITEM_THEME,
        ITEM_HUD_THEME,
        ITEM_HUD_TEXT,
        ITEM_HUD_SHADOW,
        ITEM_HUD_ICON,
        ITEM_CLOCK_ICON,
        ITEM_HUD_BOX,
        ITEM_HUD_BOX_BG,
        ITEM_MENU_TEXT,
        ITEM_MENU_SELECT,
        ITEM_MENU_BORDER,
        ITEM_MENUBG,
        ITEM_TOP_BAR
    };

    if (index < 0) {
        index = 0;
    }

    if (menu_page == MENU_PAGE_PROFILE) {
        if (index >= 3) index = 2;
        return profile_items[index];
    }

    if (menu_page == MENU_PAGE_THEME) {
        if (index >= 13) index = 12;
        return theme_items[index];
    }

    if (index >= 24) index = 23;
    return main_items[index];
}

static int current_menu_item(void) {
    return current_menu_item_at(menu_index);
}

static void enter_menu_page(int page) {
    menu_page = page;
    menu_index = 0;
    menu_scroll = 0;
    hold_up_frames = 0;
    hold_down_frames = 0;
    menu_nav_dir = 0;
    menu_nav_hold_start_tick = 0;
    menu_nav_next_repeat_tick = 0;
    menu_lr_dir = 0;
    menu_lr_hold_start_tick = 0;
    menu_lr_next_repeat_tick = 0;
}

static const char *current_menu_title(void) {
    if (menu_page == MENU_PAGE_PROFILE) {
        switch (hud_language) {
            case LANG_ES: return "MENU DE PERFIL";
            case LANG_FR: return "MENU PROFIL";
            case LANG_DE: return "PROFIL MENU";
            case LANG_IT: return "MENU PROFILO";
            case LANG_PT: return "MENU PERFIL";
            case LANG_NL: return "PROFIEL MENU";
            case LANG_ID: return "MENU PROFIL";
            case LANG_TR: return "PROFIL MENU";
            case LANG_PL: return "MENU PROFILU";
            default: return "PROFILE MENU";
        }
    }

    if (menu_page == MENU_PAGE_THEME) {
        switch (hud_language) {
            case LANG_ES: return "TEMA / COLOR";
            case LANG_FR: return "THEME / COULEUR";
            case LANG_DE: return "THEMA / FARBE";
            case LANG_IT: return "TEMA / COLORE";
            case LANG_PT: return "TEMA / COR";
            case LANG_NL: return "THEMA / KLEUR";
            case LANG_ID: return "TEMA / WARNA";
            case LANG_TR: return "TEMA / RENK";
            case LANG_PL: return "MOTYW / KOLOR";
            default: return "THEME / COLOR";
        }
    }

    return tr_menu_title();
}

static const char *menu_label(int item) {
    if (hud_language == LANG_ES) {
        switch (item) {
            case ITEM_HUD:          return "HUD";
            case ITEM_LAYOUT:       return "DISENO";
            case ITEM_POSITION:     return "POSICION";
            case ITEM_X_OFFSET:     return "AJUSTE X";
            case ITEM_Y_OFFSET:     return "AJUSTE Y";
            case ITEM_SIZE:         return "TAMANO";
            case ITEM_FONT:         return "FUENTE";
            case ITEM_FPS:          return "FPS";
            case ITEM_BATTERY:      return "BATERIA";
            case ITEM_TIME:         return "RELOJ";
            case ITEM_CHARGING:     return "CARGANDO";
            case ITEM_TIMEMODE:     return "MODO HORA";
            case ITEM_THEME_MENU:   return "TEMA / COLOR";
            case ITEM_THEME:        return "TEMA PRESET";
            case ITEM_HUD_THEME:    return "TEMA HUD";
            case ITEM_HUD_TEXT:     return "TEXTO HUD";
            case ITEM_HUD_SHADOW:   return "SOMBRA HUD";
            case ITEM_HUD_ICON:     return "ICONO BATERIA";
            case ITEM_CLOCK_ICON:   return "ICONO RELOJ";
            case ITEM_HUD_BOX:      return "CAJA HUD";
            case ITEM_HUD_BOX_BG:   return "FONDO CAJA HUD";
            case ITEM_MENU_TEXT:    return "TEXTO MENU";
            case ITEM_MENU_SELECT:  return "SELECCION MENU";
            case ITEM_MENU_BORDER:  return "BORDE MENU";
            case ITEM_MENUBG:       return "FONDO MENU";
            case ITEM_TOP_BAR:      return "BARRA SUPERIOR";
            case ITEM_PROFILE_MENU: return "MENU PERFIL";
            case ITEM_PROFILE:      return "RANURA PERFIL";
            case ITEM_LANGUAGE:     return "IDIOMA";
            case ITEM_AUTO_HIDE:    return "AUTO OCULTAR";
            case ITEM_TOGGLE:       return "ATAJO HUD";
            case ITEM_SAVE_PROFILE: return "GUARDAR PERFIL";
            case ITEM_LOAD_PROFILE: return "CARGAR PERFIL";
            case ITEM_CPU_HUD:      return "HUD CPU";
            case ITEM_BUS_HUD:      return "HUD BUS";
            case ITEM_GPU_HUD:      return "HUD GPU";
            case ITEM_APP_ID_HUD:   return "HUD APP ID";
            case ITEM_RAM_HUD:      return "HUD RAM";
            case ITEM_IP_HUD:       return "HUD IP";
            case ITEM_RESET:        return "VALORES DEFAULT";
            default:                return "";
        }
    }

    switch (item) {
        case ITEM_HUD:          return "HUD";
        case ITEM_LAYOUT:       return "LAYOUT";
        case ITEM_POSITION:     return "POSITION";
        case ITEM_X_OFFSET:     return "X OFFSET";
        case ITEM_Y_OFFSET:     return "Y OFFSET";
        case ITEM_SIZE:         return "SIZE";
        case ITEM_FONT:         return "FONT";
        case ITEM_FPS:          return "FPS";
        case ITEM_BATTERY:      return "BATTERY";
        case ITEM_TIME:         return "CLOCK";
        case ITEM_CHARGING:     return "CHARGING";
        case ITEM_TIMEMODE:     return "TIME MODE";
        case ITEM_THEME_MENU:   return "THEME / COLOR";
        case ITEM_THEME:        return "THEME PRESET";
        case ITEM_HUD_THEME:    return "HUD THEME";
        case ITEM_HUD_TEXT:     return "HUD TEXT";
        case ITEM_HUD_SHADOW:   return "HUD SHADOW";
        case ITEM_HUD_ICON:     return "BATTERY ICON";
        case ITEM_CLOCK_ICON:   return "CLOCK ICON";
        case ITEM_HUD_BOX:      return "HUD BOX";
        case ITEM_HUD_BOX_BG:   return "HUD BOX BACKGROUND";
        case ITEM_MENU_TEXT:    return "MENU TEXT";
        case ITEM_MENU_SELECT:  return "MENU SELECT";
        case ITEM_MENU_BORDER:  return "MENU BORDER";
        case ITEM_MENUBG:       return "MENU BACKGROUND";
        case ITEM_TOP_BAR:      return "TOP MENU BAR";
        case ITEM_PROFILE_MENU: return "PROFILE MENU";
        case ITEM_PROFILE:      return "PROFILE SLOT";
        case ITEM_LANGUAGE:     return "LANGUAGE";
        case ITEM_AUTO_HIDE:    return "AUTO HIDE";
        case ITEM_TOGGLE:       return "HUD TOGGLE";
        case ITEM_SAVE_PROFILE: return "SAVE PROFILE";
        case ITEM_LOAD_PROFILE: return "LOAD PROFILE";
        case ITEM_CPU_HUD:      return "CPU HUD";
        case ITEM_BUS_HUD:      return "BUS HUD";
        case ITEM_GPU_HUD:      return "GPU HUD";
        case ITEM_APP_ID_HUD:   return "APP ID HUD";
        case ITEM_RAM_HUD:      return "RAM HUD";
        case ITEM_IP_HUD:       return "IP HUD";
        case ITEM_RESET:        return "RESET DEFAULTS";
        default:                return "";
    }
}

static const char *menu_value(int item) {
    switch (item) {
        case ITEM_HUD:          return onoff_name(hud_enabled);
        case ITEM_LAYOUT:       return layout_name();
        case ITEM_POSITION:     return position_name();
        case ITEM_SIZE:         return size_name();
        case ITEM_FONT:         return font_name();
        case ITEM_FPS:          return onoff_name(show_fps);
        case ITEM_BATTERY:      return onoff_name(show_battery);
        case ITEM_TIME:         return onoff_name(show_time);
        case ITEM_CHARGING:     return onoff_name(show_charging);
        case ITEM_TIMEMODE:     return time_mode_name();
        case ITEM_HUD_TEXT:     return color_name_generic(hud_text_color);
        case ITEM_HUD_SHADOW:   return color_name_generic(hud_shadow_color);
        case ITEM_HUD_ICON:     return color_name_generic(hud_icon_color);
        case ITEM_CLOCK_ICON:   return color_name_generic(clock_icon_color);
        case ITEM_MENU_TEXT:    return color_name_generic(menu_text_color);
        case ITEM_MENU_SELECT:  return color_name_generic(menu_select_color);
        case ITEM_MENU_BORDER:  return color_name_generic(menu_border_color);
        case ITEM_MENUBG:       return menu_bg_name();
        case ITEM_TOP_BAR:      return color_name_generic(top_menu_bar_color);
        case ITEM_HUD_BOX:      return onoff_name(hud_box_enabled);
        case ITEM_HUD_BOX_BG:   return menu_bg_name_for(hud_box_bg_color);
        case ITEM_THEME_MENU:   return word_open();
        case ITEM_THEME:        return theme_name();
        case ITEM_HUD_THEME:    return hud_theme_name();
        case ITEM_PROFILE_MENU: return word_open();
        case ITEM_PROFILE:      return profile_name();
        case ITEM_LANGUAGE:     return language_name();
        case ITEM_AUTO_HIDE:    return auto_hide_name();
        case ITEM_TOGGLE:       return toggle_name();
        case ITEM_SAVE_PROFILE: return save_message_frames > 0 ? word_saved() : word_press_x();
        case ITEM_LOAD_PROFILE: return save_message_frames > 0 ? word_loaded() : word_press_x();
        case ITEM_CPU_HUD:      return onoff_name(show_cpu);
        case ITEM_BUS_HUD:      return onoff_name(show_bus);
        case ITEM_GPU_HUD:      return onoff_name(show_gpu);
        case ITEM_APP_ID_HUD:   return onoff_name(show_app_id);
        case ITEM_RAM_HUD:      return onoff_name(show_ram);
        case ITEM_IP_HUD:       return onoff_name(show_ip);
        case ITEM_RESET:        return reset_message_frames > 0 ? "RESET" : word_press_x();
        default:                return "";
    }
}

static int menu_offset_step(void) {
    if (menu_lr_step < 2) {
        return 2;
    }

    if (menu_lr_step > 32) {
        return 32;
    }

    return menu_lr_step;
}

static void menu_change(int dir) {
    int item = current_menu_item();

    switch (item) {
        case ITEM_THEME_MENU:
            enter_menu_page(MENU_PAGE_THEME);
            break;

        case ITEM_PROFILE_MENU:
            enter_menu_page(MENU_PAGE_PROFILE);
            break;

        case ITEM_HUD:
            hud_enabled = !hud_enabled;
            break;

        case ITEM_LAYOUT:
            hud_layout += dir;
            if (hud_layout < 0) hud_layout = LAYOUT_COUNT - 1;
            if (hud_layout >= LAYOUT_COUNT) hud_layout = 0;
            break;

        case ITEM_POSITION:
            hud_position += dir;
            if (hud_position < 0) hud_position = POS_COUNT - 1;
            if (hud_position >= POS_COUNT) hud_position = 0;
            break;

        case ITEM_X_OFFSET:
            hud_x_offset += dir * menu_offset_step();
            if (hud_x_offset < 0) hud_x_offset = 0;
            if (hud_x_offset > 960) hud_x_offset = 960;
            break;

        case ITEM_Y_OFFSET:
            hud_y_offset += dir * menu_offset_step();
            if (hud_y_offset < 0) hud_y_offset = 0;
            if (hud_y_offset > 544) hud_y_offset = 544;
            break;

        case ITEM_SIZE:
            hud_size += dir;
            if (hud_size < 0) hud_size = SIZE_COUNT - 1;
            if (hud_size >= SIZE_COUNT) hud_size = 0;
            break;

        case ITEM_FONT:
            font_style += dir;
            if (font_style < 0) font_style = FONT_COUNT - 1;
            if (font_style >= FONT_COUNT) font_style = 0;
            break;

        case ITEM_FPS:
            show_fps = !show_fps;
            break;

        case ITEM_BATTERY:
            show_battery = !show_battery;
            break;

        case ITEM_TIME:
            show_time = !show_time;
            break;

        case ITEM_CHARGING:
            show_charging = !show_charging;
            break;

        case ITEM_TIMEMODE:
            use_24h_time = !use_24h_time;
            break;

        case ITEM_HUD_TEXT:
            hud_text_color += dir;
            if (hud_text_color < 1) hud_text_color = COLOR_COUNT - 1;
            if (hud_text_color >= COLOR_COUNT) hud_text_color = 1;
            break;

        case ITEM_HUD_SHADOW:
            hud_shadow_color += dir;
            if (hud_shadow_color < 1) hud_shadow_color = COLOR_COUNT - 1;
            if (hud_shadow_color >= COLOR_COUNT) hud_shadow_color = 1;
            break;

        case ITEM_HUD_ICON:
            hud_icon_color += dir;
            if (hud_icon_color < 0) hud_icon_color = COLOR_COUNT - 1;
            if (hud_icon_color >= COLOR_COUNT) hud_icon_color = 0;
            break;

        case ITEM_CLOCK_ICON:
            clock_icon_color += dir;
            if (clock_icon_color < 0) clock_icon_color = COLOR_COUNT - 1;
            if (clock_icon_color >= COLOR_COUNT) clock_icon_color = 0;
            break;

        case ITEM_HUD_BOX:
            hud_box_enabled = !hud_box_enabled;
            break;

        case ITEM_HUD_BOX_BG:
            hud_box_bg_color += dir;
            if (hud_box_bg_color < 0) hud_box_bg_color = BG_COUNT - 1;
            if (hud_box_bg_color >= BG_COUNT) hud_box_bg_color = 0;
            break;

        case ITEM_MENU_TEXT:
            menu_text_color += dir;
            if (menu_text_color < 1) menu_text_color = COLOR_COUNT - 1;
            if (menu_text_color >= COLOR_COUNT) menu_text_color = 1;
            break;

        case ITEM_MENU_SELECT:
            menu_select_color += dir;
            if (menu_select_color < 1) menu_select_color = COLOR_COUNT - 1;
            if (menu_select_color >= COLOR_COUNT) menu_select_color = 1;
            break;

        case ITEM_MENU_BORDER:
            menu_border_color += dir;
            if (menu_border_color < 1) menu_border_color = COLOR_COUNT - 1;
            if (menu_border_color >= COLOR_COUNT) menu_border_color = 1;
            break;

        case ITEM_MENUBG:
            menu_bg_color += dir;
            if (menu_bg_color < 0) menu_bg_color = BG_COUNT - 1;
            if (menu_bg_color >= BG_COUNT) menu_bg_color = 0;
            break;

        case ITEM_TOP_BAR:
            top_menu_bar_color += dir;
            if (top_menu_bar_color < 1) top_menu_bar_color = COLOR_COUNT - 1;
            if (top_menu_bar_color >= COLOR_COUNT) top_menu_bar_color = 1;
            break;

        case ITEM_LANGUAGE:
            hud_language += dir;
            if (hud_language < 0) hud_language = LANG_COUNT - 1;
            if (hud_language >= LANG_COUNT) hud_language = 0;
            break;

        case ITEM_AUTO_HIDE:
            auto_hide_mode += dir;
            if (auto_hide_mode < 0) auto_hide_mode = AUTO_HIDE_COUNT - 1;
            if (auto_hide_mode >= AUTO_HIDE_COUNT) auto_hide_mode = 0;
            break;

        case ITEM_TOGGLE:
            toggle_combo_mode += dir;
            if (toggle_combo_mode < 0) toggle_combo_mode = TOGGLE_COUNT - 1;
            if (toggle_combo_mode >= TOGGLE_COUNT) toggle_combo_mode = 0;
            break;

        case ITEM_THEME:
            theme_id += dir;
            if (theme_id < 0) theme_id = THEME_COUNT - 1;
            if (theme_id >= THEME_COUNT) theme_id = 0;
            apply_theme();
            menu_scroll = 0;
            break;

        case ITEM_HUD_THEME:
            hud_theme_id += dir;
            if (hud_theme_id < 0) hud_theme_id = HUD_THEME_COUNT - 1;
            if (hud_theme_id >= HUD_THEME_COUNT) hud_theme_id = 0;
            apply_hud_theme();
            break;

        case ITEM_PROFILE:
            profile_id += dir;
            if (profile_id < 0) profile_id = PROFILE_COUNT - 1;
            if (profile_id >= PROFILE_COUNT) profile_id = 0;
            break;

        case ITEM_SAVE_PROFILE:
            save_profile();
            break;

        case ITEM_LOAD_PROFILE:
            load_profile();
            break;

        case ITEM_CPU_HUD:
            show_cpu = !show_cpu;
            break;

        case ITEM_BUS_HUD:
            show_bus = !show_bus;
            break;

        case ITEM_GPU_HUD:
            show_gpu = !show_gpu;
            break;

        case ITEM_APP_ID_HUD:
            show_app_id = !show_app_id;
            break;

        case ITEM_RAM_HUD:
            show_ram = !show_ram;
            break;

        case ITEM_IP_HUD:
            show_ip = !show_ip;
            break;

        case ITEM_RESET:
            reset_defaults();
            break;

        default:
            break;
    }
}

static void draw_menu_line(
    unsigned int *pixels,
    int pitch,
    int x,
    int y,
    int row_w,
    int selected,
    const char *label,
    const char *value
) {
    unsigned int text_col = color_value(menu_text_color, 0xFFFFFFFF);
    unsigned int sel_col = color_value(menu_select_color, 0xFF00FFFF);
    unsigned int accent_col = color_value(menu_select_color, 0xFF00FFFF);
    unsigned int sep_col = 0xFF2A2A2A;
    int label_x = x + 10;
    int value_w = text_width(value, 1);
    int value_x = x + row_w - 10 - value_w;

    if (value_x < x + 196) {
        value_x = x + 196;
    }

    /*
     * Clean selection without a black slab:
     * left accent bar + thin top/bottom rails.
     */
    if (selected) {
        draw_rect(pixels, pitch, x + 1, y - 1, 2, 11, accent_col);
        draw_rect(pixels, pitch, x + 4, y - 1, row_w - 6, 1, accent_col);
        draw_rect(pixels, pitch, x + 4, y + 9, row_w - 6, 1, accent_col);
        draw_text_shadow(pixels, pitch, label_x, y, label, sel_col, 1);
        draw_text_shadow(pixels, pitch, value_x, y, value, sel_col, 1);
    } else {
        draw_rect(pixels, pitch, x + 4, y + 10, row_w - 8, 1, sep_col);
        draw_text_shadow(pixels, pitch, label_x, y, label, text_col, 1);
        draw_text_shadow(pixels, pitch, value_x, y, value, text_col, 1);
    }
}

static void draw_menu(unsigned int *pixels, int pitch, int screen_w, int screen_h) {
    int x = 28;
    int y = 42;
    int w = 430;
    int h = 248;
    int line_y;
    int i;
    int visible = 15;
    int panel_x;
    int panel_y;
    int panel_w;
    int panel_h;
    int content_x;
    int row_w;
    const char *title_text;
    int title_w;
    int title_x;
    int title_y;

    unsigned int border = color_value(menu_border_color, 0xFFFFFFFF);
    unsigned int title_col = color_value(menu_select_color, 0xFF00FFFF);
    unsigned int text_col = color_value(menu_text_color, 0xFFFFFFFF);
    unsigned int inner_line = 0xFF2A2A2A;

    if (screen_w <= 480 || screen_h <= 272) {
        x = 10;
        y = 10;
        w = screen_w - 20;
        h = screen_h - 20;
        visible = 11;
    }

    if (menu_index < menu_scroll) {
        menu_scroll = menu_index;
    }

    if (menu_index >= menu_scroll + visible) {
        menu_scroll = menu_index - visible + 1;
    }

    if (menu_scroll < 0) {
        menu_scroll = 0;
    }

    if (menu_scroll > current_menu_count() - visible) {
        menu_scroll = current_menu_count() - visible;
    }

    if (menu_scroll < 0) {
        menu_scroll = 0;
    }

    panel_x = x - 10;
    panel_y = y - 10;
    panel_w = w + 4;
    panel_h = h + 4;
    content_x = x + 2;
    row_w = w - 18;

    /*
     * Menu background:
     * Uses MENU BG setting again.
     * Default is BLACK, but user can change it with MENU BG.
     */
    if (menu_bg_color != BG_TRANSPARENT) {
        draw_rect(pixels, pitch, panel_x, panel_y, panel_w, panel_h, get_menu_bg());
    }

    /* Ultimate menu skin: layered header, side rail, corner accents, and inner glow. */
    draw_rect(pixels, pitch, panel_x + 2, panel_y + 2, panel_w - 4, 14, color_value(top_menu_bar_color, 0xFF000000));
    draw_rect(pixels, pitch, panel_x + 2, panel_y + 2, 3, panel_h - 4, title_col);
    draw_rect(pixels, pitch, panel_x + panel_w - 5, panel_y + 2, 3, panel_h - 4, title_col);
    draw_rect(pixels, pitch, panel_x + 5, panel_y + 5, 24, 1, title_col);
    draw_rect(pixels, pitch, panel_x + 5, panel_y + 5, 1, 16, title_col);
    draw_rect(pixels, pitch, panel_x + panel_w - 29, panel_y + 5, 24, 1, title_col);
    draw_rect(pixels, pitch, panel_x + panel_w - 6, panel_y + 5, 1, 16, title_col);
    draw_rect(pixels, pitch, panel_x + 5, panel_y + panel_h - 6, 24, 1, title_col);
    draw_rect(pixels, pitch, panel_x + 5, panel_y + panel_h - 21, 1, 16, title_col);
    draw_rect(pixels, pitch, panel_x + panel_w - 29, panel_y + panel_h - 6, 24, 1, title_col);
    draw_rect(pixels, pitch, panel_x + panel_w - 6, panel_y + panel_h - 21, 1, 16, title_col);

    draw_rect(pixels, pitch, panel_x, panel_y, panel_w, 1, border);
    draw_rect(pixels, pitch, panel_x, panel_y + panel_h - 1, panel_w, 1, border);
    draw_rect(pixels, pitch, panel_x, panel_y, 1, panel_h, border);
    draw_rect(pixels, pitch, panel_x + panel_w - 1, panel_y, 1, panel_h, border);

    draw_rect(pixels, pitch, panel_x + 2, panel_y + 17, panel_w - 4, 1, border);
    draw_rect(pixels, pitch, panel_x + 2, panel_y + panel_h - 20, panel_w - 4, 1, border);
    draw_rect(pixels, pitch, panel_x + 2, panel_y + 18, panel_w - 4, 1, inner_line);

    title_text = current_menu_title();
    title_w = text_width(title_text, 1);
    title_x = panel_x + ((panel_w - title_w) / 2);
    title_y = panel_y + 4;

    if (title_x < panel_x + 12) {
        title_x = panel_x + 12;
    }

    draw_text_shadow(pixels, pitch, title_x, title_y, title_text, title_col, 1);

    if (menu_scroll > 0) {
        draw_text_shadow(pixels, pitch, panel_x + (panel_w / 2) - 4, y + 1, "^", title_col, 1);
    }

    if (menu_scroll + visible < current_menu_count()) {
        draw_text_shadow(pixels, pitch, panel_x + (panel_w / 2) - 4, y + h - 36, "V", title_col, 1);
    }

    line_y = y + 18;

    for (i = menu_scroll; i < current_menu_count() && i < menu_scroll + visible; i++) {
        draw_menu_line(
            pixels,
            pitch,
            content_x,
            line_y,
            row_w,
            menu_index == i,
            menu_label(current_menu_item_at(i)),
            menu_value(current_menu_item_at(i))
        );

        line_y += 12;
    }

    draw_text_shadow(
        pixels,
        pitch,
        x,
        y + h - 22,
        tr_footer(),
        text_col,
        1
    );
}

static void move_menu_index(int dir) {
    int count = current_menu_count();

    if (count <= 0) {
        menu_index = 0;
        return;
    }

    menu_index += dir;

    if (menu_index < 0) {
        menu_index = count - 1;
    }

    if (menu_index >= count) {
        menu_index = 0;
    }
}

static int menu_repeat_interval_usec(SceUInt64 held_usec) {
    if (held_usec < 1200000) {
        return 260000;
    }

    if (held_usec < 2200000) {
        return 160000;
    }

    if (held_usec < 3600000) {
        return 95000;
    }

    return 65000;
}

static void handle_menu_vertical(unsigned int buttons, unsigned int pressed) {
    int dir = 0;
    SceRtcTick tick;
    SceUInt64 now;
    SceUInt64 held_usec;
    int interval;

    if (buttons & SCE_CTRL_UP) {
        dir = -1;
    } else if (buttons & SCE_CTRL_DOWN) {
        dir = 1;
    }

    if (dir == 0) {
        hold_up_frames = 0;
        hold_down_frames = 0;
        menu_nav_dir = 0;
        menu_nav_hold_start_tick = 0;
        menu_nav_next_repeat_tick = 0;
        return;
    }

    sceRtcGetCurrentTick(&tick);
    now = tick.tick;

    if ((dir < 0 && (pressed & SCE_CTRL_UP)) || (dir > 0 && (pressed & SCE_CTRL_DOWN)) || menu_nav_dir != dir) {
        move_menu_index(dir);
        menu_nav_dir = dir;
        menu_nav_hold_start_tick = now;
        menu_nav_next_repeat_tick = now + 650000;

        if (dir < 0) {
            hold_up_frames = 1;
            hold_down_frames = 0;
        } else {
            hold_down_frames = 1;
            hold_up_frames = 0;
        }

        return;
    }

    if (menu_nav_hold_start_tick == 0) {
        menu_nav_hold_start_tick = now;
        menu_nav_next_repeat_tick = now + 650000;
        return;
    }

    if (now >= menu_nav_next_repeat_tick) {
        move_menu_index(dir);
        held_usec = now - menu_nav_hold_start_tick;
        interval = menu_repeat_interval_usec(held_usec);
        menu_nav_next_repeat_tick = now + interval;

        if (dir < 0) {
            hold_up_frames++;
        } else {
            hold_down_frames++;
        }
    }
}

static int current_item_allows_hold_lr(void) {
    int item = current_menu_item();
    return item == ITEM_X_OFFSET || item == ITEM_Y_OFFSET;
}

static void reset_menu_lr_repeat(void) {
    menu_lr_dir = 0;
    menu_lr_hold_start_tick = 0;
    menu_lr_next_repeat_tick = 0;
    menu_lr_step = 4;
}

static int menu_lr_repeat_interval_usec(SceUInt64 held_usec) {
    if (held_usec < 300000) {
        return 90000;
    }

    if (held_usec < 900000) {
        return 55000;
    }

    if (held_usec < 1600000) {
        return 33000;
    }

    return 18000;
}

static int menu_lr_step_for_hold(SceUInt64 held_usec) {
    if (held_usec < 300000) {
        return 4;
    }

    if (held_usec < 900000) {
        return 8;
    }

    if (held_usec < 1600000) {
        return 14;
    }

    if (held_usec < 2600000) {
        return 22;
    }

    return 32;
}

static void handle_menu_horizontal(unsigned int buttons, unsigned int pressed) {
    SceRtcTick tick;
    SceUInt64 now;
    SceUInt64 held_usec;
    int dir = 0;
    int interval;

    if (buttons & SCE_CTRL_LEFT) {
        dir = -1;
    } else if (buttons & SCE_CTRL_RIGHT) {
        dir = 1;
    }

    if (dir == 0) {
        reset_menu_lr_repeat();
        return;
    }

    if (!current_item_allows_hold_lr()) {
        if ((dir < 0 && (pressed & SCE_CTRL_LEFT)) || (dir > 0 && (pressed & SCE_CTRL_RIGHT))) {
            menu_change(dir);
        }
        reset_menu_lr_repeat();
        return;
    }

    sceRtcGetCurrentTick(&tick);
    now = tick.tick;

    if ((dir < 0 && (pressed & SCE_CTRL_LEFT)) || (dir > 0 && (pressed & SCE_CTRL_RIGHT)) || menu_lr_dir != dir) {
        menu_lr_step = 4;
        menu_change(dir);
        menu_lr_dir = dir;
        menu_lr_hold_start_tick = now;
        menu_lr_next_repeat_tick = now + 260000;
        return;
    }

    if (menu_lr_hold_start_tick == 0) {
        menu_lr_hold_start_tick = now;
        menu_lr_next_repeat_tick = now + 260000;
        return;
    }

    if (now >= menu_lr_next_repeat_tick) {
        held_usec = now - menu_lr_hold_start_tick;
        menu_lr_step = menu_lr_step_for_hold(held_usec);
        menu_change(dir);
        interval = menu_lr_repeat_interval_usec(held_usec);
        menu_lr_next_repeat_tick = now + interval;
    }
}

static void handle_input(void) {
    SceCtrlData pad;
    unsigned int buttons;
    unsigned int pressed;
    unsigned int toggle_combo;
    int toggle_down;
    int menu_down;
    int frames = 0;

    sceCtrlPeekBufferPositive(0, &pad, 1);

    buttons = pad.buttons;
    pressed = buttons & ~last_buttons;

    toggle_combo = get_toggle_combo();

    toggle_down = ((buttons & toggle_combo) == toggle_combo);
    menu_down = ((buttons & MENU_COMBO) == MENU_COMBO);

    if (menu_down && !last_menu_down) {
        menu_open = !menu_open;
    }

    if (!menu_open && toggle_down && !last_toggle_down) {
        if (auto_hide_mode == AUTO_HIDE_OFF) {
            hud_enabled = !hud_enabled;
        } else {
            if (auto_hide_mode == AUTO_HIDE_3S) frames = 180;
            if (auto_hide_mode == AUTO_HIDE_5S) frames = 300;
            if (auto_hide_mode == AUTO_HIDE_10S) frames = 600;
            temporary_show_frames = frames;
        }
    }

    if (menu_open) {
        handle_menu_vertical(buttons, pressed);

        handle_menu_horizontal(buttons, pressed);

        if (pressed & SCE_CTRL_CROSS) {
            menu_change(1);
        }

        if (pressed & SCE_CTRL_CIRCLE) {
            if (menu_page != MENU_PAGE_MAIN) {
                enter_menu_page(MENU_PAGE_MAIN);
            } else {
                menu_open = 0;
            }
        }
    }

    last_toggle_down = toggle_down;
    last_menu_down = menu_down;
    last_buttons = buttons;
}

static int add_text_block(
    unsigned int *pixels,
    int pitch,
    int x,
    int y,
    const char *text,
    unsigned int color,
    int scale
) {
    draw_text_shadow(pixels, pitch, x, y, text, color, scale);
    return text_width(text, scale);
}

static void draw_hud(unsigned int *pixels, int pitch, int screen_w, int screen_h) {
    int battery = scePowerGetBatteryLifePercent();

    char fps_text[16];
    char battery_text[8];
    char time_text[16];
    char charging_text[8];
    char cpu_text[16];
    char bus_text[16];
    char gpu_text[16];
    char app_id_text[24];
    char ram_text[16];
    char ip_text[20];

    int scale;
    int gap_small;
    int gap_big;

    int icon_scale;
    int fps_w;
    int battery_text_w;
    int time_w;
    int charging_w;
    int cpu_w;
    int bus_w;
    int gpu_w;
    int app_id_w;
    int ram_w;
    int ip_w;
    int force_stacked;

    int battery_icon_w;
    int battery_icon_h;
    int clock_icon_w;
    int clock_icon_h;

    int total_w = 0;
    int total_h;
    int text_h;
    int margin_x = hud_x_offset;
    int margin_y = hud_y_offset;
    int start_x;
    int start_y;
    int x;
    int y;

    unsigned int text_color = color_value(hud_text_color, 0xFFFFFFFF);

    get_hud_metrics(&scale, &gap_small, &gap_big);

    icon_scale = scale;

    build_fps_text(fps_text);
    build_battery_text(battery_text, battery);
    build_time_text(time_text);
    build_charging_text(charging_text);
    build_cpu_text(cpu_text);
    build_bus_text(bus_text);
    build_gpu_text(gpu_text);
    build_app_id_text(app_id_text);
    build_ram_text(ram_text);
    build_ip_text(ip_text);

    force_stacked = 0;

    fps_w = show_fps ? text_width(fps_text, scale) : 0;
    battery_text_w = show_battery ? text_width(battery_text, scale) : 0;
    time_w = show_time ? text_width(time_text, scale) : 0;
    charging_w = show_charging ? text_width(charging_text, scale) : 0;
    cpu_w = show_cpu ? text_width(cpu_text, scale) : 0;
    bus_w = show_bus ? text_width(bus_text, scale) : 0;
    gpu_w = show_gpu ? text_width(gpu_text, scale) : 0;
    app_id_w = show_app_id ? text_width(app_id_text, scale) : 0;
    ram_w = show_ram ? text_width(ram_text, scale) : 0;
    ip_w = show_ip ? text_width(ip_text, scale) : 0;

    battery_icon_w = show_battery ? ((13 * icon_scale) + (2 * icon_scale)) : 0;
    battery_icon_h = 7 * icon_scale;

    clock_icon_w = show_time ? (7 * icon_scale) : 0;
    clock_icon_h = 7 * icon_scale;

    text_h = 7 * scale;

    if (hud_layout == LAYOUT_STACKED || force_stacked) {
        total_w = 0;

        if (show_fps && fps_w > total_w) total_w = fps_w;
        if (show_battery && battery_icon_w + gap_small + battery_text_w > total_w) total_w = battery_icon_w + gap_small + battery_text_w;
        if (show_time && clock_icon_w + gap_small + time_w > total_w) total_w = clock_icon_w + gap_small + time_w;
        if (show_charging && charging_w > total_w) total_w = charging_w;
        if (show_cpu && cpu_w > total_w) total_w = cpu_w;
        if (show_bus && bus_w > total_w) total_w = bus_w;
        if (show_gpu && gpu_w > total_w) total_w = gpu_w;
        if (show_app_id && app_id_w > total_w) total_w = app_id_w;
        if (show_ram && ram_w > total_w) total_w = ram_w;
        if (show_ip && ip_w > total_w) total_w = ip_w;

        total_h = 0;

        if (show_fps) total_h += text_h + gap_small;
        if (show_battery) total_h += text_h + gap_small;
        if (show_time) total_h += text_h + gap_small;
        if (show_charging) total_h += text_h + gap_small;
        if (show_cpu) total_h += text_h + gap_small;
        if (show_bus) total_h += text_h + gap_small;
        if (show_gpu) total_h += text_h + gap_small;
        if (show_app_id) total_h += text_h + gap_small;
        if (show_ram) total_h += text_h + gap_small;
        if (show_ip) total_h += text_h + gap_small;

        if (total_h > 0) total_h -= gap_small;
    } else {
        if (show_fps) {
            total_w += fps_w;
        }

        if (show_battery) {
            if (total_w > 0) total_w += gap_big;
            total_w += battery_icon_w + gap_small + battery_text_w;
        }

        if (show_time) {
            if (total_w > 0) total_w += gap_big;
            total_w += clock_icon_w + gap_small + time_w;
        }

        if (show_charging) {
            if (total_w > 0) total_w += gap_big;
            total_w += charging_w;
        }

        if (show_cpu) {
            if (total_w > 0) total_w += gap_big;
            total_w += cpu_w;
        }

        if (show_bus) {
            if (total_w > 0) total_w += gap_big;
            total_w += bus_w;
        }

        if (show_gpu) {
            if (total_w > 0) total_w += gap_big;
            total_w += gpu_w;
        }

        if (show_app_id) {
            if (total_w > 0) total_w += gap_big;
            total_w += app_id_w;
        }

        if (show_ram) {
            if (total_w > 0) total_w += gap_big;
            total_w += ram_w;
        }

        if (show_ip) {
            if (total_w > 0) total_w += gap_big;
            total_w += ip_w;
        }

        total_h = text_h;
    }

    if (total_w <= 0 || total_h <= 0) {
        return;
    }

    start_x = margin_x;
    start_y = margin_y;

    if (hud_position == POS_BOTTOM_RIGHT || hud_position == POS_TOP_RIGHT) {
        start_x = screen_w - total_w - margin_x;
    }

    if (hud_position == POS_BOTTOM_RIGHT || hud_position == POS_BOTTOM_LEFT) {
        start_y = screen_h - total_h - margin_y;
    }

    if (start_x < 0) {
        start_x = 0;
    }

    if (start_y < 0) {
        start_y = 0;
    }

    if (start_x + total_w > screen_w) {
        start_x = screen_w - total_w;
        if (start_x < 0) start_x = 0;
    }

    if (start_y + total_h > screen_h) {
        start_y = screen_h - total_h;
        if (start_y < 0) start_y = 0;
    }

    /*
     * Visual clean-up fix:
     * Clear the last HUD footprint and draw a small black backing behind
     * the current HUD. This prevents stale text like "IP OFF 69" from
     * hanging around when toggles/layout change.
     */
    last_hud_clear_x = start_x - 6;
    last_hud_clear_y = start_y - 6;
    last_hud_clear_w = total_w + 12;
    last_hud_clear_h = total_h + 12;

    if (hud_box_enabled) {
        /* HUD box background only. No outline. */
        draw_rect(pixels, pitch, last_hud_clear_x, last_hud_clear_y, last_hud_clear_w, last_hud_clear_h, get_menu_bg_for(hud_box_bg_color));
    }

    /*
     * HUD background:
     * Transparent again. No HUD BG option.
     * We still track the last HUD footprint for layout bookkeeping,
     * but we do not draw a black rectangle behind FPS/battery/time.
     */

    x = start_x;
    y = start_y;

    if (hud_layout == LAYOUT_STACKED || force_stacked) {
        if (show_fps) {
            draw_text_shadow(pixels, pitch, x, y, fps_text, text_color, scale);
            y += text_h + gap_small;
        }

        if (show_battery) {
            draw_battery_icon(pixels, pitch, x, y + ((text_h - battery_icon_h) / 2), battery, icon_scale);
            draw_text_shadow(pixels, pitch, x + battery_icon_w + gap_small, y, battery_text, text_color, scale);
            y += text_h + gap_small;
        }

        if (show_time) {
            draw_clock_icon(pixels, pitch, x, y + ((text_h - clock_icon_h) / 2), icon_scale);
            draw_text_shadow(pixels, pitch, x + clock_icon_w + gap_small, y, time_text, text_color, scale);
            y += text_h + gap_small;
        }

        if (show_charging) {
            draw_text_shadow(pixels, pitch, x, y, charging_text, text_color, scale);
            y += text_h + gap_small;
        }

        if (show_cpu) {
            draw_text_shadow(pixels, pitch, x, y, cpu_text, text_color, scale);
            y += text_h + gap_small;
        }

        if (show_bus) {
            draw_text_shadow(pixels, pitch, x, y, bus_text, text_color, scale);
            y += text_h + gap_small;
        }

        if (show_gpu) {
            draw_text_shadow(pixels, pitch, x, y, gpu_text, text_color, scale);
            y += text_h + gap_small;
        }

        if (show_app_id) {
            draw_text_shadow(pixels, pitch, x, y, app_id_text, text_color, scale);
            y += text_h + gap_small;
        }

        if (show_ram) {
            draw_text_shadow(pixels, pitch, x, y, ram_text, text_color, scale);
            y += text_h + gap_small;
        }

        if (show_ip) {
            draw_text_shadow(pixels, pitch, x, y, ip_text, text_color, scale);
        }

        return;
    }

    if (show_fps) {
        x += add_text_block(pixels, pitch, x, start_y, fps_text, text_color, scale);
    }

    if (show_battery) {
        if (x != start_x) x += gap_big;

        draw_battery_icon(
            pixels,
            pitch,
            x,
            start_y + ((text_h - battery_icon_h) / 2),
            battery,
            icon_scale
        );

        x += battery_icon_w + gap_small;

        draw_text_shadow(pixels, pitch, x, start_y, battery_text, text_color, scale);
        x += battery_text_w;
    }

    if (show_time) {
        if (x != start_x) x += gap_big;

        draw_clock_icon(
            pixels,
            pitch,
            x,
            start_y + ((text_h - clock_icon_h) / 2),
            icon_scale
        );

        x += clock_icon_w + gap_small;

        draw_text_shadow(pixels, pitch, x, start_y, time_text, text_color, scale);
        x += time_w;
    }

    if (show_charging) {
        if (x != start_x) x += gap_big;
        draw_text_shadow(pixels, pitch, x, start_y, charging_text, text_color, scale);
        x += charging_w;
    }

    if (show_cpu) {
        if (x != start_x) x += gap_big;
        draw_text_shadow(pixels, pitch, x, start_y, cpu_text, text_color, scale);
        x += cpu_w;
    }

    if (show_bus) {
        if (x != start_x) x += gap_big;
        draw_text_shadow(pixels, pitch, x, start_y, bus_text, text_color, scale);
        x += bus_w;
    }

    if (show_gpu) {
        if (x != start_x) x += gap_big;
        draw_text_shadow(pixels, pitch, x, start_y, gpu_text, text_color, scale);
        x += gpu_w;
    }

    if (show_app_id) {
        if (x != start_x) x += gap_big;
        draw_text_shadow(pixels, pitch, x, start_y, app_id_text, text_color, scale);
        x += app_id_w;
    }

    if (show_ram) {
        if (x != start_x) x += gap_big;
        draw_text_shadow(pixels, pitch, x, start_y, ram_text, text_color, scale);
        x += ram_w;
    }

    if (show_ip) {
        if (x != start_x) x += gap_big;
        draw_text_shadow(pixels, pitch, x, start_y, ip_text, text_color, scale);
        x += ip_w;
    }
}

static void draw_all_from_framebuf(const SceDisplayFrameBuf *fb) {
    unsigned int *pixels;
    int screen_w;
    int screen_h;
    int pitch;
    int should_draw_hud = 0;

    if (!fb || !fb->base) {
        return;
    }

    if (fb->pixelformat != SCE_DISPLAY_PIXELFORMAT_A8B8G8R8) {
        return;
    }

    if (fb->width <= 0 || fb->height <= 0 || fb->pitch <= 0) {
        return;
    }

    pixels = (unsigned int *)fb->base;
    screen_w = fb->width;
    screen_h = fb->height;
    pitch = fb->pitch;

    g_screen_w = screen_w;
    g_screen_h = screen_h;

    if (hud_enabled) {
        if (auto_hide_mode == AUTO_HIDE_OFF) {
            should_draw_hud = 1;
        } else if (temporary_show_frames > 0 || menu_open) {
            should_draw_hud = 1;
        }
    }

    if (should_draw_hud) {
        draw_hud(pixels, pitch, screen_w, screen_h);
    } else {
        last_hud_clear_x = -1;
        last_hud_clear_y = -1;
        last_hud_clear_w = 0;
        last_hud_clear_h = 0;
    }

    if (menu_open) {
        draw_menu(pixels, pitch, screen_w, screen_h);
    }
}

typedef int (*VitaHUD_SetFrameBufFn)(const SceDisplayFrameBuf *pParam, int sync);

static int vitahud_continue_setframebuf(const SceDisplayFrameBuf *pParam, int sync) {
    struct _tai_hook_user *cur = (struct _tai_hook_user *)g_display_hook;
    struct _tai_hook_user *next = 0;
    VitaHUD_SetFrameBufFn fn = 0;

    if (!cur) {
        return 0;
    }

    next = (struct _tai_hook_user *)cur->next;
    if (next) {
        fn = (VitaHUD_SetFrameBufFn)next->func;
    } else {
        fn = (VitaHUD_SetFrameBufFn)cur->old;
    }

    if (!fn) {
        return 0;
    }

    return fn(pParam, sync);
}

static int sceDisplaySetFrameBuf_patched(const SceDisplayFrameBuf *pParam, int sync) {
    /*
     * Core fix copied from VITABatteryPlus behavior:
     * draw during sceDisplaySetFrameBuf, using the submitted buffer.
     * This avoids polling sceDisplayGetFrameBuf from a separate HUD thread,
     * which was the source of unstable/zero buffers and menu jitter.
     */
    handle_input();
    update_fps();
    update_system_cache();
    draw_all_from_framebuf(pParam);

    if (save_message_frames > 0) {
        save_message_frames--;
    }

    if (reset_message_frames > 0) {
        reset_message_frames--;
    }

    if (temporary_show_frames > 0) {
        temporary_show_frames--;
    }

    return vitahud_continue_setframebuf(pParam, sync);
}

int module_start(SceSize args, void *argp) {
    (void)args;
    (void)argp;

    load_profile();

    /* Same import hook method used by VITABatteryPlus 1.8. */
    g_display_hook_uid = taiHookFunctionImport(
        &g_display_hook,
        TAI_MAIN_MODULE,
        0x4FAACD11,
        0x7A410B64,
        sceDisplaySetFrameBuf_patched
    );

    return SCE_KERNEL_START_SUCCESS;
}

int module_stop(SceSize args, void *argp) {
    (void)args;
    (void)argp;

    if (g_display_hook_uid >= 0) {
        taiHookRelease(g_display_hook_uid, g_display_hook);
        g_display_hook_uid = -1;
    }

    return SCE_KERNEL_STOP_SUCCESS;
}
