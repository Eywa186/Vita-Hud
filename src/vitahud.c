#include <psp2/kernel/modulemgr.h>
#include <psp2/kernel/threadmgr.h>
#include <psp2/ctrl.h>
#include <psp2/power.h>
#include <psp2/rtc.h>
#include <psp2/display.h>

#define TOGGLE_COMBO (SCE_CTRL_LTRIGGER | SCE_CTRL_RTRIGGER | SCE_CTRL_SELECT)

static int hud_enabled = 1;
static int last_combo_down = 0;

static unsigned int frame_count = 0;
static unsigned int fps_value = 0;
static SceUInt64 last_tick = 0;

static void put_2digits(char *out, int value) {
    out[0] = '0' + ((value / 10) % 10);
    out[1] = '0' + (value % 10);
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

static void handle_toggle(void) {
    SceCtrlData pad;
    sceCtrlPeekBufferPositive(0, &pad, 1);

    int combo_down = ((pad.buttons & TOGGLE_COMBO) == TOGGLE_COMBO);

    if (combo_down && !last_combo_down) {
        hud_enabled = !hud_enabled;
    }

    last_combo_down = combo_down;
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
        case 'F': { static const unsigned char g[7] = {0x1F,0x10,0x10,0x1E,0x10,0x10,0x10}; return g[row]; }
        case 'M': { static const unsigned char g[7] = {0x11,0x1B,0x15,0x15,0x11,0x11,0x11}; return g[row]; }
        case 'P': { static const unsigned char g[7] = {0x1E,0x11,0x11,0x1E,0x10,0x10,0x10}; return g[row]; }
        case 'S': { static const unsigned char g[7] = {0x0F,0x10,0x10,0x0E,0x01,0x01,0x1E}; return g[row]; }

        case ':': { static const unsigned char g[7] = {0x00,0x04,0x04,0x00,0x04,0x04,0x00}; return g[row]; }
        case '%': { static const unsigned char g[7] = {0x19,0x19,0x02,0x04,0x08,0x13,0x13}; return g[row]; }
        case ' ': return 0x00;

        default:
            return 0x00;
    }
}

static void draw_rect(
    unsigned int *pixels,
    int pitch,
    int x,
    int y,
    int w,
    int h,
    unsigned int color
) {
    for (int yy = 0; yy < h; yy++) {
        for (int xx = 0; xx < w; xx++) {
            pixels[(y + yy) * pitch + (x + xx)] = color;
        }
    }
}

static void draw_char(
    unsigned int *pixels,
    int pitch,
    int x,
    int y,
    char c,
    unsigned int color,
    int scale
) {
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

static void draw_text(
    unsigned int *pixels,
    int pitch,
    int x,
    int y,
    const char *text,
    unsigned int color,
    int scale
) {
    int cursor_x = x;

    while (*text) {
        draw_char(pixels, pitch, cursor_x, y, *text, color, scale);
        cursor_x += 6 * scale;
        text++;
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

static void draw_battery_icon(
    unsigned int *pixels,
    int pitch,
    int x,
    int y,
    int battery,
    int scale
) {
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

    draw_rect(
        pixels,
        pitch,
        x + body_w,
        y + (2 * scale),
        cap_w,
        cap_h,
        white
    );

    int inner_x = x + border;
    int inner_y = y + border;
    int inner_w = body_w - (border * 2);
    int inner_h = body_h - (border * 2);

    int fill_w = (inner_w * battery) / 100;

    if (fill_w > 0) {
        draw_rect(pixels, pitch, inner_x, inner_y, fill_w, inner_h, fill);
    }
}

static void draw_hud_text(void) {
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

    int battery = scePowerGetBatteryLifePercent();

    char fps_text[12];
    char battery_text[8];
    char time_text[10];

    build_fps_text(fps_text);
    build_battery_text(battery_text, battery);
    build_time_text(time_text);

    int scale = 2;

    int fps_w = text_width(fps_text, scale);
    int battery_text_w = text_width(battery_text, scale);
    int time_w = text_width(time_text, scale);

    int icon_w = (13 * scale) + (2 * scale);
    int icon_h = 7 * scale;

    int gap_small = 4 * scale;
    int gap_big = 8 * scale;

    /*
     * New order:
     * FPS 59   91% [icon]   10:56 PM
     */
    int total_w =
        fps_w +
        gap_big +
        battery_text_w +
        gap_small +
        icon_w +
        gap_big +
        time_w;

    int text_h = 7 * scale;
    int total_h = text_h;

    int start_x = screen_w - total_w - 8;
    int start_y = screen_h - total_h - 8;

    if (start_x < 0 || start_y < 0) {
        return;
    }

    unsigned int white = 0xFFFFFFFF;
    unsigned int shadow = 0xFF000000;

    int x = start_x;

    // FPS text
    draw_text(pixels, pitch, x + 1, start_y + 1, fps_text, shadow, scale);
    draw_text(pixels, pitch, x, start_y, fps_text, white, scale);
    x += fps_w + gap_big;

    // Battery percent text FIRST
    draw_text(pixels, pitch, x + 1, start_y + 1, battery_text, shadow, scale);
    draw_text(pixels, pitch, x, start_y, battery_text, white, scale);
    x += battery_text_w + gap_small;

    // Battery icon to the RIGHT of the percentage
    int icon_y = start_y + ((text_h - icon_h) / 2);
    draw_battery_icon(pixels, pitch, x, icon_y, battery, scale);
    x += icon_w + gap_big;

    // Time text
    draw_text(pixels, pitch, x + 1, start_y + 1, time_text, shadow, scale);
    draw_text(pixels, pitch, x, start_y, time_text, white, scale);
}

static int hud_thread(SceSize args, void *argp) {
    (void)args;
    (void)argp;

    while (1) {
        sceDisplayWaitVblankStart();

        handle_toggle();
        update_fps();

        if (hud_enabled) {
            draw_hud_text();
        }
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
