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

static void build_hud_text(char *out) {
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

    int battery = scePowerGetBatteryLifePercent();
    int pos = 0;

    pos = append_text(out, pos, "FPS ");
    pos = append_2digit_number(out, pos, (int)fps_value);

    pos = append_text(out, pos, "  BAT ");
    pos = append_battery_number(out, pos, battery);
    out[pos++] = '%';

    pos = append_text(out, pos, "  ");

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
        case '0': {
            static const unsigned char g[7] = {
                0x0E, 0x11, 0x13, 0x15, 0x19, 0x11, 0x0E
            };
            return g[row];
        }

        case '1': {
            static const unsigned char g[7] = {
                0x04, 0x0C, 0x04, 0x04, 0x04, 0x04, 0x0E
            };
            return g[row];
        }

        case '2': {
            static const unsigned char g[7] = {
                0x0E, 0x11, 0x01, 0x02, 0x04, 0x08, 0x1F
            };
            return g[row];
        }

        case '3': {
            static const unsigned char g[7] = {
                0x1E, 0x01, 0x01, 0x0E, 0x01, 0x01, 0x1E
            };
            return g[row];
        }

        case '4': {
            static const unsigned char g[7] = {
                0x02, 0x06, 0x0A, 0x12, 0x1F, 0x02, 0x02
            };
            return g[row];
        }

        case '5': {
            static const unsigned char g[7] = {
                0x1F, 0x10, 0x10, 0x1E, 0x01, 0x01, 0x1E
            };
            return g[row];
        }

        case '6': {
            static const unsigned char g[7] = {
                0x0E, 0x10, 0x10, 0x1E, 0x11, 0x11, 0x0E
            };
            return g[row];
        }

        case '7': {
            static const unsigned char g[7] = {
                0x1F, 0x01, 0x02, 0x04, 0x08, 0x08, 0x08
            };
            return g[row];
        }

        case '8': {
            static const unsigned char g[7] = {
                0x0E, 0x11, 0x11, 0x0E, 0x11, 0x11, 0x0E
            };
            return g[row];
        }

        case '9': {
            static const unsigned char g[7] = {
                0x0E, 0x11, 0x11, 0x0F, 0x01, 0x01, 0x0E
            };
            return g[row];
        }

        case 'A': {
            static const unsigned char g[7] = {
                0x0E, 0x11, 0x11, 0x1F, 0x11, 0x11, 0x11
            };
            return g[row];
        }

        case 'B': {
            static const unsigned char g[7] = {
                0x1E, 0x11, 0x11, 0x1E, 0x11, 0x11, 0x1E
            };
            return g[row];
        }

        case 'F': {
            static const unsigned char g[7] = {
                0x1F, 0x10, 0x10, 0x1E, 0x10, 0x10, 0x10
            };
            return g[row];
        }

        case 'M': {
            static const unsigned char g[7] = {
                0x11, 0x1B, 0x15, 0x15, 0x11, 0x11, 0x11
            };
            return g[row];
        }

        case 'P': {
            static const unsigned char g[7] = {
                0x1E, 0x11, 0x11, 0x1E, 0x10, 0x10, 0x10
            };
            return g[row];
        }

        case 'S': {
            static const unsigned char g[7] = {
                0x0F, 0x10, 0x10, 0x0E, 0x01, 0x01, 0x1E
            };
            return g[row];
        }

        case 'T': {
            static const unsigned char g[7] = {
                0x1F, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04
            };
            return g[row];
        }

        case ':': {
            static const unsigned char g[7] = {
                0x00, 0x04, 0x04, 0x00, 0x04, 0x04, 0x00
            };
            return g[row];
        }

        case '%': {
            static const unsigned char g[7] = {
                0x19, 0x19, 0x02, 0x04, 0x08, 0x13, 0x13
            };
            return g[row];
        }

        case ' ': {
            return 0x00;
        }

        default:
            return 0x00;
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

    char text[40];
    build_hud_text(text);

    int scale = 1;
    int w = text_width(text, scale);
    int h = 7 * scale;

    int start_x = screen_w - w - 8;
    int start_y = screen_h - h - 8;

    if (start_x < 0 || start_y < 0) {
        return;
    }

    unsigned int white = 0xFFFFFFFF;
    unsigned int shadow = 0xFF000000;

    draw_text(pixels, pitch, start_x + 1, start_y + 1, text, shadow, scale);
    draw_text(pixels, pitch, start_x, start_y, text, white, scale);
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
