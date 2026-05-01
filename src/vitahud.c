#include <psp2/kernel/modulemgr.h>
#include <psp2/kernel/threadmgr.h>
#include <psp2/ctrl.h>
#include <psp2/power.h>
#include <psp2/rtc.h>
#include <psp2/display.h>
#include <psp2/io/fcntl.h>
#include <psp2/io/stat.h>

#define CONFIG_DIR  "ux0:data/VitaHUD"
#define CONFIG_PATH "ux0:data/VitaHUD/config.txt"

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

#define COLOR_WHITE    0
#define COLOR_GREEN    1
#define COLOR_YELLOW   2
#define COLOR_RED      3
#define COLOR_CYAN     4
#define COLOR_MAGENTA  5
#define COLOR_ORANGE   6
#define COLOR_BLUE     7
#define COLOR_PINK     8
#define COLOR_LIME     9
#define COLOR_SILVER   10
#define COLOR_COUNT    11

#define BG_TRANSPARENT 0
#define BG_BLACK       1
#define BG_GRAY        2
#define BG_NAVY        3
#define BG_GREEN       4
#define BG_PURPLE      5
#define BG_TEAL        6
#define BG_COUNT       7

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

#define ITEM_HUD        0
#define ITEM_POSITION   1
#define ITEM_SIZE       2
#define ITEM_TEXTCOLOR  3
#define ITEM_MENUBG     4
#define ITEM_FONT       5
#define ITEM_FPS        6
#define ITEM_BATTERY    7
#define ITEM_TIME       8
#define ITEM_TIMEMODE   9
#define ITEM_LANGUAGE   10
#define ITEM_TOGGLE     11
#define ITEM_SAVE       12
#define ITEM_COUNT      13

static int hud_enabled = 1;
static int menu_open = 0;
static int menu_index = 0;

static int hud_position = POS_BOTTOM_RIGHT;
static int hud_size = SIZE_NORMAL;
static int hud_color = COLOR_WHITE;
static int menu_bg_color = BG_BLACK;
static int hud_language = LANG_EN;
static int font_style = FONT_DEFAULT;

static int show_fps = 1;
static int show_battery = 1;
static int show_time = 1;
static int use_24h_time = 0;
static int toggle_combo_mode = TOGGLE_SELECT;

static int save_message_frames = 0;

static int last_toggle_down = 0;
static int last_menu_down = 0;
static unsigned int last_buttons = 0;

static unsigned int frame_count = 0;
static unsigned int fps_value = 0;
static SceUInt64 last_tick = 0;

static void put_2digits(char *out, int value) {
    out[0] = '0' + ((value / 10) % 10);
    out[1] = '0' + (value % 10);
}

static int append_text(char *out, int pos, const char *text) {
    while (*text) {
        out[pos++] = *text++;
    }

    return pos;
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
    if (value < 0) {
        value = 0;
    }

    if (value > 99) {
        value = 99;
    }

    out[pos++] = '0' + ((value / 10) % 10);
    out[pos++] = '0' + (value % 10);

    return pos;
}

static int append_battery_number(char *out, int pos, int value) {
    if (value < 0) {
        value = 0;
    }

    if (value > 100) {
        value = 100;
    }

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
        if (*src != *key) {
            return 0;
        }

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
            while (*p && *p != '=') {
                p++;
            }

            if (*p == '=') {
                p++;
                return parse_int_value(p);
            }
        }

        while (*p && *p != '\n') {
            p++;
        }

        if (*p == '\n') {
            p++;
        }
    }

    return default_value;
}

static void clamp_settings(void) {
    if (hud_enabled < 0 || hud_enabled > 1) hud_enabled = 1;
    if (hud_position < 0 || hud_position >= POS_COUNT) hud_position = POS_BOTTOM_RIGHT;
    if (hud_size < 0 || hud_size >= SIZE_COUNT) hud_size = SIZE_NORMAL;
    if (hud_color < 0 || hud_color >= COLOR_COUNT) hud_color = COLOR_WHITE;
    if (menu_bg_color < 0 || menu_bg_color >= BG_COUNT) menu_bg_color = BG_BLACK;
    if (font_style < 0 || font_style >= FONT_COUNT) font_style = FONT_DEFAULT;
    if (show_fps < 0 || show_fps > 1) show_fps = 1;
    if (show_battery < 0 || show_battery > 1) show_battery = 1;
    if (show_time < 0 || show_time > 1) show_time = 1;
    if (use_24h_time < 0 || use_24h_time > 1) use_24h_time = 0;
    if (hud_language < 0 || hud_language >= LANG_COUNT) hud_language = LANG_EN;
    if (toggle_combo_mode < 0 || toggle_combo_mode >= TOGGLE_COUNT) toggle_combo_mode = TOGGLE_SELECT;
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

static int save_config(void) {
    SceUID fd;

    sceIoMkdir(CONFIG_DIR, 0777);

    fd = sceIoOpen(CONFIG_PATH, SCE_O_WRONLY | SCE_O_CREAT | SCE_O_TRUNC, 0777);

    if (fd < 0) {
        save_message_frames = 180;
        return fd;
    }

    write_config_line(fd, "hud_enabled", hud_enabled);
    write_config_line(fd, "position", hud_position);
    write_config_line(fd, "size", hud_size);
    write_config_line(fd, "text_color", hud_color);
    write_config_line(fd, "menu_bg", menu_bg_color);
    write_config_line(fd, "font", font_style);
    write_config_line(fd, "show_fps", show_fps);
    write_config_line(fd, "show_battery", show_battery);
    write_config_line(fd, "show_time", show_time);
    write_config_line(fd, "time_24h", use_24h_time);
    write_config_line(fd, "language", hud_language);
    write_config_line(fd, "toggle_combo", toggle_combo_mode);

    sceIoClose(fd);

    save_message_frames = 180;
    return 0;
}

static void load_config(void) {
    SceUID fd;
    char buf[1024];
    int read_size;

    fd = sceIoOpen(CONFIG_PATH, SCE_O_RDONLY, 0);

    if (fd < 0) {
        return;
    }

    read_size = sceIoRead(fd, buf, sizeof(buf) - 1);
    sceIoClose(fd);

    if (read_size <= 0) {
        return;
    }

    buf[read_size] = '\0';

    hud_enabled = get_config_int(buf, "hud_enabled", hud_enabled);
    hud_position = get_config_int(buf, "position", hud_position);
    hud_size = get_config_int(buf, "size", hud_size);
    hud_color = get_config_int(buf, "text_color", hud_color);
    menu_bg_color = get_config_int(buf, "menu_bg", menu_bg_color);
    font_style = get_config_int(buf, "font", font_style);
    show_fps = get_config_int(buf, "show_fps", show_fps);
    show_battery = get_config_int(buf, "show_battery", show_battery);
    show_time = get_config_int(buf, "show_time", show_time);
    use_24h_time = get_config_int(buf, "time_24h", use_24h_time);
    hud_language = get_config_int(buf, "language", hud_language);
    toggle_combo_mode = get_config_int(buf, "toggle_combo", toggle_combo_mode);

    clamp_settings();
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

    pos = append_text(out, pos, "FPS ");
    pos = append_2digit_number(out, pos, (int)fps_value);

    out[pos] = '\0';
}

static void build_battery_text(char *out, int battery) {
    int pos = 0;

    pos = append_battery_number(out, pos, battery);
    out[pos++] = '%';

    out[pos] = '\0';
}

static void build_time_text(char *out) {
    SceDateTime time;
    int pos = 0;
    int hour;
    int is_pm = 0;

    sceRtcGetCurrentClockLocalTime(&time);

    if (use_24h_time) {
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

static unsigned int get_text_color(void) {
    switch (hud_color) {
        case COLOR_GREEN:   return 0xFF00FF00;
        case COLOR_YELLOW:  return 0xFF00FFFF;
        case COLOR_RED:     return 0xFF0000FF;
        case COLOR_CYAN:    return 0xFFFFFF00;
        case COLOR_MAGENTA: return 0xFFFF00FF;
        case COLOR_ORANGE:  return 0xFF0080FF;
        case COLOR_BLUE:    return 0xFFFF0000;
        case COLOR_PINK:    return 0xFFFF80FF;
        case COLOR_LIME:    return 0xFF80FF00;
        case COLOR_SILVER:  return 0xFFC0C0C0;
        case COLOR_WHITE:
        default:            return 0xFFFFFFFF;
    }
}

static unsigned int get_menu_bg(void) {
    switch (menu_bg_color) {
        case BG_GRAY:        return 0xCC404040;
        case BG_NAVY:        return 0xCC401020;
        case BG_GREEN:       return 0xCC203010;
        case BG_PURPLE:      return 0xCC401030;
        case BG_TEAL:        return 0xCC383810;
        case BG_BLACK:       return 0xCC000000;
        case BG_TRANSPARENT:
        default:             return 0x00000000;
    }
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

    if (x < 0 || y < 0 || w <= 0 || h <= 0) {
        return;
    }

    for (yy = 0; yy < h; yy++) {
        for (xx = 0; xx < w; xx++) {
            pixels[(y + yy) * pitch + (x + xx)] = color;
        }
    }
}

static void draw_char(unsigned int *pixels, int pitch, int x, int y, char c, unsigned int color, int scale) {
    int row;
    int col;
    int sy;
    int sx;
    int bold = font_bold_style();

    for (row = 0; row < 7; row++) {
        unsigned char bits = font5x7(c, row);

        for (col = 0; col < 5; col++) {
            if (bits & (1 << (4 - col))) {
                for (sy = 0; sy < scale; sy++) {
                    for (sx = 0; sx < scale; sx++) {
                        pixels[(y + row * scale + sy) * pitch + (x + col * scale + sx)] = color;

                        if (bold >= 1) {
                            pixels[(y + row * scale + sy) * pitch + (x + col * scale + sx + 1)] = color;
                        }

                        if (bold >= 2) {
                            pixels[(y + row * scale + sy + 1) * pitch + (x + col * scale + sx)] = color;
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

    if (spacing < 3) {
        spacing = 3;
    }

    while (text[count]) {
        count++;
    }

    return count * spacing;
}

static void draw_text(unsigned int *pixels, int pitch, int x, int y, const char *text, unsigned int color, int scale) {
    int cursor_x = x;
    int spacing = 6 * scale + font_extra_spacing();

    if (spacing < 3) {
        spacing = 3;
    }

    while (*text) {
        draw_char(pixels, pitch, cursor_x, y, *text, color, scale);
        cursor_x += spacing;
        text++;
    }
}

static void draw_text_shadow(unsigned int *pixels, int pitch, int x, int y, const char *text, unsigned int color, int scale) {
    int shadow = font_shadow_style();

    if (shadow == 1) {
        draw_text(pixels, pitch, x + 1, y + 1, text, 0xFF000000, scale);
    } else if (shadow == 2) {
        draw_text(pixels, pitch, x + 1, y, text, 0xFF000000, scale);
        draw_text(pixels, pitch, x, y + 1, text, 0xFF000000, scale);
    }

    draw_text(pixels, pitch, x, y, text, color, scale);
}

static void draw_battery_icon(unsigned int *pixels, int pitch, int x, int y, int battery, int scale) {
    unsigned int white = 0xFFFFFFFF;
    unsigned int fill = get_battery_color(battery);

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

    if (battery < 0) {
        battery = 0;
    }

    if (battery > 100) {
        battery = 100;
    }

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
    unsigned int white = 0xFFFFFFFF;
    int s = scale;

    if (s < 1) {
        s = 1;
    }

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

static const char *onoff_name(int value) {
    return value ? "ON" : "OFF";
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

static const char *size_name(void) {
    switch (hud_size) {
        case SIZE_MICRO:  return "MICRO";
        case SIZE_NORMAL: return "NORMAL";
        case SIZE_LARGE:
        default:          return "LARGE";
    }
}

static const char *color_name(void) {
    switch (hud_color) {
        case COLOR_GREEN:   return "GREEN";
        case COLOR_YELLOW:  return "YELLOW";
        case COLOR_RED:     return "RED";
        case COLOR_CYAN:    return "CYAN";
        case COLOR_MAGENTA: return "MAGENTA";
        case COLOR_ORANGE:  return "ORANGE";
        case COLOR_BLUE:    return "BLUE";
        case COLOR_PINK:    return "PINK";
        case COLOR_LIME:    return "LIME";
        case COLOR_SILVER:  return "SILVER";
        case COLOR_WHITE:
        default:            return "WHITE";
    }
}

static const char *menu_bg_name(void) {
    switch (menu_bg_color) {
        case BG_TRANSPARENT: return "TRANSPARENT";
        case BG_GRAY:        return "GRAY";
        case BG_NAVY:        return "NAVY";
        case BG_GREEN:       return "GREEN";
        case BG_PURPLE:      return "PURPLE";
        case BG_TEAL:        return "TEAL";
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
    switch (hud_language) {
        case LANG_ES: return "MENU VITAHUD";
        case LANG_FR: return "MENU VITAHUD";
        case LANG_DE: return "VITAHUD MENU";
        case LANG_IT: return "MENU VITAHUD";
        case LANG_PT: return "MENU VITAHUD";
        case LANG_NL: return "VITAHUD MENU";
        case LANG_ID: return "MENU VITAHUD";
        case LANG_TR: return "VITAHUD MENU";
        case LANG_PL: return "MENU VITAHUD";
        case LANG_EN:
        default:      return "VITAHUD MENU";
    }
}

static const char *tr_footer(void) {
    switch (hud_language) {
        case LANG_ES: return "ARRIBA/ABAJO MOVER  IZQ/DER CAMBIAR  O CERRAR";
        case LANG_FR: return "HAUT/BAS BOUGER  GAUCHE/DROITE CHANGER  O FERMER";
        case LANG_DE: return "HOCH/RUNTER WAHLEN  LINKS/RECHTS ANDERN  O ZU";
        case LANG_IT: return "SU/GIU MUOVI  SINISTRA/DESTRA CAMBIA  O CHIUDI";
        case LANG_PT: return "CIMA/BAIXO MOVER  ESQ/DIR MUDAR  O FECHAR";
        case LANG_NL: return "OMHOOG/OMLAAG KIES  LINKS/RECHTS WIJZIG  O SLUIT";
        case LANG_ID: return "ATAS/BAWAH PILIH  KIRI/KANAN UBAH  O TUTUP";
        case LANG_TR: return "YUKARI/ASAGI SEC  SOL/SAG DEGISTIR  O KAPAT";
        case LANG_PL: return "GORA/DOL WYBOR  LEWO/PRAWO ZMIEN  O ZAMKNIJ";
        case LANG_EN:
        default:      return "UP/DOWN MOVE  LEFT/RIGHT CHANGE  O CLOSE";
    }
}

static const char *tr_label(int item) {
    switch (hud_language) {
        case LANG_ES:
            switch (item) {
                case ITEM_HUD: return "HUD";
                case ITEM_POSITION: return "POSICION";
                case ITEM_SIZE: return "TAMANO";
                case ITEM_TEXTCOLOR: return "COLOR TEXTO";
                case ITEM_MENUBG: return "FONDO MENU";
                case ITEM_FONT: return "FUENTE";
                case ITEM_FPS: return "FPS";
                case ITEM_BATTERY: return "BATERIA";
                case ITEM_TIME: return "HORA";
                case ITEM_TIMEMODE: return "FORMATO HORA";
                case ITEM_LANGUAGE: return "IDIOMA";
                case ITEM_TOGGLE: return "COMBO HUD";
                case ITEM_SAVE: return "GUARDAR";
                default: return "";
            }

        case LANG_FR:
            switch (item) {
                case ITEM_HUD: return "HUD";
                case ITEM_POSITION: return "POSITION";
                case ITEM_SIZE: return "TAILLE";
                case ITEM_TEXTCOLOR: return "COULEUR TEXTE";
                case ITEM_MENUBG: return "FOND MENU";
                case ITEM_FONT: return "POLICE";
                case ITEM_FPS: return "FPS";
                case ITEM_BATTERY: return "BATTERIE";
                case ITEM_TIME: return "HEURE";
                case ITEM_TIMEMODE: return "FORMAT HEURE";
                case ITEM_LANGUAGE: return "LANGUE";
                case ITEM_TOGGLE: return "COMBO HUD";
                case ITEM_SAVE: return "SAUVER";
                default: return "";
            }

        case LANG_DE:
            switch (item) {
                case ITEM_HUD: return "HUD";
                case ITEM_POSITION: return "POSITION";
                case ITEM_SIZE: return "GROESSE";
                case ITEM_TEXTCOLOR: return "TEXT FARBE";
                case ITEM_MENUBG: return "MENU BG";
                case ITEM_FONT: return "SCHRIFT";
                case ITEM_FPS: return "FPS";
                case ITEM_BATTERY: return "BATTERIE";
                case ITEM_TIME: return "ZEIT";
                case ITEM_TIMEMODE: return "ZEIT FORMAT";
                case ITEM_LANGUAGE: return "SPRACHE";
                case ITEM_TOGGLE: return "HUD COMBO";
                case ITEM_SAVE: return "SPEICHERN";
                default: return "";
            }

        case LANG_IT:
            switch (item) {
                case ITEM_HUD: return "HUD";
                case ITEM_POSITION: return "POSIZIONE";
                case ITEM_SIZE: return "DIMENSIONE";
                case ITEM_TEXTCOLOR: return "COLORE TESTO";
                case ITEM_MENUBG: return "SFONDO MENU";
                case ITEM_FONT: return "FONT";
                case ITEM_FPS: return "FPS";
                case ITEM_BATTERY: return "BATTERIA";
                case ITEM_TIME: return "ORA";
                case ITEM_TIMEMODE: return "FORMATO ORA";
                case ITEM_LANGUAGE: return "LINGUA";
                case ITEM_TOGGLE: return "COMBO HUD";
                case ITEM_SAVE: return "SALVA";
                default: return "";
            }

        case LANG_PT:
            switch (item) {
                case ITEM_HUD: return "HUD";
                case ITEM_POSITION: return "POSICAO";
                case ITEM_SIZE: return "TAMANHO";
                case ITEM_TEXTCOLOR: return "COR TEXTO";
                case ITEM_MENUBG: return "FUNDO MENU";
                case ITEM_FONT: return "FONTE";
                case ITEM_FPS: return "FPS";
                case ITEM_BATTERY: return "BATERIA";
                case ITEM_TIME: return "HORA";
                case ITEM_TIMEMODE: return "FORMATO HORA";
                case ITEM_LANGUAGE: return "IDIOMA";
                case ITEM_TOGGLE: return "COMBO HUD";
                case ITEM_SAVE: return "SALVAR";
                default: return "";
            }

        case LANG_NL:
            switch (item) {
                case ITEM_HUD: return "HUD";
                case ITEM_POSITION: return "POSITIE";
                case ITEM_SIZE: return "GROOTTE";
                case ITEM_TEXTCOLOR: return "TEKST KLEUR";
                case ITEM_MENUBG: return "MENU BG";
                case ITEM_FONT: return "LETTERTYPE";
                case ITEM_FPS: return "FPS";
                case ITEM_BATTERY: return "BATTERIJ";
                case ITEM_TIME: return "TIJD";
                case ITEM_TIMEMODE: return "TIJD MODE";
                case ITEM_LANGUAGE: return "TAAL";
                case ITEM_TOGGLE: return "HUD COMBO";
                case ITEM_SAVE: return "OPSLAAN";
                default: return "";
            }

        case LANG_ID:
            switch (item) {
                case ITEM_HUD: return "HUD";
                case ITEM_POSITION: return "POSISI";
                case ITEM_SIZE: return "UKURAN";
                case ITEM_TEXTCOLOR: return "WARNA TEKS";
                case ITEM_MENUBG: return "LATAR MENU";
                case ITEM_FONT: return "FONT";
                case ITEM_FPS: return "FPS";
                case ITEM_BATTERY: return "BATERAI";
                case ITEM_TIME: return "WAKTU";
                case ITEM_TIMEMODE: return "MODE WAKTU";
                case ITEM_LANGUAGE: return "BAHASA";
                case ITEM_TOGGLE: return "COMBO HUD";
                case ITEM_SAVE: return "SIMPAN";
                default: return "";
            }

        case LANG_TR:
            switch (item) {
                case ITEM_HUD: return "HUD";
                case ITEM_POSITION: return "KONUM";
                case ITEM_SIZE: return "BOYUT";
                case ITEM_TEXTCOLOR: return "YAZI RENK";
                case ITEM_MENUBG: return "MENU ARKA";
                case ITEM_FONT: return "FONT";
                case ITEM_FPS: return "FPS";
                case ITEM_BATTERY: return "BATARYA";
                case ITEM_TIME: return "SAAT";
                case ITEM_TIMEMODE: return "SAAT MODU";
                case ITEM_LANGUAGE: return "DIL";
                case ITEM_TOGGLE: return "HUD COMBO";
                case ITEM_SAVE: return "KAYDET";
                default: return "";
            }

        case LANG_PL:
            switch (item) {
                case ITEM_HUD: return "HUD";
                case ITEM_POSITION: return "POZYCJA";
                case ITEM_SIZE: return "ROZMIAR";
                case ITEM_TEXTCOLOR: return "KOLOR TEKST";
                case ITEM_MENUBG: return "TLO MENU";
                case ITEM_FONT: return "CZCIONKA";
                case ITEM_FPS: return "FPS";
                case ITEM_BATTERY: return "BATERIA";
                case ITEM_TIME: return "CZAS";
                case ITEM_TIMEMODE: return "TRYB CZASU";
                case ITEM_LANGUAGE: return "JEZYK";
                case ITEM_TOGGLE: return "HUD COMBO";
                case ITEM_SAVE: return "ZAPISZ";
                default: return "";
            }

        case LANG_EN:
        default:
            switch (item) {
                case ITEM_HUD: return "HUD";
                case ITEM_POSITION: return "POSITION";
                case ITEM_SIZE: return "SIZE";
                case ITEM_TEXTCOLOR: return "TEXT COLOR";
                case ITEM_MENUBG: return "MENU BG";
                case ITEM_FONT: return "FONT";
                case ITEM_FPS: return "FPS";
                case ITEM_BATTERY: return "BATTERY";
                case ITEM_TIME: return "TIME";
                case ITEM_TIMEMODE: return "TIME MODE";
                case ITEM_LANGUAGE: return "LANGUAGE";
                case ITEM_TOGGLE: return "HUD TOGGLE";
                case ITEM_SAVE: return "SAVE CONFIG";
                default: return "";
            }
    }
}

static const char *menu_label(int item) {
    return tr_label(item);
}

static const char *menu_value(int item) {
    switch (item) {
        case ITEM_HUD:       return onoff_name(hud_enabled);
        case ITEM_POSITION:  return position_name();
        case ITEM_SIZE:      return size_name();
        case ITEM_TEXTCOLOR: return color_name();
        case ITEM_MENUBG:    return menu_bg_name();
        case ITEM_FONT:      return font_name();
        case ITEM_FPS:       return onoff_name(show_fps);
        case ITEM_BATTERY:   return onoff_name(show_battery);
        case ITEM_TIME:      return onoff_name(show_time);
        case ITEM_TIMEMODE:  return time_mode_name();
        case ITEM_LANGUAGE:  return language_name();
        case ITEM_TOGGLE:    return toggle_name();
        case ITEM_SAVE:      return save_message_frames > 0 ? "SAVED" : "PRESS X";
        default:             return "";
    }
}

static void menu_change(int dir) {
    switch (menu_index) {
        case ITEM_HUD:
            hud_enabled = !hud_enabled;
            break;

        case ITEM_POSITION:
            hud_position += dir;
            if (hud_position < 0) hud_position = POS_COUNT - 1;
            if (hud_position >= POS_COUNT) hud_position = 0;
            break;

        case ITEM_SIZE:
            hud_size += dir;
            if (hud_size < 0) hud_size = SIZE_COUNT - 1;
            if (hud_size >= SIZE_COUNT) hud_size = 0;
            break;

        case ITEM_TEXTCOLOR:
            hud_color += dir;
            if (hud_color < 0) hud_color = COLOR_COUNT - 1;
            if (hud_color >= COLOR_COUNT) hud_color = 0;
            break;

        case ITEM_MENUBG:
            menu_bg_color += dir;
            if (menu_bg_color < 0) menu_bg_color = BG_COUNT - 1;
            if (menu_bg_color >= BG_COUNT) menu_bg_color = 0;
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

        case ITEM_TIMEMODE:
            use_24h_time = !use_24h_time;
            break;

        case ITEM_LANGUAGE:
            hud_language += dir;
            if (hud_language < 0) hud_language = LANG_COUNT - 1;
            if (hud_language >= LANG_COUNT) hud_language = 0;
            break;

        case ITEM_TOGGLE:
            toggle_combo_mode += dir;
            if (toggle_combo_mode < 0) toggle_combo_mode = TOGGLE_COUNT - 1;
            if (toggle_combo_mode >= TOGGLE_COUNT) toggle_combo_mode = 0;
            break;

        case ITEM_SAVE:
            save_config();
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
    int selected,
    const char *label,
    const char *value
) {
    unsigned int line_color = selected ? 0xFF00FFFF : 0xFFFFFFFF;

    if (selected) {
        draw_text_shadow(pixels, pitch, x, y, ">", line_color, 1);
    }

    draw_text_shadow(pixels, pitch, x + 14, y, label, line_color, 1);
    draw_text_shadow(pixels, pitch, x + 180, y, value, line_color, 1);
}

static void draw_menu(unsigned int *pixels, int pitch, int screen_w, int screen_h) {
    int x = 28;
    int y = 52;
    int w = 410;
    int h = 220;
    int line_y;
    int i;

    unsigned int bg = get_menu_bg();
    unsigned int border = get_text_color();

    (void)screen_w;
    (void)screen_h;

    if (menu_bg_color != BG_TRANSPARENT) {
        draw_rect(pixels, pitch, x - 8, y - 8, w, h, bg);
    }

    draw_rect(pixels, pitch, x - 8, y - 8, w, 1, border);
    draw_rect(pixels, pitch, x - 8, y + h - 1, w, 1, border);
    draw_rect(pixels, pitch, x - 8, y - 8, 1, h, border);
    draw_rect(pixels, pitch, x + w - 9, y - 8, 1, h, border);

    draw_text_shadow(pixels, pitch, x, y, tr_menu_title(), 0xFF00FFFF, 1);

    line_y = y + 16;

    for (i = 0; i < ITEM_COUNT; i++) {
        draw_menu_line(
            pixels,
            pitch,
            x,
            line_y,
            menu_index == i,
            menu_label(i),
            menu_value(i)
        );

        line_y += 12;
    }

    draw_text_shadow(
        pixels,
        pitch,
        x,
        y + 192,
        tr_footer(),
        0xFFFFFFFF,
        1
    );
}

static void handle_input(void) {
    SceCtrlData pad;
    unsigned int buttons;
    unsigned int pressed;
    unsigned int toggle_combo;
    int toggle_down;
    int menu_down;

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
        hud_enabled = !hud_enabled;
    }

    if (menu_open) {
        if (pressed & SCE_CTRL_UP) {
            menu_index--;

            if (menu_index < 0) {
                menu_index = ITEM_COUNT - 1;
            }
        }

        if (pressed & SCE_CTRL_DOWN) {
            menu_index++;

            if (menu_index >= ITEM_COUNT) {
                menu_index = 0;
            }
        }

        if (pressed & SCE_CTRL_LEFT) {
            menu_change(-1);
        }

        if (pressed & SCE_CTRL_RIGHT) {
            menu_change(1);
        }

        if (pressed & SCE_CTRL_CROSS) {
            menu_change(1);
        }

        if (pressed & SCE_CTRL_CIRCLE) {
            menu_open = 0;
        }
    }

    last_toggle_down = toggle_down;
    last_menu_down = menu_down;
    last_buttons = buttons;
}

static void draw_hud(unsigned int *pixels, int pitch, int screen_w, int screen_h) {
    int battery = scePowerGetBatteryLifePercent();

    char fps_text[16];
    char battery_text[8];
    char time_text[16];

    int scale;
    int gap_small;
    int gap_big;

    int icon_scale;

    int fps_w;
    int battery_text_w;
    int time_w;

    int battery_icon_w;
    int battery_icon_h;

    int clock_icon_w;
    int clock_icon_h;

    int total_w = 0;
    int text_h;
    int total_h;
    int margin = 8;

    int start_x = margin;
    int start_y = margin;
    int x;

    unsigned int text_color = get_text_color();

    get_hud_metrics(&scale, &gap_small, &gap_big);

    icon_scale = scale;

    build_fps_text(fps_text);
    build_battery_text(battery_text, battery);
    build_time_text(time_text);

    fps_w = show_fps ? text_width(fps_text, scale) : 0;
    battery_text_w = show_battery ? text_width(battery_text, scale) : 0;
    time_w = show_time ? text_width(time_text, scale) : 0;

    battery_icon_w = show_battery ? ((13 * icon_scale) + (2 * icon_scale)) : 0;
    battery_icon_h = 7 * icon_scale;

    clock_icon_w = show_time ? (7 * icon_scale) : 0;
    clock_icon_h = 7 * icon_scale;

    if (show_fps) {
        total_w += fps_w;
    }

    if (show_battery) {
        if (total_w > 0) {
            total_w += gap_big;
        }

        total_w += battery_icon_w + gap_small + battery_text_w;
    }

    if (show_time) {
        if (total_w > 0) {
            total_w += gap_big;
        }

        total_w += clock_icon_w + gap_small + time_w;
    }

    if (total_w <= 0) {
        return;
    }

    text_h = 7 * scale;
    total_h = text_h;

    if (hud_position == POS_BOTTOM_RIGHT || hud_position == POS_TOP_RIGHT) {
        start_x = screen_w - total_w - margin;
    }

    if (hud_position == POS_BOTTOM_RIGHT || hud_position == POS_BOTTOM_LEFT) {
        start_y = screen_h - total_h - margin;
    }

    if (start_x < 0 || start_y < 0) {
        return;
    }

    x = start_x;

    if (show_fps) {
        draw_text_shadow(pixels, pitch, x, start_y, fps_text, text_color, scale);
        x += fps_w;
    }

    if (show_battery) {
        if (x != start_x) {
            x += gap_big;
        }

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
        if (x != start_x) {
            x += gap_big;
        }

        draw_clock_icon(
            pixels,
            pitch,
            x,
            start_y + ((text_h - clock_icon_h) / 2),
            icon_scale
        );

        x += clock_icon_w + gap_small;

        draw_text_shadow(pixels, pitch, x, start_y, time_text, text_color, scale);
    }

    (void)total_h;
}

static void draw_all(void) {
    SceDisplayFrameBuf fb;
    unsigned int *pixels;
    int screen_w;
    int screen_h;
    int pitch;

    fb.size = sizeof(SceDisplayFrameBuf);

    /*
     * Anti-glitch patch:
     * IMMEDIATE is better for VitaShell/LiveArea-style menus
     * that do not always behave like games/apps.
     */
    if (sceDisplayGetFrameBuf(&fb, SCE_DISPLAY_SETBUF_IMMEDIATE) < 0 || !fb.base) {
        return;
    }

    if (fb.pixelformat != SCE_DISPLAY_PIXELFORMAT_A8B8G8R8) {
        return;
    }

    pixels = (unsigned int *)fb.base;
    screen_w = fb.width;
    screen_h = fb.height;
    pitch = fb.pitch;

    if (hud_enabled) {
        draw_hud(pixels, pitch, screen_w, screen_h);
    }

    if (menu_open) {
        draw_menu(pixels, pitch, screen_w, screen_h);
    }
}

static int hud_thread(SceSize args, void *argp) {
    (void)args;
    (void)argp;

    while (1) {
        sceDisplayWaitVblankStart();

        handle_input();
        update_fps();

        /*
         * Anti-glitch patch:
         * Small delay gives some menus time to finish drawing
         * before VitaHUD writes over the framebuffer.
         */
        sceKernelDelayThread(1000);

        draw_all();

        if (save_message_frames > 0) {
            save_message_frames--;
        }
    }

    return 0;
}

int module_start(SceSize args, void *argp) {
    SceUID thid;

    (void)args;
    (void)argp;

    load_config();

    thid = sceKernelCreateThread(
        "VitaHUD Thread",
        hud_thread,
        0x10000100,
        0x10000,
        0,
        0,
        0
    );

    if (thid >= 0) {
        sceKernelStartThread(thid, 0, 0);
    }

    return SCE_KERNEL_START_SUCCESS;
}

int module_stop(SceSize args, void *argp) {
    (void)args;
    (void)argp;

    return SCE_KERNEL_STOP_SUCCESS;
}
