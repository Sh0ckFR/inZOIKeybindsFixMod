# inZOI Keybinds Fix Mod

This is a project to enable AZERTY and ARROWS key in the game inZOI.

## How to use the mod

Download the mod: https://github.com/Sh0ckFR/inZOIKeybindsFixMod/releases/download/1.0/InzoiKeybindsFixMod.zip
Just start InzoiKeybindsFixMod.exe to enable the mod.

## How to compile the code yourself

You can use clang++ on Windows:

```
clang++ main.cpp -o InzoiKeybindsFix.dll -shared -lUser32.lib
clang++ injector.cpp -o InzoiKeybindsFixMod.exe -lUser32
```
