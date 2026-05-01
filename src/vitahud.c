#include <psp2/kernel/modulemgr.h>
#include <psp2/kernel/threadmgr.h>
#include <psp2/ctrl.h>
#include <psp2/power.h>
#include <psp2/rtc.h>
#include <psp2/display.h>
#include <taihen.h>

#define TOGGLE_COMBO (SCE_CTRL_LTRIGGER | SCE_CTRL_RTRIGGER | SCE_CTRL_SELECT)

static int hud_enabled = 1;
static int last_combo_down = 0;

static unsigned int frame_count = 0;
static unsigned int fps_value = 0;
static SceUInt64 last_tick = 0;

static SceUID display_hook_uid = -1;
static tai_hook_ref_t display_hook_ref;

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

static void draw_test_box(const SceDisplayFrameBuf *fb) {
    if (!fb || !fb->base) {
        return;
    }

    if (fb->pixelformat != SCE_DISPLAY_PIXELFORMAT_A8B8G8R8 &&
        fb->pixelformat != SCE_DISPLAY_PIXELFORMAT_X8B8G8R8) {
        return;
    }

    unsigned int *pixels = (unsigned int *)fb->base;

    int screen_w = fb->width;
    int screen_h = fb->height;
    int pitch = fb->pitch;

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

static int sceDisplaySetFrameBuf_hook(const SceDisplayFrameBuf *pParam, int sync) {
    int ret = TAI_CONTINUE(int, display_hook_ref, pParam, sync);

    handle_toggle();
    update_fps();

    if (hud_enabled) {
        int battery = scePowerGetBatteryLifePercent();

        char time_text[9];
        get_time_12h(time_text);

        draw_test_box(pParam);

        (void)battery;
        (void)time_text;
        (void)fps_value;
    }

    return ret;
}

int module_start(SceSize args, void *argp) {
    (void)args;
    (void)argp;

    display_hook_uid = taiHookFunctionImport(
        &display_hook_ref,
        TAI_MAIN_MODULE,
        TAI_ANY_LIBRARY,
        0x7A410B64,
        sceDisplaySetFrameBuf_hook
    );

    return SCE_KERNEL_START_SUCCESS;
}

int module_stop(SceSize args, void *argp) {
    (void)args;
    (void)argp;

    if (display_hook_uid >= 0) {
        taiHookRelease(display_hook_uid, display_hook_ref);
    }

    return SCE_KERNEL_STOP_SUCCESS;
}
