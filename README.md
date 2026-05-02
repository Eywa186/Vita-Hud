# VitaHUD Anti-Glitch Fixed Build

This is a recovery build focused on stopping the HUD/menu flicker/glitching caused by unstable direct-framebuffer drawing.

## Fixes in this build

- Keeps your original `vitahud.suprx` HUD path.
- Removes the PSVshellPlus duplicate-plugin direction.
- Adds a framebuffer stability gate: VitaHUD waits until the same framebuffer pointer/size is stable before drawing.
- Draws the HUD at a lower, steadier cadence when the menu is closed.
- Keeps the menu responsive when opened.
- Adds a compact black backing plate behind HUD text to stop stale/ghost pixels in menus that do not repaint cleanly.
- Keeps logging at `ur0:data/VitaHUD/vitahud_log.txt`.

## Install

Copy the built `vitahud.suprx` to:

```txt
ur0:tai/vitahud.suprx
```

In `ur0:tai/config.txt`, under `*main`, use only:

```txt
ur0:tai/vitahud.suprx
```

Do not add this under `*KERNEL`.

Full reboot after install.

## Toggle/menu

- HUD toggle: your configured toggle
- Menu: `L + R + START`
