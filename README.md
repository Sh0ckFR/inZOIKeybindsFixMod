# inZOI Keybinds Fix Mod

This is a project to enable AZERTY and ARROW keys in the game inZOI to move the character.

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

## VirusTotal

The 3 detections are false positives, but if you don't trust me, just compile it yourself or ask to a developer to compile it for you.

https://www.virustotal.com/gui/file/e1ca883d0cc1e89847a5bad518acc07398080fe55d3b3f1a7fc04359ddb9575d


