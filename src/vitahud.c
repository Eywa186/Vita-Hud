#include <psp2/kernel/modulemgr.h>
#include <psp2/kernel/threadmgr.h>
#include <psp2/ctrl.h>
#include <psp2/power.h>
#include <psp2/rtc.h>
#include <psp2/display.h>

#define MENU_COMBO (SCE_CTRL_LTRIGGER | SCE_CTRL_RTRIGGER | SCE_CTRL_START)

#define POS_BOTTOM_RIGHT 0
#define POS_BOTTOM_LEFT  1
#define POS_TOP_RIGHT    2
#define POS_TOP_LEFT     3

#define SIZE_MINI   0
#define SIZE_NORMAL 1
#define SIZE_LARGE  2

#define COLOR_WHITE   0
#define COLOR_GREEN   1
#define COLOR_YELLOW  2
#define COLOR_RED     3
#define COLOR_CYAN    4
#define COLOR_MAGENTA 5
#define COLOR_ORANGE  6

#define LANG_EN 0
#define LANG_ES 1

#define TOGGLE_SELECT 0
#define TOGGLE_UP     1
#define TOGGLE_DOWN   2

static int hud_enabled = 1;
static int menu_open = 0;
static int menu_index = 0;

static int hud_position = POS_BOTTOM_RIGHT;
static int hud_size = SIZE_NORMAL;
static int hud_color = COLOR_WHITE;
static int show_fps = 1;
static int show_battery = 1;
static int show_time = 1;
static int hud_language = LANG_EN;
static int toggle_combo_mode = TOGGLE_SELECT;

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

static unsigned int get_toggle_combo(void) {
    if (toggle_combo_mode == TOGGLE_UP) {
        return SCE_CTRL_LTRIGGER | SCE_CTRL_RTRIGGER | SCE_CTRL_UP;
    }

    if (toggle_combo_mode == TOGGLE_DOWN) {
        return SCE_CTRL_LTRIGGER | SCE_CTRL_RTRIGGER | SCE_CTRL_DOWN;
    }

    return SCE_CTRL_LTRIGGER | SCE_CTRL_RTRIGGER | SCE_CTRL_SELECT;
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

static int append_text(char *out, int pos, const char *text) {
    while (*text) {
        out[pos++] = *text++;
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
    sceRtcGetCurrentClockLocalTime(&time);

    int hour = time.hour;
    int is_pm = 0;

    if (hour >= 12) {
        is_pm = 1;
    }

    hour = hour % 12;

    if (hour == 0) {
        hour = 12;
    }

    int pos = 0;

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
        case COLOR_YELLOW:  return 0xFFFFFF00;
        case COLOR_RED:     return 0xFFFF0000;
        case COLOR_CYAN:    return 0xFF00FFFF;
        case COLOR_MAGENTA: return 0xFFFF00FF;
        case COLOR_ORANGE:  return 0xFFFF8000;
        case COLOR_WHITE:
        default:            return 0xFFFFFFFF;
    }
}

static unsigned int get_battery_color(int battery) {
    if (battery >= 60) {
        return 0xFF00FF00;
    }

    if (battery >= 21) {
        return 0xFFFFFF00;
    }

    return 0xFFFF0000;
}

static int get_scale(void) {
    if (hud_size == SIZE_MINI) {
        return 1;
    }

    if (hud_size == SIZE_LARGE) {
        return 3;
    }

    return 2;
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
    for (int yy = 0; yy < h; yy++) {
        for (int xx = 0; xx < w; xx++) {
            pixels[(y + yy) * pitch + (x + xx)] = color;
        }
    }
}

static void draw_char(unsigned int *pixels, int pitch, int x, int y, char c, unsigned int color, int scale) {
    for (int row = 0; row < 7; row++) {
        unsigned char bits = font5x7(c, row);

        for (int col = 0; col < 5; col++) {
            if (bits & (1 << (4 - col))) {
                for (int sy = 0; sy < scale; sy++) {
                    for (int sx = 0; sx < scale; sx++) {
                        pixels[(y + row * scale + sy) * pitch + (x + col * scale + sx)] = color;
                    }
                }
            }
        }
    }
}

static int text_width(const char *text, int scale) {
    int count = 0;

    while (text[count]) {
        count++;
    }

    return count * 6 * scale;
}

static void draw_text(unsigned int *pixels, int pitch, int x, int y, const char *text, unsigned int color, int scale) {
    int cursor_x = x;

    while (*text) {
        draw_char(pixels, pitch, cursor_x, y, *text, color, scale);
        cursor_x += 6 * scale;
        text++;
    }
}

static void draw_text_shadow(unsigned int *pixels, int pitch, int x, int y, const char *text, unsigned int color, int scale) {
    draw_text(pixels, pitch, x + 1, y + 1, text, 0xFF000000, scale);
    draw_text(pixels, pitch, x, y, text, color, scale);
}

static void draw_battery_icon(unsigned int *pixels, int pitch, int x, int y, int battery, int scale) {
    if (battery < 0) battery = 0;
    if (battery > 100) battery = 100;

    unsigned int white = 0xFFFFFFFF;
    unsigned int fill = get_battery_color(battery);

    int body_w = 13 * scale;
    int body_h = 7 * scale;
    int cap_w = 2 * scale;
    int cap_h = 3 * scale;
    int border = scale;

    draw_rect(pixels, pitch, x, y, body_w, border, white);
    draw_rect(pixels, pitch, x, y + body_h - border, body_w, border, white);
    draw_rect(pixels, pitch, x, y, border, body_h, white);
    draw_rect(pixels, pitch, x + body_w - border, y, border, body_h, white);

    draw_rect(pixels, pitch, x + body_w, y + (2 * scale), cap_w, cap_h, white);

    int inner_x = x + border;
    int inner_y = y + border;
    int inner_w = body_w - (border * 2);
    int inner_h = body_h - (border * 2);

    int fill_w = (inner_w * battery) / 100;

    if (fill_w > 0) {
        draw_rect(pixels, pitch, inner_x, inner_y, fill_w, inner_h, fill);
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

static const char *size_name(void) {
    switch (hud_size) {
        case SIZE_MINI:  return "MINI";
        case SIZE_LARGE: return "LARGE";
        case SIZE_NORMAL:
        default:         return "NORMAL";
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
        case COLOR_WHITE:
        default:            return "WHITE";
    }
}

static const char *language_name(void) {
    if (hud_language == LANG_ES) {
        return "ESPANOL";
    }

    return "ENGLISH";
}

static const char *toggle_name(void) {
    switch (toggle_combo_mode) {
        case TOGGLE_UP:   return "L+R+UP";
        case TOGGLE_DOWN: return "L+R+DOWN";
        case TOGGLE_SELECT:
        default:          return "L+R+SELECT";
    }
}

static const char *onoff_name(int value) {
    return value ? "ON" : "OFF";
}

static void menu_change(int dir) {
    if (menu_index == 0) {
        hud_enabled = !hud_enabled;
    } else if (menu_index == 1) {
        hud_position += dir;
        if (hud_position < 0) hud_position = 3;
        if (hud_position > 3) hud_position = 0;
    } else if (menu_index == 2) {
        hud_size += dir;
        if (hud_size < 0) hud_size = 2;
        if (hud_size > 2) hud_size = 0;
    } else if (menu_index == 3) {
        hud_color += dir;
        if (hud_color < 0) hud_color = 6;
        if (hud_color > 6) hud_color = 0;
    } else if (menu_index == 4) {
        show_fps = !show_fps;
    } else if (menu_index == 5) {
        show_battery = !show_battery;
    } else if (menu_index == 6) {
        show_time = !show_time;
    } else if (menu_index == 7) {
        hud_language = !hud_language;
    } else if (menu_index == 8) {
        toggle_combo_mode += dir;
        if (toggle_combo_mode < 0) toggle_combo_mode = 2;
        if (toggle_combo_mode > 2) toggle_combo_mode = 0;
    }
}

static void draw_menu_line(unsigned int *pixels, int pitch, int x, int y, int selected, const char *label, const char *value) {
    int scale = 1;
    unsigned int color = selected ? 0xFFFFFF00 : 0xFFFFFFFF;

    if (selected) {
        draw_text_shadow(pixels, pitch, x, y, ">", color, scale);
    }

    draw_text_shadow(pixels, pitch, x + 12, y, label, color, scale);
    draw_text_shadow(pixels, pitch, x + 150, y, value, color, scale);
}

static void draw_menu(unsigned int *pixels, int pitch, int screen_w, int screen_h) {
    int x = 20;
    int y = 24;
    int w = 330;
    int h = 150;

    draw_rect(pixels, pitch, x - 8, y - 8, w, h, 0xCC000000);
    draw_rect(pixels, pitch, x - 8, y - 8, w, 1, 0xFFFFFFFF);
    draw_rect(pixels, pitch, x - 8, y + h - 1, w, 1, 0xFFFFFFFF);
    draw_rect(pixels, pitch, x - 8, y - 8, 1, h, 0xFFFFFFFF);
    draw_rect(pixels, pitch, x + w - 9, y - 8, 1, h, 0xFFFFFFFF);

    if (hud_language == LANG_ES) {
        draw_text_shadow(pixels, pitch, x, y, "VITAHUD MENU", 0xFF00FFFF, 1);
        y += 14;
        draw_menu_line(pixels, pitch, x, y, menu_index == 0, "HUD", onoff_name(hud_enabled)); y += 12;
        draw_menu_line(pixels, pitch, x, y, menu_index == 1, "POSICION", position_name()); y += 12;
        draw_menu_line(pixels, pitch, x, y, menu_index == 2, "TAMANO", size_name()); y += 12;
        draw_menu_line(pixels, pitch, x, y, menu_index == 3, "COLOR", color_name()); y += 12;
        draw_menu_line(pixels, pitch, x, y, menu_index == 4, "FPS", onoff_name(show_fps)); y += 12;
        draw_menu_line(pixels, pitch, x, y, menu_index == 5, "BATERIA", onoff_name(show_battery)); y += 12;
        draw_menu_line(pixels, pitch, x, y, menu_index == 6, "HORA", onoff_name(show_time)); y += 12;
        draw_menu_line(pixels, pitch, x, y, menu_index == 7, "IDIOMA", language_name()); y += 12;
        draw_menu_line(pixels, pitch, x, y, menu_index == 8, "COMBO", toggle_name());
    } else {
        draw_text_shadow(pixels, pitch, x, y, "VITAHUD MENU", 0xFF00FFFF, 1);
        y += 14;
        draw_menu_line(pixels, pitch, x, y, menu_index == 0, "HUD", onoff_name(hud_enabled)); y += 12;
        draw_menu_line(pixels, pitch, x, y, menu_index == 1, "POSITION", position_name()); y += 12;
        draw_menu_line(pixels, pitch, x, y, menu_index == 2, "SIZE", size_name()); y += 12;
        draw_menu_line(pixels, pitch, x, y, menu_index == 3, "COLOR", color_name()); y += 12;
        draw_menu_line(pixels, pitch, x, y, menu_index == 4, "FPS", onoff_name(show_fps)); y += 12;
        draw_menu_line(pixels, pitch, x, y, menu_index == 5, "BATTERY", onoff_name(show_battery)); y += 12;
        draw_menu_line(pixels, pitch, x, y, menu_index == 6, "TIME", onoff_name(show_time)); y += 12;
        draw_menu_line(pixels, pitch, x, y, menu_index == 7, "LANGUAGE", language_name()); y += 12;
        draw_menu_line(pixels, pitch, x, y, menu_index == 8, "COMBO", toggle_name());
    }

    draw_text_shadow(pixels, pitch, x, y + 14, "UP/DOWN MOVE  LEFT/RIGHT CHANGE  O CLOSE", 0xFFFFFFFF, 1);

    (void)screen_w;
    (void)screen_h;
}

static void handle_input(void) {
    SceCtrlData pad;
    sceCtrlPeekBufferPositive(0, &pad, 1);

    unsigned int buttons = pad.buttons;
    unsigned int pressed = buttons & ~last_buttons;

    unsigned int toggle_combo = get_toggle_combo();
    int toggle_down = ((buttons & toggle_combo) == toggle_combo);
    int menu_down = ((buttons & MENU_COMBO) == MENU_COMBO);

    if (menu_down && !last_menu_down) {
        menu_open = !menu_open;
    }

    if (!menu_open && toggle_down && !last_toggle_down) {
        hud_enabled = !hud_enabled;
    }

    if (menu_open) {
        if (pressed & SCE_CTRL_UP) {
            menu_index--;
            if (menu_index < 0) menu_index = 8;
        }

        if (pressed & SCE_CTRL_DOWN) {
            menu_index++;
            if (menu_index > 8) menu_index = 0;
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

    char fps_text[12];
    char battery_text[8];
    char time_text[10];

    build_fps_text(fps_text);
    build_battery_text(battery_text, battery);
    build_time_text(time_text);

    int scale = get_scale();

    int gap_small = 2 * scale;
    int gap_big = 5 * scale;

    int fps_w = show_fps ? text_width(fps_text, scale) : 0;
    int battery_text_w = show_battery ? text_width(battery_text, scale) : 0;
    int time_w = show_time ? text_width(time_text, scale) : 0;

    int icon_w = show_battery ? ((13 * scale) + (2 * scale)) : 0;
    int icon_h = 7 * scale;

    int total_w = 0;

    if (show_fps) total_w += fps_w;
    if (show_fps && show_battery) total_w += gap_big;
    if (show_battery) total_w += battery_text_w + gap_small + icon_w;
    if ((show_fps || show_battery) && show_time) total_w += gap_big;
    if (show_time) total_w += time_w;

    if (total_w <= 0) {
        return;
    }

    int text_h = 7 * scale;
    int total_h = text_h;

    int margin = 8;

    int start_x = margin;
    int start_y = margin;

    if (hud_position == POS_BOTTOM_RIGHT || hud_position == POS_TOP_RIGHT) {
        start_x = screen_w - total_w - margin;
    }

    if (hud_position == POS_BOTTOM_RIGHT || hud_position == POS_BOTTOM_LEFT) {
        start_y = screen_h - total_h - margin;
    }

    if (start_x < 0 || start_y < 0) {
        return;
    }

    unsigned int text_color = get_text_color();

    int x = start_x;

    if (show_fps) {
        draw_text_shadow(pixels, pitch, x, start_y, fps_text, text_color, scale);
        x += fps_w;

        if (show_battery || show_time) {
            x += gap_big;
        }
    }

    if (show_battery) {
        draw_text_shadow(pixels, pitch, x, start_y, battery_text, text_color, scale);
        x += battery_text_w + gap_small;

        int icon_y = start_y + ((text_h - icon_h) / 2);
        draw_battery_icon(pixels, pitch, x, icon_y, battery, scale);
        x += icon_w;

        if (show_time) {
            x += gap_big;
        }
    }

    if (show_time) {
        draw_text_shadow(pixels, pitch, x, start_y, time_text, text_color, scale);
    }
}

static void draw_all(void) {
    SceDisplayFrameBuf fb;
    fb.size = sizeof(SceDisplayFrameBuf);

    int res = sceDisplayGetFrameBuf(&fb, SCE_DISPLAY_SETBUF_NEXTFRAME);

    if (res < 0 || !fb.base) {
        return;
    }

    if (fb.pixelformat != SCE_DISPLAY_PIXELFORMAT_A8B8G8R8) {
        return;
    }

    unsigned int *pixels = (unsigned int *)fb.base;

    int screen_w = fb.width;
    int screen_h = fb.height;
    int pitch = fb.pitch;

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

        draw_all();
    }

    return 0;
}

int module_start(SceSize args, void *argp) {
    (void)args;
    (void)argp;

    SceUID thid = sceKernelCreateThread(
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
