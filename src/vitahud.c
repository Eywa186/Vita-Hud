#include <psp2/kernel/modulemgr.h>
#include <psp2/kernel/threadmgr.h>
#include <psp2/kernel/sysmem.h>
#include <psp2/ctrl.h>
#include <psp2/power.h>
#include <psp2/rtc.h>
#include <psp2/display.h>
#include <psp2/appmgr.h>
#include <psp2/io/fcntl.h>
#include <psp2/io/stat.h>
#include <psp2/types.h>
#include <taihen.h>

#define CONFIG_DIR  "ur0:data/VitaHUD"
#define CONFIG_PATH "ur0:data/VitaHUD/config.txt"

#define MENU_COMBO (SCE_CTRL_LTRIGGER | SCE_CTRL_RTRIGGER | SCE_CTRL_START)

#define POS_BOTTOM_RIGHT 0
#define POS_BOTTOM_LEFT  1
#define POS_TOP_RIGHT    2
#define POS_TOP_LEFT     3
#define POS_TOP_CENTER   4
#define POS_BOTTOM_CENTER 5
#define POS_COUNT        6

#define SIZE_MICRO   0
#define SIZE_NORMAL  1
#define SIZE_LARGE   2
#define SIZE_COUNT   3

#define MAIN_MENU_SIZE_COMPACT 0
#define MAIN_MENU_SIZE_DEFAULT 1
#define MAIN_MENU_SIZE_LARGE   2
#define MAIN_MENU_SIZE_COUNT   3

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
#define COLOR_BROWN      31
#define COLOR_COPPER     32
#define COLOR_CHOCOLATE  33
#define COLOR_HOT_PINK   34
#define COLOR_NEON_GREEN 35
#define COLOR_ELECTRIC_BLUE 36
#define COLOR_MIDNIGHT   37
#define COLOR_SNOW       38
#define COLOR_CREAM      39
#define COLOR_COUNT      40

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
#define TOGGLE_START  5
#define TOGGLE_TRIANGLE 6
#define TOGGLE_CIRCLE 7
#define TOGGLE_SQUARE 8
#define TOGGLE_CROSS 9
#define TOGGLE_SELECT_UP 10
#define TOGGLE_SELECT_DOWN 11
#define TOGGLE_START_UP 12
#define TOGGLE_START_DOWN 13
#define TOGGLE_L_SELECT 14
#define TOGGLE_R_SELECT 15
#define TOGGLE_L_START 16
#define TOGGLE_R_START 17
#define TOGGLE_COUNT  18

#define OPACITY_100 0
#define OPACITY_75  1
#define OPACITY_50  2
#define OPACITY_25  3
#define OPACITY_COUNT 4

#define FPS_STYLE_ICON_NUMBER 0
#define FPS_STYLE_NUMBER_ONLY 1
#define FPS_STYLE_ICON_ONLY   2
#define FPS_STYLE_LABEL_NUMBER 3
#define FPS_STYLE_COUNT       4

#define BATTERY_STYLE_ICON_PERCENT 0
#define BATTERY_STYLE_PERCENT_ONLY 1
#define BATTERY_STYLE_ICON_ONLY    2
#define BATTERY_STYLE_BAR_ONLY     3
#define BATTERY_STYLE_COUNT        4

#define CLOCK_STYLE_ICON_TIME 0
#define CLOCK_STYLE_TIME_ONLY 1
#define CLOCK_STYLE_ICON_ONLY 2
#define CLOCK_STYLE_DATE_TIME 3
#define CLOCK_STYLE_COUNT     4

#define ICON_STYLE_ORIGINAL   0
#define ICON_STYLE_OUTLINE    1
#define ICON_STYLE_SOLID      2
#define ICON_STYLE_DIAMOND    3
#define ICON_STYLE_CROSS      4
#define ICON_STYLE_DOTS       5
#define ICON_STYLE_CHEVRON    6
#define ICON_STYLE_MINI       7
#define ICON_STYLE_BARS       8
#define ICON_STYLE_TARGET     9
#define ICON_STYLE_STARBIRD   10
#define ICON_STYLE_IMPERIAL   11
#define ICON_STYLE_JEDI       12
#define ICON_STYLE_SITH       13
#define ICON_STYLE_SABER      14
#define ICON_STYLE_SKULL      15
#define ICON_STYLE_CROWN      16
#define ICON_STYLE_GEM        17
#define ICON_STYLE_FLAME      18
#define ICON_STYLE_ORBIT      19
#define ICON_STYLE_PHOENIX    20
#define ICON_STYLE_ECLIPSE    21
#define ICON_STYLE_NEXUS      22
#define ICON_STYLE_VIPER      23
#define ICON_STYLE_RUNE       24
#define ICON_STYLE_HALO       25
#define ICON_STYLE_PRISM      26
#define ICON_STYLE_COMET      27
#define ICON_STYLE_SIGIL      28
#define ICON_STYLE_VORTEX     29
#define ICON_STYLE_COUNT      30

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
#define THEME_PLASMA    7
#define THEME_CYBER     8
#define THEME_SUNSET    9
#define THEME_OCEAN     10
#define THEME_FOREST    11
#define THEME_ROYAL     12
#define THEME_CANDY     13
#define THEME_STEEL     14
#define THEME_FIRE      15
#define THEME_GHOST     16
#define THEME_VOID      17
#define THEME_AURORA    18
#define THEME_OBSIDIAN  19
#define THEME_NOVA      20
#define THEME_DRAGON    21
#define THEME_COSMIC    22
#define THEME_TOXIC     23
#define THEME_INFERNO   24
#define THEME_ABYSS     25
#define THEME_CHROME    26
#define THEME_COUNT     27

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

#define HUD_ORDER_BATTERY 0
#define HUD_ORDER_CLOCK   1
#define HUD_ORDER_CPU     2
#define HUD_ORDER_BUS     3
#define HUD_ORDER_GPU     4
#define HUD_ORDER_APP_ID  5
#define HUD_ORDER_RAM     6
#define HUD_ORDER_FPS     7
#define HUD_ORDER_DATE    8
#define HUD_ORDER_COUNT   9

/*
 * MENU ORDER ONLY.
 * Render/glitch hook path is intentionally untouched.
 * CPU/BUS/GPU/APP/RAM are now directly under CLOCK for faster toggling.
 * Theme/color and profile groups are exposed as clear menu sections.
 */
#define ITEM_HUD          0
#define ITEM_LAYOUT       1
#define ITEM_POSITION     2
#define ITEM_X_OFFSET     3
#define ITEM_Y_OFFSET     4
#define ITEM_SIZE         5
#define ITEM_MENU_SIZE    6
#define ITEM_FONT         7
#define ITEM_FPS          8
#define ITEM_BATTERY      9
#define ITEM_TIME         10
#define ITEM_CPU_HUD      11
#define ITEM_BUS_HUD      12
#define ITEM_GPU_HUD      13
#define ITEM_APP_ID_HUD   14
#define ITEM_RAM_HUD      15
#define ITEM_HUD_ORDER_MENU 16
#define ITEM_ALL_HUD_OVERLAYS_MENU 17
#define ITEM_TIMEMODE     18
#define ITEM_THEME_MENU   19
#define ITEM_THEME        20
#define ITEM_HUD_THEME    21
#define ITEM_HUD_TEXT     22
#define ITEM_HUD_SHADOW   23
#define ITEM_HUD_ICON     24
#define ITEM_CLOCK_ICON   25
#define ITEM_FPS_ICON     47
#define ITEM_CPU_ICON     42
#define ITEM_BUS_ICON     43
#define ITEM_GPU_ICON     44
#define ITEM_APP_ICON     45
#define ITEM_RAM_ICON     46
#define ITEM_HUD_BOX      26
#define ITEM_HUD_BOX_BG   27
#define ITEM_MENU_TEXT    28
#define ITEM_MENU_SELECT  29
#define ITEM_MENU_BORDER  30
#define ITEM_MENUBG       31
#define ITEM_TOP_BAR      32
#define ITEM_MENU_PICTURE_BG 33
#define ITEM_PROFILE_MENU 34
#define ITEM_PROFILE      35
#define ITEM_SAVE_PROFILE 36
#define ITEM_LOAD_PROFILE 37
#define ITEM_LANGUAGE     38
#define ITEM_AUTO_HIDE    39
#define ITEM_TOGGLE       40
#define ITEM_RESET        41
#define ITEM_MENU_HUD_SIZE_MENU 48
#define ITEM_HUD_COLORS_MENU 49
#define ITEM_MENU_COLORS_MENU 50
#define ITEM_ICON_COLORS_MENU 51
#define ITEM_PERFORMANCE_ALERTS_MENU 52
#define ITEM_FPS_WARNING 53
#define ITEM_FPS_LOW_LIMIT 54
#define ITEM_BATTERY_WARNING 55
#define ITEM_BATTERY_LOW_LIMIT 56
#define ITEM_RAM_WARNING 57
#define ITEM_RESET_HUD_POSITION 58
#define ITEM_RESET_COLORS 59
#define ITEM_RESET_OVERLAYS 60
#define ITEM_RESET_PROFILES 61
#define ITEM_RESET_ALL_DEFAULTS 62
#define ITEM_DEBUG_MENU 63
#define ITEM_DEBUG_HUD 64
#define ITEM_DEBUG_POSITION 65
#define ITEM_DEBUG_X_OFFSET 66
#define ITEM_DEBUG_Y_OFFSET 67
#define ITEM_DEBUG_SIZE 68
#define ITEM_DEBUG_FRAMEBUF 69
#define ITEM_DEBUG_CACHE 70
#define ITEM_DEBUG_INPUT 71
#define ITEM_DEBUG_SYSTEM 72
#define ITEM_TOGGLE_HUD_COMBO 73
#define ITEM_TOGGLE_MENU_COMBO 74
#define ITEM_CYCLE_PROFILE_ENABLE 75
#define ITEM_CYCLE_PROFILE_COMBO 76
#define ITEM_CYCLE_THEME_ENABLE 77
#define ITEM_CYCLE_THEME_COMBO 78
#define ITEM_HUD_OPACITY 79
#define ITEM_MENU_OPACITY 80
#define ITEM_FPS_STYLE 81
#define ITEM_BATTERY_STYLE 82
#define ITEM_CLOCK_STYLE 83
#define ITEM_HUD_MENU 84
#define ITEM_DEBUG_COLOR 85
#define ITEM_DEBUG_FONT 86
#define ITEM_DATE_HUD 87
#define ITEM_DATE_ICON 88
#define ITEM_ICON_CHANGER_MENU 89
#define ITEM_FPS_ICON_STYLE 90
#define ITEM_BATTERY_ICON_STYLE 91
#define ITEM_CLOCK_ICON_STYLE 92
#define ITEM_DATE_ICON_STYLE 93
#define ITEM_CPU_ICON_STYLE 94
#define ITEM_BUS_ICON_STYLE 95
#define ITEM_GPU_ICON_STYLE 96
#define ITEM_RAM_ICON_STYLE 97
#define ITEM_APP_ICON_STYLE 98
#define ITEM_CREATE_HUD_MENU_SIZE 99
#define ITEM_CREATE_MAIN_MENU_SIZE 100
#define ITEM_DEBUG_MENU_INFO 101
#define ITEM_DEBUG_PROFILE 102
#define ITEM_DEBUG_THEME 103
#define ITEM_DEBUG_ALERT 104
#define ITEM_DEBUG_HUD_INFO 105
#define ITEM_COUNT        106

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
static int show_date = 0;
static int show_cpu = 0;
static int show_bus = 0;
static int show_gpu = 0;
static int show_app_id = 0;
static int show_ram = 0;
static int use_24h_time = 0;
static int fps_display_style = FPS_STYLE_ICON_NUMBER;
static int battery_display_style = BATTERY_STYLE_ICON_PERCENT;
static int clock_display_style = CLOCK_STYLE_ICON_TIME;

static int fps_icon_style = ICON_STYLE_ORIGINAL;
static int battery_icon_style = ICON_STYLE_ORIGINAL;
static int clock_icon_style = ICON_STYLE_ORIGINAL;
static int date_icon_style = ICON_STYLE_ORIGINAL;
static int cpu_icon_style = ICON_STYLE_ORIGINAL;
static int bus_icon_style = ICON_STYLE_ORIGINAL;
static int gpu_icon_style = ICON_STYLE_ORIGINAL;
static int ram_icon_style = ICON_STYLE_ORIGINAL;
static int app_icon_style = ICON_STYLE_ORIGINAL;

static int debug_enabled = 0;
static int debug_show_framebuf = 1;
static int debug_show_cache = 1;
static int debug_show_input = 1;
static int debug_show_system = 1;
static int debug_show_menu_info = 1;
static int debug_show_profile = 1;
static int debug_show_theme = 1;
static int debug_show_alert = 1;
static int debug_show_hud_info = 1;
static int debug_position = POS_BOTTOM_LEFT;
static int debug_x_offset = 8;
static int debug_y_offset = 8;
static int debug_size = SIZE_MICRO;
static int debug_text_color = COLOR_WHITE;
static int debug_font_style = FONT_DEFAULT;

static volatile int cycle_worker_busy = 0;
static volatile int cycle_worker_action = 0;

static int menu_toggle_combo_mode = TOGGLE_START;
static int cycle_profile_enabled = 0;
static int cycle_profile_combo_mode = TOGGLE_SELECT_UP;
static int cycle_theme_enabled = 0;
static int cycle_theme_combo_mode = TOGGLE_SELECT_DOWN;
static int last_profile_cycle_down = 0;
static int last_theme_cycle_down = 0;

static int hud_opacity = OPACITY_100;
static int menu_opacity = OPACITY_100;
static int create_hud_menu_size = 100;
static int create_main_menu_size = 100;

static int fps_warning_enabled = 0;
static int fps_low_limit_index = 0;
static int battery_warning_enabled = 0;
static int battery_low_limit_index = 0;
static int ram_warning_enabled = 0;

static int hud_order[HUD_ORDER_COUNT] = {
    HUD_ORDER_FPS,
    HUD_ORDER_BATTERY,
    HUD_ORDER_CLOCK,
    HUD_ORDER_DATE,
    HUD_ORDER_CPU,
    HUD_ORDER_BUS,
    HUD_ORDER_GPU,
    HUD_ORDER_RAM,
    HUD_ORDER_APP_ID
};

static int hud_text_color = COLOR_WHITE;
static int hud_shadow_color = COLOR_BLACK;
static int hud_icon_color = COLOR_AUTO;
static int clock_icon_color = COLOR_AUTO;
static int date_icon_color = COLOR_AUTO;
static int fps_icon_color = COLOR_AUTO;
static int cpu_icon_color = COLOR_AUTO;
static int bus_icon_color = COLOR_AUTO;
static int gpu_icon_color = COLOR_AUTO;
static int app_icon_color = COLOR_AUTO;
static int ram_icon_color = COLOR_AUTO;
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
#define MENU_PAGE_CHOICE  3
#define MENU_PAGE_HUD_ORDER 4
#define MENU_PAGE_OVERLAYS 5
#define MENU_PAGE_SIZE    6
#define MENU_PAGE_HUD_COLORS 7
#define MENU_PAGE_MENU_COLORS 8
#define MENU_PAGE_ICON_COLORS 9
#define MENU_PAGE_PERFORMANCE 10
#define MENU_PAGE_RESET_OPTIONS 11
#define MENU_PAGE_DEBUG 12
#define MENU_PAGE_TOGGLE_COMBOS 13
#define MENU_PAGE_HUD_MENU 14
#define MENU_PAGE_ICON_CHANGER 15
#define MENU_PAGE_CREATE_HUD_SIZE 16
#define MENU_PAGE_CREATE_MAIN_SIZE 17
#define ITEM_CHOICE_BASE  1000
#define ITEM_HUD_ORDER_BASE 2000

static int menu_page = MENU_PAGE_MAIN;
static int menu_nav_dir = 0;
static SceUInt64 menu_nav_hold_start_tick = 0;
static SceUInt64 menu_nav_next_repeat_tick = 0;
static int menu_lr_dir = 0;
static SceUInt64 menu_lr_hold_start_tick = 0;
static SceUInt64 menu_lr_next_repeat_tick = 0;
static int menu_lr_step = 4;
static int choice_target_item = ITEM_THEME;
static int choice_return_page = MENU_PAGE_MAIN;
static int main_menu_size = MAIN_MENU_SIZE_DEFAULT;
static int menu_picture_bg = 0;
static int g_menu_text_mode = 0;
static int g_forced_draw_opacity = -1;

/* Remember selected rows/scroll positions when backing out of submenus. */
static int saved_main_index = 0;
static int saved_main_scroll = 0;
static int saved_profile_index = 0;
static int saved_profile_scroll = 0;
static int saved_theme_index = 0;
static int saved_theme_scroll = 0;
static int saved_hud_order_index = 0;
static int saved_hud_order_scroll = 0;
static int saved_overlays_index = 0;
static int saved_overlays_scroll = 0;
static int saved_size_index = 0;
static int saved_size_scroll = 0;
static int saved_hud_colors_index = 0;
static int saved_hud_colors_scroll = 0;
static int saved_menu_colors_index = 0;
static int saved_menu_colors_scroll = 0;
static int saved_icon_colors_index = 0;
static int saved_icon_colors_scroll = 0;
static int saved_hud_menu_index = 0;
static int saved_hud_menu_scroll = 0;
static int saved_performance_index = 0;
static int saved_performance_scroll = 0;
static int saved_reset_options_index = 0;
static int saved_reset_options_scroll = 0;
static int saved_debug_index = 0;
static int saved_debug_scroll = 0;
static int saved_toggle_index = 0;
static int saved_toggle_scroll = 0;
static int saved_icon_changer_index = 0;
static int saved_icon_changer_scroll = 0;

static int current_menu_count(void);

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
static int cached_ram_free_mb = -1;
static int active_fps_alert = 0;
static int active_battery_alert = 0;

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


static void reset_hud_order_defaults(void) {
    hud_order[0] = HUD_ORDER_FPS;
    hud_order[1] = HUD_ORDER_BATTERY;
    hud_order[2] = HUD_ORDER_CLOCK;
    hud_order[3] = HUD_ORDER_DATE;
    hud_order[4] = HUD_ORDER_CPU;
    hud_order[5] = HUD_ORDER_BUS;
    hud_order[6] = HUD_ORDER_GPU;
    hud_order[7] = HUD_ORDER_RAM;
    hud_order[8] = HUD_ORDER_APP_ID;
}

static void clamp_hud_order(void) {
    int used[HUD_ORDER_COUNT];
    int i;
    int id;

    for (i = 0; i < HUD_ORDER_COUNT; i++) {
        used[i] = 0;
    }

    for (i = 0; i < HUD_ORDER_COUNT; i++) {
        id = hud_order[i];

        if (id < 0 || id >= HUD_ORDER_COUNT || used[id]) {
            reset_hud_order_defaults();
            return;
        }

        used[id] = 1;
    }
}

static const char *hud_order_name_for(int id) {
    switch (id) {
        case HUD_ORDER_BATTERY: return "BATTERY HUD";
        case HUD_ORDER_CLOCK:   return "CLOCK HUD";
        case HUD_ORDER_DATE:    return "DATE HUD";
        case HUD_ORDER_CPU:     return "CPU";
        case HUD_ORDER_BUS:     return "BUS";
        case HUD_ORDER_GPU:     return "GPU";
        case HUD_ORDER_APP_ID:  return "APP ID";
        case HUD_ORDER_RAM:     return "RAM";
        case HUD_ORDER_FPS:     return "FPS HUD";
        default:                return "UNKNOWN";
    }
}

static void move_hud_order_item(int index, int dir) {
    int other;
    int temp;

    if (index < 0 || index >= HUD_ORDER_COUNT) {
        return;
    }

    other = index + dir;

    if (other < 0) {
        other = HUD_ORDER_COUNT - 1;
    }

    if (other >= HUD_ORDER_COUNT) {
        other = 0;
    }

    temp = hud_order[index];
    hud_order[index] = hud_order[other];
    hud_order[other] = temp;
    menu_index = other;
}

static int fps_low_limit_value(void) {
    switch (fps_low_limit_index) {
        case 1: return 45;
        case 2: return 50;
        case 3: return 20;
        case 4: return 15;
        case 5: return 10;
        case 0:
        default: return 30;
    }
}

static int battery_low_limit_value(void) {
    switch (battery_low_limit_index) {
        case 1: return 15;
        case 2: return 10;
        case 0:
        default: return 20;
    }
}

static const char *fps_low_limit_name(void) {
    switch (fps_low_limit_index) {
        case 1: return "45";
        case 2: return "50";
        case 3: return "20";
        case 4: return "15";
        case 5: return "10";
        case 0:
        default: return "30";
    }
}

static const char *battery_low_limit_name(void) {
    switch (battery_low_limit_index) {
        case 1: return "15";
        case 2: return "10";
        case 0:
        default: return "20";
    }
}

static void reset_hud_position_defaults(void) {
    hud_position = POS_BOTTOM_RIGHT;
    hud_x_offset = 8;
    hud_y_offset = 8;
    reset_message_frames = 180;
}

static void reset_color_defaults(void) {
    theme_id = THEME_DEFAULT;
    hud_theme_id = HUD_THEME_DEFAULT;
    hud_text_color = COLOR_WHITE;
    hud_shadow_color = COLOR_BLACK;
    hud_icon_color = COLOR_AUTO;
    clock_icon_color = COLOR_AUTO;
    date_icon_color = COLOR_AUTO;
    fps_icon_color = COLOR_AUTO;
    cpu_icon_color = COLOR_AUTO;
    bus_icon_color = COLOR_AUTO;
    gpu_icon_color = COLOR_AUTO;
    app_icon_color = COLOR_AUTO;
    ram_icon_color = COLOR_AUTO;
    menu_text_color = COLOR_WHITE;
    menu_select_color = COLOR_YELLOW;
    menu_border_color = COLOR_WHITE;
    menu_bg_color = BG_BLACK;
    top_menu_bar_color = COLOR_BLACK;
    menu_picture_bg = 0;
    hud_box_enabled = 0;
    hud_box_bg_color = BG_BLACK;
    reset_message_frames = 180;
}

static void reset_overlay_defaults(void) {
    show_fps = 1;
    show_battery = 1;
    show_time = 1;
    show_date = 0;
    show_cpu = 0;
    show_bus = 0;
    show_gpu = 0;
    show_app_id = 0;
    show_ram = 0;
    reset_hud_order_defaults();
    reset_message_frames = 180;
}

static void reset_profile_files(void) {
    sceIoRemove("ur0:data/VitaHUD/profile1.txt");
    sceIoRemove("ur0:data/VitaHUD/profile2.txt");
    sceIoRemove("ur0:data/VitaHUD/profile3.txt");
    profile_id = PROFILE_1;
    reset_message_frames = 180;
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
    main_menu_size = MAIN_MENU_SIZE_DEFAULT;
    font_style = FONT_DEFAULT;

    show_fps = 1;
    show_battery = 1;
    show_time = 1;
    show_date = 0;
    show_cpu = 0;
    show_bus = 0;
    show_gpu = 0;
    show_app_id = 0;
    show_ram = 0;
    use_24h_time = 0;
    fps_display_style = FPS_STYLE_ICON_NUMBER;
    battery_display_style = BATTERY_STYLE_ICON_PERCENT;
    clock_display_style = CLOCK_STYLE_ICON_TIME;
    fps_icon_style = ICON_STYLE_ORIGINAL;
    battery_icon_style = ICON_STYLE_ORIGINAL;
    clock_icon_style = ICON_STYLE_ORIGINAL;
    date_icon_style = ICON_STYLE_ORIGINAL;
    cpu_icon_style = ICON_STYLE_ORIGINAL;
    bus_icon_style = ICON_STYLE_ORIGINAL;
    gpu_icon_style = ICON_STYLE_ORIGINAL;
    ram_icon_style = ICON_STYLE_ORIGINAL;
    app_icon_style = ICON_STYLE_ORIGINAL;
    fps_warning_enabled = 0;
    fps_low_limit_index = 0;
    battery_warning_enabled = 0;
    battery_low_limit_index = 0;
    ram_warning_enabled = 0;
    reset_hud_order_defaults();

    hud_text_color = COLOR_WHITE;
    hud_shadow_color = COLOR_BLACK;
    hud_icon_color = COLOR_AUTO;
    clock_icon_color = COLOR_AUTO;
    date_icon_color = COLOR_AUTO;
    fps_icon_color = COLOR_AUTO;
    cpu_icon_color = COLOR_AUTO;
    bus_icon_color = COLOR_AUTO;
    gpu_icon_color = COLOR_AUTO;
    app_icon_color = COLOR_AUTO;
    ram_icon_color = COLOR_AUTO;
    menu_text_color = COLOR_WHITE;
    menu_select_color = COLOR_YELLOW;
    menu_border_color = COLOR_WHITE;
    menu_bg_color = BG_BLACK;
    top_menu_bar_color = COLOR_BLACK;
    menu_picture_bg = 0;
    hud_box_enabled = 0;
    hud_box_bg_color = BG_BLACK;

    hud_language = LANG_EN;
    auto_hide_mode = AUTO_HIDE_OFF;
    toggle_combo_mode = TOGGLE_SELECT;
    menu_toggle_combo_mode = TOGGLE_START;
    cycle_profile_enabled = 0;
    cycle_profile_combo_mode = TOGGLE_SELECT_UP;
    cycle_theme_enabled = 0;
    cycle_theme_combo_mode = TOGGLE_SELECT_DOWN;
    hud_opacity = OPACITY_100;
    menu_opacity = OPACITY_100;
    create_hud_menu_size = 100;
    create_main_menu_size = 100;
    debug_enabled = 0;
    debug_show_framebuf = 1;
    debug_show_cache = 1;
    debug_show_input = 1;
    debug_show_system = 1;
    debug_show_menu_info = 1;
    debug_show_profile = 1;
    debug_show_theme = 1;
    debug_show_alert = 1;
    debug_show_hud_info = 1;
    debug_position = POS_BOTTOM_LEFT;
    debug_x_offset = 8;
    debug_y_offset = 8;
    debug_size = SIZE_MICRO;
    debug_text_color = COLOR_WHITE;
    debug_font_style = FONT_DEFAULT;
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
    if (main_menu_size < 0 || main_menu_size >= MAIN_MENU_SIZE_COUNT) main_menu_size = MAIN_MENU_SIZE_DEFAULT;
    if (font_style < 0 || font_style >= FONT_COUNT) font_style = FONT_DEFAULT;

    if (show_fps < 0 || show_fps > 1) show_fps = 1;
    if (show_battery < 0 || show_battery > 1) show_battery = 1;
    if (show_time < 0 || show_time > 1) show_time = 1;
    if (show_date < 0 || show_date > 1) show_date = 0;
    if (show_cpu < 0 || show_cpu > 1) show_cpu = 0;
    if (show_bus < 0 || show_bus > 1) show_bus = 0;
    if (show_gpu < 0 || show_gpu > 1) show_gpu = 0;
    if (show_app_id < 0 || show_app_id > 1) show_app_id = 0;
    if (show_ram < 0 || show_ram > 1) show_ram = 0;
    if (use_24h_time < 0 || use_24h_time > 1) use_24h_time = 0;
    if (fps_display_style < 0 || fps_display_style >= FPS_STYLE_COUNT) fps_display_style = FPS_STYLE_ICON_NUMBER;
    if (battery_display_style < 0 || battery_display_style >= BATTERY_STYLE_COUNT) battery_display_style = BATTERY_STYLE_ICON_PERCENT;
    if (clock_display_style < 0 || clock_display_style >= CLOCK_STYLE_COUNT) clock_display_style = CLOCK_STYLE_ICON_TIME;
    if (fps_icon_style < 0 || fps_icon_style >= ICON_STYLE_COUNT) fps_icon_style = ICON_STYLE_ORIGINAL;
    if (battery_icon_style < 0 || battery_icon_style >= ICON_STYLE_COUNT) battery_icon_style = ICON_STYLE_ORIGINAL;
    if (clock_icon_style < 0 || clock_icon_style >= ICON_STYLE_COUNT) clock_icon_style = ICON_STYLE_ORIGINAL;
    if (date_icon_style < 0 || date_icon_style >= ICON_STYLE_COUNT) date_icon_style = ICON_STYLE_ORIGINAL;
    if (cpu_icon_style < 0 || cpu_icon_style >= ICON_STYLE_COUNT) cpu_icon_style = ICON_STYLE_ORIGINAL;
    if (bus_icon_style < 0 || bus_icon_style >= ICON_STYLE_COUNT) bus_icon_style = ICON_STYLE_ORIGINAL;
    if (gpu_icon_style < 0 || gpu_icon_style >= ICON_STYLE_COUNT) gpu_icon_style = ICON_STYLE_ORIGINAL;
    if (ram_icon_style < 0 || ram_icon_style >= ICON_STYLE_COUNT) ram_icon_style = ICON_STYLE_ORIGINAL;
    if (app_icon_style < 0 || app_icon_style >= ICON_STYLE_COUNT) app_icon_style = ICON_STYLE_ORIGINAL;
    if (debug_enabled < 0 || debug_enabled > 1) debug_enabled = 0;
    if (debug_show_framebuf < 0 || debug_show_framebuf > 1) debug_show_framebuf = 1;
    if (debug_show_cache < 0 || debug_show_cache > 1) debug_show_cache = 1;
    if (debug_show_input < 0 || debug_show_input > 1) debug_show_input = 1;
    if (debug_show_system < 0 || debug_show_system > 1) debug_show_system = 1;
    if (debug_show_menu_info < 0 || debug_show_menu_info > 1) debug_show_menu_info = 1;
    if (debug_show_profile < 0 || debug_show_profile > 1) debug_show_profile = 1;
    if (debug_show_theme < 0 || debug_show_theme > 1) debug_show_theme = 1;
    if (debug_show_alert < 0 || debug_show_alert > 1) debug_show_alert = 1;
    if (debug_show_hud_info < 0 || debug_show_hud_info > 1) debug_show_hud_info = 1;
    if (debug_position < 0 || debug_position >= POS_COUNT) debug_position = POS_BOTTOM_LEFT;
    if (debug_x_offset < 0) debug_x_offset = 0;
    if (debug_x_offset > 960) debug_x_offset = 960;
    if (debug_y_offset < 0) debug_y_offset = 0;
    if (debug_y_offset > 544) debug_y_offset = 544;
    if (debug_size < 0 || debug_size >= SIZE_COUNT) debug_size = SIZE_MICRO;
    if (debug_text_color < 0 || debug_text_color >= COLOR_COUNT) debug_text_color = COLOR_WHITE;
    if (debug_font_style < 0 || debug_font_style >= FONT_COUNT) debug_font_style = FONT_DEFAULT;
    if (hud_opacity < 0 || hud_opacity >= OPACITY_COUNT) hud_opacity = OPACITY_100;
    if (menu_opacity < 0 || menu_opacity >= OPACITY_COUNT) menu_opacity = OPACITY_100;
    if (create_hud_menu_size < 10) create_hud_menu_size = 10;
    if (create_hud_menu_size > 150) create_hud_menu_size = 150;
    if (create_main_menu_size < 10) create_main_menu_size = 10;
    if (create_main_menu_size > 150) create_main_menu_size = 150;
    if (cycle_profile_enabled < 0 || cycle_profile_enabled > 1) cycle_profile_enabled = 0;
    if (cycle_theme_enabled < 0 || cycle_theme_enabled > 1) cycle_theme_enabled = 0;
    if (fps_warning_enabled < 0 || fps_warning_enabled > 1) fps_warning_enabled = 0;
    if (fps_low_limit_index < 0 || fps_low_limit_index > 5) fps_low_limit_index = 0;
    if (battery_warning_enabled < 0 || battery_warning_enabled > 1) battery_warning_enabled = 0;
    if (battery_low_limit_index < 0 || battery_low_limit_index > 2) battery_low_limit_index = 0;
    if (ram_warning_enabled < 0 || ram_warning_enabled > 1) ram_warning_enabled = 0;
    clamp_hud_order();

    if (hud_text_color < 0 || hud_text_color >= COLOR_COUNT) hud_text_color = COLOR_WHITE;
    if (hud_shadow_color < 0 || hud_shadow_color >= COLOR_COUNT) hud_shadow_color = COLOR_BLACK;
    if (hud_icon_color < 0 || hud_icon_color >= COLOR_COUNT) hud_icon_color = COLOR_AUTO;
    if (clock_icon_color < 0 || clock_icon_color >= COLOR_COUNT) clock_icon_color = COLOR_AUTO;
    if (date_icon_color < 0 || date_icon_color >= COLOR_COUNT) date_icon_color = COLOR_AUTO;
    if (fps_icon_color < 0 || fps_icon_color >= COLOR_COUNT) fps_icon_color = COLOR_AUTO;
    if (cpu_icon_color < 0 || cpu_icon_color >= COLOR_COUNT) cpu_icon_color = COLOR_AUTO;
    if (bus_icon_color < 0 || bus_icon_color >= COLOR_COUNT) bus_icon_color = COLOR_AUTO;
    if (gpu_icon_color < 0 || gpu_icon_color >= COLOR_COUNT) gpu_icon_color = COLOR_AUTO;
    if (app_icon_color < 0 || app_icon_color >= COLOR_COUNT) app_icon_color = COLOR_AUTO;
    if (ram_icon_color < 0 || ram_icon_color >= COLOR_COUNT) ram_icon_color = COLOR_AUTO;
    if (menu_text_color < 0 || menu_text_color >= COLOR_COUNT) menu_text_color = COLOR_WHITE;
    if (menu_select_color < 0 || menu_select_color >= COLOR_COUNT) menu_select_color = COLOR_YELLOW;
    if (menu_border_color < 0 || menu_border_color >= COLOR_COUNT) menu_border_color = COLOR_WHITE;
    if (menu_bg_color < 0 || menu_bg_color >= BG_COUNT) menu_bg_color = BG_BLACK;
    if (top_menu_bar_color < 1 || top_menu_bar_color >= COLOR_COUNT) top_menu_bar_color = COLOR_BLACK;
    if (hud_box_enabled < 0 || hud_box_enabled > 1) hud_box_enabled = 0;
    if (hud_box_bg_color < 0 || hud_box_bg_color >= BG_COUNT) hud_box_bg_color = BG_BLACK;

    if (hud_language < 0 || hud_language >= LANG_COUNT) hud_language = LANG_EN;
    /* Auto Hide removed from the menu and forced off. */
    auto_hide_mode = AUTO_HIDE_OFF;
    if (toggle_combo_mode < 0 || toggle_combo_mode >= TOGGLE_COUNT) toggle_combo_mode = TOGGLE_SELECT;
    if (menu_toggle_combo_mode < 0 || menu_toggle_combo_mode >= TOGGLE_COUNT) menu_toggle_combo_mode = TOGGLE_START;
    if (cycle_profile_combo_mode < 0 || cycle_profile_combo_mode >= TOGGLE_COUNT) cycle_profile_combo_mode = TOGGLE_SELECT_UP;
    if (cycle_theme_combo_mode < 0 || cycle_theme_combo_mode >= TOGGLE_COUNT) cycle_theme_combo_mode = TOGGLE_SELECT_DOWN;
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
    write_config_line(fd, "profile_full_save_version", 2);
    write_config_line(fd, "hud_enabled", hud_enabled);
    write_config_line(fd, "layout", hud_layout);
    write_config_line(fd, "position", hud_position);
    write_config_line(fd, "x_offset", hud_x_offset);
    write_config_line(fd, "y_offset", hud_y_offset);
    write_config_line(fd, "size", hud_size);
    write_config_line(fd, "main_menu_size", main_menu_size);
    write_config_line(fd, "font", font_style);

    write_config_line(fd, "show_fps", show_fps);
    write_config_line(fd, "show_battery", show_battery);
    write_config_line(fd, "show_time", show_time);
    write_config_line(fd, "show_date", show_date);
    write_config_line(fd, "show_cpu", show_cpu);
    write_config_line(fd, "show_bus", show_bus);
    write_config_line(fd, "show_gpu", show_gpu);
    write_config_line(fd, "show_app_id", show_app_id);
    write_config_line(fd, "show_ram", show_ram);
    write_config_line(fd, "time_24h", use_24h_time);
    write_config_line(fd, "fps_style", fps_display_style);
    write_config_line(fd, "battery_style", battery_display_style);
    write_config_line(fd, "clock_style", clock_display_style);
    write_config_line(fd, "fps_icon_style", fps_icon_style);
    write_config_line(fd, "battery_icon_style", battery_icon_style);
    write_config_line(fd, "clock_icon_style", clock_icon_style);
    write_config_line(fd, "date_icon_style", date_icon_style);
    write_config_line(fd, "cpu_icon_style", cpu_icon_style);
    write_config_line(fd, "bus_icon_style", bus_icon_style);
    write_config_line(fd, "gpu_icon_style", gpu_icon_style);
    write_config_line(fd, "ram_icon_style", ram_icon_style);
    write_config_line(fd, "app_icon_style", app_icon_style);
    write_config_line(fd, "debug_enabled", debug_enabled);
    write_config_line(fd, "debug_framebuf", debug_show_framebuf);
    write_config_line(fd, "debug_cache", debug_show_cache);
    write_config_line(fd, "debug_input", debug_show_input);
    write_config_line(fd, "debug_system", debug_show_system);
    write_config_line(fd, "debug_menu_info", debug_show_menu_info);
    write_config_line(fd, "debug_profile", debug_show_profile);
    write_config_line(fd, "debug_theme", debug_show_theme);
    write_config_line(fd, "debug_alert", debug_show_alert);
    write_config_line(fd, "debug_hud_info", debug_show_hud_info);
    write_config_line(fd, "debug_position", debug_position);
    write_config_line(fd, "debug_x_offset", debug_x_offset);
    write_config_line(fd, "debug_y_offset", debug_y_offset);
    write_config_line(fd, "debug_size", debug_size);
    write_config_line(fd, "debug_text_color", debug_text_color);
    write_config_line(fd, "debug_font", debug_font_style);
    write_config_line(fd, "hud_opacity", hud_opacity);
    write_config_line(fd, "menu_opacity", menu_opacity);
    write_config_line(fd, "create_hud_menu_size", create_hud_menu_size);
    write_config_line(fd, "create_main_menu_size", create_main_menu_size);
    write_config_line(fd, "menu_combo", menu_toggle_combo_mode);
    write_config_line(fd, "cycle_profile_enabled", cycle_profile_enabled);
    write_config_line(fd, "cycle_profile_combo", cycle_profile_combo_mode);
    write_config_line(fd, "cycle_theme_enabled", cycle_theme_enabled);
    write_config_line(fd, "cycle_theme_combo", cycle_theme_combo_mode);
    write_config_line(fd, "fps_warning", fps_warning_enabled);
    write_config_line(fd, "fps_low_limit", fps_low_limit_index);
    write_config_line(fd, "battery_warning", battery_warning_enabled);
    write_config_line(fd, "battery_low_limit", battery_low_limit_index);
    write_config_line(fd, "ram_warning", ram_warning_enabled);
    write_config_line(fd, "hud_order0", hud_order[0]);
    write_config_line(fd, "hud_order1", hud_order[1]);
    write_config_line(fd, "hud_order2", hud_order[2]);
    write_config_line(fd, "hud_order3", hud_order[3]);
    write_config_line(fd, "hud_order4", hud_order[4]);
    write_config_line(fd, "hud_order5", hud_order[5]);
    write_config_line(fd, "hud_order6", hud_order[6]);
    write_config_line(fd, "hud_order7", hud_order[7]);
    write_config_line(fd, "hud_order8", hud_order[8]);

    write_config_line(fd, "hud_text_color", hud_text_color);
    write_config_line(fd, "hud_shadow_color", hud_shadow_color);
    write_config_line(fd, "hud_icon_color", hud_icon_color);
    write_config_line(fd, "clock_icon_color", clock_icon_color);
    write_config_line(fd, "date_icon_color", date_icon_color);
    write_config_line(fd, "fps_icon_color", fps_icon_color);
    write_config_line(fd, "cpu_icon_color", cpu_icon_color);
    write_config_line(fd, "bus_icon_color", bus_icon_color);
    write_config_line(fd, "gpu_icon_color", gpu_icon_color);
    write_config_line(fd, "app_icon_color", app_icon_color);
    write_config_line(fd, "ram_icon_color", ram_icon_color);
    write_config_line(fd, "menu_text_color", menu_text_color);
    write_config_line(fd, "menu_select_color", menu_select_color);
    write_config_line(fd, "menu_border_color", menu_border_color);
    write_config_line(fd, "menu_bg", menu_bg_color);
    write_config_line(fd, "top_menu_bar", top_menu_bar_color);
    write_config_line(fd, "menu_picture_bg", menu_picture_bg);
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
    main_menu_size = get_config_int(buf, "main_menu_size", main_menu_size);
    font_style = get_config_int(buf, "font", font_style);

    show_fps = get_config_int(buf, "show_fps", show_fps);
    show_battery = get_config_int(buf, "show_battery", show_battery);
    show_time = get_config_int(buf, "show_time", show_time);
    show_date = get_config_int(buf, "show_date", show_date);
    show_cpu = get_config_int(buf, "show_cpu", show_cpu);
    show_bus = get_config_int(buf, "show_bus", show_bus);
    show_gpu = get_config_int(buf, "show_gpu", show_gpu);
    show_app_id = get_config_int(buf, "show_app_id", show_app_id);
    show_ram = get_config_int(buf, "show_ram", show_ram);
    use_24h_time = get_config_int(buf, "time_24h", use_24h_time);
    fps_display_style = get_config_int(buf, "fps_style", fps_display_style);
    battery_display_style = get_config_int(buf, "battery_style", battery_display_style);
    clock_display_style = get_config_int(buf, "clock_style", clock_display_style);
    fps_icon_style = get_config_int(buf, "fps_icon_style", fps_icon_style);
    battery_icon_style = get_config_int(buf, "battery_icon_style", battery_icon_style);
    clock_icon_style = get_config_int(buf, "clock_icon_style", clock_icon_style);
    date_icon_style = get_config_int(buf, "date_icon_style", date_icon_style);
    cpu_icon_style = get_config_int(buf, "cpu_icon_style", cpu_icon_style);
    bus_icon_style = get_config_int(buf, "bus_icon_style", bus_icon_style);
    gpu_icon_style = get_config_int(buf, "gpu_icon_style", gpu_icon_style);
    ram_icon_style = get_config_int(buf, "ram_icon_style", ram_icon_style);
    app_icon_style = get_config_int(buf, "app_icon_style", app_icon_style);
    debug_enabled = get_config_int(buf, "debug_enabled", debug_enabled);
    debug_show_framebuf = get_config_int(buf, "debug_framebuf", debug_show_framebuf);
    debug_show_cache = get_config_int(buf, "debug_cache", debug_show_cache);
    debug_show_input = get_config_int(buf, "debug_input", debug_show_input);
    debug_show_system = get_config_int(buf, "debug_system", debug_show_system);
    debug_show_menu_info = get_config_int(buf, "debug_menu_info", debug_show_menu_info);
    debug_show_profile = get_config_int(buf, "debug_profile", debug_show_profile);
    debug_show_theme = get_config_int(buf, "debug_theme", debug_show_theme);
    debug_show_alert = get_config_int(buf, "debug_alert", debug_show_alert);
    debug_show_hud_info = get_config_int(buf, "debug_hud_info", debug_show_hud_info);
    debug_position = get_config_int(buf, "debug_position", debug_position);
    debug_x_offset = get_config_int(buf, "debug_x_offset", debug_x_offset);
    debug_y_offset = get_config_int(buf, "debug_y_offset", debug_y_offset);
    debug_size = get_config_int(buf, "debug_size", debug_size);
    debug_text_color = get_config_int(buf, "debug_text_color", debug_text_color);
    debug_font_style = get_config_int(buf, "debug_font", debug_font_style);
    hud_opacity = get_config_int(buf, "hud_opacity", hud_opacity);
    menu_opacity = get_config_int(buf, "menu_opacity", menu_opacity);
    create_hud_menu_size = get_config_int(buf, "create_hud_menu_size", create_hud_menu_size);
    create_main_menu_size = get_config_int(buf, "create_main_menu_size", create_main_menu_size);
    menu_toggle_combo_mode = get_config_int(buf, "menu_combo", menu_toggle_combo_mode);
    cycle_profile_enabled = get_config_int(buf, "cycle_profile_enabled", cycle_profile_enabled);
    cycle_profile_combo_mode = get_config_int(buf, "cycle_profile_combo", cycle_profile_combo_mode);
    cycle_theme_enabled = get_config_int(buf, "cycle_theme_enabled", cycle_theme_enabled);
    cycle_theme_combo_mode = get_config_int(buf, "cycle_theme_combo", cycle_theme_combo_mode);
    fps_warning_enabled = get_config_int(buf, "fps_warning", fps_warning_enabled);
    fps_low_limit_index = get_config_int(buf, "fps_low_limit", fps_low_limit_index);
    battery_warning_enabled = get_config_int(buf, "battery_warning", battery_warning_enabled);
    battery_low_limit_index = get_config_int(buf, "battery_low_limit", battery_low_limit_index);
    ram_warning_enabled = get_config_int(buf, "ram_warning", ram_warning_enabled);
    hud_order[0] = get_config_int(buf, "hud_order0", hud_order[0]);
    hud_order[1] = get_config_int(buf, "hud_order1", hud_order[1]);
    hud_order[2] = get_config_int(buf, "hud_order2", hud_order[2]);
    hud_order[3] = get_config_int(buf, "hud_order3", hud_order[3]);
    hud_order[4] = get_config_int(buf, "hud_order4", hud_order[4]);
    hud_order[5] = get_config_int(buf, "hud_order5", hud_order[5]);
    hud_order[6] = get_config_int(buf, "hud_order6", hud_order[6]);
    hud_order[7] = get_config_int(buf, "hud_order7", hud_order[7]);
    hud_order[8] = get_config_int(buf, "hud_order8", hud_order[8]);

    hud_text_color = get_config_int(buf, "hud_text_color", hud_text_color);
    hud_shadow_color = get_config_int(buf, "hud_shadow_color", hud_shadow_color);
    hud_icon_color = get_config_int(buf, "hud_icon_color", hud_icon_color);
    clock_icon_color = get_config_int(buf, "clock_icon_color", clock_icon_color);
    date_icon_color = get_config_int(buf, "date_icon_color", date_icon_color);
    fps_icon_color = get_config_int(buf, "fps_icon_color", fps_icon_color);
    cpu_icon_color = get_config_int(buf, "cpu_icon_color", cpu_icon_color);
    bus_icon_color = get_config_int(buf, "bus_icon_color", bus_icon_color);
    gpu_icon_color = get_config_int(buf, "gpu_icon_color", gpu_icon_color);
    app_icon_color = get_config_int(buf, "app_icon_color", app_icon_color);
    ram_icon_color = get_config_int(buf, "ram_icon_color", ram_icon_color);
    menu_text_color = get_config_int(buf, "menu_text_color", menu_text_color);
    menu_select_color = get_config_int(buf, "menu_select_color", menu_select_color);
    menu_border_color = get_config_int(buf, "menu_border_color", menu_border_color);
    menu_bg_color = get_config_int(buf, "menu_bg", menu_bg_color);
    top_menu_bar_color = get_config_int(buf, "top_menu_bar", top_menu_bar_color);
    menu_picture_bg = get_config_int(buf, "menu_picture_bg", menu_picture_bg);
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
     * HUD, layout, offsets, size scaling, display styles, opacity, font,
     * debug options, alerts, overlay order, icons/icon styles, colors,
     * menu background, language, combos, theme, and profile id.
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
            fps_icon_color = COLOR_AUTO;
            clock_icon_color = COLOR_AUTO;
            menu_text_color = COLOR_WHITE;
            menu_select_color = COLOR_WHITE;
            menu_border_color = COLOR_WHITE;
            menu_bg_color = BG_TRANSPARENT;
            top_menu_bar_color = COLOR_BLACK;
            hud_box_bg_color = BG_TRANSPARENT;
            break;

        case THEME_PLASMA:
            hud_text_color = COLOR_MAGENTA;
            hud_shadow_color = COLOR_BLACK;
            hud_icon_color = COLOR_CYAN;
            fps_icon_color = COLOR_ELECTRIC_BLUE;
            clock_icon_color = COLOR_MAGENTA;
            menu_text_color = COLOR_MAGENTA;
            menu_select_color = COLOR_CYAN;
            menu_border_color = COLOR_ELECTRIC_BLUE;
            menu_bg_color = BG_PURPLE;
            top_menu_bar_color = COLOR_VIOLET;
            hud_box_bg_color = BG_PURPLE;
            break;

        case THEME_CYBER:
            hud_text_color = COLOR_AQUA;
            hud_shadow_color = COLOR_BLACK;
            hud_icon_color = COLOR_ELECTRIC_BLUE;
            fps_icon_color = COLOR_NEON_GREEN;
            clock_icon_color = COLOR_AQUA;
            menu_text_color = COLOR_AQUA;
            menu_select_color = COLOR_NEON_GREEN;
            menu_border_color = COLOR_ELECTRIC_BLUE;
            menu_bg_color = BG_BLACK;
            top_menu_bar_color = COLOR_MIDNIGHT;
            hud_box_bg_color = BG_BLACK;
            break;

        case THEME_SUNSET:
            hud_text_color = COLOR_ORANGE;
            hud_shadow_color = COLOR_BLACK;
            hud_icon_color = COLOR_GOLD;
            fps_icon_color = COLOR_CRIMSON;
            clock_icon_color = COLOR_ORANGE;
            menu_text_color = COLOR_ORANGE;
            menu_select_color = COLOR_GOLD;
            menu_border_color = COLOR_CRIMSON;
            menu_bg_color = BG_MAROON;
            top_menu_bar_color = COLOR_MAROON;
            hud_box_bg_color = BG_MAROON;
            break;

        case THEME_OCEAN:
            hud_text_color = COLOR_SKY;
            hud_shadow_color = COLOR_BLACK;
            hud_icon_color = COLOR_AQUA;
            fps_icon_color = COLOR_CYAN;
            clock_icon_color = COLOR_SKY;
            menu_text_color = COLOR_SKY;
            menu_select_color = COLOR_WHITE;
            menu_border_color = COLOR_AQUA;
            menu_bg_color = BG_NAVY;
            top_menu_bar_color = COLOR_NAVY;
            hud_box_bg_color = BG_NAVY;
            break;

        case THEME_FOREST:
            hud_text_color = COLOR_LIME;
            hud_shadow_color = COLOR_BLACK;
            hud_icon_color = COLOR_FOREST;
            fps_icon_color = COLOR_EMERALD;
            clock_icon_color = COLOR_MINT;
            menu_text_color = COLOR_LIME;
            menu_select_color = COLOR_MINT;
            menu_border_color = COLOR_FOREST;
            menu_bg_color = BG_FOREST;
            top_menu_bar_color = COLOR_FOREST;
            hud_box_bg_color = BG_FOREST;
            break;

        case THEME_ROYAL:
            hud_text_color = COLOR_LAVENDER;
            hud_shadow_color = COLOR_BLACK;
            hud_icon_color = COLOR_GOLD;
            fps_icon_color = COLOR_VIOLET;
            clock_icon_color = COLOR_GOLD;
            menu_text_color = COLOR_LAVENDER;
            menu_select_color = COLOR_GOLD;
            menu_border_color = COLOR_PURPLE;
            menu_bg_color = BG_PURPLE;
            top_menu_bar_color = COLOR_PURPLE;
            hud_box_bg_color = BG_PURPLE;
            break;

        case THEME_CANDY:
            hud_text_color = COLOR_HOT_PINK;
            hud_shadow_color = COLOR_BLACK;
            hud_icon_color = COLOR_ROSE;
            fps_icon_color = COLOR_CYAN;
            clock_icon_color = COLOR_HOT_PINK;
            menu_text_color = COLOR_HOT_PINK;
            menu_select_color = COLOR_CREAM;
            menu_border_color = COLOR_ROSE;
            menu_bg_color = BG_MAGENTA;
            top_menu_bar_color = COLOR_ROSE;
            hud_box_bg_color = BG_MAGENTA;
            break;

        case THEME_STEEL:
            hud_text_color = COLOR_SILVER;
            hud_shadow_color = COLOR_BLACK;
            hud_icon_color = COLOR_SLATE;
            fps_icon_color = COLOR_CYAN;
            clock_icon_color = COLOR_SILVER;
            menu_text_color = COLOR_SILVER;
            menu_select_color = COLOR_WHITE;
            menu_border_color = COLOR_SLATE;
            menu_bg_color = BG_SLATE;
            top_menu_bar_color = COLOR_DARK_GRAY;
            hud_box_bg_color = BG_SLATE;
            break;

        case THEME_FIRE:
            hud_text_color = COLOR_RED;
            hud_shadow_color = COLOR_BLACK;
            hud_icon_color = COLOR_ORANGE;
            fps_icon_color = COLOR_YELLOW;
            clock_icon_color = COLOR_RED;
            menu_text_color = COLOR_ORANGE;
            menu_select_color = COLOR_YELLOW;
            menu_border_color = COLOR_RED;
            menu_bg_color = BG_RED;
            top_menu_bar_color = COLOR_CRIMSON;
            hud_box_bg_color = BG_RED;
            break;

        case THEME_GHOST:
            hud_text_color = COLOR_SNOW;
            hud_shadow_color = COLOR_DARK_GRAY;
            hud_icon_color = COLOR_SILVER;
            fps_icon_color = COLOR_SNOW;
            clock_icon_color = COLOR_SILVER;
            menu_text_color = COLOR_SNOW;
            menu_select_color = COLOR_CYAN;
            menu_border_color = COLOR_SILVER;
            menu_bg_color = BG_TRANSPARENT;
            top_menu_bar_color = COLOR_DARK_GRAY;
            hud_box_bg_color = BG_TRANSPARENT;
            break;

        case THEME_VOID:
            hud_text_color = COLOR_VIOLET;
            hud_shadow_color = COLOR_BLACK;
            hud_icon_color = COLOR_PURPLE;
            fps_icon_color = COLOR_ELECTRIC_BLUE;
            clock_icon_color = COLOR_LAVENDER;
            menu_text_color = COLOR_LAVENDER;
            menu_select_color = COLOR_ELECTRIC_BLUE;
            menu_border_color = COLOR_VIOLET;
            menu_bg_color = BG_BLACK;
            top_menu_bar_color = COLOR_MIDNIGHT;
            hud_box_bg_color = BG_BLACK;
            break;

        case THEME_AURORA:
            hud_text_color = COLOR_MINT;
            hud_shadow_color = COLOR_BLACK;
            hud_icon_color = COLOR_AQUA;
            fps_icon_color = COLOR_ROSE;
            clock_icon_color = COLOR_SKY;
            menu_text_color = COLOR_MINT;
            menu_select_color = COLOR_ROSE;
            menu_border_color = COLOR_AQUA;
            menu_bg_color = BG_TEAL;
            top_menu_bar_color = COLOR_TURQUOISE;
            hud_box_bg_color = BG_TEAL;
            break;

        case THEME_OBSIDIAN:
            hud_text_color = COLOR_SILVER;
            hud_shadow_color = COLOR_BLACK;
            hud_icon_color = COLOR_DARK_GRAY;
            fps_icon_color = COLOR_CRIMSON;
            clock_icon_color = COLOR_SILVER;
            menu_text_color = COLOR_SILVER;
            menu_select_color = COLOR_CRIMSON;
            menu_border_color = COLOR_DARK_GRAY;
            menu_bg_color = BG_BLACK;
            top_menu_bar_color = COLOR_BLACK;
            hud_box_bg_color = BG_BLACK;
            break;

        case THEME_NOVA:
            hud_text_color = COLOR_GOLD;
            hud_shadow_color = COLOR_BLACK;
            hud_icon_color = COLOR_ORANGE;
            fps_icon_color = COLOR_ELECTRIC_BLUE;
            clock_icon_color = COLOR_GOLD;
            menu_text_color = COLOR_GOLD;
            menu_select_color = COLOR_ELECTRIC_BLUE;
            menu_border_color = COLOR_ORANGE;
            menu_bg_color = BG_NAVY;
            top_menu_bar_color = COLOR_NAVY;
            hud_box_bg_color = BG_NAVY;
            break;

        case THEME_DRAGON:
            hud_text_color = COLOR_CRIMSON;
            hud_shadow_color = COLOR_BLACK;
            hud_icon_color = COLOR_GOLD;
            fps_icon_color = COLOR_ORANGE;
            clock_icon_color = COLOR_AMBER;
            menu_text_color = COLOR_CRIMSON;
            menu_select_color = COLOR_GOLD;
            menu_border_color = COLOR_ORANGE;
            menu_bg_color = BG_MAROON;
            top_menu_bar_color = COLOR_MAROON;
            hud_box_bg_color = BG_MAROON;
            break;

        case THEME_COSMIC:
            hud_text_color = COLOR_LAVENDER;
            hud_shadow_color = COLOR_BLACK;
            hud_icon_color = COLOR_MAGENTA;
            fps_icon_color = COLOR_CYAN;
            clock_icon_color = COLOR_VIOLET;
            menu_text_color = COLOR_LAVENDER;
            menu_select_color = COLOR_CYAN;
            menu_border_color = COLOR_MAGENTA;
            menu_bg_color = BG_PURPLE;
            top_menu_bar_color = COLOR_PURPLE;
            hud_box_bg_color = BG_PURPLE;
            break;

        case THEME_TOXIC:
            hud_text_color = COLOR_NEON_GREEN;
            hud_shadow_color = COLOR_BLACK;
            hud_icon_color = COLOR_LIME;
            fps_icon_color = COLOR_YELLOW;
            clock_icon_color = COLOR_NEON_GREEN;
            menu_text_color = COLOR_NEON_GREEN;
            menu_select_color = COLOR_YELLOW;
            menu_border_color = COLOR_LIME;
            menu_bg_color = BG_GREEN;
            top_menu_bar_color = COLOR_FOREST;
            hud_box_bg_color = BG_GREEN;
            break;

        case THEME_INFERNO:
            hud_text_color = COLOR_ORANGE;
            hud_shadow_color = COLOR_BLACK;
            hud_icon_color = COLOR_RED;
            fps_icon_color = COLOR_GOLD;
            clock_icon_color = COLOR_ORANGE;
            menu_text_color = COLOR_ORANGE;
            menu_select_color = COLOR_GOLD;
            menu_border_color = COLOR_RED;
            menu_bg_color = BG_RED;
            top_menu_bar_color = COLOR_CRIMSON;
            hud_box_bg_color = BG_RED;
            break;

        case THEME_ABYSS:
            hud_text_color = COLOR_SKY;
            hud_shadow_color = COLOR_BLACK;
            hud_icon_color = COLOR_NAVY;
            fps_icon_color = COLOR_AQUA;
            clock_icon_color = COLOR_SKY;
            menu_text_color = COLOR_SKY;
            menu_select_color = COLOR_AQUA;
            menu_border_color = COLOR_BLUE;
            menu_bg_color = BG_NAVY;
            top_menu_bar_color = COLOR_MIDNIGHT;
            hud_box_bg_color = BG_NAVY;
            break;

        case THEME_CHROME:
            hud_text_color = COLOR_SNOW;
            hud_shadow_color = COLOR_DARK_GRAY;
            hud_icon_color = COLOR_SILVER;
            fps_icon_color = COLOR_ELECTRIC_BLUE;
            clock_icon_color = COLOR_SNOW;
            menu_text_color = COLOR_SNOW;
            menu_select_color = COLOR_ELECTRIC_BLUE;
            menu_border_color = COLOR_SILVER;
            menu_bg_color = BG_SLATE;
            top_menu_bar_color = COLOR_DARK_GRAY;
            hud_box_bg_color = BG_SLATE;
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

static unsigned int combo_buttons(int combo_mode) {
    switch (combo_mode) {
        case TOGGLE_UP: return SCE_CTRL_LTRIGGER | SCE_CTRL_RTRIGGER | SCE_CTRL_UP;
        case TOGGLE_DOWN: return SCE_CTRL_LTRIGGER | SCE_CTRL_RTRIGGER | SCE_CTRL_DOWN;
        case TOGGLE_LEFT: return SCE_CTRL_LTRIGGER | SCE_CTRL_RTRIGGER | SCE_CTRL_LEFT;
        case TOGGLE_RIGHT: return SCE_CTRL_LTRIGGER | SCE_CTRL_RTRIGGER | SCE_CTRL_RIGHT;
        case TOGGLE_START: return SCE_CTRL_LTRIGGER | SCE_CTRL_RTRIGGER | SCE_CTRL_START;
        case TOGGLE_TRIANGLE: return SCE_CTRL_LTRIGGER | SCE_CTRL_RTRIGGER | SCE_CTRL_TRIANGLE;
        case TOGGLE_CIRCLE: return SCE_CTRL_LTRIGGER | SCE_CTRL_RTRIGGER | SCE_CTRL_CIRCLE;
        case TOGGLE_SQUARE: return SCE_CTRL_LTRIGGER | SCE_CTRL_RTRIGGER | SCE_CTRL_SQUARE;
        case TOGGLE_CROSS: return SCE_CTRL_LTRIGGER | SCE_CTRL_RTRIGGER | SCE_CTRL_CROSS;
        case TOGGLE_SELECT_UP: return SCE_CTRL_SELECT | SCE_CTRL_UP;
        case TOGGLE_SELECT_DOWN: return SCE_CTRL_SELECT | SCE_CTRL_DOWN;
        case TOGGLE_START_UP: return SCE_CTRL_START | SCE_CTRL_UP;
        case TOGGLE_START_DOWN: return SCE_CTRL_START | SCE_CTRL_DOWN;
        case TOGGLE_L_SELECT: return SCE_CTRL_LTRIGGER | SCE_CTRL_SELECT;
        case TOGGLE_R_SELECT: return SCE_CTRL_RTRIGGER | SCE_CTRL_SELECT;
        case TOGGLE_L_START: return SCE_CTRL_LTRIGGER | SCE_CTRL_START;
        case TOGGLE_R_START: return SCE_CTRL_RTRIGGER | SCE_CTRL_START;
        case TOGGLE_SELECT:
        default: return SCE_CTRL_LTRIGGER | SCE_CTRL_RTRIGGER | SCE_CTRL_SELECT;
    }
}

static unsigned int get_toggle_combo(void) { return combo_buttons(toggle_combo_mode); }
static unsigned int get_menu_combo(void) { return combo_buttons(menu_toggle_combo_mode); }

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
    if (fps_display_style == FPS_STYLE_LABEL_NUMBER) pos = append_text(out, pos, "FPS ");
    pos = append_2digit_number(out, pos, (int)fps_value);
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
     * RAM HUD working pass.
     * Uses VitaSDK user-mode sysmem free-size API.
     * Keep this cached here, not in draw_hud(), so drawing remains stable.
     */
    {
        SceKernelFreeMemorySizeInfo mem_info;
        int ret;
        int free_bytes;
        int free_mb;
        int pos;

        mem_info.size = sizeof(SceKernelFreeMemorySizeInfo);
        mem_info.size_user = 0;
        mem_info.size_cdram = 0;
        mem_info.size_phycont = 0;

        ret = sceKernelGetFreeMemorySize(&mem_info);

        if (ret >= 0) {
            /*
             * Total visible free user memory pools.
             * size_user + size_cdram + size_phycont are returned in bytes.
             */
            free_bytes = mem_info.size_user + mem_info.size_cdram + mem_info.size_phycont;
            free_mb = free_bytes / (1024 * 1024);
            cached_ram_free_mb = free_mb;

            pos = 0;
            pos = append_text(cached_ram_text, pos, "RAM ");
            pos = append_int(cached_ram_text, pos, free_mb);
            pos = append_text(cached_ram_text, pos, "M");
            cached_ram_text[pos] = '\0';
        } else {
            cached_ram_free_mb = -1;
            copy_cstr(cached_ram_text, sizeof(cached_ram_text), "RAM ERR");
        }
    }

}

static void build_time_text(char *out) {
    SceDateTime time;
    int pos = 0;
    int hour;
    int is_pm = 0;
    sceRtcGetCurrentClockLocalTime(&time);
    if (clock_display_style == CLOCK_STYLE_DATE_TIME) {
        pos = append_2digit_number(out, pos, time.month);
        out[pos++] = '/';
        pos = append_2digit_number(out, pos, time.day);
        out[pos++] = ' ';
    }
    if (use_24h_time || hud_layout == LAYOUT_COMPACT || hud_layout == LAYOUT_ICONS) {
        put_2digits(&out[pos], time.hour); pos += 2; out[pos++] = ':';
        put_2digits(&out[pos], time.minute); pos += 2; out[pos] = '\0'; return;
    }
    hour = time.hour;
    if (hour >= 12) is_pm = 1;
    hour = hour % 12;
    if (hour == 0) hour = 12;
    put_2digits(&out[pos], hour); pos += 2; out[pos++] = ':';
    put_2digits(&out[pos], time.minute); pos += 2;
    out[pos++] = is_pm ? 'P' : 'A'; out[pos++] = 'M'; out[pos] = '\0';
}

static void build_date_text(char *out) {
    SceDateTime time;
    int pos = 0;

    sceRtcGetCurrentClockLocalTime(&time);
    pos = append_2digit_number(out, pos, time.month);
    out[pos++] = '/';
    pos = append_2digit_number(out, pos, time.day);
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
        case COLOR_BROWN:    return 0xFF2A2A80;
        case COLOR_COPPER:   return 0xFF3278B8;
        case COLOR_CHOCOLATE:return 0xFF1E3C78;
        case COLOR_HOT_PINK: return 0xFFFF69FF;
        case COLOR_NEON_GREEN:return 0xFF39FF39;
        case COLOR_ELECTRIC_BLUE:return 0xFFFF7F00;
        case COLOR_MIDNIGHT:return 0xFF301000;
        case COLOR_SNOW:     return 0xFFFFFAFA;
        case COLOR_CREAM:    return 0xFFDCC8FF;
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

static unsigned int apply_opacity_to_color(unsigned int color, int opacity_id) {
    unsigned int alpha = color & 0xFF000000;

    if (alpha == 0) {
        return color;
    }

    switch (opacity_id) {
        case OPACITY_75: alpha = 0xC0000000; break;
        case OPACITY_50: alpha = 0x80000000; break;
        case OPACITY_25: alpha = 0x40000000; break;
        case OPACITY_100:
        default:         alpha = 0xFF000000; break;
    }

    return (color & 0x00FFFFFF) | alpha;
}

static unsigned int apply_forced_draw_opacity(unsigned int color) {
    if (g_forced_draw_opacity < 0) {
        return color;
    }

    return apply_opacity_to_color(color, g_forced_draw_opacity);
}

static unsigned int blend_argb_over(unsigned int src, unsigned int dst) {
    unsigned int a = (src >> 24) & 0xFF;
    unsigned int rb;
    unsigned int g;

    if (a == 0) {
        return dst;
    }

    if (a >= 255) {
        return src | 0xFF000000;
    }

    rb = (((src & 0x00FF00FF) * a) + ((dst & 0x00FF00FF) * (255 - a))) >> 8;
    g = (((src & 0x0000FF00) * a) + ((dst & 0x0000FF00) * (255 - a))) >> 8;

    return 0xFF000000 | (rb & 0x00FF00FF) | (g & 0x0000FF00);
}

static void write_pixel(unsigned int *pixels, int pitch, int x, int y, unsigned int color) {
    unsigned int final_color;

    if (x < 0 || x >= g_screen_w || y < 0 || y >= g_screen_h) {
        return;
    }

    final_color = apply_forced_draw_opacity(color);
    pixels[y * pitch + x] = blend_argb_over(final_color, pixels[y * pitch + x]);
}

static unsigned int get_menu_bg(void) {
    return apply_opacity_to_color(get_menu_bg_for(menu_bg_color), menu_opacity);
}

static unsigned int get_hud_box_bg(void) {
    return get_menu_bg_for(hud_box_bg_color);
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
    if (g_menu_text_mode) return 0;
    switch (font_style) {
        case FONT_WIDE:
        case FONT_PSP:
        case FONT_ARCADE:
            return 2;

        case FONT_SOFT:
        case FONT_RETRO:
        case FONT_ROUNDED:
        case FONT_DIGITAL:
        case FONT_VITA:
            return 1;

        case FONT_COMPACT:
        case FONT_MINI:
        case FONT_THIN:
        case FONT_CLEAN:
            return -1;

        case FONT_TALL:
        case FONT_SHARP:
        default:
            return 0;
    }
}

static int font_shadow_style(void) {
    if (g_menu_text_mode) return 1;
    switch (font_style) {
        case FONT_THIN:
        case FONT_CLEAN:
        case FONT_SHARP:
            return 0;

        case FONT_SOFT:
            return 2;

        case FONT_DIGITAL:
            return 3;

        case FONT_RETRO:
            return 4;

        case FONT_VITA:
            return 5;

        case FONT_SHADOW:
        case FONT_PSP:
        default:
            return 1;
    }
}

static int font_bold_style(void) {
    if (g_menu_text_mode) return 0;
    switch (font_style) {
        case FONT_BOLD:
        case FONT_BLOCK:
        case FONT_SQUARE:
        case FONT_ARCADE:
        case FONT_ROUNDED:
            return 1;

        case FONT_DOUBLE:
            return 2;

        case FONT_THIN:
        case FONT_CLEAN:
            return -1;

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

        case '^': { static const unsigned char g[7] = {0x04,0x0A,0x11,0x00,0x00,0x00,0x00}; return g[row]; }
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
    unsigned int final_color;
    unsigned int alpha;

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

    final_color = apply_forced_draw_opacity(color);
    alpha = (final_color >> 24) & 0xFF;

    if (alpha == 0) {
        return;
    }

    /* Fast path: most HUD/menu rectangles are fully opaque.
     * Avoid calling write_pixel()/blend_argb_over() for every panel pixel.
     */
    if (alpha >= 255) {
        final_color |= 0xFF000000;
        for (yy = y; yy < y2; yy++) {
            unsigned int *row = pixels + (yy * pitch);
            for (xx = x; xx < x2; xx++) {
                row[xx] = final_color;
            }
        }
        return;
    }

    for (yy = y; yy < y2; yy++) {
        unsigned int *row = pixels + (yy * pitch);
        for (xx = x; xx < x2; xx++) {
            row[xx] = blend_argb_over(final_color, row[xx]);
        }
    }
}

static void draw_rect_menu_opacity_fast(unsigned int *pixels, int pitch, int x, int y, int w, int h, unsigned int color, int opacity_id) {
    int yy;
    int xx;
    int x2;
    int y2;
    unsigned int final_color;

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

    final_color = (color & 0x00FFFFFF) | 0xFF000000;

    /*
     * Performance fix: menu panels are the largest draw cost when the menu opens.
     * Do not alpha-blend thousands of pixels every frame. Use a fast direct-fill
     * dither for lower opacity levels so MENU OPACITY still visibly changes,
     * but opening the menu does not tank the game.
     */
    for (yy = y; yy < y2; yy++) {
        unsigned int *row = pixels + (yy * pitch);

        for (xx = x; xx < x2; xx++) {
            if (opacity_id == OPACITY_75) {
                if (((xx + yy) & 3) == 0) continue;
            } else if (opacity_id == OPACITY_50) {
                if (((xx + yy) & 1) != 0) continue;
            } else if (opacity_id == OPACITY_25) {
                if (((xx + yy) & 3) != 0) continue;
            }

            row[xx] = final_color;
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
    int slant;
    int bold = font_bold_style();
    int thin = (!g_menu_text_mode && font_style == FONT_THIN);
    int tall = (!g_menu_text_mode && font_style == FONT_TALL);
    int soft = (!g_menu_text_mode && font_style == FONT_SOFT);
    int sharp = (!g_menu_text_mode && font_style == FONT_SHARP);
    int retro = (!g_menu_text_mode && font_style == FONT_RETRO);
    int vita = (!g_menu_text_mode && font_style == FONT_VITA);
    int rounded = (!g_menu_text_mode && font_style == FONT_ROUNDED);
    int digital = (!g_menu_text_mode && font_style == FONT_DIGITAL);
    int clean = (!g_menu_text_mode && font_style == FONT_CLEAN);

    for (row = 0; row < 7; row++) {
        unsigned char bits = font5x7(c, row);

        if (digital) {
            /* Digital mode: hard segmented look. */
            if (row != 0 && row != 3 && row != 6) {
                bits &= 0x11;
            }
        }

        for (col = 0; col < 5; col++) {
            if (bits & (1 << (4 - col))) {
                for (sy = 0; sy < scale; sy++) {
                    for (sx = 0; sx < scale; sx++) {
                        if (thin && scale > 1 && sx == scale - 1) {
                            continue;
                        }

                        if (clean && scale > 1 && sy == scale - 1 && row != 6) {
                            continue;
                        }

                        slant = 0;
                        if (retro) slant = (6 - row) / 3;
                        if (vita) slant = row / 4;
                        if (sharp) slant = (row & 1) ? 0 : 1;

                        px = x + col * scale + sx + slant;
                        py = y + row * scale + sy;

                        if (px >= 0 && px < g_screen_w && py >= 0 && py < g_screen_h) {
                            write_pixel(pixels, pitch, px, py, color);
                        }

                        if (tall) {
                            py = y + row * scale + sy + 1;
                            if (px >= 0 && px < g_screen_w && py >= 0 && py < g_screen_h) {
                                write_pixel(pixels, pitch, px, py, color);
                            }
                        }

                        if (soft && ((sx == 0 && scale > 1) || (sy == 0 && scale > 1))) {
                            if (px + 1 >= 0 && px + 1 < g_screen_w && py >= 0 && py < g_screen_h) {
                                write_pixel(pixels, pitch, px + 1, py, color);
                            }
                        }

                        if (rounded && (row == 1 || row == 5) && (col == 0 || col == 4)) {
                            if (px >= 0 && px < g_screen_w && py + 1 >= 0 && py + 1 < g_screen_h) {
                                write_pixel(pixels, pitch, px, py + 1, color);
                            }
                        }

                        if (digital && (row == 0 || row == 3 || row == 6)) {
                            if (px + 1 >= 0 && px + 1 < g_screen_w && py >= 0 && py < g_screen_h) {
                                write_pixel(pixels, pitch, px + 1, py, color);
                            }
                        }

                        if (bold >= 1) {
                            px = x + col * scale + sx + slant + 1;
                            py = y + row * scale + sy;

                            if (px >= 0 && px < g_screen_w && py >= 0 && py < g_screen_h) {
                                write_pixel(pixels, pitch, px, py, color);
                            }
                        }

                        if (bold >= 2) {
                            px = x + col * scale + sx + slant;
                            py = y + row * scale + sy + 1;

                            if (px >= 0 && px < g_screen_w && py >= 0 && py < g_screen_h) {
                                write_pixel(pixels, pitch, px, py, color);
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
    } else if (shadow == 3) {
        /* DIGITAL: crisp lower rail shadow. */
        draw_text(pixels, pitch, x, y + 2, text, shadow_color, scale);
    } else if (shadow == 4) {
        /* RETRO: offset arcade-style trail. */
        draw_text(pixels, pitch, x + 2, y + 1, text, shadow_color, scale);
    } else if (shadow == 5) {
        /* VITA: bright blue/cyan-style side glow. */
        draw_text(pixels, pitch, x + 1, y, text, color_value(COLOR_CYAN, shadow_color), scale);
        draw_text(pixels, pitch, x, y + 1, text, color_value(COLOR_BLUE, shadow_color), scale);
    }

    draw_text(pixels, pitch, x, y, text, color, scale);
}

static void draw_icon_style_variant(unsigned int *pixels, int pitch, int x, int y, int scale, int style, unsigned int col) {
    int s = scale;
    if (s < 1) s = 1;

    switch (style) {
        case ICON_STYLE_OUTLINE:
            draw_rect(pixels, pitch, x + s, y + s, 7 * s, s, col);
            draw_rect(pixels, pitch, x + s, y + (5 * s), 7 * s, s, col);
            draw_rect(pixels, pitch, x + s, y + s, s, 5 * s, col);
            draw_rect(pixels, pitch, x + (7 * s), y + s, s, 5 * s, col);
            break;
        case ICON_STYLE_SOLID:
            draw_rect(pixels, pitch, x + s, y + s, 7 * s, 5 * s, col);
            break;
        case ICON_STYLE_DIAMOND:
            draw_rect(pixels, pitch, x + (4 * s), y, s, s, col);
            draw_rect(pixels, pitch, x + (3 * s), y + s, 3 * s, s, col);
            draw_rect(pixels, pitch, x + (2 * s), y + (2 * s), 5 * s, s, col);
            draw_rect(pixels, pitch, x + s, y + (3 * s), 7 * s, s, col);
            draw_rect(pixels, pitch, x + (2 * s), y + (4 * s), 5 * s, s, col);
            draw_rect(pixels, pitch, x + (3 * s), y + (5 * s), 3 * s, s, col);
            draw_rect(pixels, pitch, x + (4 * s), y + (6 * s), s, s, col);
            break;
        case ICON_STYLE_CROSS:
            draw_rect(pixels, pitch, x + (4 * s), y, s, 7 * s, col);
            draw_rect(pixels, pitch, x + s, y + (3 * s), 7 * s, s, col);
            break;
        case ICON_STYLE_DOTS:
            draw_rect(pixels, pitch, x + s, y + s, 2 * s, 2 * s, col);
            draw_rect(pixels, pitch, x + (6 * s), y + s, 2 * s, 2 * s, col);
            draw_rect(pixels, pitch, x + (3 * s), y + (3 * s), 3 * s, s, col);
            draw_rect(pixels, pitch, x + s, y + (5 * s), 2 * s, 2 * s, col);
            draw_rect(pixels, pitch, x + (6 * s), y + (5 * s), 2 * s, 2 * s, col);
            break;
        case ICON_STYLE_CHEVRON:
            draw_rect(pixels, pitch, x + s, y + s, 2 * s, s, col);
            draw_rect(pixels, pitch, x + (3 * s), y + (2 * s), 2 * s, s, col);
            draw_rect(pixels, pitch, x + (5 * s), y + (3 * s), 2 * s, s, col);
            draw_rect(pixels, pitch, x + (3 * s), y + (4 * s), 2 * s, s, col);
            draw_rect(pixels, pitch, x + s, y + (5 * s), 2 * s, s, col);
            break;
        case ICON_STYLE_MINI:
            draw_rect(pixels, pitch, x + (3 * s), y + (2 * s), 3 * s, 3 * s, col);
            draw_rect(pixels, pitch, x + (4 * s), y + s, s, s, col);
            draw_rect(pixels, pitch, x + (4 * s), y + (5 * s), s, s, col);
            break;
        case ICON_STYLE_BARS:
            draw_rect(pixels, pitch, x + s, y + s, 7 * s, s, col);
            draw_rect(pixels, pitch, x + (2 * s), y + (3 * s), 5 * s, s, col);
            draw_rect(pixels, pitch, x + (3 * s), y + (5 * s), 3 * s, s, col);
            break;
        case ICON_STYLE_TARGET:
            draw_rect(pixels, pitch, x + (2 * s), y, 5 * s, s, col);
            draw_rect(pixels, pitch, x + s, y + s, s, s, col);
            draw_rect(pixels, pitch, x + (7 * s), y + s, s, s, col);
            draw_rect(pixels, pitch, x, y + (2 * s), s, 3 * s, col);
            draw_rect(pixels, pitch, x + (8 * s), y + (2 * s), s, 3 * s, col);
            draw_rect(pixels, pitch, x + s, y + (5 * s), s, s, col);
            draw_rect(pixels, pitch, x + (7 * s), y + (5 * s), s, s, col);
            draw_rect(pixels, pitch, x + (2 * s), y + (6 * s), 5 * s, s, col);
            draw_rect(pixels, pitch, x + (4 * s), y + (3 * s), s, s, col);
            break;
        case ICON_STYLE_STARBIRD:
            draw_rect(pixels, pitch, x + (4 * s), y, s, s, col);
            draw_rect(pixels, pitch, x + (3 * s), y + s, 3 * s, s, col);
            draw_rect(pixels, pitch, x + (2 * s), y + (2 * s), 2 * s, s, col);
            draw_rect(pixels, pitch, x + (5 * s), y + (2 * s), 2 * s, s, col);
            draw_rect(pixels, pitch, x + s, y + (3 * s), 2 * s, s, col);
            draw_rect(pixels, pitch, x + (6 * s), y + (3 * s), 2 * s, s, col);
            draw_rect(pixels, pitch, x + (2 * s), y + (4 * s), 5 * s, s, col);
            draw_rect(pixels, pitch, x + (3 * s), y + (5 * s), 3 * s, s, col);
            draw_rect(pixels, pitch, x + (4 * s), y + (6 * s), s, s, col);
            break;
        case ICON_STYLE_IMPERIAL:
            draw_rect(pixels, pitch, x + (3 * s), y, 3 * s, s, col);
            draw_rect(pixels, pitch, x + s, y + s, 2 * s, s, col);
            draw_rect(pixels, pitch, x + (6 * s), y + s, 2 * s, s, col);
            draw_rect(pixels, pitch, x, y + (2 * s), s, 3 * s, col);
            draw_rect(pixels, pitch, x + (8 * s), y + (2 * s), s, 3 * s, col);
            draw_rect(pixels, pitch, x + s, y + (5 * s), 2 * s, s, col);
            draw_rect(pixels, pitch, x + (6 * s), y + (5 * s), 2 * s, s, col);
            draw_rect(pixels, pitch, x + (3 * s), y + (6 * s), 3 * s, s, col);
            draw_rect(pixels, pitch, x + (4 * s), y + (2 * s), s, 3 * s, col);
            draw_rect(pixels, pitch, x + (2 * s), y + (3 * s), 5 * s, s, col);
            break;
        case ICON_STYLE_JEDI:
            draw_rect(pixels, pitch, x + (4 * s), y, s, 6 * s, col);
            draw_rect(pixels, pitch, x + (3 * s), y + (5 * s), 3 * s, s, col);
            draw_rect(pixels, pitch, x + (2 * s), y + (6 * s), 5 * s, s, col);
            draw_rect(pixels, pitch, x + (2 * s), y + (2 * s), 5 * s, s, col);
            break;
        case ICON_STYLE_SITH:
            draw_rect(pixels, pitch, x + (4 * s), y, s, 7 * s, col);
            draw_rect(pixels, pitch, x + (2 * s), y + s, s, s, col);
            draw_rect(pixels, pitch, x + (6 * s), y + (2 * s), s, s, col);
            draw_rect(pixels, pitch, x + (2 * s), y + (4 * s), s, s, col);
            draw_rect(pixels, pitch, x + (6 * s), y + (5 * s), s, s, col);
            draw_rect(pixels, pitch, x + (3 * s), y + (3 * s), 3 * s, s, col);
            break;
        case ICON_STYLE_SABER:
            draw_rect(pixels, pitch, x + (4 * s), y, s, 5 * s, col);
            draw_rect(pixels, pitch, x + (3 * s), y + (5 * s), 3 * s, s, col);
            draw_rect(pixels, pitch, x + (2 * s), y + (6 * s), 5 * s, s, col);
            draw_rect(pixels, pitch, x + (2 * s), y + (4 * s), 2 * s, s, col);
            draw_rect(pixels, pitch, x + (5 * s), y + (4 * s), 2 * s, s, col);
            break;
        case ICON_STYLE_SKULL:
            draw_rect(pixels, pitch, x + (2 * s), y, 5 * s, s, col);
            draw_rect(pixels, pitch, x + s, y + s, 7 * s, 3 * s, col);
            draw_rect(pixels, pitch, x + (2 * s), y + (4 * s), 5 * s, 2 * s, col);
            draw_rect(pixels, pitch, x + (2 * s), y + (2 * s), s, s, 0x00000000);
            draw_rect(pixels, pitch, x + (6 * s), y + (2 * s), s, s, 0x00000000);
            draw_rect(pixels, pitch, x + (4 * s), y + (3 * s), s, s, 0x00000000);
            draw_rect(pixels, pitch, x + (3 * s), y + (6 * s), s, s, col);
            draw_rect(pixels, pitch, x + (5 * s), y + (6 * s), s, s, col);
            break;
        case ICON_STYLE_CROWN:
            draw_rect(pixels, pitch, x, y + (5 * s), 9 * s, s, col);
            draw_rect(pixels, pitch, x + s, y + (4 * s), s, s, col);
            draw_rect(pixels, pitch, x + (3 * s), y + (3 * s), s, s, col);
            draw_rect(pixels, pitch, x + (5 * s), y + (4 * s), s, s, col);
            draw_rect(pixels, pitch, x + (7 * s), y + (3 * s), s, s, col);
            draw_rect(pixels, pitch, x + (2 * s), y + s, s, 2 * s, col);
            draw_rect(pixels, pitch, x + (6 * s), y + s, s, 2 * s, col);
            draw_rect(pixels, pitch, x + (4 * s), y, s, 2 * s, col);
            break;
        case ICON_STYLE_GEM:
            draw_rect(pixels, pitch, x + (4 * s), y, s, s, col);
            draw_rect(pixels, pitch, x + (2 * s), y + s, 5 * s, s, col);
            draw_rect(pixels, pitch, x + s, y + (2 * s), 7 * s, s, col);
            draw_rect(pixels, pitch, x + (2 * s), y + (3 * s), 5 * s, 2 * s, col);
            draw_rect(pixels, pitch, x + (3 * s), y + (5 * s), 3 * s, s, col);
            draw_rect(pixels, pitch, x + (4 * s), y + (6 * s), s, s, col);
            break;
        case ICON_STYLE_FLAME:
            draw_rect(pixels, pitch, x + (4 * s), y, s, s, col);
            draw_rect(pixels, pitch, x + (3 * s), y + s, 2 * s, s, col);
            draw_rect(pixels, pitch, x + (2 * s), y + (2 * s), 4 * s, s, col);
            draw_rect(pixels, pitch, x + s, y + (3 * s), 5 * s, s, col);
            draw_rect(pixels, pitch, x + (2 * s), y + (4 * s), 4 * s, s, col);
            draw_rect(pixels, pitch, x + (3 * s), y + (5 * s), 3 * s, s, col);
            draw_rect(pixels, pitch, x + (4 * s), y + (6 * s), s, s, col);
            break;
        case ICON_STYLE_ORBIT:
            draw_rect(pixels, pitch, x + (2 * s), y, 5 * s, s, col);
            draw_rect(pixels, pitch, x + s, y + s, s, 5 * s, col);
            draw_rect(pixels, pitch, x + (7 * s), y + s, s, 5 * s, col);
            draw_rect(pixels, pitch, x + (2 * s), y + (6 * s), 5 * s, s, col);
            draw_rect(pixels, pitch, x + (4 * s), y + (3 * s), s, s, col);
            draw_rect(pixels, pitch, x + (6 * s), y + (2 * s), s, s, col);
            break;
        case ICON_STYLE_PHOENIX:
            draw_rect(pixels, pitch, x + (4 * s), y, s, s, col);
            draw_rect(pixels, pitch, x + (2 * s), y + s, 5 * s, s, col);
            draw_rect(pixels, pitch, x + s, y + (2 * s), 2 * s, s, col);
            draw_rect(pixels, pitch, x + (6 * s), y + (2 * s), 2 * s, s, col);
            draw_rect(pixels, pitch, x + (3 * s), y + (3 * s), 3 * s, s, col);
            draw_rect(pixels, pitch, x + (2 * s), y + (4 * s), 5 * s, s, col);
            draw_rect(pixels, pitch, x + s, y + (5 * s), s, s, col);
            draw_rect(pixels, pitch, x + (7 * s), y + (5 * s), s, s, col);
            draw_rect(pixels, pitch, x + (4 * s), y + (6 * s), s, s, col);
            break;
        case ICON_STYLE_ECLIPSE:
            draw_rect(pixels, pitch, x + (2 * s), y, 5 * s, s, col);
            draw_rect(pixels, pitch, x + s, y + s, 7 * s, s, col);
            draw_rect(pixels, pitch, x, y + (2 * s), 5 * s, 3 * s, col);
            draw_rect(pixels, pitch, x + s, y + (5 * s), 7 * s, s, col);
            draw_rect(pixels, pitch, x + (2 * s), y + (6 * s), 5 * s, s, col);
            draw_rect(pixels, pitch, x + (5 * s), y + s, 3 * s, 5 * s, 0x00000000);
            break;
        case ICON_STYLE_NEXUS:
            draw_rect(pixels, pitch, x + (4 * s), y, s, 7 * s, col);
            draw_rect(pixels, pitch, x, y + (3 * s), 9 * s, s, col);
            draw_rect(pixels, pitch, x + s, y + s, 2 * s, 2 * s, col);
            draw_rect(pixels, pitch, x + (6 * s), y + s, 2 * s, 2 * s, col);
            draw_rect(pixels, pitch, x + s, y + (5 * s), 2 * s, s, col);
            draw_rect(pixels, pitch, x + (6 * s), y + (5 * s), 2 * s, s, col);
            break;
        case ICON_STYLE_VIPER:
            draw_rect(pixels, pitch, x + s, y, 7 * s, s, col);
            draw_rect(pixels, pitch, x, y + s, 3 * s, s, col);
            draw_rect(pixels, pitch, x + (3 * s), y + (2 * s), 4 * s, s, col);
            draw_rect(pixels, pitch, x + (6 * s), y + (3 * s), 2 * s, s, col);
            draw_rect(pixels, pitch, x + (3 * s), y + (4 * s), 4 * s, s, col);
            draw_rect(pixels, pitch, x, y + (5 * s), 3 * s, s, col);
            draw_rect(pixels, pitch, x + s, y + (6 * s), 7 * s, s, col);
            break;
        case ICON_STYLE_RUNE:
            draw_rect(pixels, pitch, x + (2 * s), y, 5 * s, s, col);
            draw_rect(pixels, pitch, x + s, y + s, s, 5 * s, col);
            draw_rect(pixels, pitch, x + (7 * s), y + s, s, 5 * s, col);
            draw_rect(pixels, pitch, x + (2 * s), y + (6 * s), 5 * s, s, col);
            draw_rect(pixels, pitch, x + (3 * s), y + (2 * s), 3 * s, s, col);
            draw_rect(pixels, pitch, x + (4 * s), y + (3 * s), s, 2 * s, col);
            break;
        case ICON_STYLE_HALO:
            draw_rect(pixels, pitch, x + (3 * s), y, 3 * s, s, col);
            draw_rect(pixels, pitch, x + s, y + s, 7 * s, s, col);
            draw_rect(pixels, pitch, x, y + (2 * s), s, 3 * s, col);
            draw_rect(pixels, pitch, x + (8 * s), y + (2 * s), s, 3 * s, col);
            draw_rect(pixels, pitch, x + s, y + (5 * s), 7 * s, s, col);
            draw_rect(pixels, pitch, x + (3 * s), y + (6 * s), 3 * s, s, col);
            draw_rect(pixels, pitch, x + (4 * s), y + (3 * s), s, s, col);
            break;
        case ICON_STYLE_PRISM:
            draw_rect(pixels, pitch, x + (4 * s), y, s, s, col);
            draw_rect(pixels, pitch, x + (3 * s), y + s, 3 * s, s, col);
            draw_rect(pixels, pitch, x + (2 * s), y + (2 * s), 5 * s, s, col);
            draw_rect(pixels, pitch, x + s, y + (3 * s), 7 * s, 2 * s, col);
            draw_rect(pixels, pitch, x + (2 * s), y + (5 * s), 5 * s, s, col);
            draw_rect(pixels, pitch, x + (4 * s), y + (2 * s), s, 4 * s, 0x00000000);
            break;
        case ICON_STYLE_COMET:
            draw_rect(pixels, pitch, x, y + s, 4 * s, s, col);
            draw_rect(pixels, pitch, x + s, y + (3 * s), 4 * s, s, col);
            draw_rect(pixels, pitch, x, y + (5 * s), 4 * s, s, col);
            draw_rect(pixels, pitch, x + (5 * s), y + (2 * s), 3 * s, 3 * s, col);
            draw_rect(pixels, pitch, x + (6 * s), y + s, s, 5 * s, col);
            break;
        case ICON_STYLE_SIGIL:
            draw_rect(pixels, pitch, x + (4 * s), y, s, s, col);
            draw_rect(pixels, pitch, x + (2 * s), y + s, 5 * s, s, col);
            draw_rect(pixels, pitch, x + s, y + (2 * s), 7 * s, s, col);
            draw_rect(pixels, pitch, x + (3 * s), y + (3 * s), 3 * s, s, col);
            draw_rect(pixels, pitch, x + s, y + (4 * s), 7 * s, s, col);
            draw_rect(pixels, pitch, x + (2 * s), y + (5 * s), 5 * s, s, col);
            draw_rect(pixels, pitch, x + (4 * s), y + (6 * s), s, s, col);
            break;
        case ICON_STYLE_VORTEX:
            draw_rect(pixels, pitch, x + (2 * s), y, 5 * s, s, col);
            draw_rect(pixels, pitch, x + s, y + s, s, s, col);
            draw_rect(pixels, pitch, x + (6 * s), y + s, 2 * s, s, col);
            draw_rect(pixels, pitch, x, y + (2 * s), s, 3 * s, col);
            draw_rect(pixels, pitch, x + (4 * s), y + (3 * s), 4 * s, s, col);
            draw_rect(pixels, pitch, x + s, y + (5 * s), 2 * s, s, col);
            draw_rect(pixels, pitch, x + (7 * s), y + (4 * s), s, s, col);
            draw_rect(pixels, pitch, x + (2 * s), y + (6 * s), 5 * s, s, col);
            break;
        case ICON_STYLE_ORIGINAL:
        default:
            draw_rect(pixels, pitch, x + (2 * s), y + s, 5 * s, 5 * s, col);
            draw_rect(pixels, pitch, x + (4 * s), y + (3 * s), s, s, 0x00000000);
            break;
    }
}

static void draw_battery_icon(unsigned int *pixels, int pitch, int x, int y, int battery, int scale, int charging) {
    unsigned int white = active_battery_alert ? 0xFF0000FF : color_value(hud_icon_color, 0xFFFFFFFF);
    unsigned int fill = active_battery_alert ? 0xFF0000FF : (hud_icon_color == COLOR_AUTO ? get_battery_color(battery) : color_value(hud_icon_color, get_battery_color(battery)));

    if (battery_icon_style != ICON_STYLE_ORIGINAL) {
        draw_icon_style_variant(pixels, pitch, x, y, scale, battery_icon_style, fill);
        return;
    }

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

    if (charging) {
        unsigned int bolt = hud_icon_color == COLOR_AUTO ? 0xFF00FFFF : fill;
        int bx = x + (5 * scale);
        int by = y + scale;

        /* Simple charging marker inside the battery body. */
        draw_rect(pixels, pitch, bx + (1 * scale), by, 2 * scale, scale, bolt);
        draw_rect(pixels, pitch, bx, by + scale, 3 * scale, scale, bolt);
        draw_rect(pixels, pitch, bx + scale, by + (2 * scale), 3 * scale, scale, bolt);
        draw_rect(pixels, pitch, bx, by + (3 * scale), 2 * scale, scale, bolt);
    }
}
static void draw_clock_icon(unsigned int *pixels, int pitch, int x, int y, int scale) {
    unsigned int white = color_value(clock_icon_color, color_value(hud_icon_color, 0xFFFFFFFF));
    int s = scale;

    if (clock_icon_style != ICON_STYLE_ORIGINAL) {
        draw_icon_style_variant(pixels, pitch, x, y, scale, clock_icon_style, white);
        return;
    }

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
static void draw_calendar_icon(unsigned int *pixels, int pitch, int x, int y, int scale) {
    unsigned int col = color_value(date_icon_color, color_value(hud_icon_color, 0xFFFFFFFF));
    int s = scale;

    if (date_icon_style != ICON_STYLE_ORIGINAL) {
        draw_icon_style_variant(pixels, pitch, x, y, scale, date_icon_style, col);
        return;
    }

    if (s < 1) s = 1;

    draw_rect(pixels, pitch, x + s, y + s, 7 * s, 6 * s, col);
    draw_rect(pixels, pitch, x + (2 * s), y, s, 2 * s, col);
    draw_rect(pixels, pitch, x + (6 * s), y, s, 2 * s, col);
    draw_rect(pixels, pitch, x + (2 * s), y + (3 * s), s, s, 0x00000000);
    draw_rect(pixels, pitch, x + (4 * s), y + (3 * s), s, s, 0x00000000);
    draw_rect(pixels, pitch, x + (6 * s), y + (3 * s), s, s, 0x00000000);
    draw_rect(pixels, pitch, x + (2 * s), y + (5 * s), s, s, 0x00000000);
    draw_rect(pixels, pitch, x + (4 * s), y + (5 * s), s, s, 0x00000000);
    draw_rect(pixels, pitch, x + (6 * s), y + (5 * s), s, s, 0x00000000);
}

static void draw_fps_icon(unsigned int *pixels, int pitch, int x, int y, int scale) {
    unsigned int col = active_fps_alert ? 0xFF0000FF : color_value(fps_icon_color, color_value(hud_icon_color, 0xFFFFFFFF));
    int s = scale;

    if (fps_icon_style != ICON_STYLE_ORIGINAL) {
        draw_icon_style_variant(pixels, pitch, x, y, scale, fps_icon_style, col);
        return;
    }

    if (s < 1) s = 1;

    /* Cleaner speedometer-style FPS icon within a 9x7 grid. */
    /* Outer arc */
    draw_rect(pixels, pitch, x + (2 * s), y, 5 * s, s, col);
    draw_rect(pixels, pitch, x + s, y + s, s, s, col);
    draw_rect(pixels, pitch, x + (7 * s), y + s, s, s, col);
    draw_rect(pixels, pitch, x, y + (2 * s), s, 2 * s, col);
    draw_rect(pixels, pitch, x + (8 * s), y + (2 * s), s, 2 * s, col);
    draw_rect(pixels, pitch, x + s, y + (4 * s), s, s, col);
    draw_rect(pixels, pitch, x + (7 * s), y + (4 * s), s, s, col);

    /* Gauge ticks */
    draw_rect(pixels, pitch, x + (4 * s), y + s, s, s, col);
    draw_rect(pixels, pitch, x + (2 * s), y + (2 * s), s, s, col);
    draw_rect(pixels, pitch, x + (6 * s), y + (2 * s), s, s, col);

    /* Hub */
    draw_rect(pixels, pitch, x + (3 * s), y + (4 * s), 2 * s, 2 * s, col);

    /* Needle pointing up-right */
    draw_rect(pixels, pitch, x + (4 * s), y + (3 * s), s, s, col);
    draw_rect(pixels, pitch, x + (5 * s), y + (3 * s), s, s, col);
    draw_rect(pixels, pitch, x + (6 * s), y + (2 * s), s, s, col);
    draw_rect(pixels, pitch, x + (7 * s), y + s, s, s, col);
}

static int active_extra_icon_color = COLOR_AUTO;
static int active_extra_icon_style = ICON_STYLE_ORIGINAL;

static unsigned int get_extra_icon_color(void) {
    return color_value(active_extra_icon_color, color_value(hud_icon_color, 0xFFFFFFFF));
}

static void set_active_extra_icon_color(int color_id) {
    active_extra_icon_color = color_id;
}

static void set_active_extra_icon_style(int style_id) {
    active_extra_icon_style = style_id;
}

static int hud_extra_icon_w(int scale) {
    return 9 * scale;
}

static int hud_extra_icon_h(int scale) {
    return 7 * scale;
}

static void strip_hud_label(char *out, int out_size, const char *src, const char *label) {
    int i = 0;
    int j = 0;

    if (!out || out_size <= 0) {
        return;
    }

    out[0] = '\0';

    if (!src) {
        return;
    }

    while (label && label[i] && src[i] == label[i]) {
        i++;
    }

    if (label && label[i] == '\0' && src[i] == ' ') {
        i++;
    } else {
        i = 0;
    }

    while (src[i] && j < out_size - 1) {
        out[j++] = src[i++];
    }

    out[j] = '\0';
}

static void draw_cpu_icon(unsigned int *pixels, int pitch, int x, int y, int scale) {
    unsigned int col = get_extra_icon_color();
    int s = scale;

    if (active_extra_icon_style != ICON_STYLE_ORIGINAL) {
        draw_icon_style_variant(pixels, pitch, x, y, scale, active_extra_icon_style, col);
        return;
    }

    if (s < 1) s = 1;

    draw_rect(pixels, pitch, x + (2 * s), y + (1 * s), 5 * s, 5 * s, col);
    draw_rect(pixels, pitch, x + (3 * s), y + (2 * s), 3 * s, 3 * s, 0x00000000);

    draw_rect(pixels, pitch, x + (1 * s), y + (2 * s), s, s, col);
    draw_rect(pixels, pitch, x + (1 * s), y + (4 * s), s, s, col);
    draw_rect(pixels, pitch, x + (7 * s), y + (2 * s), s, s, col);
    draw_rect(pixels, pitch, x + (7 * s), y + (4 * s), s, s, col);

    draw_rect(pixels, pitch, x + (3 * s), y, s, s, col);
    draw_rect(pixels, pitch, x + (5 * s), y, s, s, col);
    draw_rect(pixels, pitch, x + (3 * s), y + (6 * s), s, s, col);
    draw_rect(pixels, pitch, x + (5 * s), y + (6 * s), s, s, col);
}

static void draw_gpu_icon(unsigned int *pixels, int pitch, int x, int y, int scale) {
    unsigned int col = get_extra_icon_color();
    int s = scale;

    if (active_extra_icon_style != ICON_STYLE_ORIGINAL) {
        draw_icon_style_variant(pixels, pitch, x, y, scale, active_extra_icon_style, col);
        return;
    }

    if (s < 1) s = 1;

    draw_rect(pixels, pitch, x, y + (1 * s), 7 * s, 5 * s, col);
    draw_rect(pixels, pitch, x + s, y + (2 * s), 5 * s, 3 * s, 0x00000000);
    draw_rect(pixels, pitch, x + (2 * s), y + (3 * s), s, s, col);
    draw_rect(pixels, pitch, x + (4 * s), y + (3 * s), s, s, col);
    draw_rect(pixels, pitch, x + (7 * s), y + (2 * s), s, 3 * s, col);
    draw_rect(pixels, pitch, x + (8 * s), y + (3 * s), s, s, col);
}

static void draw_ram_icon(unsigned int *pixels, int pitch, int x, int y, int scale) {
    unsigned int col = get_extra_icon_color();
    int s = scale;

    if (active_extra_icon_style != ICON_STYLE_ORIGINAL) {
        draw_icon_style_variant(pixels, pitch, x, y, scale, active_extra_icon_style, col);
        return;
    }

    if (s < 1) s = 1;

    draw_rect(pixels, pitch, x, y + (2 * s), 9 * s, 4 * s, col);
    draw_rect(pixels, pitch, x + s, y + (3 * s), s, s, 0x00000000);
    draw_rect(pixels, pitch, x + (3 * s), y + (3 * s), s, s, 0x00000000);
    draw_rect(pixels, pitch, x + (5 * s), y + (3 * s), s, s, 0x00000000);
    draw_rect(pixels, pitch, x + (7 * s), y + (3 * s), s, s, 0x00000000);
    draw_rect(pixels, pitch, x + s, y + (6 * s), s, s, col);
    draw_rect(pixels, pitch, x + (3 * s), y + (6 * s), s, s, col);
    draw_rect(pixels, pitch, x + (5 * s), y + (6 * s), s, s, col);
    draw_rect(pixels, pitch, x + (7 * s), y + (6 * s), s, s, col);
}

static void draw_game_icon(unsigned int *pixels, int pitch, int x, int y, int scale) {
    unsigned int col = get_extra_icon_color();
    int s = scale;

    if (active_extra_icon_style != ICON_STYLE_ORIGINAL) {
        draw_icon_style_variant(pixels, pitch, x, y, scale, active_extra_icon_style, col);
        return;
    }

    if (s < 1) s = 1;

    draw_rect(pixels, pitch, x + s, y + (2 * s), 7 * s, 4 * s, col);
    draw_rect(pixels, pitch, x + (2 * s), y + s, 5 * s, s, col);
    draw_rect(pixels, pitch, x + (3 * s), y, 3 * s, s, col);
    draw_rect(pixels, pitch, x + (2 * s), y + (3 * s), s, s, 0x00000000);
    draw_rect(pixels, pitch, x + s, y + (4 * s), 3 * s, s, 0x00000000);
    draw_rect(pixels, pitch, x + (6 * s), y + (3 * s), s, s, 0x00000000);
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

static const char *position_name_for(int id) {
    switch (id) {
        case POS_BOTTOM_LEFT:  return "BOTTOM LEFT";
        case POS_TOP_RIGHT:    return "TOP RIGHT";
        case POS_TOP_LEFT:     return "TOP LEFT";
        case POS_TOP_CENTER:   return "TOP CENTER";
        case POS_BOTTOM_CENTER:return "BOTTOM CENTER";
        case POS_BOTTOM_RIGHT:
        default:               return "BOTTOM RIGHT";
    }
}

static const char *position_name(void) {
    switch (hud_position) {
        case POS_BOTTOM_LEFT:  return "BOTTOM LEFT";
        case POS_TOP_RIGHT:    return "TOP RIGHT";
        case POS_TOP_LEFT:     return "TOP LEFT";
        case POS_TOP_CENTER:   return "TOP CENTER";
        case POS_BOTTOM_CENTER:return "BOTTOM CENTER";
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
        case COLOR_BROWN:     return "BROWN";
        case COLOR_COPPER:    return "COPPER";
        case COLOR_CHOCOLATE: return "CHOCOLATE";
        case COLOR_HOT_PINK:  return "HOT PINK";
        case COLOR_NEON_GREEN:return "NEON GREEN";
        case COLOR_ELECTRIC_BLUE:return "ELECTRIC BLUE";
        case COLOR_MIDNIGHT:  return "MIDNIGHT";
        case COLOR_SNOW:      return "SNOW";
        case COLOR_CREAM:     return "CREAM";
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
        case THEME_PLASMA:    return "PLASMA";
        case THEME_CYBER:     return "CYBER";
        case THEME_SUNSET:    return "SUNSET";
        case THEME_OCEAN:     return "OCEAN";
        case THEME_FOREST:    return "FOREST";
        case THEME_ROYAL:     return "ROYAL";
        case THEME_CANDY:     return "CANDY";
        case THEME_STEEL:     return "STEEL";
        case THEME_FIRE:      return "FIRE";
        case THEME_GHOST:     return "GHOST";
        case THEME_VOID:      return "VOID";
        case THEME_AURORA:    return "AURORA";
        case THEME_OBSIDIAN:  return "OBSIDIAN";
        case THEME_NOVA:      return "NOVA";
        case THEME_DRAGON:    return "DRAGON";
        case THEME_COSMIC:    return "COSMIC";
        case THEME_TOXIC:     return "TOXIC";
        case THEME_INFERNO:   return "INFERNO";
        case THEME_ABYSS:     return "ABYSS";
        case THEME_CHROME:    return "CHROME";
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
            fps_icon_color = COLOR_WHITE;
            cpu_icon_color = COLOR_GREEN;
            bus_icon_color = COLOR_GREEN;
            gpu_icon_color = COLOR_GREEN;
            app_icon_color = COLOR_GREEN;
            ram_icon_color = COLOR_GREEN;
            hud_box_enabled = 1;
            hud_box_bg_color = BG_BLACK;
            hud_layout = LAYOUT_SINGLE;
            break;

        case HUD_THEME_NEON:
            hud_text_color = COLOR_CYAN;
            hud_shadow_color = COLOR_BLACK;
            hud_icon_color = COLOR_MAGENTA;
            clock_icon_color = COLOR_CYAN;
            fps_icon_color = COLOR_CYAN;
            cpu_icon_color = COLOR_CYAN;
            bus_icon_color = COLOR_AQUA;
            gpu_icon_color = COLOR_MAGENTA;
            app_icon_color = COLOR_ELECTRIC_BLUE;
            ram_icon_color = COLOR_LIME;
            hud_box_enabled = 1;
            hud_box_bg_color = BG_PURPLE;
            hud_layout = LAYOUT_SINGLE;
            break;

        case HUD_THEME_MATRIX:
            hud_text_color = COLOR_LIME;
            hud_shadow_color = COLOR_BLACK;
            hud_icon_color = COLOR_GREEN;
            clock_icon_color = COLOR_LIME;
            fps_icon_color = COLOR_LIME;
            hud_box_enabled = 1;
            hud_box_bg_color = BG_BLACK;
            hud_layout = LAYOUT_COMPACT;
            break;

        case HUD_THEME_GOLD:
            hud_text_color = COLOR_GOLD;
            hud_shadow_color = COLOR_BLACK;
            hud_icon_color = COLOR_GOLD;
            clock_icon_color = COLOR_AMBER;
            fps_icon_color = COLOR_AMBER;
            hud_box_enabled = 1;
            hud_box_bg_color = BG_BLACK;
            hud_layout = LAYOUT_SINGLE;
            break;

        case HUD_THEME_ICE:
            hud_text_color = COLOR_SKY;
            hud_shadow_color = COLOR_NAVY;
            hud_icon_color = COLOR_AQUA;
            clock_icon_color = COLOR_SKY;
            fps_icon_color = COLOR_SKY;
            cpu_icon_color = COLOR_AQUA;
            bus_icon_color = COLOR_SKY;
            gpu_icon_color = COLOR_ELECTRIC_BLUE;
            app_icon_color = COLOR_CYAN;
            ram_icon_color = COLOR_MINT;
            hud_box_enabled = 1;
            hud_box_bg_color = BG_NAVY;
            hud_layout = LAYOUT_SINGLE;
            break;

        case HUD_THEME_BLOOD:
            hud_text_color = COLOR_RED;
            hud_shadow_color = COLOR_BLACK;
            hud_icon_color = COLOR_CRIMSON;
            clock_icon_color = COLOR_RED;
            fps_icon_color = COLOR_RED;
            cpu_icon_color = COLOR_CRIMSON;
            bus_icon_color = COLOR_RED;
            gpu_icon_color = COLOR_MAROON;
            app_icon_color = COLOR_ROSE;
            ram_icon_color = COLOR_CRIMSON;
            hud_box_enabled = 1;
            hud_box_bg_color = BG_MAROON;
            hud_layout = LAYOUT_SINGLE;
            break;

        case HUD_THEME_CLEAN:
            hud_text_color = COLOR_WHITE;
            hud_shadow_color = COLOR_BLACK;
            hud_icon_color = COLOR_AUTO;
            clock_icon_color = COLOR_WHITE;
            fps_icon_color = COLOR_WHITE;
            cpu_icon_color = COLOR_AUTO;
            bus_icon_color = COLOR_AUTO;
            gpu_icon_color = COLOR_AUTO;
            app_icon_color = COLOR_AUTO;
            ram_icon_color = COLOR_AUTO;
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
            fps_icon_color = COLOR_AUTO;
            cpu_icon_color = COLOR_AUTO;
            bus_icon_color = COLOR_AUTO;
            gpu_icon_color = COLOR_AUTO;
            app_icon_color = COLOR_AUTO;
            ram_icon_color = COLOR_AUTO;
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

static const char *combo_name_for(int combo_mode) {
    switch (combo_mode) {
        case TOGGLE_UP: return "L+R+UP";
        case TOGGLE_DOWN: return "L+R+DOWN";
        case TOGGLE_LEFT: return "L+R+LEFT";
        case TOGGLE_RIGHT: return "L+R+RIGHT";
        case TOGGLE_START: return "L+R+START";
        case TOGGLE_TRIANGLE: return "L+R+TRIANGLE";
        case TOGGLE_CIRCLE: return "L+R+CIRCLE";
        case TOGGLE_SQUARE: return "L+R+SQUARE";
        case TOGGLE_CROSS: return "L+R+CROSS";
        case TOGGLE_SELECT_UP: return "SELECT+UP";
        case TOGGLE_SELECT_DOWN: return "SELECT+DOWN";
        case TOGGLE_START_UP: return "START+UP";
        case TOGGLE_START_DOWN: return "START+DOWN";
        case TOGGLE_L_SELECT: return "L+SELECT";
        case TOGGLE_R_SELECT: return "R+SELECT";
        case TOGGLE_L_START: return "L+START";
        case TOGGLE_R_START: return "R+START";
        case TOGGLE_SELECT:
        default: return "L+R+SELECT";
    }
}

static const char *toggle_name(void) { return combo_name_for(toggle_combo_mode); }
static const char *opacity_name_for(int id) { if (id == OPACITY_75) return "75%"; if (id == OPACITY_50) return "50%"; if (id == OPACITY_25) return "25%"; return "100%"; }

static const char *percent_name_for(int value) {
    static char buf[8];
    int pos = 0;
    if (value < 0) value = 0;
    if (value > 999) value = 999;
    pos = append_int(buf, pos, value);
    buf[pos++] = '%';
    buf[pos] = '\0';
    return buf;
}

static const char *icon_style_name_for(int id) {
    switch (id) {
        case ICON_STYLE_OUTLINE: return "OUTLINE";
        case ICON_STYLE_SOLID: return "SOLID";
        case ICON_STYLE_DIAMOND: return "DIAMOND";
        case ICON_STYLE_CROSS: return "CROSS";
        case ICON_STYLE_DOTS: return "DOTS";
        case ICON_STYLE_CHEVRON: return "CHEVRON";
        case ICON_STYLE_MINI: return "MINI";
        case ICON_STYLE_BARS: return "BARS";
        case ICON_STYLE_TARGET: return "TARGET";
        case ICON_STYLE_STARBIRD: return "STARBIRD";
        case ICON_STYLE_IMPERIAL: return "IMPERIAL";
        case ICON_STYLE_JEDI: return "JEDI";
        case ICON_STYLE_SITH: return "SITH";
        case ICON_STYLE_SABER: return "LIGHTSABER";
        case ICON_STYLE_SKULL: return "SKULL";
        case ICON_STYLE_CROWN: return "CROWN";
        case ICON_STYLE_GEM: return "GEM";
        case ICON_STYLE_FLAME: return "FLAME";
        case ICON_STYLE_ORBIT: return "ORBIT";
        case ICON_STYLE_PHOENIX: return "PHOENIX";
        case ICON_STYLE_ECLIPSE: return "ECLIPSE";
        case ICON_STYLE_NEXUS: return "NEXUS";
        case ICON_STYLE_VIPER: return "VIPER";
        case ICON_STYLE_RUNE: return "RUNE";
        case ICON_STYLE_HALO: return "HALO";
        case ICON_STYLE_PRISM: return "PRISM";
        case ICON_STYLE_COMET: return "COMET";
        case ICON_STYLE_SIGIL: return "SIGIL";
        case ICON_STYLE_VORTEX: return "VORTEX";
        case ICON_STYLE_ORIGINAL:
        default: return "ORIGINAL";
    }
}
static const char *fps_style_name(void) { if (fps_display_style == FPS_STYLE_NUMBER_ONLY) return "NUMBER ONLY"; if (fps_display_style == FPS_STYLE_ICON_ONLY) return "ICON ONLY"; if (fps_display_style == FPS_STYLE_LABEL_NUMBER) return "FPS + NUMBER"; return "ICON + NUMBER"; }
static const char *battery_style_name(void) { if (battery_display_style == BATTERY_STYLE_PERCENT_ONLY) return "PERCENT ONLY"; if (battery_display_style == BATTERY_STYLE_ICON_ONLY) return "ICON ONLY"; if (battery_display_style == BATTERY_STYLE_BAR_ONLY) return "BAR ONLY"; return "ICON + PERCENT"; }
static const char *clock_style_name(void) { if (clock_display_style == CLOCK_STYLE_TIME_ONLY) return "TIME ONLY"; if (clock_display_style == CLOCK_STYLE_ICON_ONLY) return "ICON ONLY"; if (clock_display_style == CLOCK_STYLE_DATE_TIME) return "DATE + TIME"; return "ICON + TIME"; }

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

static const char *layout_name_for(int id) {
    switch (id) {
        case LAYOUT_COMPACT: return "COMPACT";
        case LAYOUT_STACKED: return "STACKED";
        case LAYOUT_ICONS:   return "ICONS";
        case LAYOUT_SINGLE:
        default:             return "SINGLE";
    }
}

static const char *size_name_for(int id) {
    switch (id) {
        case SIZE_MICRO:  return "MICRO";
        case SIZE_NORMAL: return "NORMAL";
        case SIZE_LARGE:
        default:          return "LARGE";
    }
}

static const char *main_menu_size_name(void) {
    switch (main_menu_size) {
        case MAIN_MENU_SIZE_COMPACT: return "COMPACT";
        case MAIN_MENU_SIZE_LARGE:   return "LARGE";
        case MAIN_MENU_SIZE_DEFAULT:
        default:                     return "DEFAULT";
    }
}

static const char *main_menu_size_name_for(int id) {
    switch (id) {
        case MAIN_MENU_SIZE_COMPACT: return "COMPACT";
        case MAIN_MENU_SIZE_LARGE:   return "LARGE";
        case MAIN_MENU_SIZE_DEFAULT:
        default:                     return "DEFAULT";
    }
}

static const char *font_name_for(int id) {
    switch (id) {
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

static const char *language_name_for(int id) {
    switch (id) {
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

static const char *theme_name_for(int id) {
    switch (id) {
        case THEME_VITASHELL: return "VITASHELL";
        case THEME_PSP:       return "PSP";
        case THEME_MATRIX:    return "MATRIX";
        case THEME_NEON:      return "NEON";
        case THEME_RETRO:     return "RETRO";
        case THEME_MINIMAL:   return "MINIMAL";
        case THEME_PLASMA:    return "PLASMA";
        case THEME_CYBER:     return "CYBER";
        case THEME_SUNSET:    return "SUNSET";
        case THEME_OCEAN:     return "OCEAN";
        case THEME_FOREST:    return "FOREST";
        case THEME_ROYAL:     return "ROYAL";
        case THEME_CANDY:     return "CANDY";
        case THEME_STEEL:     return "STEEL";
        case THEME_FIRE:      return "FIRE";
        case THEME_GHOST:     return "GHOST";
        case THEME_VOID:      return "VOID";
        case THEME_AURORA:    return "AURORA";
        case THEME_OBSIDIAN:  return "OBSIDIAN";
        case THEME_NOVA:      return "NOVA";
        case THEME_DRAGON:    return "DRAGON";
        case THEME_COSMIC:    return "COSMIC";
        case THEME_TOXIC:     return "TOXIC";
        case THEME_INFERNO:   return "INFERNO";
        case THEME_ABYSS:     return "ABYSS";
        case THEME_CHROME:    return "CHROME";
        case THEME_DEFAULT:
        default:              return "DEFAULT";
    }
}

static const char *hud_theme_name_for(int id) {
    switch (id) {
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

static int item_uses_color_menu(int item) {
    return item == ITEM_HUD_TEXT || item == ITEM_HUD_SHADOW || item == ITEM_HUD_ICON ||
           item == ITEM_CLOCK_ICON || item == ITEM_DATE_ICON || item == ITEM_FPS_ICON || item == ITEM_CPU_ICON || item == ITEM_BUS_ICON ||
           item == ITEM_GPU_ICON || item == ITEM_APP_ICON || item == ITEM_RAM_ICON ||
           item == ITEM_MENU_TEXT || item == ITEM_MENU_SELECT ||
           item == ITEM_MENU_BORDER || item == ITEM_TOP_BAR || item == ITEM_DEBUG_COLOR;
}

static int item_uses_bg_menu(int item) {
    return item == ITEM_HUD_BOX_BG || item == ITEM_MENUBG;
}

static int item_uses_combo_menu(int item) {
    return item == ITEM_TOGGLE_HUD_COMBO || item == ITEM_TOGGLE_MENU_COMBO ||
           item == ITEM_CYCLE_PROFILE_COMBO || item == ITEM_CYCLE_THEME_COMBO;
}

static int item_uses_icon_style_menu(int item) {
    return item == ITEM_FPS_ICON_STYLE || item == ITEM_BATTERY_ICON_STYLE ||
           item == ITEM_CLOCK_ICON_STYLE || item == ITEM_DATE_ICON_STYLE ||
           item == ITEM_CPU_ICON_STYLE || item == ITEM_BUS_ICON_STYLE ||
           item == ITEM_GPU_ICON_STYLE || item == ITEM_RAM_ICON_STYLE ||
           item == ITEM_APP_ICON_STYLE;
}

static int item_uses_choice_menu(int item) {
    return item == ITEM_LAYOUT || item == ITEM_SIZE || item == ITEM_MENU_SIZE ||
           item == ITEM_FONT || item == ITEM_LANGUAGE || item == ITEM_THEME ||
           item == ITEM_HUD_THEME || item == ITEM_DEBUG_POSITION || item == ITEM_DEBUG_SIZE || item == ITEM_DEBUG_FONT ||
           item == ITEM_HUD_OPACITY || item == ITEM_MENU_OPACITY ||
           item == ITEM_FPS_STYLE || item == ITEM_BATTERY_STYLE || item == ITEM_CLOCK_STYLE ||
           item_uses_combo_menu(item) || item_uses_icon_style_menu(item) || item_uses_color_menu(item) || item_uses_bg_menu(item);
}

static int choice_count_for_target(int target) {
    if (item_uses_color_menu(target)) return COLOR_COUNT;
    if (item_uses_bg_menu(target)) return BG_COUNT;
    if (item_uses_combo_menu(target)) return TOGGLE_COUNT;
    if (item_uses_icon_style_menu(target)) return ICON_STYLE_COUNT;

    switch (target) {
        case ITEM_LAYOUT:    return LAYOUT_COUNT;
        case ITEM_SIZE:      return SIZE_COUNT;
        case ITEM_MENU_SIZE: return MAIN_MENU_SIZE_COUNT;
        case ITEM_FONT:      return FONT_COUNT;
        case ITEM_LANGUAGE:  return LANG_COUNT;
        case ITEM_THEME:     return THEME_COUNT;
        case ITEM_HUD_THEME: return HUD_THEME_COUNT;
        case ITEM_DEBUG_POSITION: return POS_COUNT;
        case ITEM_DEBUG_SIZE: return SIZE_COUNT;
        case ITEM_DEBUG_FONT: return FONT_COUNT;
        case ITEM_HUD_OPACITY: return OPACITY_COUNT;
        case ITEM_MENU_OPACITY: return OPACITY_COUNT;
        case ITEM_FPS_STYLE: return FPS_STYLE_COUNT;
        case ITEM_BATTERY_STYLE: return BATTERY_STYLE_COUNT;
        case ITEM_CLOCK_STYLE: return CLOCK_STYLE_COUNT;
        default:             return 1;
    }
}

static int choice_current_index_for_target(int target) {
    if (item_uses_color_menu(target)) {
        switch (target) {
            case ITEM_HUD_TEXT:    return hud_text_color;
            case ITEM_HUD_SHADOW:  return hud_shadow_color;
            case ITEM_HUD_ICON:    return hud_icon_color;
            case ITEM_CLOCK_ICON:  return clock_icon_color;
            case ITEM_DATE_ICON:   return date_icon_color;
            case ITEM_FPS_ICON:    return fps_icon_color;
            case ITEM_CPU_ICON:    return cpu_icon_color;
            case ITEM_BUS_ICON:    return bus_icon_color;
            case ITEM_GPU_ICON:    return gpu_icon_color;
            case ITEM_APP_ICON:    return app_icon_color;
            case ITEM_RAM_ICON:    return ram_icon_color;
            case ITEM_MENU_TEXT:   return menu_text_color;
            case ITEM_MENU_SELECT: return menu_select_color;
            case ITEM_MENU_BORDER: return menu_border_color;
            case ITEM_TOP_BAR:     return top_menu_bar_color;
            case ITEM_DEBUG_COLOR: return debug_text_color;
            default: return 0;
        }
    }

    if (item_uses_bg_menu(target)) {
        return target == ITEM_HUD_BOX_BG ? hud_box_bg_color : menu_bg_color;
    }

    if (item_uses_combo_menu(target)) {
        if (target == ITEM_TOGGLE_HUD_COMBO) return toggle_combo_mode;
        if (target == ITEM_TOGGLE_MENU_COMBO) return menu_toggle_combo_mode;
        if (target == ITEM_CYCLE_PROFILE_COMBO) return cycle_profile_combo_mode;
        if (target == ITEM_CYCLE_THEME_COMBO) return cycle_theme_combo_mode;
    }

    if (item_uses_icon_style_menu(target)) {
        if (target == ITEM_FPS_ICON_STYLE) return fps_icon_style;
        if (target == ITEM_BATTERY_ICON_STYLE) return battery_icon_style;
        if (target == ITEM_CLOCK_ICON_STYLE) return clock_icon_style;
        if (target == ITEM_DATE_ICON_STYLE) return date_icon_style;
        if (target == ITEM_CPU_ICON_STYLE) return cpu_icon_style;
        if (target == ITEM_BUS_ICON_STYLE) return bus_icon_style;
        if (target == ITEM_GPU_ICON_STYLE) return gpu_icon_style;
        if (target == ITEM_RAM_ICON_STYLE) return ram_icon_style;
        if (target == ITEM_APP_ICON_STYLE) return app_icon_style;
    }

    switch (target) {
        case ITEM_LAYOUT:    return hud_layout;
        case ITEM_SIZE:      return hud_size;
        case ITEM_MENU_SIZE: return main_menu_size;
        case ITEM_FONT:      return font_style;
        case ITEM_LANGUAGE:  return hud_language;
        case ITEM_THEME:     return theme_id;
        case ITEM_HUD_THEME: return hud_theme_id;
        case ITEM_DEBUG_POSITION: return debug_position;
        case ITEM_DEBUG_SIZE: return debug_size;
        case ITEM_DEBUG_FONT: return debug_font_style;
        case ITEM_HUD_OPACITY: return hud_opacity;
        case ITEM_MENU_OPACITY: return menu_opacity;
        case ITEM_FPS_STYLE: return fps_display_style;
        case ITEM_BATTERY_STYLE: return battery_display_style;
        case ITEM_CLOCK_STYLE: return clock_display_style;
        default:             return 0;
    }
}

static const char *choice_name_for_target(int target, int index) {
    if (item_uses_combo_menu(target)) return combo_name_for(index);
    if (item_uses_icon_style_menu(target)) return icon_style_name_for(index);
    if (target == ITEM_DEBUG_POSITION) return position_name_for(index);
    if (target == ITEM_DEBUG_SIZE) return size_name_for(index);
    if (target == ITEM_DEBUG_FONT) return font_name_for(index);
    if (target == ITEM_HUD_OPACITY || target == ITEM_MENU_OPACITY) return opacity_name_for(index);
    if (target == ITEM_FPS_STYLE) { if (index == FPS_STYLE_NUMBER_ONLY) return "NUMBER ONLY"; if (index == FPS_STYLE_ICON_ONLY) return "ICON ONLY"; if (index == FPS_STYLE_LABEL_NUMBER) return "FPS + NUMBER"; return "ICON + NUMBER"; }
    if (target == ITEM_BATTERY_STYLE) { if (index == BATTERY_STYLE_PERCENT_ONLY) return "PERCENT ONLY"; if (index == BATTERY_STYLE_ICON_ONLY) return "ICON ONLY"; if (index == BATTERY_STYLE_BAR_ONLY) return "BAR ONLY"; return "ICON + PERCENT"; }
    if (target == ITEM_CLOCK_STYLE) { if (index == CLOCK_STYLE_TIME_ONLY) return "TIME ONLY"; if (index == CLOCK_STYLE_ICON_ONLY) return "ICON ONLY"; if (index == CLOCK_STYLE_DATE_TIME) return "DATE + TIME"; return "ICON + TIME"; }
    if (item_uses_color_menu(target)) return color_name_generic(index);
    if (item_uses_bg_menu(target)) return menu_bg_name_for(index);

    switch (target) {
        case ITEM_LAYOUT:    return layout_name_for(index);
        case ITEM_SIZE:      return size_name_for(index);
        case ITEM_MENU_SIZE: return main_menu_size_name_for(index);
        case ITEM_FONT:      return font_name_for(index);
        case ITEM_LANGUAGE:  return language_name_for(index);
        case ITEM_THEME:     return theme_name_for(index);
        case ITEM_HUD_THEME: return hud_theme_name_for(index);
        default:             return "";
    }
}

static void set_choice_for_target(int target, int index) {
    if (item_uses_color_menu(target)) {
        if (index < 0) index = 0;
        if (index >= COLOR_COUNT) index = COLOR_COUNT - 1;

        switch (target) {
            case ITEM_HUD_TEXT:    hud_text_color = index; break;
            case ITEM_HUD_SHADOW:  hud_shadow_color = index; break;
            case ITEM_HUD_ICON:    hud_icon_color = index; break;
            case ITEM_CLOCK_ICON:  clock_icon_color = index; break;
            case ITEM_DATE_ICON:   date_icon_color = index; break;
            case ITEM_FPS_ICON:    fps_icon_color = index; break;
            case ITEM_CPU_ICON:    cpu_icon_color = index; break;
            case ITEM_BUS_ICON:    bus_icon_color = index; break;
            case ITEM_GPU_ICON:    gpu_icon_color = index; break;
            case ITEM_APP_ICON:    app_icon_color = index; break;
            case ITEM_RAM_ICON:    ram_icon_color = index; break;
            case ITEM_MENU_TEXT:   menu_text_color = index; break;
            case ITEM_MENU_SELECT: menu_select_color = index; break;
            case ITEM_MENU_BORDER: menu_border_color = index; break;
            case ITEM_TOP_BAR:     top_menu_bar_color = index; break;
            case ITEM_DEBUG_COLOR: debug_text_color = index; break;
            default: break;
        }
        return;
    }

    if (item_uses_bg_menu(target)) {
        if (index < 0) index = 0;
        if (index >= BG_COUNT) index = BG_COUNT - 1;
        if (target == ITEM_HUD_BOX_BG) hud_box_bg_color = index;
        if (target == ITEM_MENUBG) menu_bg_color = index;
        return;
    }

    if (item_uses_combo_menu(target)) {
        if (index < 0) index = 0;
        if (index >= TOGGLE_COUNT) index = TOGGLE_COUNT - 1;
        if (target == ITEM_TOGGLE_HUD_COMBO) toggle_combo_mode = index;
        if (target == ITEM_TOGGLE_MENU_COMBO) menu_toggle_combo_mode = index;
        if (target == ITEM_CYCLE_PROFILE_COMBO) cycle_profile_combo_mode = index;
        if (target == ITEM_CYCLE_THEME_COMBO) cycle_theme_combo_mode = index;
        return;
    }

    if (item_uses_icon_style_menu(target)) {
        if (index < 0) index = 0;
        if (index >= ICON_STYLE_COUNT) index = ICON_STYLE_COUNT - 1;
        if (target == ITEM_FPS_ICON_STYLE) fps_icon_style = index;
        if (target == ITEM_BATTERY_ICON_STYLE) battery_icon_style = index;
        if (target == ITEM_CLOCK_ICON_STYLE) clock_icon_style = index;
        if (target == ITEM_DATE_ICON_STYLE) date_icon_style = index;
        if (target == ITEM_CPU_ICON_STYLE) cpu_icon_style = index;
        if (target == ITEM_BUS_ICON_STYLE) bus_icon_style = index;
        if (target == ITEM_GPU_ICON_STYLE) gpu_icon_style = index;
        if (target == ITEM_RAM_ICON_STYLE) ram_icon_style = index;
        if (target == ITEM_APP_ICON_STYLE) app_icon_style = index;
        return;
    }

    switch (target) {
        case ITEM_LAYOUT:
            hud_layout = index;
            if (hud_layout < 0) hud_layout = 0;
            if (hud_layout >= LAYOUT_COUNT) hud_layout = LAYOUT_COUNT - 1;
            break;
        case ITEM_SIZE:
            hud_size = index;
            if (hud_size < 0) hud_size = 0;
            if (hud_size >= SIZE_COUNT) hud_size = SIZE_COUNT - 1;
            break;
        case ITEM_MENU_SIZE:
            main_menu_size = index;
            if (main_menu_size < 0) main_menu_size = 0;
            if (main_menu_size >= MAIN_MENU_SIZE_COUNT) main_menu_size = MAIN_MENU_SIZE_COUNT - 1;
            break;
        case ITEM_FONT:
            font_style = index;
            if (font_style < 0) font_style = 0;
            if (font_style >= FONT_COUNT) font_style = FONT_COUNT - 1;
            break;
        case ITEM_LANGUAGE:
            hud_language = index;
            if (hud_language < 0) hud_language = 0;
            if (hud_language >= LANG_COUNT) hud_language = LANG_COUNT - 1;
            break;
        case ITEM_THEME:
            theme_id = index;
            if (theme_id < 0) theme_id = 0;
            if (theme_id >= THEME_COUNT) theme_id = THEME_COUNT - 1;
            apply_theme();
            break;
        case ITEM_HUD_THEME:
            hud_theme_id = index;
            if (hud_theme_id < 0) hud_theme_id = 0;
            if (hud_theme_id >= HUD_THEME_COUNT) hud_theme_id = HUD_THEME_COUNT - 1;
            apply_hud_theme();
            break;
        case ITEM_DEBUG_POSITION: debug_position = index; break;
        case ITEM_DEBUG_SIZE: debug_size = index; break;
        case ITEM_DEBUG_FONT: debug_font_style = index; break;
        case ITEM_HUD_OPACITY: hud_opacity = index; break;
        case ITEM_MENU_OPACITY: menu_opacity = index; break;
        case ITEM_FPS_STYLE: fps_display_style = index; break;
        case ITEM_BATTERY_STYLE: battery_display_style = index; break;
        case ITEM_CLOCK_STYLE: clock_display_style = index; break;
        default:
            break;
    }
}

static void save_current_menu_page_state(void) {
    if (menu_page == MENU_PAGE_MAIN) {
        saved_main_index = menu_index;
        saved_main_scroll = menu_scroll;
    } else if (menu_page == MENU_PAGE_PROFILE) {
        saved_profile_index = menu_index;
        saved_profile_scroll = menu_scroll;
    } else if (menu_page == MENU_PAGE_THEME) {
        saved_theme_index = menu_index;
        saved_theme_scroll = menu_scroll;
    } else if (menu_page == MENU_PAGE_HUD_ORDER) {
        saved_hud_order_index = menu_index;
        saved_hud_order_scroll = menu_scroll;
    } else if (menu_page == MENU_PAGE_OVERLAYS) {
        saved_overlays_index = menu_index;
        saved_overlays_scroll = menu_scroll;
    } else if (menu_page == MENU_PAGE_SIZE) {
        saved_size_index = menu_index;
        saved_size_scroll = menu_scroll;
    } else if (menu_page == MENU_PAGE_HUD_COLORS) {
        saved_hud_colors_index = menu_index;
        saved_hud_colors_scroll = menu_scroll;
    } else if (menu_page == MENU_PAGE_MENU_COLORS) {
        saved_menu_colors_index = menu_index;
        saved_menu_colors_scroll = menu_scroll;
    } else if (menu_page == MENU_PAGE_ICON_COLORS) {
        saved_icon_colors_index = menu_index;
        saved_icon_colors_scroll = menu_scroll;
    } else if (menu_page == MENU_PAGE_HUD_MENU) {
        saved_hud_menu_index = menu_index;
        saved_hud_menu_scroll = menu_scroll;
    } else if (menu_page == MENU_PAGE_PERFORMANCE) {
        saved_performance_index = menu_index;
        saved_performance_scroll = menu_scroll;
    } else if (menu_page == MENU_PAGE_RESET_OPTIONS) {
        saved_reset_options_index = menu_index;
        saved_reset_options_scroll = menu_scroll;
    } else if (menu_page == MENU_PAGE_DEBUG) {
        saved_debug_index = menu_index;
        saved_debug_scroll = menu_scroll;
    } else if (menu_page == MENU_PAGE_TOGGLE_COMBOS) {
        saved_toggle_index = menu_index;
        saved_toggle_scroll = menu_scroll;
    } else if (menu_page == MENU_PAGE_ICON_CHANGER) {
        saved_icon_changer_index = menu_index;
        saved_icon_changer_scroll = menu_scroll;
    }
}

static void clear_menu_repeat_state(void) {
    hold_up_frames = 0;
    hold_down_frames = 0;
    menu_nav_dir = 0;
    menu_nav_hold_start_tick = 0;
    menu_nav_next_repeat_tick = 0;
    menu_lr_dir = 0;
    menu_lr_hold_start_tick = 0;
    menu_lr_next_repeat_tick = 0;
}

static void enter_choice_menu(int target) {
    int cur;

    save_current_menu_page_state();

    choice_target_item = target;
    choice_return_page = menu_page;
    menu_page = MENU_PAGE_CHOICE;
    menu_scroll = 0;
    cur = choice_current_index_for_target(target);
    if (cur < 0) cur = 0;
    if (cur >= choice_count_for_target(target)) cur = choice_count_for_target(target) - 1;
    menu_index = cur;
    clear_menu_repeat_state();
}

static int current_menu_count(void) {
    switch (menu_page) {
        case MENU_PAGE_PROFILE:
            return 3;
        case MENU_PAGE_THEME:
            return 10;
        case MENU_PAGE_HUD_COLORS:
            return 4;
        case MENU_PAGE_MENU_COLORS:
            return 5;
        case MENU_PAGE_ICON_COLORS:
            return 9;
        case MENU_PAGE_PERFORMANCE:
            return 5;
        case MENU_PAGE_RESET_OPTIONS:
            return 5;
        case MENU_PAGE_DEBUG:
            return 14;
        case MENU_PAGE_TOGGLE_COMBOS:
            return 6;
        case MENU_PAGE_ICON_CHANGER:
            return 9;
        case MENU_PAGE_HUD_MENU:
            return 6;
        case MENU_PAGE_HUD_ORDER:
            return HUD_ORDER_COUNT;
        case MENU_PAGE_OVERLAYS:
            return 9;
        case MENU_PAGE_SIZE:
            return 7;
        case MENU_PAGE_CREATE_HUD_SIZE:
            return 1;
        case MENU_PAGE_CREATE_MAIN_SIZE:
            return 1;
        case MENU_PAGE_CHOICE:
            return choice_count_for_target(choice_target_item);
        case MENU_PAGE_MAIN:
        default:
            return 12;
    }
}

static int current_menu_item_at(int index) {
    static const int main_items[12] = {
        ITEM_HUD,
        ITEM_HUD_MENU,
        ITEM_DEBUG_MENU,
        ITEM_PROFILE_MENU,
        ITEM_ALL_HUD_OVERLAYS_MENU,
        ITEM_THEME_MENU,
        ITEM_ICON_CHANGER_MENU,
        ITEM_PERFORMANCE_ALERTS_MENU,
        ITEM_MENU_HUD_SIZE_MENU,
        ITEM_LANGUAGE,
        ITEM_TOGGLE,
        ITEM_RESET
    };

    static const int profile_items[3] = {
        ITEM_PROFILE,
        ITEM_SAVE_PROFILE,
        ITEM_LOAD_PROFILE
    };

    static const int theme_items[10] = {
        ITEM_THEME,
        ITEM_FONT,
        ITEM_DEBUG_FONT,
        ITEM_DEBUG_COLOR,
        ITEM_HUD_COLORS_MENU,
        ITEM_MENU_COLORS_MENU,
        ITEM_ICON_COLORS_MENU,
        ITEM_HUD_OPACITY,
        ITEM_MENU_OPACITY,
        ITEM_MENU_PICTURE_BG
    };

    static const int hud_color_items[4] = {
        ITEM_HUD_TEXT,
        ITEM_HUD_SHADOW,
        ITEM_HUD_BOX,
        ITEM_HUD_BOX_BG
    };

    static const int menu_color_items[5] = {
        ITEM_MENU_TEXT,
        ITEM_MENU_SELECT,
        ITEM_MENU_BORDER,
        ITEM_MENUBG,
        ITEM_TOP_BAR
    };

    static const int icon_color_items[9] = {
        ITEM_FPS_ICON,
        ITEM_HUD_ICON,
        ITEM_CLOCK_ICON,
        ITEM_DATE_ICON,
        ITEM_CPU_ICON,
        ITEM_BUS_ICON,
        ITEM_GPU_ICON,
        ITEM_RAM_ICON,
        ITEM_APP_ICON
    };

    static const int icon_changer_items[9] = {
        ITEM_FPS_ICON_STYLE,
        ITEM_BATTERY_ICON_STYLE,
        ITEM_CLOCK_ICON_STYLE,
        ITEM_DATE_ICON_STYLE,
        ITEM_CPU_ICON_STYLE,
        ITEM_BUS_ICON_STYLE,
        ITEM_GPU_ICON_STYLE,
        ITEM_RAM_ICON_STYLE,
        ITEM_APP_ICON_STYLE
    };

    static const int overlay_items[9] = {
        ITEM_FPS,
        ITEM_BATTERY,
        ITEM_TIME,
        ITEM_DATE_HUD,
        ITEM_CPU_HUD,
        ITEM_BUS_HUD,
        ITEM_GPU_HUD,
        ITEM_RAM_HUD,
        ITEM_APP_ID_HUD
    };

    static const int size_items[7] = {
        ITEM_FPS_STYLE,
        ITEM_BATTERY_STYLE,
        ITEM_CLOCK_STYLE,
        ITEM_SIZE,
        ITEM_MENU_SIZE,
        ITEM_CREATE_HUD_MENU_SIZE,
        ITEM_CREATE_MAIN_MENU_SIZE
    };

    static const int performance_items[5] = {
        ITEM_FPS_WARNING,
        ITEM_FPS_LOW_LIMIT,
        ITEM_BATTERY_WARNING,
        ITEM_BATTERY_LOW_LIMIT,
        ITEM_RAM_WARNING
    };

    static const int reset_items[5] = {
        ITEM_RESET_HUD_POSITION,
        ITEM_RESET_COLORS,
        ITEM_RESET_OVERLAYS,
        ITEM_RESET_PROFILES,
        ITEM_RESET_ALL_DEFAULTS
    };

    static const int hud_menu_items[6] = {
        ITEM_LAYOUT,
        ITEM_POSITION,
        ITEM_X_OFFSET,
        ITEM_Y_OFFSET,
        ITEM_HUD_ORDER_MENU,
        ITEM_TIMEMODE
    };

    static const int debug_items[14] = {
        ITEM_DEBUG_HUD,
        ITEM_DEBUG_POSITION,
        ITEM_DEBUG_X_OFFSET,
        ITEM_DEBUG_Y_OFFSET,
        ITEM_DEBUG_SIZE,
        ITEM_DEBUG_FRAMEBUF,
        ITEM_DEBUG_CACHE,
        ITEM_DEBUG_INPUT,
        ITEM_DEBUG_SYSTEM,
        ITEM_DEBUG_MENU_INFO,
        ITEM_DEBUG_PROFILE,
        ITEM_DEBUG_THEME,
        ITEM_DEBUG_ALERT,
        ITEM_DEBUG_HUD_INFO
    };

    static const int toggle_items[6] = {
        ITEM_TOGGLE_HUD_COMBO,
        ITEM_TOGGLE_MENU_COMBO,
        ITEM_CYCLE_PROFILE_ENABLE,
        ITEM_CYCLE_PROFILE_COMBO,
        ITEM_CYCLE_THEME_ENABLE,
        ITEM_CYCLE_THEME_COMBO
    };

    if (index < 0) index = 0;

    if (menu_page == MENU_PAGE_HUD_ORDER) {
        if (index >= HUD_ORDER_COUNT) index = HUD_ORDER_COUNT - 1;
        return ITEM_HUD_ORDER_BASE + index;
    }

    if (menu_page == MENU_PAGE_CHOICE) {
        if (index >= choice_count_for_target(choice_target_item)) index = choice_count_for_target(choice_target_item) - 1;
        return ITEM_CHOICE_BASE + index;
    }

    if (menu_page == MENU_PAGE_PROFILE) {
        if (index >= 3) index = 2;
        return profile_items[index];
    }

    if (menu_page == MENU_PAGE_THEME) {
        if (index >= 10) index = 9;
        return theme_items[index];
    }

    if (menu_page == MENU_PAGE_HUD_COLORS) {
        if (index >= 4) index = 3;
        return hud_color_items[index];
    }

    if (menu_page == MENU_PAGE_MENU_COLORS) {
        if (index >= 5) index = 4;
        return menu_color_items[index];
    }

    if (menu_page == MENU_PAGE_ICON_COLORS) {
        if (index >= 9) index = 8;
        return icon_color_items[index];
    }

    if (menu_page == MENU_PAGE_ICON_CHANGER) {
        if (index >= 9) index = 8;
        return icon_changer_items[index];
    }

    if (menu_page == MENU_PAGE_OVERLAYS) {
        if (index >= 9) index = 8;
        return overlay_items[index];
    }

    if (menu_page == MENU_PAGE_SIZE) {
        if (index >= 7) index = 6;
        return size_items[index];
    }

    if (menu_page == MENU_PAGE_CREATE_HUD_SIZE) {
        return ITEM_CREATE_HUD_MENU_SIZE;
    }

    if (menu_page == MENU_PAGE_CREATE_MAIN_SIZE) {
        return ITEM_CREATE_MAIN_MENU_SIZE;
    }

    if (menu_page == MENU_PAGE_HUD_MENU) {
        if (index >= 6) index = 5;
        return hud_menu_items[index];
    }

    if (menu_page == MENU_PAGE_PERFORMANCE) {
        if (index >= 5) index = 4;
        return performance_items[index];
    }

    if (menu_page == MENU_PAGE_RESET_OPTIONS) {
        if (index >= 5) index = 4;
        return reset_items[index];
    }

    if (menu_page == MENU_PAGE_DEBUG) {
        if (index >= 14) index = 13;
        return debug_items[index];
    }

    if (menu_page == MENU_PAGE_TOGGLE_COMBOS) {
        if (index >= 6) index = 5;
        return toggle_items[index];
    }

    if (index >= 12) index = 11;
    return main_items[index];
}

static int current_menu_item(void) {
    return current_menu_item_at(menu_index);
}

static void enter_menu_page(int page) {
    int count;

    save_current_menu_page_state();

    menu_page = page;

    if (page == MENU_PAGE_MAIN) {
        menu_index = saved_main_index;
        menu_scroll = saved_main_scroll;
    } else if (page == MENU_PAGE_PROFILE) {
        menu_index = saved_profile_index;
        menu_scroll = saved_profile_scroll;
    } else if (page == MENU_PAGE_THEME) {
        menu_index = saved_theme_index;
        menu_scroll = saved_theme_scroll;
    } else if (page == MENU_PAGE_HUD_ORDER) {
        menu_index = saved_hud_order_index;
        menu_scroll = saved_hud_order_scroll;
    } else if (page == MENU_PAGE_OVERLAYS) {
        menu_index = saved_overlays_index;
        menu_scroll = saved_overlays_scroll;
    } else if (page == MENU_PAGE_SIZE) {
        menu_index = saved_size_index;
        menu_scroll = saved_size_scroll;
    } else if (page == MENU_PAGE_HUD_COLORS) {
        menu_index = saved_hud_colors_index;
        menu_scroll = saved_hud_colors_scroll;
    } else if (page == MENU_PAGE_MENU_COLORS) {
        menu_index = saved_menu_colors_index;
        menu_scroll = saved_menu_colors_scroll;
    } else if (page == MENU_PAGE_ICON_COLORS) {
        menu_index = saved_icon_colors_index;
        menu_scroll = saved_icon_colors_scroll;
    } else if (page == MENU_PAGE_HUD_MENU) {
        menu_index = saved_hud_menu_index;
        menu_scroll = saved_hud_menu_scroll;
    } else if (page == MENU_PAGE_PERFORMANCE) {
        menu_index = saved_performance_index;
        menu_scroll = saved_performance_scroll;
    } else if (page == MENU_PAGE_RESET_OPTIONS) {
        menu_index = saved_reset_options_index;
        menu_scroll = saved_reset_options_scroll;
    } else if (page == MENU_PAGE_DEBUG) {
        menu_index = saved_debug_index;
        menu_scroll = saved_debug_scroll;
    } else if (page == MENU_PAGE_TOGGLE_COMBOS) {
        menu_index = saved_toggle_index;
        menu_scroll = saved_toggle_scroll;
    } else if (page == MENU_PAGE_ICON_CHANGER) {
        menu_index = saved_icon_changer_index;
        menu_scroll = saved_icon_changer_scroll;
    } else {
        menu_index = 0;
        menu_scroll = 0;
    }

    count = current_menu_count();
    if (count <= 0) {
        menu_index = 0;
        menu_scroll = 0;
    } else {
        if (menu_index < 0) menu_index = 0;
        if (menu_index >= count) menu_index = count - 1;
        if (menu_scroll < 0) menu_scroll = 0;
        if (menu_scroll > menu_index) menu_scroll = menu_index;
    }

    clear_menu_repeat_state();
}


static const char *translated_menu_label_for_language(int lang, int item) {
    switch (lang) {
        case LANG_ES:
            switch (item) {
                case ITEM_HUD: return "HUD";
                case ITEM_LAYOUT: return "DISENO";
                case ITEM_POSITION: return "POSICION";
                case ITEM_X_OFFSET: return "AJUSTE X";
                case ITEM_Y_OFFSET: return "AJUSTE Y";
                case ITEM_SIZE: return "TAMANO HUD";
                case ITEM_MENU_SIZE: return "TAMANO MENU";
                case ITEM_FONT: return "FUENTE";
                case ITEM_FPS: return "HUD FPS";
                case ITEM_BATTERY: return "HUD BATERIA";
                case ITEM_TIME: return "HUD RELOJ";
                case ITEM_DATE_HUD: return "HUD FECHA";
                case ITEM_TIMEMODE: return "MODO HORA";
                case ITEM_THEME_MENU: return "TEMA / COLOR / FUENTE";
                case ITEM_HUD_COLORS_MENU: return "COLORES HUD";
                case ITEM_MENU_COLORS_MENU: return "COLORES MENU";
                case ITEM_ICON_COLORS_MENU: return "COLORES ICONOS";
                case ITEM_PERFORMANCE_ALERTS_MENU: return "ALERTAS RENDIMIENTO";
                case ITEM_ALL_HUD_OVERLAYS_MENU: return "TODOS LOS HUD";
                case ITEM_MENU_HUD_SIZE_MENU: return "ESTILOS / MENU / TAMANOS HUD";
                case ITEM_THEME: return "PRESET TEMA";
                case ITEM_HUD_THEME: return "TEMA HUD";
                case ITEM_HUD_TEXT: return "TEXTO HUD";
                case ITEM_HUD_SHADOW: return "SOMBRA HUD";
                case ITEM_HUD_ICON: return "ICONO BATERIA";
                case ITEM_CLOCK_ICON: return "ICONO RELOJ";
                case ITEM_DATE_ICON: return "ICONO FECHA";
                case ITEM_FPS_ICON: return "ICONO FPS";
                case ITEM_CPU_ICON: return "ICONO CPU";
                case ITEM_BUS_ICON: return "ICONO BUS";
                case ITEM_GPU_ICON: return "ICONO GPU";
                case ITEM_APP_ICON: return "ICONO APP ID";
                case ITEM_RAM_ICON: return "ICONO RAM";
                case ITEM_HUD_BOX: return "CAJA HUD";
                case ITEM_HUD_BOX_BG: return "FONDO HUD";
                case ITEM_MENU_TEXT: return "TEXTO MENU";
                case ITEM_MENU_SELECT: return "SELECCION MENU";
                case ITEM_MENU_BORDER: return "BORDE MENU";
                case ITEM_MENUBG: return "FONDO MENU";
                case ITEM_TOP_BAR: return "BARRA SUPERIOR";
                case ITEM_MENU_PICTURE_BG: return "FONDO IMAGEN";
                case ITEM_PROFILE_MENU: return "MENU PERFIL";
                case ITEM_PROFILE: return "RANURA PERFIL";
                case ITEM_LANGUAGE: return "IDIOMA";
                case ITEM_AUTO_HIDE: return "AUTO OCULTAR";
                case ITEM_TOGGLE: return "COMBOS ACTIVAR";
                case ITEM_TOGGLE_HUD_COMBO: return "COMBO HUD";
                case ITEM_TOGGLE_MENU_COMBO: return "COMBO MENU";
                case ITEM_CYCLE_PROFILE_ENABLE: return "CICLAR PERFIL";
                case ITEM_CYCLE_PROFILE_COMBO: return "COMBO PERFIL";
                case ITEM_CYCLE_THEME_ENABLE: return "CICLAR TEMA";
                case ITEM_CYCLE_THEME_COMBO: return "COMBO TEMA";
                case ITEM_DEBUG_MENU: return "MENU DEBUG";
                case ITEM_DEBUG_HUD: return "DEBUG HUD";
                case ITEM_DEBUG_POSITION: return "POSICION DEBUG";
                case ITEM_DEBUG_X_OFFSET: return "AJUSTE X DEBUG";
                case ITEM_DEBUG_Y_OFFSET: return "AJUSTE Y DEBUG";
                case ITEM_DEBUG_SIZE: return "TAMANO DEBUG";
                case ITEM_DEBUG_FRAMEBUF: return "DEBUG FRAMEBUF";
                case ITEM_DEBUG_CACHE: return "DEBUG CACHE";
                case ITEM_DEBUG_INPUT: return "DEBUG ENTRADA";
                case ITEM_DEBUG_SYSTEM: return "DEBUG SISTEMA";
                case ITEM_HUD_OPACITY: return "OPACIDAD HUD";
                case ITEM_MENU_OPACITY: return "OPACIDAD MENU";
                case ITEM_FPS_STYLE: return "ESTILO FPS";
                case ITEM_BATTERY_STYLE: return "ESTILO BATERIA";
                case ITEM_CLOCK_STYLE: return "ESTILO RELOJ";
                case ITEM_SAVE_PROFILE: return "GUARDAR PERFIL";
                case ITEM_LOAD_PROFILE: return "CARGAR PERFIL";
                case ITEM_CPU_HUD: return "HUD CPU";
                case ITEM_BUS_HUD: return "HUD BUS";
                case ITEM_GPU_HUD: return "HUD GPU";
                case ITEM_APP_ID_HUD: return "HUD APP ID";
                case ITEM_RAM_HUD: return "HUD RAM";
                case ITEM_HUD_ORDER_MENU: return "ORDEN HUD";
                case ITEM_FPS_WARNING: return "ALERTA FPS";
                case ITEM_FPS_LOW_LIMIT: return "LIMITE BAJO FPS";
                case ITEM_BATTERY_WARNING: return "ALERTA BATERIA";
                case ITEM_BATTERY_LOW_LIMIT: return "LIMITE BAJO BATERIA";
                case ITEM_RAM_WARNING: return "ALERTA RAM";
                case ITEM_RESET_HUD_POSITION: return "REINICIAR POS HUD";
                case ITEM_RESET_COLORS: return "REINICIAR COLORES";
                case ITEM_RESET_OVERLAYS: return "REINICIAR HUDS";
                case ITEM_RESET_PROFILES: return "REINICIAR PERFILES";
                case ITEM_RESET_ALL_DEFAULTS: return "REINICIAR TODO";
                case ITEM_HUD_MENU: return "MENU HUD";
                case ITEM_DEBUG_FONT: return "FUENTE DEBUG";
                case ITEM_DEBUG_COLOR: return "COLORES DEBUG";
                case ITEM_ICON_CHANGER_MENU: return "MENU CAMBIAR ICONOS";
                case ITEM_FPS_ICON_STYLE: return "ICONO FPS";
                case ITEM_BATTERY_ICON_STYLE: return "ICONO BATERIA";
                case ITEM_CLOCK_ICON_STYLE: return "ICONO RELOJ";
                case ITEM_DATE_ICON_STYLE: return "ICONO FECHA";
                case ITEM_CPU_ICON_STYLE: return "ICONO CPU";
                case ITEM_BUS_ICON_STYLE: return "ICONO BUS";
                case ITEM_GPU_ICON_STYLE: return "ICONO GPU";
                case ITEM_RAM_ICON_STYLE: return "ICONO RAM";
                case ITEM_APP_ICON_STYLE: return "ICONO APP ID";
                case ITEM_CREATE_HUD_MENU_SIZE: return "CREAR TAMANO HUD";
                case ITEM_CREATE_MAIN_MENU_SIZE: return "CREAR TAMANO MENU";
                case ITEM_DEBUG_MENU_INFO: return "DEBUG MENU";
                case ITEM_DEBUG_PROFILE: return "DEBUG PERFIL";
                case ITEM_DEBUG_THEME: return "DEBUG TEMA";
                case ITEM_DEBUG_ALERT: return "DEBUG ALERTA";
                case ITEM_DEBUG_HUD_INFO: return "INFO DEBUG HUD";
                case ITEM_RESET: return "OPCIONES RESET";
                default: return "";
            }
        case LANG_FR:
            switch (item) {
                case ITEM_HUD: return "HUD";
                case ITEM_LAYOUT: return "DISPOSITION";
                case ITEM_POSITION: return "POSITION";
                case ITEM_X_OFFSET: return "X OFFSET";
                case ITEM_Y_OFFSET: return "Y OFFSET";
                case ITEM_SIZE: return "TAILLE HUD";
                case ITEM_MENU_SIZE: return "TAILLE MENU";
                case ITEM_FONT: return "POLICE";
                case ITEM_FPS: return "FPS HUD";
                case ITEM_BATTERY: return "BATTERIE HUD";
                case ITEM_TIME: return "HORLOGE HUD";
                case ITEM_DATE_HUD: return "DATE HUD";
                case ITEM_TIMEMODE: return "HEURE MODE";
                case ITEM_THEME_MENU: return "THEME / COULEUR / POLICE";
                case ITEM_HUD_COLORS_MENU: return "HUD COULEURS";
                case ITEM_MENU_COLORS_MENU: return "MENU COULEURS";
                case ITEM_ICON_COLORS_MENU: return "ICONE COULEURS";
                case ITEM_PERFORMANCE_ALERTS_MENU: return "PERFORMANCE ALERTS MENU";
                case ITEM_ALL_HUD_OVERLAYS_MENU: return "MENU TOUS HUD";
                case ITEM_MENU_HUD_SIZE_MENU: return "STYLES / MENU / TAILLES HUD";
                case ITEM_THEME: return "THEME PRESET";
                case ITEM_HUD_THEME: return "HUD THEME";
                case ITEM_HUD_TEXT: return "HUD TEXTE";
                case ITEM_HUD_SHADOW: return "HUD OMBRE";
                case ITEM_HUD_ICON: return "BATTERIE ICONE";
                case ITEM_CLOCK_ICON: return "HORLOGE ICONE";
                case ITEM_DATE_ICON: return "DATE ICONE";
                case ITEM_FPS_ICON: return "FPS ICONE";
                case ITEM_CPU_ICON: return "CPU ICONE";
                case ITEM_BUS_ICON: return "BUS ICONE";
                case ITEM_GPU_ICON: return "GPU ICONE";
                case ITEM_APP_ICON: return "APP ID ICONE";
                case ITEM_RAM_ICON: return "RAM ICONE";
                case ITEM_HUD_BOX: return "HUD BOX";
                case ITEM_HUD_BOX_BG: return "HUD FOND";
                case ITEM_MENU_TEXT: return "MENU TEXTE";
                case ITEM_MENU_SELECT: return "MENU SELECTION";
                case ITEM_MENU_BORDER: return "MENU BORDURE";
                case ITEM_MENUBG: return "MENU FOND";
                case ITEM_TOP_BAR: return "MENU TOP BAR";
                case ITEM_MENU_PICTURE_BG: return "PICTURE FOND";
                case ITEM_PROFILE_MENU: return "PROFIL MENU";
                case ITEM_PROFILE: return "PROFIL SLOT";
                case ITEM_LANGUAGE: return "LANGUE";
                case ITEM_AUTO_HIDE: return "AUTO HIDE";
                case ITEM_TOGGLE: return "COMBOS BASCULE";
                case ITEM_TOGGLE_HUD_COMBO: return "HUD COMBO";
                case ITEM_TOGGLE_MENU_COMBO: return "MENU COMBO";
                case ITEM_CYCLE_PROFILE_ENABLE: return "CYCLE PROFIL";
                case ITEM_CYCLE_PROFILE_COMBO: return "PROFIL COMBO";
                case ITEM_CYCLE_THEME_ENABLE: return "CYCLE THEME";
                case ITEM_CYCLE_THEME_COMBO: return "THEME COMBO";
                case ITEM_DEBUG_MENU: return "DEBUG MENU";
                case ITEM_DEBUG_HUD: return "DEBUG HUD";
                case ITEM_DEBUG_POSITION: return "DEBUG POSITION";
                case ITEM_DEBUG_X_OFFSET: return "DEBUG X OFFSET";
                case ITEM_DEBUG_Y_OFFSET: return "DEBUG Y OFFSET";
                case ITEM_DEBUG_SIZE: return "DEBUG SIZE";
                case ITEM_DEBUG_FRAMEBUF: return "FRAMEBUF DEBUG";
                case ITEM_DEBUG_CACHE: return "CACHE DEBUG";
                case ITEM_DEBUG_INPUT: return "INPUT DEBUG";
                case ITEM_DEBUG_SYSTEM: return "SYSTEM DEBUG";
                case ITEM_HUD_OPACITY: return "HUD OPACITY";
                case ITEM_MENU_OPACITY: return "MENU OPACITY";
                case ITEM_FPS_STYLE: return "FPS STYLE";
                case ITEM_BATTERY_STYLE: return "BATTERIE STYLE";
                case ITEM_CLOCK_STYLE: return "HORLOGE STYLE";
                case ITEM_SAVE_PROFILE: return "SAUVER PROFIL";
                case ITEM_LOAD_PROFILE: return "CHARGER PROFIL";
                case ITEM_CPU_HUD: return "CPU HUD";
                case ITEM_BUS_HUD: return "BUS HUD";
                case ITEM_GPU_HUD: return "GPU HUD";
                case ITEM_APP_ID_HUD: return "APP ID HUD";
                case ITEM_RAM_HUD: return "RAM HUD";
                case ITEM_HUD_ORDER_MENU: return "HUD ORDRE";
                case ITEM_FPS_WARNING: return "FPS ALERTE";
                case ITEM_FPS_LOW_LIMIT: return "FPS LIMITE BASSE";
                case ITEM_BATTERY_WARNING: return "BATTERIE ALERTE";
                case ITEM_BATTERY_LOW_LIMIT: return "BATTERIE LIMITE BASSE";
                case ITEM_RAM_WARNING: return "RAM ALERTE";
                case ITEM_RESET_HUD_POSITION: return "RESET HUD POSITION";
                case ITEM_RESET_COLORS: return "RESET COULEURS";
                case ITEM_RESET_OVERLAYS: return "RESET OVERLAYS";
                case ITEM_RESET_PROFILES: return "RESET PROFILS";
                case ITEM_RESET_ALL_DEFAULTS: return "RESET ALL DEFAULTS";
                case ITEM_HUD_MENU: return "HUD MENU";
                case ITEM_DEBUG_FONT: return "DEBUG POLICE";
                case ITEM_DEBUG_COLOR: return "DEBUG COULEURS";
                case ITEM_ICON_CHANGER_MENU: return "ICONE CHANGER MENU";
                case ITEM_FPS_ICON_STYLE: return "FPS ICONE";
                case ITEM_BATTERY_ICON_STYLE: return "BATTERIE ICONE";
                case ITEM_CLOCK_ICON_STYLE: return "HORLOGE ICONE";
                case ITEM_DATE_ICON_STYLE: return "DATE ICONE";
                case ITEM_CPU_ICON_STYLE: return "CPU ICONE";
                case ITEM_BUS_ICON_STYLE: return "BUS ICONE";
                case ITEM_GPU_ICON_STYLE: return "GPU ICONE";
                case ITEM_RAM_ICON_STYLE: return "RAM ICONE";
                case ITEM_APP_ICON_STYLE: return "APP ID ICONE";
                case ITEM_CREATE_HUD_MENU_SIZE: return "CREER HUD MENU SIZE";
                case ITEM_CREATE_MAIN_MENU_SIZE: return "CREER TAILLE MENU";
                case ITEM_DEBUG_MENU_INFO: return "MENU DEBUG";
                case ITEM_DEBUG_PROFILE: return "PROFIL DEBUG";
                case ITEM_DEBUG_THEME: return "THEME DEBUG";
                case ITEM_DEBUG_ALERT: return "ALERT DEBUG";
                case ITEM_DEBUG_HUD_INFO: return "HUD DEBUG INFO";
                case ITEM_RESET: return "RESET OPTIONS";
                default: return "";
            }
        case LANG_DE:
            switch (item) {
                case ITEM_HUD: return "HUD";
                case ITEM_LAYOUT: return "LAYOUT";
                case ITEM_POSITION: return "POSITION";
                case ITEM_X_OFFSET: return "X OFFSET";
                case ITEM_Y_OFFSET: return "Y OFFSET";
                case ITEM_SIZE: return "HUD GROSSE";
                case ITEM_MENU_SIZE: return "MENU GROSSE";
                case ITEM_FONT: return "SCHRIFT";
                case ITEM_FPS: return "FPS HUD";
                case ITEM_BATTERY: return "BATTERIE HUD";
                case ITEM_TIME: return "UHR HUD";
                case ITEM_DATE_HUD: return "DATUM HUD";
                case ITEM_TIMEMODE: return "ZEIT MODE";
                case ITEM_THEME_MENU: return "THEMA / FARBE / SCHRIFT";
                case ITEM_HUD_COLORS_MENU: return "HUD FARBEN";
                case ITEM_MENU_COLORS_MENU: return "MENU FARBEN";
                case ITEM_ICON_COLORS_MENU: return "SYMBOL FARBEN";
                case ITEM_PERFORMANCE_ALERTS_MENU: return "PERFORMANCE ALERTS MENU";
                case ITEM_ALL_HUD_OVERLAYS_MENU: return "ALLE HUD ANZEIGEN";
                case ITEM_MENU_HUD_SIZE_MENU: return "STILE / MENU / HUD GROSSEN";
                case ITEM_THEME: return "THEME PRESET";
                case ITEM_HUD_THEME: return "HUD THEME";
                case ITEM_HUD_TEXT: return "HUD TEXT";
                case ITEM_HUD_SHADOW: return "HUD SCHATTEN";
                case ITEM_HUD_ICON: return "BATTERIE SYMBOL";
                case ITEM_CLOCK_ICON: return "UHR SYMBOL";
                case ITEM_DATE_ICON: return "DATUM SYMBOL";
                case ITEM_FPS_ICON: return "FPS SYMBOL";
                case ITEM_CPU_ICON: return "CPU SYMBOL";
                case ITEM_BUS_ICON: return "BUS SYMBOL";
                case ITEM_GPU_ICON: return "GPU SYMBOL";
                case ITEM_APP_ICON: return "APP ID SYMBOL";
                case ITEM_RAM_ICON: return "RAM SYMBOL";
                case ITEM_HUD_BOX: return "HUD BOX";
                case ITEM_HUD_BOX_BG: return "HUD HINTERGRUND";
                case ITEM_MENU_TEXT: return "MENU TEXT";
                case ITEM_MENU_SELECT: return "MENU AUSWAHL";
                case ITEM_MENU_BORDER: return "MENU RAND";
                case ITEM_MENUBG: return "MENU HINTERGRUND";
                case ITEM_TOP_BAR: return "MENU TOP BAR";
                case ITEM_MENU_PICTURE_BG: return "PICTURE HINTERGRUND";
                case ITEM_PROFILE_MENU: return "PROFIL MENU";
                case ITEM_PROFILE: return "PROFIL SLOT";
                case ITEM_LANGUAGE: return "SPRACHE";
                case ITEM_AUTO_HIDE: return "AUTO HIDE";
                case ITEM_TOGGLE: return "UMSCHALT KOMBOS";
                case ITEM_TOGGLE_HUD_COMBO: return "HUD COMBO";
                case ITEM_TOGGLE_MENU_COMBO: return "MENU COMBO";
                case ITEM_CYCLE_PROFILE_ENABLE: return "CYCLE PROFIL";
                case ITEM_CYCLE_PROFILE_COMBO: return "PROFIL COMBO";
                case ITEM_CYCLE_THEME_ENABLE: return "CYCLE THEME";
                case ITEM_CYCLE_THEME_COMBO: return "THEME COMBO";
                case ITEM_DEBUG_MENU: return "DEBUG MENU";
                case ITEM_DEBUG_HUD: return "DEBUG HUD";
                case ITEM_DEBUG_POSITION: return "DEBUG POSITION";
                case ITEM_DEBUG_X_OFFSET: return "DEBUG X OFFSET";
                case ITEM_DEBUG_Y_OFFSET: return "DEBUG Y OFFSET";
                case ITEM_DEBUG_SIZE: return "DEBUG SIZE";
                case ITEM_DEBUG_FRAMEBUF: return "FRAMEBUF DEBUG";
                case ITEM_DEBUG_CACHE: return "CACHE DEBUG";
                case ITEM_DEBUG_INPUT: return "INPUT DEBUG";
                case ITEM_DEBUG_SYSTEM: return "SYSTEM DEBUG";
                case ITEM_HUD_OPACITY: return "HUD OPACITY";
                case ITEM_MENU_OPACITY: return "MENU OPACITY";
                case ITEM_FPS_STYLE: return "FPS STIL";
                case ITEM_BATTERY_STYLE: return "BATTERIE STIL";
                case ITEM_CLOCK_STYLE: return "UHR STIL";
                case ITEM_SAVE_PROFILE: return "SPEICHERN PROFIL";
                case ITEM_LOAD_PROFILE: return "LADEN PROFIL";
                case ITEM_CPU_HUD: return "CPU HUD";
                case ITEM_BUS_HUD: return "BUS HUD";
                case ITEM_GPU_HUD: return "GPU HUD";
                case ITEM_APP_ID_HUD: return "APP ID HUD";
                case ITEM_RAM_HUD: return "RAM HUD";
                case ITEM_HUD_ORDER_MENU: return "HUD REIHENFOLGE";
                case ITEM_FPS_WARNING: return "FPS WARNUNG";
                case ITEM_FPS_LOW_LIMIT: return "FPS UNTERE GRENZE";
                case ITEM_BATTERY_WARNING: return "BATTERIE WARNUNG";
                case ITEM_BATTERY_LOW_LIMIT: return "BATTERIE UNTERE GRENZE";
                case ITEM_RAM_WARNING: return "RAM WARNUNG";
                case ITEM_RESET_HUD_POSITION: return "RESET HUD POSITION";
                case ITEM_RESET_COLORS: return "RESET FARBEN";
                case ITEM_RESET_OVERLAYS: return "RESET OVERLAYS";
                case ITEM_RESET_PROFILES: return "RESET PROFILS";
                case ITEM_RESET_ALL_DEFAULTS: return "RESET ALL DEFAULTS";
                case ITEM_HUD_MENU: return "HUD MENU";
                case ITEM_DEBUG_FONT: return "DEBUG SCHRIFT";
                case ITEM_DEBUG_COLOR: return "DEBUG FARBEN";
                case ITEM_ICON_CHANGER_MENU: return "SYMBOL CHANGER MENU";
                case ITEM_FPS_ICON_STYLE: return "FPS SYMBOL";
                case ITEM_BATTERY_ICON_STYLE: return "BATTERIE SYMBOL";
                case ITEM_CLOCK_ICON_STYLE: return "UHR SYMBOL";
                case ITEM_DATE_ICON_STYLE: return "DATUM SYMBOL";
                case ITEM_CPU_ICON_STYLE: return "CPU SYMBOL";
                case ITEM_BUS_ICON_STYLE: return "BUS SYMBOL";
                case ITEM_GPU_ICON_STYLE: return "GPU SYMBOL";
                case ITEM_RAM_ICON_STYLE: return "RAM SYMBOL";
                case ITEM_APP_ICON_STYLE: return "APP ID SYMBOL";
                case ITEM_CREATE_HUD_MENU_SIZE: return "ERSTELLEN HUD MENU SIZE";
                case ITEM_CREATE_MAIN_MENU_SIZE: return "ERSTELLEN MENU GROSSE";
                case ITEM_DEBUG_MENU_INFO: return "MENU DEBUG";
                case ITEM_DEBUG_PROFILE: return "PROFIL DEBUG";
                case ITEM_DEBUG_THEME: return "THEME DEBUG";
                case ITEM_DEBUG_ALERT: return "ALERT DEBUG";
                case ITEM_DEBUG_HUD_INFO: return "HUD DEBUG INFO";
                case ITEM_RESET: return "RESET OPTIONEN";
                default: return "";
            }
        case LANG_IT:
            switch (item) {
                case ITEM_HUD: return "HUD";
                case ITEM_LAYOUT: return "LAYOUT";
                case ITEM_POSITION: return "POSIZIONE";
                case ITEM_X_OFFSET: return "X OFFSET";
                case ITEM_Y_OFFSET: return "Y OFFSET";
                case ITEM_SIZE: return "DIM HUD";
                case ITEM_MENU_SIZE: return "DIM MENU";
                case ITEM_FONT: return "FONT";
                case ITEM_FPS: return "FPS HUD";
                case ITEM_BATTERY: return "BATTERIA HUD";
                case ITEM_TIME: return "OROLOGIO HUD";
                case ITEM_DATE_HUD: return "DATA HUD";
                case ITEM_TIMEMODE: return "ORA MODE";
                case ITEM_THEME_MENU: return "TEMA / COLORE / FONT";
                case ITEM_HUD_COLORS_MENU: return "HUD COLOREI";
                case ITEM_MENU_COLORS_MENU: return "MENU COLOREI";
                case ITEM_ICON_COLORS_MENU: return "ICONA COLOREI";
                case ITEM_PERFORMANCE_ALERTS_MENU: return "PERFORMANCE ALERTS MENU";
                case ITEM_ALL_HUD_OVERLAYS_MENU: return "MENU TUTTI HUD";
                case ITEM_MENU_HUD_SIZE_MENU: return "STILI / MENU / DIM HUD";
                case ITEM_THEME: return "THEME PRESET";
                case ITEM_HUD_THEME: return "HUD THEME";
                case ITEM_HUD_TEXT: return "HUD TESTO";
                case ITEM_HUD_SHADOW: return "HUD OMBRA";
                case ITEM_HUD_ICON: return "BATTERIA ICONA";
                case ITEM_CLOCK_ICON: return "OROLOGIO ICONA";
                case ITEM_DATE_ICON: return "DATA ICONA";
                case ITEM_FPS_ICON: return "FPS ICONA";
                case ITEM_CPU_ICON: return "CPU ICONA";
                case ITEM_BUS_ICON: return "BUS ICONA";
                case ITEM_GPU_ICON: return "GPU ICONA";
                case ITEM_APP_ICON: return "APP ID ICONA";
                case ITEM_RAM_ICON: return "RAM ICONA";
                case ITEM_HUD_BOX: return "HUD BOX";
                case ITEM_HUD_BOX_BG: return "HUD SFONDO";
                case ITEM_MENU_TEXT: return "MENU TESTO";
                case ITEM_MENU_SELECT: return "MENU SELEZIONE";
                case ITEM_MENU_BORDER: return "MENU BORDO";
                case ITEM_MENUBG: return "MENU SFONDO";
                case ITEM_TOP_BAR: return "MENU TOP BAR";
                case ITEM_MENU_PICTURE_BG: return "PICTURE SFONDO";
                case ITEM_PROFILE_MENU: return "PROFILO MENU";
                case ITEM_PROFILE: return "PROFILO SLOT";
                case ITEM_LANGUAGE: return "LINGUA";
                case ITEM_AUTO_HIDE: return "AUTO HIDE";
                case ITEM_TOGGLE: return "COMBO ATTIVA";
                case ITEM_TOGGLE_HUD_COMBO: return "HUD COMBO";
                case ITEM_TOGGLE_MENU_COMBO: return "MENU COMBO";
                case ITEM_CYCLE_PROFILE_ENABLE: return "CYCLE PROFILO";
                case ITEM_CYCLE_PROFILE_COMBO: return "PROFILO COMBO";
                case ITEM_CYCLE_THEME_ENABLE: return "CYCLE THEME";
                case ITEM_CYCLE_THEME_COMBO: return "THEME COMBO";
                case ITEM_DEBUG_MENU: return "DEBUG MENU";
                case ITEM_DEBUG_HUD: return "DEBUG HUD";
                case ITEM_DEBUG_POSITION: return "DEBUG POSIZIONE";
                case ITEM_DEBUG_X_OFFSET: return "DEBUG X OFFSET";
                case ITEM_DEBUG_Y_OFFSET: return "DEBUG Y OFFSET";
                case ITEM_DEBUG_SIZE: return "DEBUG SIZE";
                case ITEM_DEBUG_FRAMEBUF: return "FRAMEBUF DEBUG";
                case ITEM_DEBUG_CACHE: return "CACHE DEBUG";
                case ITEM_DEBUG_INPUT: return "INPUT DEBUG";
                case ITEM_DEBUG_SYSTEM: return "SYSTEM DEBUG";
                case ITEM_HUD_OPACITY: return "HUD OPACITY";
                case ITEM_MENU_OPACITY: return "MENU OPACITY";
                case ITEM_FPS_STYLE: return "FPS STILE";
                case ITEM_BATTERY_STYLE: return "BATTERIA STILE";
                case ITEM_CLOCK_STYLE: return "OROLOGIO STILE";
                case ITEM_SAVE_PROFILE: return "SALVA PROFILO";
                case ITEM_LOAD_PROFILE: return "CARICA PROFILO";
                case ITEM_CPU_HUD: return "CPU HUD";
                case ITEM_BUS_HUD: return "BUS HUD";
                case ITEM_GPU_HUD: return "GPU HUD";
                case ITEM_APP_ID_HUD: return "APP ID HUD";
                case ITEM_RAM_HUD: return "RAM HUD";
                case ITEM_HUD_ORDER_MENU: return "HUD ORDINE";
                case ITEM_FPS_WARNING: return "FPS AVVISO";
                case ITEM_FPS_LOW_LIMIT: return "FPS LIMITE BASSO";
                case ITEM_BATTERY_WARNING: return "BATTERIA AVVISO";
                case ITEM_BATTERY_LOW_LIMIT: return "BATTERIA LIMITE BASSO";
                case ITEM_RAM_WARNING: return "RAM AVVISO";
                case ITEM_RESET_HUD_POSITION: return "RESET HUD POSIZIONE";
                case ITEM_RESET_COLORS: return "RESET COLOREI";
                case ITEM_RESET_OVERLAYS: return "RESET OVERLAYS";
                case ITEM_RESET_PROFILES: return "RESET PROFILOS";
                case ITEM_RESET_ALL_DEFAULTS: return "RESET ALL DEFAULTS";
                case ITEM_HUD_MENU: return "HUD MENU";
                case ITEM_DEBUG_FONT: return "DEBUG FONT";
                case ITEM_DEBUG_COLOR: return "DEBUG COLOREI";
                case ITEM_ICON_CHANGER_MENU: return "ICONA CHANGER MENU";
                case ITEM_FPS_ICON_STYLE: return "FPS ICONA";
                case ITEM_BATTERY_ICON_STYLE: return "BATTERIA ICONA";
                case ITEM_CLOCK_ICON_STYLE: return "OROLOGIO ICONA";
                case ITEM_DATE_ICON_STYLE: return "DATA ICONA";
                case ITEM_CPU_ICON_STYLE: return "CPU ICONA";
                case ITEM_BUS_ICON_STYLE: return "BUS ICONA";
                case ITEM_GPU_ICON_STYLE: return "GPU ICONA";
                case ITEM_RAM_ICON_STYLE: return "RAM ICONA";
                case ITEM_APP_ICON_STYLE: return "APP ID ICONA";
                case ITEM_CREATE_HUD_MENU_SIZE: return "CREA HUD MENU SIZE";
                case ITEM_CREATE_MAIN_MENU_SIZE: return "CREA DIM MENU";
                case ITEM_DEBUG_MENU_INFO: return "MENU DEBUG";
                case ITEM_DEBUG_PROFILE: return "PROFILO DEBUG";
                case ITEM_DEBUG_THEME: return "THEME DEBUG";
                case ITEM_DEBUG_ALERT: return "ALERT DEBUG";
                case ITEM_DEBUG_HUD_INFO: return "HUD DEBUG INFO";
                case ITEM_RESET: return "RESET OPZIONI";
                default: return "";
            }
        case LANG_PT:
            switch (item) {
                case ITEM_HUD: return "HUD";
                case ITEM_LAYOUT: return "LAYOUT";
                case ITEM_POSITION: return "POSICAO";
                case ITEM_X_OFFSET: return "X OFFSET";
                case ITEM_Y_OFFSET: return "Y OFFSET";
                case ITEM_SIZE: return "TAMANHO HUD";
                case ITEM_MENU_SIZE: return "TAMANHO MENU";
                case ITEM_FONT: return "FONTE";
                case ITEM_FPS: return "FPS HUD";
                case ITEM_BATTERY: return "BATERIA HUD";
                case ITEM_TIME: return "RELOGIO HUD";
                case ITEM_DATE_HUD: return "DATA HUD";
                case ITEM_TIMEMODE: return "HORA MODE";
                case ITEM_THEME_MENU: return "TEMA / COR / FONTE";
                case ITEM_HUD_COLORS_MENU: return "HUD CORES";
                case ITEM_MENU_COLORS_MENU: return "MENU CORES";
                case ITEM_ICON_COLORS_MENU: return "ICONE CORES";
                case ITEM_PERFORMANCE_ALERTS_MENU: return "PERFORMANCE ALERTS MENU";
                case ITEM_ALL_HUD_OVERLAYS_MENU: return "MENU TODOS HUD";
                case ITEM_MENU_HUD_SIZE_MENU: return "ESTILOS / MENU / TAM HUD";
                case ITEM_THEME: return "THEME PRESET";
                case ITEM_HUD_THEME: return "HUD THEME";
                case ITEM_HUD_TEXT: return "HUD TEXTO";
                case ITEM_HUD_SHADOW: return "HUD SOMBRA";
                case ITEM_HUD_ICON: return "BATERIA ICONE";
                case ITEM_CLOCK_ICON: return "RELOGIO ICONE";
                case ITEM_DATE_ICON: return "DATA ICONE";
                case ITEM_FPS_ICON: return "FPS ICONE";
                case ITEM_CPU_ICON: return "CPU ICONE";
                case ITEM_BUS_ICON: return "BUS ICONE";
                case ITEM_GPU_ICON: return "GPU ICONE";
                case ITEM_APP_ICON: return "APP ID ICONE";
                case ITEM_RAM_ICON: return "RAM ICONE";
                case ITEM_HUD_BOX: return "HUD BOX";
                case ITEM_HUD_BOX_BG: return "HUD FUNDO";
                case ITEM_MENU_TEXT: return "MENU TEXTO";
                case ITEM_MENU_SELECT: return "MENU SELECAO";
                case ITEM_MENU_BORDER: return "MENU BORDA";
                case ITEM_MENUBG: return "MENU FUNDO";
                case ITEM_TOP_BAR: return "MENU TOP BAR";
                case ITEM_MENU_PICTURE_BG: return "PICTURE FUNDO";
                case ITEM_PROFILE_MENU: return "PERFIL MENU";
                case ITEM_PROFILE: return "PERFIL SLOT";
                case ITEM_LANGUAGE: return "IDIOMA";
                case ITEM_AUTO_HIDE: return "AUTO HIDE";
                case ITEM_TOGGLE: return "COMBOS ALTERNAR";
                case ITEM_TOGGLE_HUD_COMBO: return "HUD COMBO";
                case ITEM_TOGGLE_MENU_COMBO: return "MENU COMBO";
                case ITEM_CYCLE_PROFILE_ENABLE: return "CYCLE PERFIL";
                case ITEM_CYCLE_PROFILE_COMBO: return "PERFIL COMBO";
                case ITEM_CYCLE_THEME_ENABLE: return "CYCLE THEME";
                case ITEM_CYCLE_THEME_COMBO: return "THEME COMBO";
                case ITEM_DEBUG_MENU: return "DEBUG MENU";
                case ITEM_DEBUG_HUD: return "DEBUG HUD";
                case ITEM_DEBUG_POSITION: return "DEBUG POSICAO";
                case ITEM_DEBUG_X_OFFSET: return "DEBUG X OFFSET";
                case ITEM_DEBUG_Y_OFFSET: return "DEBUG Y OFFSET";
                case ITEM_DEBUG_SIZE: return "DEBUG SIZE";
                case ITEM_DEBUG_FRAMEBUF: return "FRAMEBUF DEBUG";
                case ITEM_DEBUG_CACHE: return "CACHE DEBUG";
                case ITEM_DEBUG_INPUT: return "INPUT DEBUG";
                case ITEM_DEBUG_SYSTEM: return "SYSTEM DEBUG";
                case ITEM_HUD_OPACITY: return "HUD OPACITY";
                case ITEM_MENU_OPACITY: return "MENU OPACITY";
                case ITEM_FPS_STYLE: return "FPS ESTILO";
                case ITEM_BATTERY_STYLE: return "BATERIA ESTILO";
                case ITEM_CLOCK_STYLE: return "RELOGIO ESTILO";
                case ITEM_SAVE_PROFILE: return "SALVAR PERFIL";
                case ITEM_LOAD_PROFILE: return "CARREGAR PERFIL";
                case ITEM_CPU_HUD: return "CPU HUD";
                case ITEM_BUS_HUD: return "BUS HUD";
                case ITEM_GPU_HUD: return "GPU HUD";
                case ITEM_APP_ID_HUD: return "APP ID HUD";
                case ITEM_RAM_HUD: return "RAM HUD";
                case ITEM_HUD_ORDER_MENU: return "HUD ORDEM";
                case ITEM_FPS_WARNING: return "FPS ALERTA";
                case ITEM_FPS_LOW_LIMIT: return "FPS LIMITE BAIXO";
                case ITEM_BATTERY_WARNING: return "BATERIA ALERTA";
                case ITEM_BATTERY_LOW_LIMIT: return "BATERIA LIMITE BAIXO";
                case ITEM_RAM_WARNING: return "RAM ALERTA";
                case ITEM_RESET_HUD_POSITION: return "RESET HUD POSICAO";
                case ITEM_RESET_COLORS: return "RESET CORES";
                case ITEM_RESET_OVERLAYS: return "RESET OVERLAYS";
                case ITEM_RESET_PROFILES: return "RESET PERFILS";
                case ITEM_RESET_ALL_DEFAULTS: return "RESET ALL DEFAULTS";
                case ITEM_HUD_MENU: return "HUD MENU";
                case ITEM_DEBUG_FONT: return "DEBUG FONTE";
                case ITEM_DEBUG_COLOR: return "DEBUG CORES";
                case ITEM_ICON_CHANGER_MENU: return "ICONE CHANGER MENU";
                case ITEM_FPS_ICON_STYLE: return "FPS ICONE";
                case ITEM_BATTERY_ICON_STYLE: return "BATERIA ICONE";
                case ITEM_CLOCK_ICON_STYLE: return "RELOGIO ICONE";
                case ITEM_DATE_ICON_STYLE: return "DATA ICONE";
                case ITEM_CPU_ICON_STYLE: return "CPU ICONE";
                case ITEM_BUS_ICON_STYLE: return "BUS ICONE";
                case ITEM_GPU_ICON_STYLE: return "GPU ICONE";
                case ITEM_RAM_ICON_STYLE: return "RAM ICONE";
                case ITEM_APP_ICON_STYLE: return "APP ID ICONE";
                case ITEM_CREATE_HUD_MENU_SIZE: return "CRIAR HUD MENU SIZE";
                case ITEM_CREATE_MAIN_MENU_SIZE: return "CRIAR TAMANHO MENU";
                case ITEM_DEBUG_MENU_INFO: return "MENU DEBUG";
                case ITEM_DEBUG_PROFILE: return "PERFIL DEBUG";
                case ITEM_DEBUG_THEME: return "THEME DEBUG";
                case ITEM_DEBUG_ALERT: return "ALERT DEBUG";
                case ITEM_DEBUG_HUD_INFO: return "HUD DEBUG INFO";
                case ITEM_RESET: return "RESET OPCOES";
                default: return "";
            }
        case LANG_NL:
            switch (item) {
                case ITEM_HUD: return "HUD";
                case ITEM_LAYOUT: return "INDELING";
                case ITEM_POSITION: return "POSITIE";
                case ITEM_X_OFFSET: return "X OFFSET";
                case ITEM_Y_OFFSET: return "Y OFFSET";
                case ITEM_SIZE: return "HUD GROOTTE";
                case ITEM_MENU_SIZE: return "MENU GROOTTE";
                case ITEM_FONT: return "LETTERTYPE";
                case ITEM_FPS: return "FPS HUD";
                case ITEM_BATTERY: return "BATTERIJ HUD";
                case ITEM_TIME: return "KLOK HUD";
                case ITEM_DATE_HUD: return "DATUM HUD";
                case ITEM_TIMEMODE: return "TIJD MODE";
                case ITEM_THEME_MENU: return "THEMA / KLEUR / LETTERTYPE";
                case ITEM_HUD_COLORS_MENU: return "HUD KLEUREN";
                case ITEM_MENU_COLORS_MENU: return "MENU KLEUREN";
                case ITEM_ICON_COLORS_MENU: return "ICOON KLEUREN";
                case ITEM_PERFORMANCE_ALERTS_MENU: return "PERFORMANCE ALERTS MENU";
                case ITEM_ALL_HUD_OVERLAYS_MENU: return "ALLE HUD OVERLAYS";
                case ITEM_MENU_HUD_SIZE_MENU: return "STIJLEN / MENU / HUD GROOTTE";
                case ITEM_THEME: return "THEME PRESET";
                case ITEM_HUD_THEME: return "HUD THEME";
                case ITEM_HUD_TEXT: return "HUD TEKST";
                case ITEM_HUD_SHADOW: return "HUD SCHADUW";
                case ITEM_HUD_ICON: return "BATTERIJ ICOON";
                case ITEM_CLOCK_ICON: return "KLOK ICOON";
                case ITEM_DATE_ICON: return "DATUM ICOON";
                case ITEM_FPS_ICON: return "FPS ICOON";
                case ITEM_CPU_ICON: return "CPU ICOON";
                case ITEM_BUS_ICON: return "BUS ICOON";
                case ITEM_GPU_ICON: return "GPU ICOON";
                case ITEM_APP_ICON: return "APP ID ICOON";
                case ITEM_RAM_ICON: return "RAM ICOON";
                case ITEM_HUD_BOX: return "HUD BOX";
                case ITEM_HUD_BOX_BG: return "HUD ACHTERGROND";
                case ITEM_MENU_TEXT: return "MENU TEKST";
                case ITEM_MENU_SELECT: return "MENU SELECTIE";
                case ITEM_MENU_BORDER: return "MENU RAND";
                case ITEM_MENUBG: return "MENU ACHTERGROND";
                case ITEM_TOP_BAR: return "MENU TOP BAR";
                case ITEM_MENU_PICTURE_BG: return "PICTURE ACHTERGROND";
                case ITEM_PROFILE_MENU: return "PROFIEL MENU";
                case ITEM_PROFILE: return "PROFIEL SLOT";
                case ITEM_LANGUAGE: return "TAAL";
                case ITEM_AUTO_HIDE: return "AUTO HIDE";
                case ITEM_TOGGLE: return "SCHAKEL COMBOS";
                case ITEM_TOGGLE_HUD_COMBO: return "HUD COMBO";
                case ITEM_TOGGLE_MENU_COMBO: return "MENU COMBO";
                case ITEM_CYCLE_PROFILE_ENABLE: return "CYCLE PROFIEL";
                case ITEM_CYCLE_PROFILE_COMBO: return "PROFIEL COMBO";
                case ITEM_CYCLE_THEME_ENABLE: return "CYCLE THEME";
                case ITEM_CYCLE_THEME_COMBO: return "THEME COMBO";
                case ITEM_DEBUG_MENU: return "DEBUG MENU";
                case ITEM_DEBUG_HUD: return "DEBUG HUD";
                case ITEM_DEBUG_POSITION: return "DEBUG POSITIE";
                case ITEM_DEBUG_X_OFFSET: return "DEBUG X OFFSET";
                case ITEM_DEBUG_Y_OFFSET: return "DEBUG Y OFFSET";
                case ITEM_DEBUG_SIZE: return "DEBUG SIZE";
                case ITEM_DEBUG_FRAMEBUF: return "FRAMEBUF DEBUG";
                case ITEM_DEBUG_CACHE: return "CACHE DEBUG";
                case ITEM_DEBUG_INPUT: return "INPUT DEBUG";
                case ITEM_DEBUG_SYSTEM: return "SYSTEM DEBUG";
                case ITEM_HUD_OPACITY: return "HUD OPACITY";
                case ITEM_MENU_OPACITY: return "MENU OPACITY";
                case ITEM_FPS_STYLE: return "FPS STIJL";
                case ITEM_BATTERY_STYLE: return "BATTERIJ STIJL";
                case ITEM_CLOCK_STYLE: return "KLOK STIJL";
                case ITEM_SAVE_PROFILE: return "OPSLAAN PROFIEL";
                case ITEM_LOAD_PROFILE: return "LADEN PROFIEL";
                case ITEM_CPU_HUD: return "CPU HUD";
                case ITEM_BUS_HUD: return "BUS HUD";
                case ITEM_GPU_HUD: return "GPU HUD";
                case ITEM_APP_ID_HUD: return "APP ID HUD";
                case ITEM_RAM_HUD: return "RAM HUD";
                case ITEM_HUD_ORDER_MENU: return "HUD VOLGORDE";
                case ITEM_FPS_WARNING: return "FPS WAARSCHUWING";
                case ITEM_FPS_LOW_LIMIT: return "FPS LAGE LIMIET";
                case ITEM_BATTERY_WARNING: return "BATTERIJ WAARSCHUWING";
                case ITEM_BATTERY_LOW_LIMIT: return "BATTERIJ LAGE LIMIET";
                case ITEM_RAM_WARNING: return "RAM WAARSCHUWING";
                case ITEM_RESET_HUD_POSITION: return "RESET HUD POSITIE";
                case ITEM_RESET_COLORS: return "RESET KLEUREN";
                case ITEM_RESET_OVERLAYS: return "RESET OVERLAYS";
                case ITEM_RESET_PROFILES: return "RESET PROFIELS";
                case ITEM_RESET_ALL_DEFAULTS: return "RESET ALL DEFAULTS";
                case ITEM_HUD_MENU: return "HUD MENU";
                case ITEM_DEBUG_FONT: return "DEBUG LETTERTYPE";
                case ITEM_DEBUG_COLOR: return "DEBUG KLEUREN";
                case ITEM_ICON_CHANGER_MENU: return "ICOON CHANGER MENU";
                case ITEM_FPS_ICON_STYLE: return "FPS ICOON";
                case ITEM_BATTERY_ICON_STYLE: return "BATTERIJ ICOON";
                case ITEM_CLOCK_ICON_STYLE: return "KLOK ICOON";
                case ITEM_DATE_ICON_STYLE: return "DATUM ICOON";
                case ITEM_CPU_ICON_STYLE: return "CPU ICOON";
                case ITEM_BUS_ICON_STYLE: return "BUS ICOON";
                case ITEM_GPU_ICON_STYLE: return "GPU ICOON";
                case ITEM_RAM_ICON_STYLE: return "RAM ICOON";
                case ITEM_APP_ICON_STYLE: return "APP ID ICOON";
                case ITEM_CREATE_HUD_MENU_SIZE: return "MAAK HUD MENU SIZE";
                case ITEM_CREATE_MAIN_MENU_SIZE: return "MAAK MENU GROOTTE";
                case ITEM_DEBUG_MENU_INFO: return "MENU DEBUG";
                case ITEM_DEBUG_PROFILE: return "PROFIEL DEBUG";
                case ITEM_DEBUG_THEME: return "THEME DEBUG";
                case ITEM_DEBUG_ALERT: return "ALERT DEBUG";
                case ITEM_DEBUG_HUD_INFO: return "HUD DEBUG INFO";
                case ITEM_RESET: return "RESET OPTIES";
                default: return "";
            }
        case LANG_ID:
            switch (item) {
                case ITEM_HUD: return "HUD";
                case ITEM_LAYOUT: return "TATA LETAK";
                case ITEM_POSITION: return "POSISI";
                case ITEM_X_OFFSET: return "X OFFSET";
                case ITEM_Y_OFFSET: return "Y OFFSET";
                case ITEM_SIZE: return "UKURAN HUD";
                case ITEM_MENU_SIZE: return "UKURAN MENU";
                case ITEM_FONT: return "FONT";
                case ITEM_FPS: return "FPS HUD";
                case ITEM_BATTERY: return "BATERAI HUD";
                case ITEM_TIME: return "JAM HUD";
                case ITEM_DATE_HUD: return "TANGGAL HUD";
                case ITEM_TIMEMODE: return "WAKTU MODE";
                case ITEM_THEME_MENU: return "TEMA / WARNA / FONT";
                case ITEM_HUD_COLORS_MENU: return "HUD WARNA";
                case ITEM_MENU_COLORS_MENU: return "MENU WARNA";
                case ITEM_ICON_COLORS_MENU: return "IKON WARNA";
                case ITEM_PERFORMANCE_ALERTS_MENU: return "PERFORMANCE ALERTS MENU";
                case ITEM_ALL_HUD_OVERLAYS_MENU: return "MENU SEMUA HUD";
                case ITEM_MENU_HUD_SIZE_MENU: return "GAYA / MENU / UKURAN HUD";
                case ITEM_THEME: return "THEME PRESET";
                case ITEM_HUD_THEME: return "HUD THEME";
                case ITEM_HUD_TEXT: return "HUD TEKS";
                case ITEM_HUD_SHADOW: return "HUD BAYANGAN";
                case ITEM_HUD_ICON: return "BATERAI IKON";
                case ITEM_CLOCK_ICON: return "JAM IKON";
                case ITEM_DATE_ICON: return "TANGGAL IKON";
                case ITEM_FPS_ICON: return "FPS IKON";
                case ITEM_CPU_ICON: return "CPU IKON";
                case ITEM_BUS_ICON: return "BUS IKON";
                case ITEM_GPU_ICON: return "GPU IKON";
                case ITEM_APP_ICON: return "APP ID IKON";
                case ITEM_RAM_ICON: return "RAM IKON";
                case ITEM_HUD_BOX: return "HUD BOX";
                case ITEM_HUD_BOX_BG: return "HUD LATAR";
                case ITEM_MENU_TEXT: return "MENU TEKS";
                case ITEM_MENU_SELECT: return "MENU PILIHAN";
                case ITEM_MENU_BORDER: return "MENU BATAS";
                case ITEM_MENUBG: return "MENU LATAR";
                case ITEM_TOP_BAR: return "MENU TOP BAR";
                case ITEM_MENU_PICTURE_BG: return "PICTURE LATAR";
                case ITEM_PROFILE_MENU: return "PROFIL MENU";
                case ITEM_PROFILE: return "PROFIL SLOT";
                case ITEM_LANGUAGE: return "BAHASA";
                case ITEM_AUTO_HIDE: return "AUTO HIDE";
                case ITEM_TOGGLE: return "KOMBO TOGGLE";
                case ITEM_TOGGLE_HUD_COMBO: return "HUD COMBO";
                case ITEM_TOGGLE_MENU_COMBO: return "MENU COMBO";
                case ITEM_CYCLE_PROFILE_ENABLE: return "CYCLE PROFIL";
                case ITEM_CYCLE_PROFILE_COMBO: return "PROFIL COMBO";
                case ITEM_CYCLE_THEME_ENABLE: return "CYCLE THEME";
                case ITEM_CYCLE_THEME_COMBO: return "THEME COMBO";
                case ITEM_DEBUG_MENU: return "DEBUG MENU";
                case ITEM_DEBUG_HUD: return "DEBUG HUD";
                case ITEM_DEBUG_POSITION: return "DEBUG POSISI";
                case ITEM_DEBUG_X_OFFSET: return "DEBUG X OFFSET";
                case ITEM_DEBUG_Y_OFFSET: return "DEBUG Y OFFSET";
                case ITEM_DEBUG_SIZE: return "DEBUG SIZE";
                case ITEM_DEBUG_FRAMEBUF: return "FRAMEBUF DEBUG";
                case ITEM_DEBUG_CACHE: return "CACHE DEBUG";
                case ITEM_DEBUG_INPUT: return "INPUT DEBUG";
                case ITEM_DEBUG_SYSTEM: return "SYSTEM DEBUG";
                case ITEM_HUD_OPACITY: return "HUD OPACITY";
                case ITEM_MENU_OPACITY: return "MENU OPACITY";
                case ITEM_FPS_STYLE: return "FPS GAYA";
                case ITEM_BATTERY_STYLE: return "BATERAI GAYA";
                case ITEM_CLOCK_STYLE: return "JAM GAYA";
                case ITEM_SAVE_PROFILE: return "SIMPAN PROFIL";
                case ITEM_LOAD_PROFILE: return "MUAT PROFIL";
                case ITEM_CPU_HUD: return "CPU HUD";
                case ITEM_BUS_HUD: return "BUS HUD";
                case ITEM_GPU_HUD: return "GPU HUD";
                case ITEM_APP_ID_HUD: return "APP ID HUD";
                case ITEM_RAM_HUD: return "RAM HUD";
                case ITEM_HUD_ORDER_MENU: return "HUD URUTAN";
                case ITEM_FPS_WARNING: return "FPS PERINGATAN";
                case ITEM_FPS_LOW_LIMIT: return "FPS BATAS RENDAH";
                case ITEM_BATTERY_WARNING: return "BATERAI PERINGATAN";
                case ITEM_BATTERY_LOW_LIMIT: return "BATERAI BATAS RENDAH";
                case ITEM_RAM_WARNING: return "RAM PERINGATAN";
                case ITEM_RESET_HUD_POSITION: return "RESET HUD POSISI";
                case ITEM_RESET_COLORS: return "RESET WARNA";
                case ITEM_RESET_OVERLAYS: return "RESET OVERLAYS";
                case ITEM_RESET_PROFILES: return "RESET PROFILS";
                case ITEM_RESET_ALL_DEFAULTS: return "RESET ALL DEFAULTS";
                case ITEM_HUD_MENU: return "HUD MENU";
                case ITEM_DEBUG_FONT: return "DEBUG FONT";
                case ITEM_DEBUG_COLOR: return "DEBUG WARNA";
                case ITEM_ICON_CHANGER_MENU: return "IKON CHANGER MENU";
                case ITEM_FPS_ICON_STYLE: return "FPS IKON";
                case ITEM_BATTERY_ICON_STYLE: return "BATERAI IKON";
                case ITEM_CLOCK_ICON_STYLE: return "JAM IKON";
                case ITEM_DATE_ICON_STYLE: return "TANGGAL IKON";
                case ITEM_CPU_ICON_STYLE: return "CPU IKON";
                case ITEM_BUS_ICON_STYLE: return "BUS IKON";
                case ITEM_GPU_ICON_STYLE: return "GPU IKON";
                case ITEM_RAM_ICON_STYLE: return "RAM IKON";
                case ITEM_APP_ICON_STYLE: return "APP ID IKON";
                case ITEM_CREATE_HUD_MENU_SIZE: return "BUAT HUD MENU SIZE";
                case ITEM_CREATE_MAIN_MENU_SIZE: return "BUAT UKURAN MENU";
                case ITEM_DEBUG_MENU_INFO: return "MENU DEBUG";
                case ITEM_DEBUG_PROFILE: return "PROFIL DEBUG";
                case ITEM_DEBUG_THEME: return "THEME DEBUG";
                case ITEM_DEBUG_ALERT: return "ALERT DEBUG";
                case ITEM_DEBUG_HUD_INFO: return "HUD DEBUG INFO";
                case ITEM_RESET: return "RESET OPSI";
                default: return "";
            }
        case LANG_TR:
            switch (item) {
                case ITEM_HUD: return "HUD";
                case ITEM_LAYOUT: return "DUZEN";
                case ITEM_POSITION: return "KONUM";
                case ITEM_X_OFFSET: return "X OFFSET";
                case ITEM_Y_OFFSET: return "Y OFFSET";
                case ITEM_SIZE: return "HUD BOYUT";
                case ITEM_MENU_SIZE: return "MENU BOYUT";
                case ITEM_FONT: return "YAZI TIPI";
                case ITEM_FPS: return "FPS HUD";
                case ITEM_BATTERY: return "PIL HUD";
                case ITEM_TIME: return "SAAT HUD";
                case ITEM_DATE_HUD: return "TARIH HUD";
                case ITEM_TIMEMODE: return "SAAT MODE";
                case ITEM_THEME_MENU: return "TEMA / RENK / YAZI";
                case ITEM_HUD_COLORS_MENU: return "HUD RENKLER";
                case ITEM_MENU_COLORS_MENU: return "MENU RENKLER";
                case ITEM_ICON_COLORS_MENU: return "SIMGE RENKLER";
                case ITEM_PERFORMANCE_ALERTS_MENU: return "PERFORMANCE ALERTS MENU";
                case ITEM_ALL_HUD_OVERLAYS_MENU: return "TUM HUD MENU";
                case ITEM_MENU_HUD_SIZE_MENU: return "STILLER / MENU / HUD BOYUT";
                case ITEM_THEME: return "THEME PRESET";
                case ITEM_HUD_THEME: return "HUD THEME";
                case ITEM_HUD_TEXT: return "HUD YAZI";
                case ITEM_HUD_SHADOW: return "HUD GOLGE";
                case ITEM_HUD_ICON: return "PIL SIMGE";
                case ITEM_CLOCK_ICON: return "SAAT SIMGE";
                case ITEM_DATE_ICON: return "TARIH SIMGE";
                case ITEM_FPS_ICON: return "FPS SIMGE";
                case ITEM_CPU_ICON: return "CPU SIMGE";
                case ITEM_BUS_ICON: return "BUS SIMGE";
                case ITEM_GPU_ICON: return "GPU SIMGE";
                case ITEM_APP_ICON: return "APP ID SIMGE";
                case ITEM_RAM_ICON: return "RAM SIMGE";
                case ITEM_HUD_BOX: return "HUD BOX";
                case ITEM_HUD_BOX_BG: return "HUD ARKA PLAN";
                case ITEM_MENU_TEXT: return "MENU YAZI";
                case ITEM_MENU_SELECT: return "MENU SECIM";
                case ITEM_MENU_BORDER: return "MENU KENAR";
                case ITEM_MENUBG: return "MENU ARKA PLAN";
                case ITEM_TOP_BAR: return "MENU TOP BAR";
                case ITEM_MENU_PICTURE_BG: return "PICTURE ARKA PLAN";
                case ITEM_PROFILE_MENU: return "PROFIL MENU";
                case ITEM_PROFILE: return "PROFIL SLOT";
                case ITEM_LANGUAGE: return "DIL";
                case ITEM_AUTO_HIDE: return "AUTO HIDE";
                case ITEM_TOGGLE: return "AC/KAPA KOMBOLAR";
                case ITEM_TOGGLE_HUD_COMBO: return "HUD COMBO";
                case ITEM_TOGGLE_MENU_COMBO: return "MENU COMBO";
                case ITEM_CYCLE_PROFILE_ENABLE: return "CYCLE PROFIL";
                case ITEM_CYCLE_PROFILE_COMBO: return "PROFIL COMBO";
                case ITEM_CYCLE_THEME_ENABLE: return "CYCLE THEME";
                case ITEM_CYCLE_THEME_COMBO: return "THEME COMBO";
                case ITEM_DEBUG_MENU: return "DEBUG MENU";
                case ITEM_DEBUG_HUD: return "DEBUG HUD";
                case ITEM_DEBUG_POSITION: return "DEBUG KONUM";
                case ITEM_DEBUG_X_OFFSET: return "DEBUG X OFFSET";
                case ITEM_DEBUG_Y_OFFSET: return "DEBUG Y OFFSET";
                case ITEM_DEBUG_SIZE: return "DEBUG SIZE";
                case ITEM_DEBUG_FRAMEBUF: return "FRAMEBUF DEBUG";
                case ITEM_DEBUG_CACHE: return "CACHE DEBUG";
                case ITEM_DEBUG_INPUT: return "INPUT DEBUG";
                case ITEM_DEBUG_SYSTEM: return "SYSTEM DEBUG";
                case ITEM_HUD_OPACITY: return "HUD OPACITY";
                case ITEM_MENU_OPACITY: return "MENU OPACITY";
                case ITEM_FPS_STYLE: return "FPS STIL";
                case ITEM_BATTERY_STYLE: return "PIL STIL";
                case ITEM_CLOCK_STYLE: return "SAAT STIL";
                case ITEM_SAVE_PROFILE: return "KAYDET PROFIL";
                case ITEM_LOAD_PROFILE: return "YUKLE PROFIL";
                case ITEM_CPU_HUD: return "CPU HUD";
                case ITEM_BUS_HUD: return "BUS HUD";
                case ITEM_GPU_HUD: return "GPU HUD";
                case ITEM_APP_ID_HUD: return "APP ID HUD";
                case ITEM_RAM_HUD: return "RAM HUD";
                case ITEM_HUD_ORDER_MENU: return "HUD SIRA";
                case ITEM_FPS_WARNING: return "FPS UYARI";
                case ITEM_FPS_LOW_LIMIT: return "FPS ALT LIMIT";
                case ITEM_BATTERY_WARNING: return "PIL UYARI";
                case ITEM_BATTERY_LOW_LIMIT: return "PIL ALT LIMIT";
                case ITEM_RAM_WARNING: return "RAM UYARI";
                case ITEM_RESET_HUD_POSITION: return "RESET HUD KONUM";
                case ITEM_RESET_COLORS: return "RESET RENKLER";
                case ITEM_RESET_OVERLAYS: return "RESET OVERLAYS";
                case ITEM_RESET_PROFILES: return "RESET PROFILS";
                case ITEM_RESET_ALL_DEFAULTS: return "RESET ALL DEFAULTS";
                case ITEM_HUD_MENU: return "HUD MENU";
                case ITEM_DEBUG_FONT: return "DEBUG YAZI TIPI";
                case ITEM_DEBUG_COLOR: return "DEBUG RENKLER";
                case ITEM_ICON_CHANGER_MENU: return "SIMGE CHANGER MENU";
                case ITEM_FPS_ICON_STYLE: return "FPS SIMGE";
                case ITEM_BATTERY_ICON_STYLE: return "PIL SIMGE";
                case ITEM_CLOCK_ICON_STYLE: return "SAAT SIMGE";
                case ITEM_DATE_ICON_STYLE: return "TARIH SIMGE";
                case ITEM_CPU_ICON_STYLE: return "CPU SIMGE";
                case ITEM_BUS_ICON_STYLE: return "BUS SIMGE";
                case ITEM_GPU_ICON_STYLE: return "GPU SIMGE";
                case ITEM_RAM_ICON_STYLE: return "RAM SIMGE";
                case ITEM_APP_ICON_STYLE: return "APP ID SIMGE";
                case ITEM_CREATE_HUD_MENU_SIZE: return "OLUSTUR HUD MENU SIZE";
                case ITEM_CREATE_MAIN_MENU_SIZE: return "OLUSTUR MENU BOYUT";
                case ITEM_DEBUG_MENU_INFO: return "MENU DEBUG";
                case ITEM_DEBUG_PROFILE: return "PROFIL DEBUG";
                case ITEM_DEBUG_THEME: return "THEME DEBUG";
                case ITEM_DEBUG_ALERT: return "ALERT DEBUG";
                case ITEM_DEBUG_HUD_INFO: return "HUD DEBUG INFO";
                case ITEM_RESET: return "RESET SECENEKLER";
                default: return "";
            }
        case LANG_PL:
            switch (item) {
                case ITEM_HUD: return "HUD";
                case ITEM_LAYOUT: return "UKLAD";
                case ITEM_POSITION: return "POZYCJA";
                case ITEM_X_OFFSET: return "X OFFSET";
                case ITEM_Y_OFFSET: return "Y OFFSET";
                case ITEM_SIZE: return "ROZMIAR HUD";
                case ITEM_MENU_SIZE: return "ROZMIAR MENU";
                case ITEM_FONT: return "CZCIONKA";
                case ITEM_FPS: return "FPS HUD";
                case ITEM_BATTERY: return "BATERIA HUD";
                case ITEM_TIME: return "ZEGAR HUD";
                case ITEM_DATE_HUD: return "DATA HUD";
                case ITEM_TIMEMODE: return "CZAS MODE";
                case ITEM_THEME_MENU: return "MOTYW / KOLOR / CZCIONKA";
                case ITEM_HUD_COLORS_MENU: return "HUD KOLORY";
                case ITEM_MENU_COLORS_MENU: return "MENU KOLORY";
                case ITEM_ICON_COLORS_MENU: return "IKONA KOLORY";
                case ITEM_PERFORMANCE_ALERTS_MENU: return "PERFORMANCE ALERTS MENU";
                case ITEM_ALL_HUD_OVERLAYS_MENU: return "MENU WSZYSTKIE HUD";
                case ITEM_MENU_HUD_SIZE_MENU: return "STYLE / MENU / ROZMIARY HUD";
                case ITEM_THEME: return "THEME PRESET";
                case ITEM_HUD_THEME: return "HUD THEME";
                case ITEM_HUD_TEXT: return "HUD TEKST";
                case ITEM_HUD_SHADOW: return "HUD CIEN";
                case ITEM_HUD_ICON: return "BATERIA IKONA";
                case ITEM_CLOCK_ICON: return "ZEGAR IKONA";
                case ITEM_DATE_ICON: return "DATA IKONA";
                case ITEM_FPS_ICON: return "FPS IKONA";
                case ITEM_CPU_ICON: return "CPU IKONA";
                case ITEM_BUS_ICON: return "BUS IKONA";
                case ITEM_GPU_ICON: return "GPU IKONA";
                case ITEM_APP_ICON: return "APP ID IKONA";
                case ITEM_RAM_ICON: return "RAM IKONA";
                case ITEM_HUD_BOX: return "HUD BOX";
                case ITEM_HUD_BOX_BG: return "HUD TLO";
                case ITEM_MENU_TEXT: return "MENU TEKST";
                case ITEM_MENU_SELECT: return "MENU WYBOR";
                case ITEM_MENU_BORDER: return "MENU RAMKA";
                case ITEM_MENUBG: return "MENU TLO";
                case ITEM_TOP_BAR: return "MENU TOP BAR";
                case ITEM_MENU_PICTURE_BG: return "PICTURE TLO";
                case ITEM_PROFILE_MENU: return "PROFIL MENU";
                case ITEM_PROFILE: return "PROFIL SLOT";
                case ITEM_LANGUAGE: return "JEZYK";
                case ITEM_AUTO_HIDE: return "AUTO HIDE";
                case ITEM_TOGGLE: return "KOMBO PRZELACZ";
                case ITEM_TOGGLE_HUD_COMBO: return "HUD COMBO";
                case ITEM_TOGGLE_MENU_COMBO: return "MENU COMBO";
                case ITEM_CYCLE_PROFILE_ENABLE: return "CYCLE PROFIL";
                case ITEM_CYCLE_PROFILE_COMBO: return "PROFIL COMBO";
                case ITEM_CYCLE_THEME_ENABLE: return "CYCLE THEME";
                case ITEM_CYCLE_THEME_COMBO: return "THEME COMBO";
                case ITEM_DEBUG_MENU: return "DEBUG MENU";
                case ITEM_DEBUG_HUD: return "DEBUG HUD";
                case ITEM_DEBUG_POSITION: return "DEBUG POZYCJA";
                case ITEM_DEBUG_X_OFFSET: return "DEBUG X OFFSET";
                case ITEM_DEBUG_Y_OFFSET: return "DEBUG Y OFFSET";
                case ITEM_DEBUG_SIZE: return "DEBUG SIZE";
                case ITEM_DEBUG_FRAMEBUF: return "FRAMEBUF DEBUG";
                case ITEM_DEBUG_CACHE: return "CACHE DEBUG";
                case ITEM_DEBUG_INPUT: return "INPUT DEBUG";
                case ITEM_DEBUG_SYSTEM: return "SYSTEM DEBUG";
                case ITEM_HUD_OPACITY: return "HUD OPACITY";
                case ITEM_MENU_OPACITY: return "MENU OPACITY";
                case ITEM_FPS_STYLE: return "FPS STYL";
                case ITEM_BATTERY_STYLE: return "BATERIA STYL";
                case ITEM_CLOCK_STYLE: return "ZEGAR STYL";
                case ITEM_SAVE_PROFILE: return "ZAPISZ PROFIL";
                case ITEM_LOAD_PROFILE: return "WCZYTAJ PROFIL";
                case ITEM_CPU_HUD: return "CPU HUD";
                case ITEM_BUS_HUD: return "BUS HUD";
                case ITEM_GPU_HUD: return "GPU HUD";
                case ITEM_APP_ID_HUD: return "APP ID HUD";
                case ITEM_RAM_HUD: return "RAM HUD";
                case ITEM_HUD_ORDER_MENU: return "HUD KOLEJNOSC";
                case ITEM_FPS_WARNING: return "FPS ALERT";
                case ITEM_FPS_LOW_LIMIT: return "FPS NISKI LIMIT";
                case ITEM_BATTERY_WARNING: return "BATERIA ALERT";
                case ITEM_BATTERY_LOW_LIMIT: return "BATERIA NISKI LIMIT";
                case ITEM_RAM_WARNING: return "RAM ALERT";
                case ITEM_RESET_HUD_POSITION: return "RESET HUD POZYCJA";
                case ITEM_RESET_COLORS: return "RESET KOLORY";
                case ITEM_RESET_OVERLAYS: return "RESET OVERLAYS";
                case ITEM_RESET_PROFILES: return "RESET PROFILS";
                case ITEM_RESET_ALL_DEFAULTS: return "RESET ALL DEFAULTS";
                case ITEM_HUD_MENU: return "HUD MENU";
                case ITEM_DEBUG_FONT: return "DEBUG CZCIONKA";
                case ITEM_DEBUG_COLOR: return "DEBUG KOLORY";
                case ITEM_ICON_CHANGER_MENU: return "IKONA CHANGER MENU";
                case ITEM_FPS_ICON_STYLE: return "FPS IKONA";
                case ITEM_BATTERY_ICON_STYLE: return "BATERIA IKONA";
                case ITEM_CLOCK_ICON_STYLE: return "ZEGAR IKONA";
                case ITEM_DATE_ICON_STYLE: return "DATA IKONA";
                case ITEM_CPU_ICON_STYLE: return "CPU IKONA";
                case ITEM_BUS_ICON_STYLE: return "BUS IKONA";
                case ITEM_GPU_ICON_STYLE: return "GPU IKONA";
                case ITEM_RAM_ICON_STYLE: return "RAM IKONA";
                case ITEM_APP_ICON_STYLE: return "APP ID IKONA";
                case ITEM_CREATE_HUD_MENU_SIZE: return "UTWORZ HUD MENU SIZE";
                case ITEM_CREATE_MAIN_MENU_SIZE: return "UTWORZ ROZMIAR MENU";
                case ITEM_DEBUG_MENU_INFO: return "MENU DEBUG";
                case ITEM_DEBUG_PROFILE: return "PROFIL DEBUG";
                case ITEM_DEBUG_THEME: return "THEME DEBUG";
                case ITEM_DEBUG_ALERT: return "ALERT DEBUG";
                case ITEM_DEBUG_HUD_INFO: return "HUD DEBUG INFO";
                case ITEM_RESET: return "RESET OPCJE";
                default: return "";
            }
        case LANG_EN:
        default:
            return "";
    }
}

static const char *menu_label(int item);

static const char *choice_title_for_target(void) {
    /* Choice popup titles now use the same translated text as the parent menu row. */
    return menu_label(choice_target_item);
}


static const char *translated_page_title_for_language(int page) {
    if (hud_language == LANG_EN) return 0;
    switch (page) {
        case MENU_PAGE_HUD_ORDER: return translated_menu_label_for_language(hud_language, ITEM_HUD_ORDER_MENU);
        case MENU_PAGE_OVERLAYS: return translated_menu_label_for_language(hud_language, ITEM_ALL_HUD_OVERLAYS_MENU);
        case MENU_PAGE_SIZE: return translated_menu_label_for_language(hud_language, ITEM_MENU_HUD_SIZE_MENU);
        case MENU_PAGE_CREATE_HUD_SIZE: return translated_menu_label_for_language(hud_language, ITEM_CREATE_HUD_MENU_SIZE);
        case MENU_PAGE_CREATE_MAIN_SIZE: return translated_menu_label_for_language(hud_language, ITEM_CREATE_MAIN_MENU_SIZE);
        case MENU_PAGE_HUD_COLORS: return translated_menu_label_for_language(hud_language, ITEM_HUD_COLORS_MENU);
        case MENU_PAGE_MENU_COLORS: return translated_menu_label_for_language(hud_language, ITEM_MENU_COLORS_MENU);
        case MENU_PAGE_ICON_COLORS: return translated_menu_label_for_language(hud_language, ITEM_ICON_COLORS_MENU);
        case MENU_PAGE_PERFORMANCE: return translated_menu_label_for_language(hud_language, ITEM_PERFORMANCE_ALERTS_MENU);
        case MENU_PAGE_RESET_OPTIONS: return translated_menu_label_for_language(hud_language, ITEM_RESET);
        case MENU_PAGE_HUD_MENU: return translated_menu_label_for_language(hud_language, ITEM_HUD_MENU);
        case MENU_PAGE_DEBUG: return translated_menu_label_for_language(hud_language, ITEM_DEBUG_MENU);
        case MENU_PAGE_TOGGLE_COMBOS: return translated_menu_label_for_language(hud_language, ITEM_TOGGLE);
        case MENU_PAGE_ICON_CHANGER: return translated_menu_label_for_language(hud_language, ITEM_ICON_CHANGER_MENU);
        case MENU_PAGE_PROFILE: return translated_menu_label_for_language(hud_language, ITEM_PROFILE_MENU);
        case MENU_PAGE_THEME: return translated_menu_label_for_language(hud_language, ITEM_THEME_MENU);
        default: return 0;
    }
}

static const char *current_menu_title(void) {
    const char *translated;

    if (menu_page == MENU_PAGE_CHOICE) return choice_title_for_target();

    translated = translated_page_title_for_language(menu_page);
    if (translated && translated[0]) return translated;

    if (menu_page == MENU_PAGE_HUD_ORDER) return "HUD ORDER";
    if (menu_page == MENU_PAGE_OVERLAYS) return "ALL HUD OVERLAYS MENU";
    if (menu_page == MENU_PAGE_SIZE) return "DISPLAY STYLES / MENU / HUD SIZES MENU";
    if (menu_page == MENU_PAGE_CREATE_HUD_SIZE) return "CREATE HUD MENU SIZE";
    if (menu_page == MENU_PAGE_CREATE_MAIN_SIZE) return "CREATE MAIN MENU SIZE";
    if (menu_page == MENU_PAGE_HUD_COLORS) return "HUD COLORS";
    if (menu_page == MENU_PAGE_MENU_COLORS) return "MENU COLORS";
    if (menu_page == MENU_PAGE_ICON_COLORS) return "ICON COLORS";
    if (menu_page == MENU_PAGE_PERFORMANCE) return "PERFORMANCE ALERTS MENU";
    if (menu_page == MENU_PAGE_RESET_OPTIONS) return "RESET OPTIONS";
    if (menu_page == MENU_PAGE_HUD_MENU) return "HUD MENU";
    if (menu_page == MENU_PAGE_DEBUG) return "DEBUG MENU";
    if (menu_page == MENU_PAGE_TOGGLE_COMBOS) return "TOGGLE COMBOS";
    if (menu_page == MENU_PAGE_ICON_CHANGER) return "ICON CHANGER MENU";
    if (menu_page == MENU_PAGE_PROFILE) return "PROFILE MENU";
    if (menu_page == MENU_PAGE_THEME) return "THEME / COLOR / FONT MENU";

    return tr_menu_title();
}

static const char *menu_label(int item) {
    if (item >= ITEM_HUD_ORDER_BASE) {
        return hud_order_name_for(hud_order[item - ITEM_HUD_ORDER_BASE]);
    }

    if (item >= ITEM_CHOICE_BASE) {
        return choice_name_for_target(choice_target_item, item - ITEM_CHOICE_BASE);
    }

    if (hud_language != LANG_EN) {
        return translated_menu_label_for_language(hud_language, item);
    }

    if (hud_language == LANG_ES) {
        switch (item) {
            case ITEM_HUD:          return "HUD";
            case ITEM_LAYOUT:       return "DISENO";
            case ITEM_POSITION:     return "POSICION";
            case ITEM_X_OFFSET:     return "AJUSTE X";
            case ITEM_Y_OFFSET:     return "AJUSTE Y";
            case ITEM_SIZE:         return "TAMANO HUD";
            case ITEM_MENU_SIZE:    return "TAMANO MENU";
            case ITEM_FONT:         return "FUENTE";
            case ITEM_FPS:          return "FPS HUD";
            case ITEM_BATTERY:      return "BATERIA HUD";
            case ITEM_TIME:         return "RELOJ HUD";
            case ITEM_DATE_HUD:     return "FECHA HUD";
            case ITEM_TIMEMODE:     return "MODO HORA";
            case ITEM_THEME_MENU:   return "THEME / COLOR / FONT MENU";
            case ITEM_HUD_COLORS_MENU: return "HUD COLORS";
            case ITEM_MENU_COLORS_MENU: return "MENU COLORS";
            case ITEM_ICON_COLORS_MENU: return "ICON COLORS";
            case ITEM_PERFORMANCE_ALERTS_MENU: return "PERFORMANCE ALERTS MENU";
            case ITEM_ALL_HUD_OVERLAYS_MENU: return "TODOS LOS HUD";
            case ITEM_MENU_HUD_SIZE_MENU: return "DISPLAY STYLES / MENU / HUD SIZES MENU";
            case ITEM_THEME:        return "TEMA PRESET";
            case ITEM_HUD_THEME:    return "TEMA HUD";
            case ITEM_HUD_TEXT:     return "TEXTO HUD";
            case ITEM_HUD_SHADOW:   return "SOMBRA HUD";
            case ITEM_HUD_ICON:     return "ICONO BATERIA";
            case ITEM_CLOCK_ICON:   return "ICONO RELOJ";
            case ITEM_DATE_ICON:    return "ICONO FECHA";
            case ITEM_FPS_ICON:     return "ICONO FPS";
            case ITEM_CPU_ICON:     return "ICONO CPU";
            case ITEM_BUS_ICON:     return "ICONO BUS";
            case ITEM_GPU_ICON:     return "ICONO GPU";
            case ITEM_APP_ICON:     return "ICONO APP ID";
            case ITEM_RAM_ICON:     return "ICONO RAM";
            case ITEM_HUD_BOX:      return "CAJA HUD";
            case ITEM_HUD_BOX_BG:   return "FONDO HUD";
            case ITEM_MENU_TEXT:    return "TEXTO MENU";
            case ITEM_MENU_SELECT:  return "SELECCION MENU";
            case ITEM_MENU_BORDER:  return "BORDE MENU";
            case ITEM_MENUBG:       return "FONDO MENU";
            case ITEM_TOP_BAR:      return "BARRA MENU TOP";
            case ITEM_MENU_PICTURE_BG: return "FONDO IMAGEN";
            case ITEM_PROFILE_MENU: return "MENU PERFIL";
            case ITEM_PROFILE:      return "RANURA PERFIL";
            case ITEM_LANGUAGE:     return "IDIOMA";
            case ITEM_AUTO_HIDE:    return "AUTO OCULTAR";
            case ITEM_TOGGLE:       return "COMBOS";
            case ITEM_TOGGLE_HUD_COMBO: return "HUD COMBO";
            case ITEM_TOGGLE_MENU_COMBO: return "MENU COMBO";
            case ITEM_CYCLE_PROFILE_ENABLE: return "CYCLE PROFILE";
            case ITEM_CYCLE_PROFILE_COMBO: return "PROFILE COMBO";
            case ITEM_CYCLE_THEME_ENABLE: return "CYCLE THEME";
            case ITEM_CYCLE_THEME_COMBO: return "THEME COMBO";
            case ITEM_DEBUG_MENU: return "DEBUG MENU";
            case ITEM_DEBUG_HUD: return "DEBUG HUD";
            case ITEM_DEBUG_POSITION: return "DEBUG POSITION";
            case ITEM_DEBUG_X_OFFSET: return "DEBUG X OFFSET";
            case ITEM_DEBUG_Y_OFFSET: return "DEBUG Y OFFSET";
            case ITEM_DEBUG_SIZE: return "DEBUG SIZE";
            case ITEM_DEBUG_FRAMEBUF: return "FRAMEBUF DEBUG";
            case ITEM_DEBUG_CACHE: return "CACHE DEBUG";
            case ITEM_DEBUG_INPUT: return "INPUT DEBUG";
            case ITEM_DEBUG_SYSTEM: return "SYSTEM DEBUG";
            case ITEM_HUD_OPACITY: return "HUD OPACITY";
            case ITEM_MENU_OPACITY: return "MENU OPACITY";
            case ITEM_FPS_STYLE: return "FPS STYLE";
            case ITEM_BATTERY_STYLE: return "BATTERY STYLE";
            case ITEM_CLOCK_STYLE: return "CLOCK STYLE";
            case ITEM_SAVE_PROFILE: return "GUARDAR PERFIL";
            case ITEM_LOAD_PROFILE: return "CARGAR PERFIL";
            case ITEM_CPU_HUD:      return "HUD CPU";
            case ITEM_BUS_HUD:      return "HUD BUS";
            case ITEM_GPU_HUD:      return "HUD GPU";
            case ITEM_APP_ID_HUD:   return "HUD APP ID";
            case ITEM_RAM_HUD:      return "HUD RAM";
            case ITEM_HUD_ORDER_MENU: return "ORDEN HUD";
            case ITEM_FPS_WARNING:  return "FPS WARNING";
            case ITEM_FPS_LOW_LIMIT:return "FPS LOW LIMIT";
            case ITEM_BATTERY_WARNING: return "BATTERY WARNING";
            case ITEM_BATTERY_LOW_LIMIT: return "BATTERY LOW LIMIT";
            case ITEM_RAM_WARNING:  return "RAM WARNING";
            case ITEM_RESET_HUD_POSITION: return "RESET HUD POSITION";
            case ITEM_RESET_COLORS: return "RESET COLORS";
            case ITEM_RESET_OVERLAYS: return "RESET OVERLAYS";
            case ITEM_RESET_PROFILES: return "RESET PROFILES";
            case ITEM_RESET_ALL_DEFAULTS: return "RESET ALL DEFAULTS";
            case ITEM_HUD_MENU:     return "HUD MENU";
            case ITEM_DEBUG_FONT:   return "DEBUG FONT";
            case ITEM_DEBUG_COLOR:  return "DEBUG COLORS";
            case ITEM_ICON_CHANGER_MENU: return "ICON CHANGER MENU";
            case ITEM_FPS_ICON_STYLE: return "FPS ICON";
            case ITEM_BATTERY_ICON_STYLE: return "BATTERY ICON";
            case ITEM_CLOCK_ICON_STYLE: return "CLOCK ICON";
            case ITEM_DATE_ICON_STYLE: return "DATE ICON";
            case ITEM_CPU_ICON_STYLE: return "CPU ICON";
            case ITEM_BUS_ICON_STYLE: return "BUS ICON";
            case ITEM_GPU_ICON_STYLE: return "GPU ICON";
            case ITEM_RAM_ICON_STYLE: return "RAM ICON";
            case ITEM_APP_ICON_STYLE: return "APP ID ICON";
            case ITEM_CREATE_HUD_MENU_SIZE: return "CREATE HUD MENU SIZE";
            case ITEM_CREATE_MAIN_MENU_SIZE: return "CREATE MAIN MENU SIZE";
            case ITEM_DEBUG_MENU_INFO: return "MENU DEBUG";
            case ITEM_DEBUG_PROFILE: return "PROFILE DEBUG";
            case ITEM_DEBUG_THEME: return "THEME DEBUG";
            case ITEM_DEBUG_ALERT: return "ALERT DEBUG";
            case ITEM_DEBUG_HUD_INFO: return "HUD DEBUG INFO";
            case ITEM_RESET:        return "RESET OPTIONS";
            default:                return "";
        }
    }

    switch (item) {
        case ITEM_HUD:          return "HUD";
        case ITEM_LAYOUT:       return "LAYOUT";
        case ITEM_POSITION:     return "POSITION";
        case ITEM_X_OFFSET:     return "X OFFSET";
        case ITEM_Y_OFFSET:     return "Y OFFSET";
        case ITEM_SIZE:         return "HUD SIZE";
        case ITEM_MENU_SIZE:    return "MAIN MENU SIZE";
        case ITEM_FONT:         return "FONT";
        case ITEM_FPS:          return "FPS HUD";
        case ITEM_BATTERY:      return "BATTERY HUD";
        case ITEM_TIME:         return "CLOCK HUD";
        case ITEM_DATE_HUD:     return "DATE HUD";
        case ITEM_TIMEMODE:     return "TIME MODE";
        case ITEM_THEME_MENU:   return "THEME / COLOR / FONT MENU";
        case ITEM_HUD_COLORS_MENU: return "HUD COLORS";
        case ITEM_MENU_COLORS_MENU: return "MENU COLORS";
        case ITEM_ICON_COLORS_MENU: return "ICON COLORS";
        case ITEM_PERFORMANCE_ALERTS_MENU: return "PERFORMANCE ALERTS MENU";
        case ITEM_ALL_HUD_OVERLAYS_MENU: return "ALL HUD OVERLAYS MENU";
        case ITEM_MENU_HUD_SIZE_MENU: return "DISPLAY STYLES / MENU / HUD SIZES MENU";
        case ITEM_THEME:        return "THEME PRESET";
        case ITEM_HUD_THEME:    return "HUD THEME";
        case ITEM_HUD_TEXT:     return "HUD TEXT";
        case ITEM_HUD_SHADOW:   return "HUD SHADOW";
        case ITEM_HUD_ICON:     return "BATTERY ICON";
        case ITEM_CLOCK_ICON:   return "CLOCK ICON";
        case ITEM_DATE_ICON:    return "DATE ICON";
        case ITEM_FPS_ICON:     return "FPS ICON";
        case ITEM_CPU_ICON:     return "CPU ICON";
        case ITEM_BUS_ICON:     return "BUS ICON";
        case ITEM_GPU_ICON:     return "GPU ICON";
        case ITEM_APP_ICON:     return "APP ID ICON";
        case ITEM_RAM_ICON:     return "RAM ICON";
        case ITEM_HUD_BOX:      return "HUD BOX";
        case ITEM_HUD_BOX_BG:   return "HUD BACKGROUND";
        case ITEM_MENU_TEXT:    return "MENU TEXT";
        case ITEM_MENU_SELECT:  return "MENU SELECT";
        case ITEM_MENU_BORDER:  return "MENU BORDER";
        case ITEM_MENUBG:       return "MENU BACKGROUND";
        case ITEM_TOP_BAR:      return "MENU TOP BAR";
        case ITEM_MENU_PICTURE_BG: return "PICTURE BACKGROUND";
        case ITEM_PROFILE_MENU: return "PROFILE MENU";
        case ITEM_PROFILE:      return "PROFILE SLOT";
        case ITEM_LANGUAGE:     return "LANGUAGE";
        case ITEM_AUTO_HIDE:    return "AUTO HIDE";
        case ITEM_TOGGLE:       return "TOGGLE COMBOS";
        case ITEM_TOGGLE_HUD_COMBO: return "HUD COMBO";
        case ITEM_TOGGLE_MENU_COMBO: return "MENU COMBO";
        case ITEM_CYCLE_PROFILE_ENABLE: return "CYCLE PROFILE";
        case ITEM_CYCLE_PROFILE_COMBO: return "PROFILE COMBO";
        case ITEM_CYCLE_THEME_ENABLE: return "CYCLE THEME";
        case ITEM_CYCLE_THEME_COMBO: return "THEME COMBO";
        case ITEM_DEBUG_MENU: return "DEBUG MENU";
        case ITEM_DEBUG_HUD: return "DEBUG HUD";
        case ITEM_DEBUG_POSITION: return "DEBUG POSITION";
        case ITEM_DEBUG_X_OFFSET: return "DEBUG X OFFSET";
        case ITEM_DEBUG_Y_OFFSET: return "DEBUG Y OFFSET";
        case ITEM_DEBUG_SIZE: return "DEBUG SIZE";
        case ITEM_DEBUG_FRAMEBUF: return "FRAMEBUF DEBUG";
        case ITEM_DEBUG_CACHE: return "CACHE DEBUG";
        case ITEM_DEBUG_INPUT: return "INPUT DEBUG";
        case ITEM_DEBUG_SYSTEM: return "SYSTEM DEBUG";
        case ITEM_HUD_OPACITY: return "HUD OPACITY";
        case ITEM_MENU_OPACITY: return "MENU OPACITY";
        case ITEM_FPS_STYLE: return "FPS STYLE";
        case ITEM_BATTERY_STYLE: return "BATTERY STYLE";
        case ITEM_CLOCK_STYLE: return "CLOCK STYLE";
        case ITEM_SAVE_PROFILE: return "SAVE PROFILE";
        case ITEM_LOAD_PROFILE: return "LOAD PROFILE";
        case ITEM_CPU_HUD:      return "CPU HUD";
        case ITEM_BUS_HUD:      return "BUS HUD";
        case ITEM_GPU_HUD:      return "GPU HUD";
        case ITEM_APP_ID_HUD:   return "APP ID HUD";
        case ITEM_RAM_HUD:      return "RAM HUD";
        case ITEM_HUD_ORDER_MENU: return "HUD ORDER";
        case ITEM_FPS_WARNING:  return "FPS WARNING";
        case ITEM_FPS_LOW_LIMIT:return "FPS LOW LIMIT";
        case ITEM_BATTERY_WARNING: return "BATTERY WARNING";
        case ITEM_BATTERY_LOW_LIMIT: return "BATTERY LOW LIMIT";
        case ITEM_RAM_WARNING:  return "RAM WARNING";
        case ITEM_RESET_HUD_POSITION: return "RESET HUD POSITION";
        case ITEM_RESET_COLORS: return "RESET COLORS";
        case ITEM_RESET_OVERLAYS: return "RESET OVERLAYS";
        case ITEM_RESET_PROFILES: return "RESET PROFILES";
        case ITEM_RESET_ALL_DEFAULTS: return "RESET ALL DEFAULTS";
        case ITEM_HUD_MENU:     return "HUD MENU";
        case ITEM_DEBUG_FONT:   return "DEBUG FONT";
        case ITEM_DEBUG_COLOR:  return "DEBUG COLORS";
        case ITEM_ICON_CHANGER_MENU: return "ICON CHANGER MENU";
        case ITEM_FPS_ICON_STYLE: return "FPS ICON";
        case ITEM_BATTERY_ICON_STYLE: return "BATTERY ICON";
        case ITEM_CLOCK_ICON_STYLE: return "CLOCK ICON";
        case ITEM_DATE_ICON_STYLE: return "DATE ICON";
        case ITEM_CPU_ICON_STYLE: return "CPU ICON";
        case ITEM_BUS_ICON_STYLE: return "BUS ICON";
        case ITEM_GPU_ICON_STYLE: return "GPU ICON";
        case ITEM_RAM_ICON_STYLE: return "RAM ICON";
        case ITEM_APP_ICON_STYLE: return "APP ID ICON";
        case ITEM_CREATE_HUD_MENU_SIZE: return "CREATE HUD MENU SIZE";
        case ITEM_CREATE_MAIN_MENU_SIZE: return "CREATE MAIN MENU SIZE";
        case ITEM_DEBUG_MENU_INFO: return "MENU DEBUG";
        case ITEM_DEBUG_PROFILE: return "PROFILE DEBUG";
        case ITEM_DEBUG_THEME: return "THEME DEBUG";
        case ITEM_DEBUG_ALERT: return "ALERT DEBUG";
        case ITEM_DEBUG_HUD_INFO: return "HUD DEBUG INFO";
        case ITEM_RESET:        return "RESET OPTIONS";
        default:                return "";
    }
}

static const char *menu_value(int item) {
    if (item >= ITEM_HUD_ORDER_BASE) {
        return "L/R MOVE";
    }

    if (item >= ITEM_CHOICE_BASE) {
        return (item - ITEM_CHOICE_BASE) == choice_current_index_for_target(choice_target_item) ? "CURRENT" : "";
    }

    switch (item) {
        case ITEM_HUD:          return onoff_name(hud_enabled);
        case ITEM_LAYOUT:       return layout_name();
        case ITEM_POSITION:     return position_name();
        case ITEM_SIZE:         return size_name();
        case ITEM_MENU_SIZE:    return main_menu_size_name();
        case ITEM_FONT:         return font_name();
        case ITEM_FPS:          return onoff_name(show_fps);
        case ITEM_BATTERY:      return onoff_name(show_battery);
        case ITEM_TIME:         return onoff_name(show_time);
        case ITEM_DATE_HUD:     return onoff_name(show_date);
        case ITEM_TIMEMODE:     return time_mode_name();
        case ITEM_HUD_TEXT:     return color_name_generic(hud_text_color);
        case ITEM_HUD_SHADOW:   return color_name_generic(hud_shadow_color);
        case ITEM_HUD_ICON:     return color_name_generic(hud_icon_color);
        case ITEM_CLOCK_ICON:   return color_name_generic(clock_icon_color);
        case ITEM_DATE_ICON:    return color_name_generic(date_icon_color);
        case ITEM_FPS_ICON:     return color_name_generic(fps_icon_color);
        case ITEM_CPU_ICON:     return color_name_generic(cpu_icon_color);
        case ITEM_BUS_ICON:     return color_name_generic(bus_icon_color);
        case ITEM_GPU_ICON:     return color_name_generic(gpu_icon_color);
        case ITEM_APP_ICON:     return color_name_generic(app_icon_color);
        case ITEM_RAM_ICON:     return color_name_generic(ram_icon_color);
        case ITEM_MENU_TEXT:    return color_name_generic(menu_text_color);
        case ITEM_MENU_SELECT:  return color_name_generic(menu_select_color);
        case ITEM_MENU_BORDER:  return color_name_generic(menu_border_color);
        case ITEM_MENUBG:       return menu_bg_name();
        case ITEM_TOP_BAR:      return color_name_generic(top_menu_bar_color);
        case ITEM_MENU_PICTURE_BG: return onoff_name(menu_picture_bg);
        case ITEM_HUD_BOX:      return onoff_name(hud_box_enabled);
        case ITEM_HUD_BOX_BG:   return menu_bg_name_for(hud_box_bg_color);
        case ITEM_THEME_MENU:   return word_open();
        case ITEM_HUD_COLORS_MENU: return word_open();
        case ITEM_MENU_COLORS_MENU: return word_open();
        case ITEM_ICON_COLORS_MENU: return word_open();
        case ITEM_ICON_CHANGER_MENU: return word_open();
        case ITEM_PERFORMANCE_ALERTS_MENU: return word_open();
        case ITEM_ALL_HUD_OVERLAYS_MENU: return word_open();
        case ITEM_MENU_HUD_SIZE_MENU: return word_open();
        case ITEM_DEBUG_MENU: return word_open();
        case ITEM_HUD_MENU: return word_open();
        case ITEM_THEME:        return theme_name();
        case ITEM_HUD_THEME:    return hud_theme_name();
        case ITEM_PROFILE_MENU: return word_open();
        case ITEM_HUD_ORDER_MENU: return word_open();
        case ITEM_PROFILE:      return profile_name();
        case ITEM_LANGUAGE:     return language_name();
        case ITEM_AUTO_HIDE:    return auto_hide_name();
        case ITEM_TOGGLE:       return word_open();
        case ITEM_TOGGLE_HUD_COMBO: return combo_name_for(toggle_combo_mode);
        case ITEM_TOGGLE_MENU_COMBO: return combo_name_for(menu_toggle_combo_mode);
        case ITEM_CYCLE_PROFILE_ENABLE: return onoff_name(cycle_profile_enabled);
        case ITEM_CYCLE_PROFILE_COMBO: return combo_name_for(cycle_profile_combo_mode);
        case ITEM_CYCLE_THEME_ENABLE: return onoff_name(cycle_theme_enabled);
        case ITEM_CYCLE_THEME_COMBO: return combo_name_for(cycle_theme_combo_mode);
        case ITEM_DEBUG_HUD: return onoff_name(debug_enabled);
        case ITEM_DEBUG_POSITION: return position_name_for(debug_position);
        case ITEM_DEBUG_SIZE: return size_name_for(debug_size);
        case ITEM_DEBUG_FRAMEBUF: return onoff_name(debug_show_framebuf);
        case ITEM_DEBUG_CACHE: return onoff_name(debug_show_cache);
        case ITEM_DEBUG_INPUT: return onoff_name(debug_show_input);
        case ITEM_DEBUG_SYSTEM: return onoff_name(debug_show_system);
        case ITEM_DEBUG_COLOR: return color_name_generic(debug_text_color);
        case ITEM_DEBUG_FONT: return font_name_for(debug_font_style);
        case ITEM_HUD_OPACITY: return opacity_name_for(hud_opacity);
        case ITEM_MENU_OPACITY: return opacity_name_for(menu_opacity);
        case ITEM_FPS_STYLE: return fps_style_name();
        case ITEM_BATTERY_STYLE: return battery_style_name();
        case ITEM_CLOCK_STYLE: return clock_style_name();
        case ITEM_FPS_ICON_STYLE: return icon_style_name_for(fps_icon_style);
        case ITEM_BATTERY_ICON_STYLE: return icon_style_name_for(battery_icon_style);
        case ITEM_CLOCK_ICON_STYLE: return icon_style_name_for(clock_icon_style);
        case ITEM_DATE_ICON_STYLE: return icon_style_name_for(date_icon_style);
        case ITEM_CPU_ICON_STYLE: return icon_style_name_for(cpu_icon_style);
        case ITEM_BUS_ICON_STYLE: return icon_style_name_for(bus_icon_style);
        case ITEM_GPU_ICON_STYLE: return icon_style_name_for(gpu_icon_style);
        case ITEM_RAM_ICON_STYLE: return icon_style_name_for(ram_icon_style);
        case ITEM_APP_ICON_STYLE: return icon_style_name_for(app_icon_style);
        case ITEM_CREATE_HUD_MENU_SIZE: return menu_page == MENU_PAGE_SIZE ? word_open() : percent_name_for(create_hud_menu_size);
        case ITEM_CREATE_MAIN_MENU_SIZE: return menu_page == MENU_PAGE_SIZE ? word_open() : percent_name_for(create_main_menu_size);
        case ITEM_DEBUG_MENU_INFO: return onoff_name(debug_show_menu_info);
        case ITEM_DEBUG_PROFILE: return onoff_name(debug_show_profile);
        case ITEM_DEBUG_THEME: return onoff_name(debug_show_theme);
        case ITEM_DEBUG_ALERT: return onoff_name(debug_show_alert);
        case ITEM_DEBUG_HUD_INFO: return onoff_name(debug_show_hud_info);
        case ITEM_SAVE_PROFILE: return save_message_frames > 0 ? word_saved() : word_press_x();
        case ITEM_LOAD_PROFILE: return save_message_frames > 0 ? word_loaded() : word_press_x();
        case ITEM_CPU_HUD:      return onoff_name(show_cpu);
        case ITEM_BUS_HUD:      return onoff_name(show_bus);
        case ITEM_GPU_HUD:      return onoff_name(show_gpu);
        case ITEM_APP_ID_HUD:   return onoff_name(show_app_id);
        case ITEM_RAM_HUD:      return onoff_name(show_ram);
        case ITEM_FPS_WARNING:  return onoff_name(fps_warning_enabled);
        case ITEM_FPS_LOW_LIMIT:return fps_low_limit_name();
        case ITEM_BATTERY_WARNING: return onoff_name(battery_warning_enabled);
        case ITEM_BATTERY_LOW_LIMIT: return battery_low_limit_name();
        case ITEM_RAM_WARNING:  return onoff_name(ram_warning_enabled);
        case ITEM_RESET:        return word_open();
        case ITEM_RESET_HUD_POSITION: return reset_message_frames > 0 ? "RESET" : word_press_x();
        case ITEM_RESET_COLORS: return reset_message_frames > 0 ? "RESET" : word_press_x();
        case ITEM_RESET_OVERLAYS: return reset_message_frames > 0 ? "RESET" : word_press_x();
        case ITEM_RESET_PROFILES: return reset_message_frames > 0 ? "RESET" : word_press_x();
        case ITEM_RESET_ALL_DEFAULTS: return reset_message_frames > 0 ? "RESET" : word_press_x();
        default:                return "";
    }
}

static int menu_offset_step(void) {
    if (menu_lr_step < 2) {
        return 2;
    }

    if (menu_lr_step > 64) {
        return 64;
    }

    return menu_lr_step;
}

static void menu_change(int dir) {
    int item = current_menu_item();

    if (item >= ITEM_HUD_ORDER_BASE) {
        move_hud_order_item(item - ITEM_HUD_ORDER_BASE, dir);
        return;
    }

    if (item >= ITEM_CHOICE_BASE) {
        /* Stay inside selection menus until O is pressed. */
        set_choice_for_target(choice_target_item, item - ITEM_CHOICE_BASE);
        return;
    }

    if (item_uses_choice_menu(item)) {
        enter_choice_menu(item);
        return;
    }

    switch (item) {
        case ITEM_THEME_MENU:
            enter_menu_page(MENU_PAGE_THEME);
            break;

        case ITEM_HUD_COLORS_MENU:
            enter_menu_page(MENU_PAGE_HUD_COLORS);
            break;

        case ITEM_MENU_COLORS_MENU:
            enter_menu_page(MENU_PAGE_MENU_COLORS);
            break;

        case ITEM_ICON_COLORS_MENU:
            enter_menu_page(MENU_PAGE_ICON_COLORS);
            break;

        case ITEM_ICON_CHANGER_MENU:
            enter_menu_page(MENU_PAGE_ICON_CHANGER);
            break;

        case ITEM_ALL_HUD_OVERLAYS_MENU:
            enter_menu_page(MENU_PAGE_OVERLAYS);
            break;

        case ITEM_PERFORMANCE_ALERTS_MENU:
            enter_menu_page(MENU_PAGE_PERFORMANCE);
            break;

        case ITEM_DEBUG_MENU:
            enter_menu_page(MENU_PAGE_DEBUG);
            break;

        case ITEM_HUD_MENU:
            enter_menu_page(MENU_PAGE_HUD_MENU);
            break;

        case ITEM_PROFILE_MENU:
            enter_menu_page(MENU_PAGE_PROFILE);
            break;

        case ITEM_MENU_HUD_SIZE_MENU:
            enter_menu_page(MENU_PAGE_SIZE);
            break;

        case ITEM_HUD_ORDER_MENU:
            enter_menu_page(MENU_PAGE_HUD_ORDER);
            break;

        case ITEM_RESET:
            enter_menu_page(MENU_PAGE_RESET_OPTIONS);
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

        case ITEM_MENU_SIZE:
            main_menu_size += dir;
            if (main_menu_size < 0) main_menu_size = MAIN_MENU_SIZE_COUNT - 1;
            if (main_menu_size >= MAIN_MENU_SIZE_COUNT) main_menu_size = 0;
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

        case ITEM_DATE_HUD:
            show_date = !show_date;
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

        case ITEM_DATE_ICON:
            date_icon_color += dir;
            if (date_icon_color < 0) date_icon_color = COLOR_COUNT - 1;
            if (date_icon_color >= COLOR_COUNT) date_icon_color = 0;
            break;

        case ITEM_FPS_ICON:
            fps_icon_color += dir;
            if (fps_icon_color < 0) fps_icon_color = COLOR_COUNT - 1;
            if (fps_icon_color >= COLOR_COUNT) fps_icon_color = 0;
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

        case ITEM_MENU_PICTURE_BG:
            menu_picture_bg = !menu_picture_bg;
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
            enter_menu_page(MENU_PAGE_TOGGLE_COMBOS);
            break;

        case ITEM_CYCLE_PROFILE_ENABLE:
            cycle_profile_enabled = !cycle_profile_enabled;
            break;

        case ITEM_CYCLE_THEME_ENABLE:
            cycle_theme_enabled = !cycle_theme_enabled;
            break;

        case ITEM_DEBUG_HUD:
            debug_enabled = !debug_enabled;
            break;

        case ITEM_DEBUG_X_OFFSET:
            debug_x_offset += dir * menu_offset_step();
            if (debug_x_offset < 0) debug_x_offset = 0;
            if (debug_x_offset > 960) debug_x_offset = 960;
            break;

        case ITEM_DEBUG_Y_OFFSET:
            debug_y_offset += dir * menu_offset_step();
            if (debug_y_offset < 0) debug_y_offset = 0;
            if (debug_y_offset > 544) debug_y_offset = 544;
            break;

        case ITEM_DEBUG_FRAMEBUF:
            debug_show_framebuf = !debug_show_framebuf;
            break;

        case ITEM_DEBUG_CACHE:
            debug_show_cache = !debug_show_cache;
            break;

        case ITEM_DEBUG_INPUT:
            debug_show_input = !debug_show_input;
            break;

        case ITEM_DEBUG_SYSTEM:
            debug_show_system = !debug_show_system;
            break;

        case ITEM_DEBUG_MENU_INFO:
            debug_show_menu_info = !debug_show_menu_info;
            break;

        case ITEM_DEBUG_PROFILE:
            debug_show_profile = !debug_show_profile;
            break;

        case ITEM_DEBUG_THEME:
            debug_show_theme = !debug_show_theme;
            break;

        case ITEM_DEBUG_ALERT:
            debug_show_alert = !debug_show_alert;
            break;

        case ITEM_DEBUG_HUD_INFO:
            debug_show_hud_info = !debug_show_hud_info;
            break;

        case ITEM_CREATE_HUD_MENU_SIZE:
            if (menu_page == MENU_PAGE_SIZE) {
                enter_menu_page(MENU_PAGE_CREATE_HUD_SIZE);
            } else {
                create_hud_menu_size += dir * 5;
                if (create_hud_menu_size < 10) create_hud_menu_size = 150;
                if (create_hud_menu_size > 150) create_hud_menu_size = 10;
            }
            break;

        case ITEM_CREATE_MAIN_MENU_SIZE:
            if (menu_page == MENU_PAGE_SIZE) {
                enter_menu_page(MENU_PAGE_CREATE_MAIN_SIZE);
            } else {
                create_main_menu_size += dir * 5;
                if (create_main_menu_size < 10) create_main_menu_size = 150;
                if (create_main_menu_size > 150) create_main_menu_size = 10;
            }
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

        case ITEM_FPS_WARNING:
            fps_warning_enabled = !fps_warning_enabled;
            break;

        case ITEM_FPS_LOW_LIMIT:
            fps_low_limit_index += dir;
            if (fps_low_limit_index < 0) fps_low_limit_index = 5;
            if (fps_low_limit_index > 5) fps_low_limit_index = 0;
            break;

        case ITEM_BATTERY_WARNING:
            battery_warning_enabled = !battery_warning_enabled;
            break;

        case ITEM_BATTERY_LOW_LIMIT:
            battery_low_limit_index += dir;
            if (battery_low_limit_index < 0) battery_low_limit_index = 2;
            if (battery_low_limit_index > 2) battery_low_limit_index = 0;
            break;

        case ITEM_RAM_WARNING:
            ram_warning_enabled = !ram_warning_enabled;
            break;

        case ITEM_RESET_HUD_POSITION:
            reset_hud_position_defaults();
            break;

        case ITEM_RESET_COLORS:
            reset_color_defaults();
            break;

        case ITEM_RESET_OVERLAYS:
            reset_overlay_defaults();
            break;

        case ITEM_RESET_PROFILES:
            reset_profile_files();
            break;

        case ITEM_RESET_ALL_DEFAULTS:
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
    int deco;

    unsigned int border = color_value(menu_border_color, 0xFFFFFFFF);
    unsigned int title_col = color_value(menu_select_color, 0xFF00FFFF);
    unsigned int text_col = color_value(menu_text_color, 0xFFFFFFFF);
    unsigned int inner_line = 0xFF2A2A2A;

    g_menu_text_mode = 1;

    if (main_menu_size == MAIN_MENU_SIZE_COMPACT) {
        x = 56;
        y = 46;
        w = 382;
        h = 222;
        visible = 13;
    } else if (main_menu_size == MAIN_MENU_SIZE_LARGE) {
        x = 20;
        y = 30;
        w = 560;
        h = 322;
        visible = 21;
    }

    if (screen_w <= 480 || screen_h <= 272) {
        x = 10;
        y = 10;
        w = screen_w - 20;
        h = screen_h - 20;
        visible = 11;
    } else {
        /* CREATE MAIN MENU SIZE scales the entire VitaHUD Ultimate menu, including submenus. */
        int custom_scale = create_main_menu_size;
        int center_x = x + (w / 2);
        int center_y = y + (h / 2);

        if (custom_scale < 10) custom_scale = 10;
        if (custom_scale > 150) custom_scale = 150;

        w = (w * custom_scale) / 100;
        h = (h * custom_scale) / 100;
        visible = (h - 54) / 12;
        if (visible < 1) visible = 1;
        if (visible > 26) visible = 26;

        x = center_x - (w / 2);
        y = center_y - (h / 2);
        if (x < 10) x = 10;
        if (y < 10) y = 10;
        if (x + w > screen_w - 10) x = screen_w - 10 - w;
        if (y + h > screen_h - 10) y = screen_h - 10 - h;
        if (x < 0) x = 0;
        if (y < 0) y = 0;
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
        draw_rect_menu_opacity_fast(pixels, pitch, panel_x, panel_y, panel_w, panel_h, get_menu_bg_for(menu_bg_color), menu_opacity);
    }

    if (menu_picture_bg) {
        for (deco = 0; deco < panel_w + panel_h; deco += 18) {
            draw_rect(pixels, pitch, panel_x + deco - panel_h, panel_y + 22, 1, panel_h - 44, 0x33101040);
            draw_rect(pixels, pitch, panel_x + panel_w - deco, panel_y + 22, 1, panel_h - 44, 0x33401010);
        }
        draw_rect(pixels, pitch, panel_x + 8, panel_y + 24, panel_w - 16, 1, 0x664040FF);
        draw_rect(pixels, pitch, panel_x + 8, panel_y + panel_h - 25, panel_w - 16, 1, 0x6600FFFF);
    }

    /* Ultimate menu skin: layered header, side rail, corner accents, and inner glow. */
    draw_rect_menu_opacity_fast(pixels, pitch, panel_x + 2, panel_y + 2, panel_w - 4, 14, color_value(top_menu_bar_color, 0xFF000000), menu_opacity);
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

    if (menu_page != MENU_PAGE_MAIN) {
        /* Lowered slightly so it does not overlap the header border. */
        draw_text_shadow(pixels, pitch, panel_x + 10, title_y + 4, "< BACK", title_col, 1);
    }

    if (menu_scroll > 0) {
        draw_text_shadow(pixels, pitch, panel_x + panel_w - 22, y + 1, "^", title_col, 1);
    }

    if (menu_scroll + visible < current_menu_count()) {
        draw_text_shadow(pixels, pitch, panel_x + panel_w - 22, y + h - 36, "v", title_col, 1);
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

    if (menu_page == MENU_PAGE_HUD_ORDER) {
        draw_text_shadow(
            pixels,
            pitch,
            x,
            y + h - 22,
            "UP/DOWN SELECT  LEFT/RIGHT MOVE  O BACK",
            text_col,
            1
        );
    } else {
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

    g_menu_text_mode = 0;
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
    if (held_usec < 700000) {
        return 140000;
    }

    if (held_usec < 1300000) {
        return 85000;
    }

    if (held_usec < 2300000) {
        return 50000;
    }

    return 28000;
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
        menu_nav_next_repeat_tick = now + 360000;

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
        menu_nav_next_repeat_tick = now + 360000;
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
    return item == ITEM_X_OFFSET || item == ITEM_Y_OFFSET || item == ITEM_DEBUG_X_OFFSET || item == ITEM_DEBUG_Y_OFFSET;
}

static void reset_menu_lr_repeat(void) {
    menu_lr_dir = 0;
    menu_lr_hold_start_tick = 0;
    menu_lr_next_repeat_tick = 0;
    menu_lr_step = 4;
}

static int menu_lr_repeat_interval_usec(SceUInt64 held_usec) {
    if (held_usec < 250000) {
        return 60000;
    }

    if (held_usec < 700000) {
        return 36000;
    }

    if (held_usec < 1200000) {
        return 22000;
    }

    return 12000;
}

static int menu_lr_step_for_hold(SceUInt64 held_usec) {
    if (held_usec < 250000) {
        return 6;
    }

    if (held_usec < 700000) {
        return 14;
    }

    if (held_usec < 1200000) {
        return 28;
    }

    if (held_usec < 2000000) {
        return 44;
    }

    return 64;
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
        menu_lr_next_repeat_tick = now + 180000;
        return;
    }

    if (menu_lr_hold_start_tick == 0) {
        menu_lr_hold_start_tick = now;
        menu_lr_next_repeat_tick = now + 180000;
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


static int combo_exact_down(unsigned int buttons, int combo_mode) {
    unsigned int mask = combo_buttons(combo_mode);
    unsigned int relevant = SCE_CTRL_LTRIGGER | SCE_CTRL_RTRIGGER | SCE_CTRL_SELECT | SCE_CTRL_START |
                            SCE_CTRL_UP | SCE_CTRL_DOWN | SCE_CTRL_LEFT | SCE_CTRL_RIGHT |
                            SCE_CTRL_TRIANGLE | SCE_CTRL_CIRCLE | SCE_CTRL_SQUARE | SCE_CTRL_CROSS;

    if (mask == 0) return 0;
    return (buttons & relevant) == mask;
}

static int cycle_worker_thread(SceSize args, void *argp) {
    int action;
    (void)args;
    (void)argp;

    action = cycle_worker_action;

    if (action == 1) {
        load_profile();
    } else if (action == 2) {
        apply_theme();
        save_message_frames = 120;
    }

    cycle_worker_action = 0;
    cycle_worker_busy = 0;
    sceKernelExitDeleteThread(0);
    return 0;
}

static void start_cycle_worker(int action) {
    SceUID thid;

    if (cycle_worker_busy) {
        return;
    }

    cycle_worker_busy = 1;
    cycle_worker_action = action;

    thid = sceKernelCreateThread("VitaHUDCycle", cycle_worker_thread, 0x10000100, 0x4000, 0, 0, NULL);
    if (thid >= 0) {
        sceKernelStartThread(thid, 0, NULL);
    } else {
        cycle_worker_action = 0;
        cycle_worker_busy = 0;
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
    menu_down = ((buttons & get_menu_combo()) == get_menu_combo());

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

    if (!menu_open) {
        int profile_cycle_down = cycle_profile_enabled && !menu_down && !toggle_down && combo_exact_down(buttons, cycle_profile_combo_mode);
        int theme_cycle_down = cycle_theme_enabled && !menu_down && !toggle_down && combo_exact_down(buttons, cycle_theme_combo_mode);

        if (profile_cycle_down && !last_profile_cycle_down && !cycle_worker_busy) {
            profile_id++;
            if (profile_id >= PROFILE_COUNT) profile_id = 0;
            start_cycle_worker(1);
        }

        if (theme_cycle_down && !last_theme_cycle_down && !cycle_worker_busy) {
            theme_id++;
            if (theme_id >= THEME_COUNT) theme_id = 0;
            start_cycle_worker(2);
        }

        last_profile_cycle_down = profile_cycle_down;
        last_theme_cycle_down = theme_cycle_down;
    } else {
        last_profile_cycle_down = 0;
        last_theme_cycle_down = 0;
    }

    if (menu_open) {
        handle_menu_vertical(buttons, pressed);

        handle_menu_horizontal(buttons, pressed);

        if ((pressed & SCE_CTRL_CROSS) && menu_page != MENU_PAGE_HUD_ORDER) {
            menu_change(1);
        }

        if (pressed & SCE_CTRL_CIRCLE) {
            if (menu_page == MENU_PAGE_CHOICE) {
                enter_menu_page(choice_return_page);
            } else if (menu_page == MENU_PAGE_HUD_ORDER) {
                enter_menu_page(MENU_PAGE_HUD_MENU);
            } else if (menu_page == MENU_PAGE_CREATE_HUD_SIZE || menu_page == MENU_PAGE_CREATE_MAIN_SIZE) {
                enter_menu_page(MENU_PAGE_SIZE);
            } else if (menu_page == MENU_PAGE_HUD_COLORS || menu_page == MENU_PAGE_MENU_COLORS || menu_page == MENU_PAGE_ICON_COLORS) {
                enter_menu_page(MENU_PAGE_THEME);
            } else if (menu_page != MENU_PAGE_MAIN) {
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
    int battery_charging = scePowerIsBatteryCharging();

    char fps_text[16];
    char battery_text[8];
    char time_text[24];
    char date_text[8];
    char cpu_text[16];
    char bus_text[16];
    char gpu_text[16];
    char app_id_text[24];
    char ram_text[16];
    char cpu_value_text[16];
    char bus_value_text[16];
    char gpu_value_text[16];
    char app_id_value_text[24];
    char ram_value_text[16];

    int scale;
    int gap_small;
    int gap_big;

    int icon_scale;
    int fps_w;
    int battery_text_w;
    int time_w;
    int date_w;
    int cpu_w;
    int bus_w;
    int gpu_w;
    int app_id_w;
    int ram_w;
    int force_stacked;

    int fps_icon_w;
    int fps_icon_h;
    int battery_icon_w;
    int battery_icon_h;
    int clock_icon_w;
    int clock_icon_h;
    int date_icon_w;
    int date_icon_h;
    int extra_icon_w;
    int extra_icon_h;

    int total_w = 0;
    int total_h;
    int text_h;
    int margin_x = hud_x_offset;
    int margin_y = hud_y_offset;
    int start_x;
    int start_y;
    int x;
    int y;
    int order_i;
    int order_id;
    int order_w;
    int draw_y;
    int has_any;

    unsigned int text_color = color_value(hud_text_color, 0xFFFFFFFF);
    int old_forced_draw_opacity = g_forced_draw_opacity;

    g_forced_draw_opacity = hud_opacity;
    unsigned int fps_text_color = text_color;
    unsigned int battery_text_color = text_color;
    unsigned int ram_text_color = text_color;
    int fps_alert_now = 0;
    int battery_alert_now = 0;
    int ram_alert_level = 0;

    get_hud_metrics(&scale, &gap_small, &gap_big);

    /* CREATE HUD MENU SIZE scales the live FPS/BATTERY/CLOCK HUD overlay. */
    if (create_hud_menu_size < 10) create_hud_menu_size = 10;
    if (create_hud_menu_size > 150) create_hud_menu_size = 150;
    scale = (scale * create_hud_menu_size + 50) / 100;
    if (scale < 1) scale = 1;
    gap_small = (gap_small * create_hud_menu_size + 50) / 100;
    if (gap_small < 1) gap_small = 1;
    gap_big = (gap_big * create_hud_menu_size + 50) / 100;
    if (gap_big < 2) gap_big = 2;

    icon_scale = scale;

    build_fps_text(fps_text);
    build_battery_text(battery_text, battery);
    build_time_text(time_text);
    build_date_text(date_text);
    build_cpu_text(cpu_text);
    build_bus_text(bus_text);
    build_gpu_text(gpu_text);
    build_app_id_text(app_id_text);
    build_ram_text(ram_text);

    strip_hud_label(cpu_value_text, sizeof(cpu_value_text), cpu_text, "CPU");
    strip_hud_label(bus_value_text, sizeof(bus_value_text), bus_text, "BUS");
    strip_hud_label(gpu_value_text, sizeof(gpu_value_text), gpu_text, "GPU");
    strip_hud_label(app_id_value_text, sizeof(app_id_value_text), app_id_text, "APP");
    strip_hud_label(ram_value_text, sizeof(ram_value_text), ram_text, "RAM");

    force_stacked = 0;

    fps_w = (show_fps && fps_display_style != FPS_STYLE_ICON_ONLY) ? text_width(fps_text, scale) : 0;
    battery_text_w = (show_battery && battery_display_style != BATTERY_STYLE_ICON_ONLY && battery_display_style != BATTERY_STYLE_BAR_ONLY) ? text_width(battery_text, scale) : 0;
    time_w = (show_time && clock_display_style != CLOCK_STYLE_ICON_ONLY) ? text_width(time_text, scale) : 0;
    date_w = show_date ? text_width(date_text, scale) : 0;
    cpu_w = show_cpu ? text_width(cpu_value_text, scale) : 0;
    bus_w = show_bus ? text_width(bus_value_text, scale) : 0;
    gpu_w = show_gpu ? text_width(gpu_value_text, scale) : 0;
    app_id_w = show_app_id ? text_width(app_id_value_text, scale) : 0;
    ram_w = show_ram ? text_width(ram_value_text, scale) : 0;

    fps_icon_w = (show_fps && fps_display_style != FPS_STYLE_NUMBER_ONLY && fps_display_style != FPS_STYLE_LABEL_NUMBER) ? (9 * icon_scale) : 0;
    fps_icon_h = 7 * icon_scale;

    battery_icon_w = (show_battery && battery_display_style != BATTERY_STYLE_PERCENT_ONLY) ? ((13 * icon_scale) + (2 * icon_scale)) : 0;
    battery_icon_h = 7 * icon_scale;

    clock_icon_w = (show_time && clock_display_style != CLOCK_STYLE_TIME_ONLY && clock_display_style != CLOCK_STYLE_DATE_TIME) ? (7 * icon_scale) : 0;
    clock_icon_h = 7 * icon_scale;
    date_icon_w = show_date ? (9 * icon_scale) : 0;
    date_icon_h = 7 * icon_scale;
    extra_icon_w = hud_extra_icon_w(icon_scale);
    extra_icon_h = hud_extra_icon_h(icon_scale);

    text_h = 7 * scale;

    fps_alert_now = fps_warning_enabled && ((int)fps_value < fps_low_limit_value());
    battery_alert_now = battery_warning_enabled && (battery <= battery_low_limit_value());

    if (ram_warning_enabled && cached_ram_free_mb >= 0) {
        if (cached_ram_free_mb < 32) {
            ram_alert_level = 2;
        } else if (cached_ram_free_mb < 64) {
            ram_alert_level = 1;
        }
    }

    if (fps_alert_now) {
        fps_text_color = 0xFF0000FF;
    }

    if (battery_alert_now) {
        battery_text_color = 0xFF0000FF;
    }

    if (ram_alert_level == 2) {
        ram_text_color = 0xFF0000FF;
    } else if (ram_alert_level == 1) {
        ram_text_color = 0xFF00FFFF;
    }

    active_fps_alert = fps_alert_now;
    active_battery_alert = battery_alert_now && (((frame_count / 15) % 2) == 0);

    if (hud_layout == LAYOUT_STACKED || force_stacked) {
        total_w = 0;
        total_h = 0;

        for (order_i = 0; order_i < HUD_ORDER_COUNT; order_i++) {
            order_id = hud_order[order_i];
            order_w = 0;

            switch (order_id) {
                case HUD_ORDER_FPS:
                    if (show_fps) order_w = fps_icon_w + fps_w + ((fps_icon_w > 0 && fps_w > 0) ? gap_small : 0);
                    break;
                case HUD_ORDER_BATTERY:
                    if (show_battery) order_w = battery_icon_w + battery_text_w + ((battery_icon_w > 0 && battery_text_w > 0) ? gap_small : 0);
                    break;
                case HUD_ORDER_CLOCK:
                    if (show_time) order_w = clock_icon_w + time_w + ((clock_icon_w > 0 && time_w > 0) ? gap_small : 0);
                    break;
                case HUD_ORDER_DATE:
                    if (show_date) order_w = date_icon_w + date_w;
                    break;
                case HUD_ORDER_CPU:
                    if (show_cpu) order_w = extra_icon_w + gap_small + cpu_w;
                    break;
                case HUD_ORDER_BUS:
                    if (show_bus) order_w = extra_icon_w + gap_small + bus_w;
                    break;
                case HUD_ORDER_GPU:
                    if (show_gpu) order_w = extra_icon_w + gap_small + gpu_w;
                    break;
                case HUD_ORDER_APP_ID:
                    if (show_app_id) order_w = extra_icon_w + gap_small + app_id_w;
                    break;
                case HUD_ORDER_RAM:
                    if (show_ram) order_w = extra_icon_w + gap_small + ram_w;
                    break;
                default:
                    break;
            }

            if (order_w > 0) {
                if (order_w > total_w) total_w = order_w;
                total_h += text_h + gap_small;
            }
        }

        if (total_h > 0) total_h -= gap_small;
    } else {
        total_w = 0;

        for (order_i = 0; order_i < HUD_ORDER_COUNT; order_i++) {
            order_id = hud_order[order_i];
            order_w = 0;

            switch (order_id) {
                case HUD_ORDER_FPS:
                    if (show_fps) order_w = fps_icon_w + fps_w + ((fps_icon_w > 0 && fps_w > 0) ? gap_small : 0);
                    break;
                case HUD_ORDER_BATTERY:
                    if (show_battery) order_w = battery_icon_w + battery_text_w + ((battery_icon_w > 0 && battery_text_w > 0) ? gap_small : 0);
                    break;
                case HUD_ORDER_CLOCK:
                    if (show_time) order_w = clock_icon_w + time_w + ((clock_icon_w > 0 && time_w > 0) ? gap_small : 0);
                    break;
                case HUD_ORDER_DATE:
                    if (show_date) order_w = date_icon_w + date_w;
                    break;
                case HUD_ORDER_CPU:
                    if (show_cpu) order_w = extra_icon_w + gap_small + cpu_w;
                    break;
                case HUD_ORDER_BUS:
                    if (show_bus) order_w = extra_icon_w + gap_small + bus_w;
                    break;
                case HUD_ORDER_GPU:
                    if (show_gpu) order_w = extra_icon_w + gap_small + gpu_w;
                    break;
                case HUD_ORDER_APP_ID:
                    if (show_app_id) order_w = extra_icon_w + gap_small + app_id_w;
                    break;
                case HUD_ORDER_RAM:
                    if (show_ram) order_w = extra_icon_w + gap_small + ram_w;
                    break;
                default:
                    break;
            }

            if (order_w > 0) {
                if (total_w > 0) total_w += gap_big;
                total_w += order_w;
            }
        }

        total_h = text_h;
    }

    if (total_w <= 0 || total_h <= 0) {
        g_forced_draw_opacity = old_forced_draw_opacity;
        return;
    }

    start_x = margin_x;
    start_y = margin_y;

    if (hud_position == POS_BOTTOM_RIGHT || hud_position == POS_TOP_RIGHT) {
        start_x = screen_w - total_w - margin_x;
    }

    if (hud_position == POS_TOP_CENTER || hud_position == POS_BOTTOM_CENTER) {
        start_x = ((screen_w - total_w) / 2) + margin_x;
    }

    if (hud_position == POS_BOTTOM_RIGHT || hud_position == POS_BOTTOM_LEFT || hud_position == POS_BOTTOM_CENTER) {
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

    last_hud_clear_x = start_x - 6;
    last_hud_clear_y = start_y - 6;
    last_hud_clear_w = total_w + 12;
    last_hud_clear_h = total_h + 12;

    if (hud_box_enabled) {
        draw_rect(pixels, pitch, last_hud_clear_x, last_hud_clear_y, last_hud_clear_w, last_hud_clear_h, get_hud_box_bg());
    }

    x = start_x;
    y = start_y;

    if (hud_layout == LAYOUT_STACKED || force_stacked) {
        for (order_i = 0; order_i < HUD_ORDER_COUNT; order_i++) {
            order_id = hud_order[order_i];
            draw_y = y;

            switch (order_id) {
                case HUD_ORDER_FPS:
                    if (show_fps) {
                        int tx = x;
                        if (fps_icon_w > 0) { draw_fps_icon(pixels, pitch, tx, draw_y + ((text_h - fps_icon_h) / 2), icon_scale); tx += fps_icon_w; if (fps_w > 0) tx += gap_small; }
                        if (fps_w > 0) draw_text_shadow(pixels, pitch, tx, draw_y, fps_text, fps_text_color, scale);
                        y += text_h + gap_small;
                    }
                    break;

                case HUD_ORDER_BATTERY:
                    if (show_battery) {
                        int tx = x;
                        if (battery_icon_w > 0) { draw_battery_icon(pixels, pitch, tx, draw_y + ((text_h - battery_icon_h) / 2), battery, icon_scale, battery_charging); tx += battery_icon_w; if (battery_text_w > 0) tx += gap_small; }
                        if (battery_text_w > 0) draw_text_shadow(pixels, pitch, tx, draw_y, battery_text, active_battery_alert ? battery_text_color : text_color, scale);
                        y += text_h + gap_small;
                    }
                    break;

                case HUD_ORDER_CLOCK:
                    if (show_time) {
                        int tx = x;
                        if (clock_icon_w > 0) { draw_clock_icon(pixels, pitch, tx, draw_y + ((text_h - clock_icon_h) / 2), icon_scale); tx += clock_icon_w; if (time_w > 0) tx += gap_small; }
                        if (time_w > 0) draw_text_shadow(pixels, pitch, tx, draw_y, time_text, text_color, scale);
                        y += text_h + gap_small;
                    }
                    break;

                case HUD_ORDER_DATE:
                    if (show_date) {
                        draw_calendar_icon(pixels, pitch, x, draw_y + ((text_h - date_icon_h) / 2), icon_scale);
                        draw_text_shadow(pixels, pitch, x + date_icon_w, draw_y, date_text, text_color, scale);
                        y += text_h + gap_small;
                    }
                    break;

                case HUD_ORDER_CPU:
                    if (show_cpu) {
                        set_active_extra_icon_color(cpu_icon_color);
                        set_active_extra_icon_style(cpu_icon_style);
                        draw_cpu_icon(pixels, pitch, x, draw_y + ((text_h - extra_icon_h) / 2), icon_scale);
                        draw_text_shadow(pixels, pitch, x + extra_icon_w + gap_small, draw_y, cpu_value_text, text_color, scale);
                        y += text_h + gap_small;
                    }
                    break;

                case HUD_ORDER_BUS:
                    if (show_bus) {
                        set_active_extra_icon_color(bus_icon_color);
                        set_active_extra_icon_style(bus_icon_style);
                        draw_cpu_icon(pixels, pitch, x, draw_y + ((text_h - extra_icon_h) / 2), icon_scale);
                        draw_text_shadow(pixels, pitch, x + extra_icon_w + gap_small, draw_y, bus_value_text, text_color, scale);
                        y += text_h + gap_small;
                    }
                    break;

                case HUD_ORDER_GPU:
                    if (show_gpu) {
                        set_active_extra_icon_color(gpu_icon_color);
                        set_active_extra_icon_style(gpu_icon_style);
                        draw_gpu_icon(pixels, pitch, x, draw_y + ((text_h - extra_icon_h) / 2), icon_scale);
                        draw_text_shadow(pixels, pitch, x + extra_icon_w + gap_small, draw_y, gpu_value_text, text_color, scale);
                        y += text_h + gap_small;
                    }
                    break;

                case HUD_ORDER_APP_ID:
                    if (show_app_id) {
                        set_active_extra_icon_color(app_icon_color);
                        set_active_extra_icon_style(app_icon_style);
                        draw_game_icon(pixels, pitch, x, draw_y + ((text_h - extra_icon_h) / 2), icon_scale);
                        draw_text_shadow(pixels, pitch, x + extra_icon_w + gap_small, draw_y, app_id_value_text, text_color, scale);
                        y += text_h + gap_small;
                    }
                    break;

                case HUD_ORDER_RAM:
                    if (show_ram) {
                        set_active_extra_icon_color(ram_alert_level == 2 ? COLOR_RED : (ram_alert_level == 1 ? COLOR_YELLOW : ram_icon_color));
                        set_active_extra_icon_style(ram_icon_style);
                        draw_ram_icon(pixels, pitch, x, draw_y + ((text_h - extra_icon_h) / 2), icon_scale);
                        draw_text_shadow(pixels, pitch, x + extra_icon_w + gap_small, draw_y, ram_value_text, ram_text_color, scale);
                        y += text_h + gap_small;
                    }
                    break;

                default:
                    break;
            }
        }

        g_forced_draw_opacity = old_forced_draw_opacity;
        return;
    }

    has_any = 0;

    for (order_i = 0; order_i < HUD_ORDER_COUNT; order_i++) {
        order_id = hud_order[order_i];
        order_w = 0;

        switch (order_id) {
            case HUD_ORDER_FPS:
                if (show_fps) order_w = fps_icon_w + fps_w + ((fps_icon_w > 0 && fps_w > 0) ? gap_small : 0);
                break;
            case HUD_ORDER_BATTERY:
                if (show_battery) order_w = battery_icon_w + battery_text_w + ((battery_icon_w > 0 && battery_text_w > 0) ? gap_small : 0);
                break;
            case HUD_ORDER_CLOCK:
                if (show_time) order_w = clock_icon_w + time_w + ((clock_icon_w > 0 && time_w > 0) ? gap_small : 0);
                break;
            case HUD_ORDER_DATE:
                if (show_date) order_w = date_icon_w + date_w;
                break;
            case HUD_ORDER_CPU:
                if (show_cpu) order_w = extra_icon_w + gap_small + cpu_w;
                break;
            case HUD_ORDER_BUS:
                if (show_bus) order_w = extra_icon_w + gap_small + bus_w;
                break;
            case HUD_ORDER_GPU:
                if (show_gpu) order_w = extra_icon_w + gap_small + gpu_w;
                break;
            case HUD_ORDER_APP_ID:
                if (show_app_id) order_w = extra_icon_w + gap_small + app_id_w;
                break;
            case HUD_ORDER_RAM:
                if (show_ram) order_w = extra_icon_w + gap_small + ram_w;
                break;
            default:
                break;
        }

        if (order_w <= 0) {
            continue;
        }

        if (has_any) {
            x += gap_big;
        }

        has_any = 1;

        switch (order_id) {
            case HUD_ORDER_FPS:
                if (fps_icon_w > 0) { draw_fps_icon(pixels, pitch, x, start_y + ((text_h - fps_icon_h) / 2), icon_scale); x += fps_icon_w; if (fps_w > 0) x += gap_small; }
                if (fps_w > 0) { draw_text_shadow(pixels, pitch, x, start_y, fps_text, fps_text_color, scale); x += fps_w; }
                break;

            case HUD_ORDER_BATTERY:
                if (battery_icon_w > 0) { draw_battery_icon(pixels, pitch, x, start_y + ((text_h - battery_icon_h) / 2), battery, icon_scale, battery_charging); x += battery_icon_w; if (battery_text_w > 0) x += gap_small; }
                if (battery_text_w > 0) { draw_text_shadow(pixels, pitch, x, start_y, battery_text, active_battery_alert ? battery_text_color : text_color, scale); x += battery_text_w; }
                break;

            case HUD_ORDER_CLOCK:
                if (clock_icon_w > 0) { draw_clock_icon(pixels, pitch, x, start_y + ((text_h - clock_icon_h) / 2), icon_scale); x += clock_icon_w; if (time_w > 0) x += gap_small; }
                if (time_w > 0) { draw_text_shadow(pixels, pitch, x, start_y, time_text, text_color, scale); x += time_w; }
                break;

            case HUD_ORDER_DATE:
                draw_calendar_icon(pixels, pitch, x, start_y + ((text_h - date_icon_h) / 2), icon_scale);
                x += date_icon_w;
                draw_text_shadow(pixels, pitch, x, start_y, date_text, text_color, scale);
                x += date_w;
                break;

            case HUD_ORDER_CPU:
                set_active_extra_icon_color(cpu_icon_color);
                set_active_extra_icon_style(cpu_icon_style);
                draw_cpu_icon(pixels, pitch, x, start_y + ((text_h - extra_icon_h) / 2), icon_scale);
                x += extra_icon_w + gap_small;
                draw_text_shadow(pixels, pitch, x, start_y, cpu_value_text, text_color, scale);
                x += cpu_w;
                break;

            case HUD_ORDER_BUS:
                set_active_extra_icon_color(bus_icon_color);
                set_active_extra_icon_style(bus_icon_style);
                draw_cpu_icon(pixels, pitch, x, start_y + ((text_h - extra_icon_h) / 2), icon_scale);
                x += extra_icon_w + gap_small;
                draw_text_shadow(pixels, pitch, x, start_y, bus_value_text, text_color, scale);
                x += bus_w;
                break;

            case HUD_ORDER_GPU:
                set_active_extra_icon_color(gpu_icon_color);
                set_active_extra_icon_style(gpu_icon_style);
                draw_gpu_icon(pixels, pitch, x, start_y + ((text_h - extra_icon_h) / 2), icon_scale);
                x += extra_icon_w + gap_small;
                draw_text_shadow(pixels, pitch, x, start_y, gpu_value_text, text_color, scale);
                x += gpu_w;
                break;

            case HUD_ORDER_APP_ID:
                set_active_extra_icon_color(app_icon_color);
                set_active_extra_icon_style(app_icon_style);
                draw_game_icon(pixels, pitch, x, start_y + ((text_h - extra_icon_h) / 2), icon_scale);
                x += extra_icon_w + gap_small;
                draw_text_shadow(pixels, pitch, x, start_y, app_id_value_text, text_color, scale);
                x += app_id_w;
                break;

            case HUD_ORDER_RAM:
                set_active_extra_icon_color(ram_alert_level == 2 ? COLOR_RED : (ram_alert_level == 1 ? COLOR_YELLOW : ram_icon_color));
                set_active_extra_icon_style(ram_icon_style);
                draw_ram_icon(pixels, pitch, x, start_y + ((text_h - extra_icon_h) / 2), icon_scale);
                x += extra_icon_w + gap_small;
                draw_text_shadow(pixels, pitch, x, start_y, ram_value_text, ram_text_color, scale);
                x += ram_w;
                break;

            default:
                break;
        }
    }

    g_forced_draw_opacity = old_forced_draw_opacity;
}

static void debug_append_line(char lines[][32], int *count, const char *label, int value) {
    int pos = 0;
    if (*count >= 16) return;
    pos = append_text(lines[*count], pos, label);
    pos = append_int(lines[*count], pos, value);
    lines[*count][pos] = '\0';
    (*count)++;
}

static void draw_debug_overlay(unsigned int *pixels, int pitch, int screen_w, int screen_h) {
    char lines[16][32];
    int count = 0;
    int scale;
    int gap_small;
    int gap_big;
    int text_h;
    int max_w = 0;
    int total_h;
    int x;
    int y;
    int i;
    int w;
    unsigned int col = color_value(debug_text_color, 0xFF00FFFF);
    int old_font;

    if (!debug_enabled) return;

    old_font = font_style;
    font_style = debug_font_style;

    get_hud_metrics(&scale, &gap_small, &gap_big);
    scale = debug_size + 1;
    if (scale < 1) scale = 1;
    text_h = 7 * scale;

    debug_append_line(lines, &count, "DBG FPS ", (int)fps_value);
    if (debug_show_framebuf) { debug_append_line(lines, &count, "FB W ", screen_w); debug_append_line(lines, &count, "FB H ", screen_h); debug_append_line(lines, &count, "PITCH ", pitch); }
    if (debug_show_cache) { debug_append_line(lines, &count, "RAM MB ", cached_ram_free_mb); }
    if (debug_show_input) { debug_append_line(lines, &count, "BTN ", (int)(last_buttons & 0xFFFF)); debug_append_line(lines, &count, "PAGE ", menu_page); }
    if (debug_show_system) { debug_append_line(lines, &count, "CPU ", scePowerGetArmClockFrequency()); debug_append_line(lines, &count, "GPU ", scePowerGetGpuClockFrequency()); }
    if (debug_show_menu_info) { debug_append_line(lines, &count, "MENU IDX ", menu_index); debug_append_line(lines, &count, "SCROLL ", menu_scroll); }
    if (debug_show_profile) { debug_append_line(lines, &count, "PROFILE ", profile_id + 1); }
    if (debug_show_theme) { debug_append_line(lines, &count, "THEME ", theme_id); }
    if (debug_show_alert) { debug_append_line(lines, &count, "FPS LIM ", fps_low_limit_value()); debug_append_line(lines, &count, "BAT LIM ", battery_low_limit_value()); }
    if (debug_show_hud_info) { debug_append_line(lines, &count, "HUD POS ", hud_position); debug_append_line(lines, &count, "HUD LAY ", hud_layout); }

    for (i = 0; i < count; i++) { w = text_width(lines[i], scale); if (w > max_w) max_w = w; }
    total_h = count * (text_h + gap_small); if (total_h > 0) total_h -= gap_small;
    x = debug_x_offset; y = debug_y_offset;
    if (debug_position == POS_BOTTOM_RIGHT || debug_position == POS_TOP_RIGHT) x = screen_w - max_w - debug_x_offset;
    if (debug_position == POS_TOP_CENTER || debug_position == POS_BOTTOM_CENTER) x = ((screen_w - max_w) / 2) + debug_x_offset;
    if (debug_position == POS_BOTTOM_RIGHT || debug_position == POS_BOTTOM_LEFT || debug_position == POS_BOTTOM_CENTER) y = screen_h - total_h - debug_y_offset;
    if (x < 0) x = 0;
    if (y < 0) y = 0;
    if (x + max_w > screen_w) x = screen_w - max_w;
    if (y + total_h > screen_h) y = screen_h - total_h;
    if (x < 0) x = 0;
    if (y < 0) y = 0;
    for (i = 0; i < count; i++) draw_text_shadow(pixels, pitch, x, y + (i * (text_h + gap_small)), lines[i], col, scale);

    font_style = old_font;
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

    draw_debug_overlay(pixels, pitch, screen_w, screen_h);

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
