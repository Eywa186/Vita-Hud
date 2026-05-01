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

static void get_time_12h(char *out) {
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

    put_2digits(&out[0], hour);
    out[2] = ':';
    put_2digits(&out[3], time.minute);
    out[5] = ' ';
    out[6] = is_pm ? 'P' : 'A';
    out[7] = 'M';
    out[8] = '\0';
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

static void draw_test_box(void) {
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

    int box_w = 100;
    int box_h = 18;

    int start_x = screen_w - box_w - 8;
    int start_y = screen_h - box_h - 8;

    if (start_x < 0 || start_y < 0) {
        return;
    }

    unsigned int color = 0xFF00FF00;

    for (int y = 0; y < box_h; y++) {
        for (int x = 0; x < box_w; x++) {
            pixels[(start_y + y) * pitch + (start_x + x)] = color;
        }
    }
}

static int hud_thread(SceSize args, void *argp) {
    (void)args;
    (void)argp;

    while (1) {
        handle_toggle();
        update_fps();

        if (hud_enabled) {
            int battery = scePowerGetBatteryLifePercent();

            char time_text[9];
            get_time_12h(time_text);

            draw_test_box();

            (void)battery;
            (void)time_text;
            (void)fps_value;
        }

        sceKernelDelayThread(16666);
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
