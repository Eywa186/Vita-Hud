#include <psp2/kernel/modulemgr.h>
#include <psp2/kernel/threadmgr.h>
#include <psp2/ctrl.h>
#include <psp2/power.h>
#include <psp2/rtc.h>
#include <stdio.h>

#define TOGGLE_COMBO (SCE_CTRL_LTRIGGER | SCE_CTRL_RTRIGGER | SCE_CTRL_SELECT)

static int hud_enabled = 1;
static int last_combo_down = 0;

static unsigned int frame_count = 0;
static unsigned int fps_value = 0;
static SceUInt64 last_tick = 0;

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

static void get_time_12h(char *out, int out_size) {
    SceDateTime time;
    sceRtcGetCurrentClockLocalTime(&time);

    int hour = time.hour;
    const char *ampm = "AM";

    if (hour >= 12) {
        ampm = "PM";
    }

    hour = hour % 12;

    if (hour == 0) {
        hour = 12;
    }

    snprintf(out, out_size, "%02d:%02d %s", hour, time.minute, ampm);
}

static int hud_thread(SceSize args, void *argp) {
    (void)args;
    (void)argp;

    while (1) {
        handle_toggle();
        update_fps();

        if (hud_enabled) {
            int battery = scePowerGetBatteryLifePercent();

            char time_text[16];
            get_time_12h(time_text, sizeof(time_text));

            /*
             * v1 logic is active here:
             * fps_value = live FPS
             * battery = live battery %
             * time_text = 12-hour time
             *
             * Drawing hook will be added after this builds cleanly.
             */
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
        NULL
    );

    if (thid >= 0) {
        sceKernelStartThread(thid, 0, NULL);
    }

    return SCE_KERNEL_START_SUCCESS;
}

int module_stop(SceSize args, void *argp) {
    (void)args;
    (void)argp;
    return SCE_KERNEL_STOP_SUCCESS;
}
