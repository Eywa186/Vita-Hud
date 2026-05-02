# VitaHUD Recovery Fixed

This is the cleaned recovery build for the original direct-framebuffer VitaHUD.

## Install

Copy the GitHub Actions artifact `vitahud.suprx` to:

```txt
ur0:tai/vitahud.suprx
```

Add under `*main` in `ur0:tai/config.txt`:

```txt
ur0:tai/vitahud.suprx
```

Remove any old temporary test lines like:

```txt
ur0:tai/vitahud_paf_v5.suprx
ur0:tai/VitaHUD_Shell.suprx
ur0:tai/VitaHUD_Kernel.skprx
```

Full reboot.

## Toggle

Default menu combo:

```txt
L + R + START
```

## Notes

This recovery build keeps the original HUD source but fixes the direct framebuffer getter to try IMMEDIATE first, then NEXTFRAME. It also logs startup to:

```txt
ur0:data/VitaHUD/vitahud_log.txt
```
