# re3-wiiu
<img src="https://github.com/GaryOderNichts/re3-wiiu/blob/master/logo.png?raw=true" alt="re3 logo" width="200">

This is a port of GTA III to the Nintendo Wii U.  

## Instructions

## Building
To build this project make sure you install the requried packages:  
`(dkp-)pacman -S ppc-mpg123 wiiu-sdl2 wut`  
You also need to build [libsndfile](https://github.com/libsndfile/libsndfile). To configure libsndfile for the Wii U take a look at this gist: https://gist.github.com/GaryOderNichts/475edaf03ff08ba100840608f92eade8  
Then clone this repo using `git clone --recursive https://github.com/GaryOderNichts/re3` and build using `make`.  
To build the channel edit `CHANNEL_BUILD := 0` to `CHANNEL_BUILD := 1` in the `Makefile`.