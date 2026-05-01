# VitaHUD

A small PS Vita taiHEN user plugin that draws a PSP-style live HUD over games/apps.

## Current v1 features

- Live FPS counter
- Live battery percentage
- 12-hour clock
- Toggle HUD on/off with **L + R + SELECT**
- Bottom-right overlay by default
- Draws directly into the framebuffer

## GitHub auto-build method

This project includes a GitHub Actions workflow. Once you upload the folder to GitHub, GitHub will build the plugin for you.

### Steps

1. Create a new GitHub repository.
2. Upload every file/folder from this project.
3. Open the repository on GitHub.
4. Click **Actions**.
5. Click **Build VitaHUD**.
6. Click **Run workflow**.
7. Wait for it to finish.
8. Open the finished workflow run.
9. Download the artifact named **vitahud-suprx**.
10. Extract it. Inside will be:

```txt
vitahud.suprx
```

## Vita install

Copy the built plugin here:

```txt
ur0:tai/vitahud.suprx
```

Then edit:

```txt
ur0:tai/config.txt
```

Add under `*ALL`:

```txt
*ALL
ur0:tai/vitahud.suprx
```

Reboot the Vita.

## Toggle combo

```txt
L + R + SELECT
```

## Manual build requirements

- VitaSDK
- taiHEN headers/stubs

## Manual build

```bash
mkdir build
cd build
cmake ..
make
```

Output:

```txt
vitahud.suprx
```

## Notes

This v1 targets common 32-bit Vita framebuffer formats first. If a game uses a different pixel format, the HUD may not draw until that format is added.
