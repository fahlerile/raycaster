#pragma once

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

// Reverse proportionality constant for wall rendering
#define LINE_LENGTH_PROPORTION_CONSTANT 750

#define ROOF_COLOR ((Color) {100, 100, 100, 255})
#define FLOOR_COLOR ((Color) {100, 100, 100, 255})

#define RAY_TYPE_TO_SHADE RayTypeHorizontal
#define SHADE_CONSTANT 0.8

#define STEP_LENGTH 0.001
#define CAMERA_SENSITIVITY (M_PI / 1500)  // rads per pixel/keypress

