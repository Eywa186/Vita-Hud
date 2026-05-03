VitaHUD No-IP Full Replacement

This build removes/disables the IP HUD path completely because NetCtl was breaking the menu.
Everything else from the current working VitaHUD is kept.

Replace:
  src/vitahud.c

Use included:
  CMakeLists.txt
  exports.yml

Build normally. Install output:
  vitahud.suprx -> ur0:tai/vitahud.suprx

config.txt:
  *main
  ur0:tai/vitahud.suprx

Render/glitch hook path was not changed.
