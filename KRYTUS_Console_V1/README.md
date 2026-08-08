# KRYTUS Console V1

Handheld console firmware for the Seeed Studio XIAO ESP32-S3 + ST7735
128x160 SPI TFT. This is the real V1 foundation: boot screen, main
menu, a game manager, a working built-in game, and a crash-safe save
system on internal flash (NVS). No OS, no AI, no Wi-Fi/cloud, no
Null Realm — those are future projects.

## 1. Project layout

```
src/
├── main.cpp            top-level state machine only
├── config.h             ALL pin/GPIO assignments live here
├── hardware/
│   ├── DisplayManager.*  wraps TFT_eSPI (clear/text/rect/etc.)
│   ├── InputManager.*    debounced buttons: up()/down()/a()/b()
│   └── StorageManager.*  raw NVS byte read/write wrapper
├── console/
│   ├── BootScreen.*
│   ├── MainMenu.*
│   ├── GameManager.*      registers/launches/updates games
│   ├── GamesListScreen.*  GAMES menu item
│   ├── SaveDataScreen.*   SAVE DATA menu item
│   ├── Settings.*         DISPLAY / SYSTEM / ABOUT
│   └── SystemInfo.*       chip/flash/heap readout
├── games/
│   ├── Game.h            abstract interface every game implements
│   └── TestGame.*         "KRYTUS TEST" built-in game
└── save/
    └── SaveManager.*      versioned, checksummed save/load API
```

## 2. Hardware wiring

**Display (ST7735, 1.8" 128x160, 3.3V logic):**

| ST7735 pin | XIAO ESP32-S3 pin |
|---|---|
| VCC/VDD | 3V3 |
| GND | GND |
| SCK/SCL | D8 / GPIO7 |
| SDA/MOSI | D10 / GPIO9 |
| CS | D1 / GPIO2 |
| DC | D2 / GPIO3 |
| RST | D3 / GPIO4 |
| BLK | D4 / GPIO5 |
| MISO | not connected |

**Buttons** (each button: one leg to GPIO, other leg to GND — internal
pull-ups are enabled in software, active LOW):

| Button | GPIO |
|---|---|
| UP | GPIO1 |
| DOWN | GPIO6 |
| A | GPIO44 |
| B | GPIO43 |

This is the XIAO ESP32-S3 pinout, **not** the SuperMini pinout — double
check your board silkscreen before wiring.

## 3. Install libraries & build (PlatformIO — recommended)

PlatformIO is strongly recommended over Arduino IDE for this project
because `platformio.ini` configures TFT_eSPI's pins/driver entirely
through `build_flags`, so you never have to hand-edit library files.

1. Install the [PlatformIO extension](https://platformio.org/) for
   VS Code (or use the PlatformIO CLI).
2. Open the `KRYTUS_Console_V1` folder as a PlatformIO project.
3. PlatformIO will automatically fetch `bodmer/TFT_eSPI` (declared in
   `platformio.ini`) on first build.
4. Build: click the checkmark (Build) in the PlatformIO toolbar, or run:
   ```
   pio run
   ```
5. Upload: connect the XIAO ESP32-S3 via USB-C, then click the arrow
   (Upload), or run:
   ```
   pio run -t upload
   ```
6. Monitor serial output if needed:
   ```
   pio device monitor
   ```

The board is pre-set to `seeed_xiao_esp32s3` in `platformio.ini` — no
manual board selection needed.

### If your display looks blank, offset, or wrong colors

ST7735 panels ship with different internal memory offsets ("tabs").
`platformio.ini` defaults to `-DST7735_GREENTAB`. If the picture is
shifted or discolored, try swapping that single line for one of:
`-DST7735_REDTAB`, `-DST7735_BLACKTAB`, or `-DST7735_GREENTAB2`, then
rebuild.

## 4. Arduino IDE alternative

If you'd rather use Arduino IDE:

1. **Board support:** Install "esp32" by Espressif Systems via
   Boards Manager, then select **Board → XIAO_ESP32S3**.
2. **Library:** Install "TFT_eSPI" by Bodmer via Library Manager.
3. **TFT_eSPI config:** Arduino IDE doesn't support per-project build
   flags, so you must edit the library's `User_Setup_Select.h` and add
   a custom setup file with the same driver/pin definitions listed in
   `platformio.ini`'s `build_flags` block. This is exactly what
   PlatformIO does for you automatically — it's the reason PlatformIO
   is recommended for this project.
4. Copy all files under `src/` into your sketch folder (Arduino IDE
   expects the `.ino`-equivalent `main.cpp` alongside the rest — you
   may need to rename `main.cpp` to match your sketch folder name, or
   just keep using PlatformIO).
5. Select the correct COM port and click Upload.

## 5. Using the console

- **Boot:** Shows `K.R.Y.T.U.S` / `CONSOLE V1`, then goes to the menu.
- **Main menu:** UP/DOWN to move the `>` selector, A to select, B to
  go back one level.
  - **PLAY** — launches the default (first registered) game.
  - **GAMES** — browse and launch any registered game (currently just
    KRYTUS TEST; the architecture supports adding more later).
  - **SAVE DATA** — shows current high score, A deletes the save
    (also clears a corrupted save so you can start fresh).
  - **SETTINGS** — DISPLAY (brightness, UP/DOWN to adjust), SYSTEM
    (firmware version, reset save data with confirmation), ABOUT.
  - **SYSTEM** — live chip model, CPU frequency, flash size, PSRAM,
    and free heap.
- **In KRYTUS TEST:** hold UP/DOWN to dodge incoming red obstacles
  with your green square. Score increments per obstacle dodged. On
  collision you get GAME OVER — A retries, B exits to the main menu.

## 6. How saving works

- Saves live in the ESP32-S3's internal flash via NVS
  (`Preferences`), under namespace `krytus` — **no SD card required**.
- Each save is versioned (`SAVE_VERSION` in `config.h`) and stored
  with a checksum. `SaveManager::loadGame()` rejects the data if the
  version doesn't match or the checksum fails, rather than trusting
  garbage — so a corrupted save can never brick the console. In that
  case the SAVE DATA screen shows `SAVE DATA ERROR` and lets you
  clear it and start a new save.
- The console only writes to flash when it matters: a new save is
  written after a game over **only if the new score beats the
  previous high score** — not on every frame or every game over.
- Games never touch NVS/Preferences directly — they only see
  `GameManager`, which calls `SaveManager`, which calls
  `StorageManager`. Swapping the underlying storage mechanism later
  (e.g. adding SD card support) won't require changing any game code.

## 7. Adding a future game

Implement the `Game` interface (`init/update/render/shutdown`, plus
optional `isGameOver()`/`getScore()`), then add one line in
`main.cpp`:

```cpp
gameManager.registerGame(&yourNewGame);
```

It will automatically show up in the GAMES list — no other changes
needed.
