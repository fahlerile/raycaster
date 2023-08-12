#ifndef CONSTANTS_H
#define CONSTANTS_H

#define DEBUG  // whether or not the player is able to see the map with M

// MATH CONSTANTS
#define PI 3.14159265359

// SDL CONSTANTS
#define WINDOW_TITLE "Raycaster"
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define WINDOW_FLAGS 0
#define RENDERER_FLAGS SDL_RENDERER_PRESENTVSYNC

// MAP CONSTANTS
#define MAP_WIDTH 64   // max width of the map, in tiles
#define MAP_HEIGHT 64  // max height of the map, in tiles

// 2D MAP & PLAYER CONSTANTS
#define MAP_SQUARE_SIZE 5    // pixels
#define MAP_BORDER_SIZE 0     // pixels
#define TWO_D_PLAYER_SIZE 5  // pixels
#define TWO_D_MAP_ZERO_COLOR (color_t) {255, 255, 255, 255}   // color of empty space in 2d map
#define TWO_D_MAP_BLOCK_COLOR (color_t) {0, 0, 0, 255}  // color of some block in 2d map
#define TWO_D_PLAYER_COLOR (color_t) {255, 0, 0, 255}         // player color in 2d map
#define TWO_D_RAY_COLOR (color_t) {0, 0, 0, 255}              // ray color in 2d map

// 3D PLAYER CONSTANTS
#define PLAYER_ANGLE_DELTA 3.0f  // speed of player's view angle, in degrees
#define FOV 60                   // degrees
#define PLAYER_SPEED 0.01f       // player's movement speed, arbitrary measure unit

// GAME GRAPHICS CONSTANTS
#define GAME_WIDTH 800
#define GAME_HEIGHT 600
#define SCALE 700            // reverse proportionality constant, used in 3d rendering
                             // (700 is optimal value for your tiles to be similar to squares)
#define SHADE_CONSTANT 0.8f  // by what scalar should color be multiplied to achieve "shading" effect
#define THREE_D_CEILING_AND_FLOOR_COLOR (color_t) {95, 95, 95, 255}

// ERROR CODES
#define ERROR_CODE_SDL_INITIALIZATION 1
#define ERROR_CODE_SDL_CREATE_WINDOW 2
#define ERROR_CODE_SDL_CREATE_RENDERER 3

// OTHER
#define MAX_PPM_SIZE_BYTES 16384  // maximum size of ppm image used as a texture (in bytes!)
#define NUM_TEXTURES 1            // number of textures used in maps/textures.txt

#endif  // #ifndef CONSTANTS_H
