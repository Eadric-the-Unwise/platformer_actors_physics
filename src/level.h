// #ifndef CHARACTER_H //if its not defined, then proceed to line 2 and define it.
// #define CHARACTER_H
//pragma once is a stronger method to ifndef <-> endif
//it makes sure that the header file is only included once
#pragma once
#include <stdio.h>
#include <gb/drawing.h>

enum Cam
{
    vertical_cam,
    horizontal_cam,
    platforming_cam,
    scroll_cam
};

typedef struct Variables
{
    UINT16 camera_max_y;
    UINT16 camera_max_x;
    UBYTE camera_x_p; //in pixels
    UBYTE joy;

    UBYTE sliding;
    UBYTE fading;
    INT8 slide_x;
    INT8 slide_y;
    // current and old positions of the camera in pixels
    WORD camera_x, camera_y, old_camera_x, old_camera_y;
    // current and old position of the map in tiles
    UBYTE map_pos_x, map_pos_y, old_map_pos_x, old_map_pos_y;
    // redraw flag, indicates that camera position was changed
    UBYTE redraw;
    //below PROVIDED BY TOXA IN DISCORD //
    UINT8 level_map_width;
    UINT8 level_map_height;
    const UINT8 *level_map_data;
    enum Cam camera_style;

    UINT8 level_map_bank;

} Variables;