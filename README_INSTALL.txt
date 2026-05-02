VitaHUD Always-On-Top Best Possible Build

Install after GitHub builds:
1. Copy build/vitahud.suprx to ur0:tai/vitahud.suprx
2. In ur0:tai/config.txt, under *main, put this FIRST:
   ur0:tai/vitahud.suprx
3. Remove old test plugins:
   ur0:tai/vitahud_paf_v5.suprx
   ur0:tai/VitaHUD_Shell.suprx
   ur0:tai/VitaHUD_Kernel.skprx
4. Full reboot.
5. Toggle menu: L + R + START

Notes:
- This is the strongest raw-framebuffer version of your existing HUD.
- It draws multiple late passes after VBlank and uses IMMEDIATE + NEXTFRAME fallback.
- It does not pause/hide during menus.
- True always-over-system-UI behavior requires a real shell/PAF overlay rebuild.
