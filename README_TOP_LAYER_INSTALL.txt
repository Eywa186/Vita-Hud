VitaHUD Force Top-Layer Build

Install:
1. Copy built vitahud.suprx to ur0:tai/vitahud.suprx
2. In ur0:tai/config.txt, put VitaHUD as the FIRST plugin under *main:

*main
ur0:tai/vitahud.suprx
...your other *main plugins below...

Do NOT put VitaHUD under *KERNEL.
Remove old test lines like vitahud_paf_v5.suprx, VitaHUD_Shell.suprx, VitaHUD_Kernel.skprx.

This build forces top-layer timing by drawing after VBlank with a late second pass.
Toggle/menu combo: L + R + START
Config/log folder: ur0:data/VitaHUD
