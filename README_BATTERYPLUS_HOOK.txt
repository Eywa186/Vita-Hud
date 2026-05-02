VitaHUD BatteryPlus-Hook Build

This version ports the stable VITABatteryPlus drawing model into VitaHUD:

- Hooks sceDisplaySetFrameBuf through taiHookFunctionImport.
- Draws directly into the frame buffer submitted by the app/system.
- Removes the old separate HUD thread that used sceDisplayGetFrameBuf polling.
- Keeps the VitaHUD menu/options from the uploaded Vita-Hud-main source.
- Uses ur0:data/VitaHUD/config.txt for config.

Install artifact:
ur0/tai/vitahud.suprx -> ur0:tai/vitahud.suprx

config.txt:
*main
ur0:tai/vitahud.suprx

Toggle menu:
L + R + START
