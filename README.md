# inZOI Keybinds Fix Mod

This is a project to enable AZERTY and ARROWS key in the game inZOI to move the character.

## How to use the mod

Download the current version of the mod (1.0): https://github.com/Sh0ckFR/inZOIKeybindsFixMod/releases/download/1.0/InzoiKeybindsFixMod.zip

Just start InzoiKeybindsFixMod.exe to enable the mod and start inZOI.

Warning: This mod doesn't work if you play in streaming with NVIDIA GeForce NOW.

## How to compile the code yourself

You can use clang++ on Windows:

```
clang++ main.cpp -o InzoiKeybindsFix.dll -shared -lUser32.lib
clang++ injector.cpp -o InzoiKeybindsFixMod.exe -lUser32
```
