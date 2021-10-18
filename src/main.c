#include <gb/gb.h>
#include <gb/metasprites.h>

// uncomment the line below to switch on debug features
//#define DEBUG
#ifdef DEBUG
#include <stdio.h>
#endif
// SUBMAP-2 BRANCH
//  Include your scene and map header files
#include "../res/tiles/brick_wide_map.h"
#include "../res/tiles/brick_wide_tiles.h"
#include "../res/tiles/collision_wide_map.h"
#include "macros.h"
#include "scene.h"

UINT8 joy, last_joy;

UINT8 floorYposition;

const unsigned char blankmap[2] = {0x00, 0x01};
extern Variables bkg;

// CHECKS WHETHER OR NOT THE OFFSET OF PLAYER POSITION COLLIDES WITH A COLLISION TILE
// BOTTOM LEFT PIXEL
// UBYTE checkcollisionBL(UINT8 newplayerx, UINT8 newplayery) {
//     UINT16 indexBLx, indexBLy, indexCamX, tileindexBL;
//     UBYTE result;

//     indexBLx = (newplayerx - 17) / 8;
//     indexBLy = (newplayery - 1) / 8;
//     indexCamX = (bkg.camera_x) / 8;
//     tileindexBL = 40 * (indexBLy) + (indexBLx + indexCamX);

//     result = COLLISION_WIDE_MAP[tileindexBL] == blankmap[1];

//     return result;
// }

/******************************/
// Define your OBJ and BGP palettes, show SPRITES, turn on DISPLAY
/******************************/
void main() {
    DISPLAY_OFF;
    BGP_REG = 0xE4;
    OBP0_REG = 0xE4;
    OBP1_REG = 0xE1;
    SPRITES_8x16;
    SHOW_BKG;
    SHOW_SPRITES;

    // set_bkg_data(0, 3, BRICK_WIDE_TILES);
    // set_bkg_tiles(0, 0, BRICK_WIDE_MAPWidth, BRICK_WIDE_MAPHeight, BRICK_WIDE_MAP);

    // game assumes floor is at the level of 100px. that is a temporary workaround, should use collision maps instead
    floorYposition = 100;

    init_submap();

    // switch on display after everything is ready
    DISPLAY_ON;
    last_joy = joy = 0;
    while (TRUE) {  // main loop runs at 60fps
        // ---------------------------------------------
        // process joystic input
        last_joy = joy;
        joy = joypad();

        if (joy & J_LEFT) {
            if (bkg.camera_style == horizontal_cam) {
                if (bkg.camera_x) {
                    bkg.camera_x--;
                    bkg.redraw = TRUE;
                }
            }
            if (PLAYER.SpdX > -MAX_WALK_SPEED) {
                PLAYER.SpdX -= WALK_VELOCITY;
            } else
                PLAYER.SpdX = -MAX_WALK_SPEED;

        } else if (joy & J_RIGHT) {
            if (bkg.camera_style == horizontal_cam) {
                if (bkg.camera_x < bkg.camera_max_x) {
                    bkg.camera_x++;
                    bkg.redraw = TRUE;
                }
            }
            if (PLAYER.SpdX < MAX_WALK_SPEED) {
                PLAYER.SpdX += WALK_VELOCITY;
            } else
                PLAYER.SpdX = MAX_WALK_SPEED;
        }

        // else if ((CHANGED_BUTTONS & J_B) && (joy & J_B) && (Jump)) {
        // }

        // #ifdef DEBUG
        // DEBUG DETECTIVE Y COORDS
        if (joy & J_B) {
            printf("CamX=%u\n", bkg.camera_x);
        }
        // #endif

        // Load stages on button press
        // if (joy & J_START) {
        // } else if (joy & J_SELECT) {
        // }
        // no joy checking below this line
        // ---------------------------------------------

        // ---------------------------------------------
        // WORLD PHYSICS:
        // GRAVITY
        PLAYER.SpdY += GRAVITY;
        if (PLAYER.SpdY > MAX_FALL_SPEED) {
            PLAYER.SpdY = MAX_FALL_SPEED;
        }

        if (PLAYER.SpdX != 0) {
            if (PLAYER.SpdX < 0)
                PLAYER.SpdX += FRICTION;
            else
                PLAYER.SpdX -= FRICTION;
        }

        // update PLAYER absolute posiiton
        PLAYER.y += PLAYER.SpdY;
        PLAYER.x += PLAYER.SpdX;

        // call level animation hook (if any), that makes other actors move (and interact in future)
        if (animate_level) animate_level();

        // render all actors on screen
        render_actors();
        if (bkg.redraw) {
            wait_vbl_done();
            set_camera();
            bkg.redraw = FALSE;
        } else
            wait_vbl_done();
    }
}
