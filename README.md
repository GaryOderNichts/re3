# re3-wiiu
<img src="https://github.com/GaryOderNichts/re3-wiiu/blob/wiiu/logo.png?raw=true" alt="re3 logo" width="200">

This is a port of GTA III to the Nintendo Wii U.  

## Instructions
For instructions take a look at the GBATemp thread here: https://gbatemp.net/threads/re3-wiiu-gta-iii-wii-u-port.579827/

## Building
To build this project make sure you install the requried packages:  
`(dkp-)pacman -S ppc-mpg123 wut`  
Build [SDL2 from yawut](https://github.com/yawut/SDL/tree/wiiu-2.0.9) to avoid issues with crashes and exiting.  
You also need to build [libsndfile](https://github.com/libsndfile/libsndfile). To configure libsndfile for the Wii U take a look at this gist: https://gist.github.com/GaryOderNichts/475edaf03ff08ba100840608f92eade8  
Then clone this repo using `git clone --recursive https://github.com/GaryOderNichts/re3` and build using `make`.  
To build the channel edit `CHANNEL_BUILD := 0` to `CHANNEL_BUILD := 1` in the `Makefile`.

# Original README

[![Build Status](https://img.shields.io/endpoint.svg?url=https%3A%2F%2Factions-badge.atrox.dev%2FGTAmodding%2Fre3%2Fbadge%3Fref%3Dmaster&style=flat)](https://actions-badge.atrox.dev/GTAmodding/re3/goto?ref=master)
<a href="https://discord.gg/aKYAwCx92H"><img src="https://img.shields.io/badge/discord-join-7289DA.svg?logo=discord&longCache=true&style=flat" /></a>

## Intro

The aim of this project is to reverse GTA III for PC by replacing
parts of the game [one by one](https://en.wikipedia.org/wiki/Ship_of_Theseus)
such that we have a working game at all times.

## How can I try it?

- re3 requires game assets to work, so you **must** own [a copy of GTA III](https://store.steampowered.com/app/12100/Grand_Theft_Auto_III/).
- Build re3 or download [the latest nightly build](https://github.com/GTAmodding/re3/actions) (You must be logged in.)
- (Optional) If you want to use optional features like Russian language or menu map, copy the files in /gamefiles folder to your game root folder.
- Move re3.exe to GTA 3 directory and run it.

## Latest standalone executables to download 

(Put content of selected archive into gamedir)

- [MacOS 64bit](https://nightly.link/GTAmodding/re3/workflows/build-cmake-conan/master/macos-latest-gl3.zip)
- [Linux 64bit](https://nightly.link/GTAmodding/re3/workflows/build-cmake-conan/master/ubuntu-latest-gl3.zip)
- [Windows D3D9 64bit](https://nightly.link/GTAmodding/re3/workflows/build-cmake-conan/master/windows-latest-d3d9.zip)
- [Windows OpenGL 64bit](https://nightly.link/GTAmodding/re3/workflows/build-cmake-conan/master/windows-latest-gl3.zip)
- [Windows D3D9 MSS 32bit](https://nightly.link/GTAmodding/re3/workflows/re3_msvc_x86/master/re3_Release_win-x86-librw_d3d9-mss.zip)

## Building from Source  

If you gonna use premake, then before starting you may want to point GTA_III_RE_DIR environment variable to GTA3 root folder, if you want executable to be moved there via post-build script.

<details><summary>Linux Premake</summary>

For Linux using premake, proceed: [Building on Linux](https://github.com/GTAmodding/re3/wiki/Building-on-Linux)

</details>

<details><summary>Linux Conan</summary>

Obtain source code.
```
git clone https://github.com/GTAmodding/re3.git
cd re3
git submodule init
git submodule update --recursive
```
Install python and conan, and then run build.
```
conan export vendor/librw librw/master@
mkdir build
cd build
conan install .. re3/master@ -if build -o re3:audio=openal -o librw:platform=gl3 -o librw:gl3_gfxlib=glfw --build missing -s re3:build_type=RelWithDebInfo -s librw:build_type=RelWithDebInfo
conan build .. -if build -bf build -pf package
```
</details>

<details><summary>FreeBSD</summary>

For FreeBSD using premake, proceed: [Building on FreeBSD](https://github.com/GTAmodding/re3/wiki/Building-on-FreeBSD)

</details>

<details><summary>Windows</summary>

Assuming you have Visual Studio:
- Clone the repo using the argument `--recursive`.
- Run one of the `premake-vsXXXX.cmd` variants on root folder.
- Open the project via Visual Studio  
    
**If you use 64-bit D3D9**: We don't ship 64-bit Dx9 SDK. You need to download it from Microsoft if you don't have it(although it should come pre-installed after some Windows version)  

**If you choose OpenAL on Windows** You must read [Running OpenAL build on Windows](https://github.com/GTAmodding/re3/wiki/Running-OpenAL-build-on-Windows).
</details>

> :information_source: There are various settings at the very bottom of [config.h](https://github.com/GTAmodding/re3/tree/master/src/core/config.h), you may want to take a look there. i.e. FIX_BUGS define fixes the bugs we've come across.

> :information_source: **Did you notice librw?** re3 uses completely homebrew RenderWare-replacement rendering engine; [librw](https://github.com/aap/librw/). librw comes as submodule of re3, but you also can use LIBRW enviorenment variable to specify path to your own librw.

## Contributing
Please read the [Coding Style](https://github.com/GTAmodding/re3/blob/master/CODING_STYLE.md) Document

### Unreversed / incomplete classes (at least the ones we know)
The following classes have only unused or practically unused code left:
```
NameGrid.cpp - only on mobile (a player name grid, either a very early player name code ala GTA1 or a multiplayer leftover)
PedDebug.cpp - only on mobile (debug code)
HandlingMgr.cpp - debug functions from mobile
CFormationInfo - unused PedAI class that could be found on mobile
CVehicle::ProcessBikeWheel - early bike code (only on mobile)
CAutomobile::DebugCode - debug function from mobile
CBoat::DebugCode - debug function from mobile
CBoat::ModifyHandlingValue - debug function from mobile
CBoat::DisplayHandlingData - debug function from mobile
CStreaming::PrintRequestList - debug function from mobile
d3d8raster.c - only on PC (slight RW modification that we don't actually need)
```
