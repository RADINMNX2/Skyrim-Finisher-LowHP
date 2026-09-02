# Finisher Low HP

A Skyrim Special Edition SKSE plugin (CommonLibSSE-NG) that lets you **execute (finish) an enemy when you're aiming at them while their health is at or below a threshold** — just press the **G** key.

Built automatically by GitHub Actions, so you get a ready-to-install `.zip` (or bare `.dll`) as a build artifact.

## Features
- Aim at an enemy whose HP is **≤ 15** (default) and press **G** to kill them in one shot.
- Configurable key and HP threshold via an INI file (no MCM).
- Doesn't run while menus/pause menus are open.
- Credits the kill to the player.

## Requirements
- Skyrim Special Edition on a supported build (works with the **Address Library** present in `Data/SKSE/Plugins`, e.g. `versionlib-1-7-104-0.bin`).
- [SKSE64](https://skse.silverlock.org/) (SKSE 2.x matching your game version).

## Installation (Mod Organizer 2)
1. Download the `FinisherLowHP-mo2-zip` artifact from the latest GitHub Actions run.
2. In MO2, click **Install** and select the zip.
3. Enable the mod and make sure its SKSE plugin is loaded (it's a `.dll` in `SKSE/Plugins/`).
4. Launch the game through MO2 + SKSE.

The plugin logs to `Documents\My Games\Skyrim Special Edition\SKSE\FinisherLowHP.log`.

## Configuration
Edit `SKSE/Plugins/FinisherLowHP/FinisherLowHP.ini` in the mod:

```ini
[General]
FinisherKey=0x22   ; G (DirectInput scan code)
FinisherHP=15
```

`FinisherKey` is a DirectInput scan code. Common ones:
- `G = 0x22`, `V = 0x2F`, `X = 0x2D`, `C = 0x2E`, `F = 0x21`, `R = 0x13`, `E = 0x12`

The installed key is printed once to the log (`Finisher: installed input sink (key=0x..)`).

## Building
The GitHub Actions workflow builds the plugin on `windows-latest` (MSVC + vcpkg + CommonLibSSE-NG) and uploads the artifacts. You don't need any local toolchain.

To build locally you'd need Visual Studio 2022, CMake, Ninja, and vcpkg. See [CommonLibSSE-NG-Template-Plugin](https://github.com/Monitor221hz/CommonLibSSE-NG-Template-Plugin) for the build prerequisites.
