#include <gb/gb.h>
#include <gb/metasprites.h>

// uncomment the line below to switch on debug features
//#define DEBUG
#ifdef DEBUG
#include <stdio.h>
#endif

// Include your scene and map header files
#include "../res/tiles/brick_map.h"
#include "../res/tiles/brick_tiles.h"
#include "../res/tiles/collision_map.h"
#include "level1.h"
#include "level2.h"
#include "scene.h"

UINT8 joy, last_joy;

UINT8 floorYposition;
UINT8 Jump, Launch, Shooting;
UBYTE launchDelay = 0;
UBYTE shooting_counter = 0;
const unsigned char blankmap[1] = {0x00};

UBYTE canplayermoveBL(UINT8 newplayerx, UINT8 newplayery);
UBYTE canplayermoveBR(UINT8 newplayerx, UINT8 newplayery);
//bottom left pixel
UBYTE canplayermoveBL(UINT8 newplayerx, UINT8 newplayery) {
    UINT16 indexBLx, indexBLy, tileindexBL;
    UBYTE result;

    indexBLx = (newplayerx - 17) / 8;
    indexBLy = (newplayery) / 8;
    tileindexBL = 20 * indexBLy + indexBLx;

    result = COLLISION_MAP[tileindexBL] == blankmap[0];

    return result;
}
//bottom right pixel
UBYTE canplayermoveBR(UINT8 newplayerx, UINT8 newplayery) {
    UINT16 indexBRx, indexBRy, tileindexBR;
    UBYTE result;

    indexBRx = (newplayerx + 2) / 8;
    indexBRy = (newplayery) / 8;
    tileindexBR = 20 * indexBRy + indexBRx;

    result = COLLISION_MAP[tileindexBR] == blankmap[0];

    return result;
}

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

    set_bkg_data(0, 3, BRICK_TILES);
    set_bkg_tiles(0, 0, BRICK_MAPWidth, BRICK_MAPHeight, BRICK_MAP);

    // game assumes floor is at the level of 100px. that is a temporary workaround, should use collision maps instead
    floorYposition = 100;
    Jump = FALSE;
    Launch = FALSE;
    Shooting = FALSE;

    load_level(&level1);

    // switch on display after everything is ready
    DISPLAY_ON;
    last_joy = joy = 0;
    while (TRUE) {  //main loop runs at 60fps
        // ---------------------------------------------
        // process joystic input
        last_joy = joy;
        joy = joypad();

        if ((joy & J_LEFT) && (!Shooting)) {
            Launch = FALSE;
            launchDelay = 0;
            if (!(joy & (J_DOWN))) {
                if (PLAYER.SpdX > -MAX_WALK_SPEED)
                    PLAYER.SpdX -= WALK_VELOCITY;
                else
                    PLAYER.SpdX = -MAX_WALK_SPEED;
                if (!Jump) {
                    SetActorDirection(&PLAYER, DIR_LEFT, PLAYER.animation_phase);
                }
            } else if (joy & (J_DOWN)) {
                if (PLAYER.SpdX > -MAX_CRAWL_SPEED)
                    PLAYER.SpdX -= WALK_VELOCITY;
                else
                    PLAYER.SpdX = -MAX_CRAWL_SPEED;
                if (!Jump) {
                    SetActorDirection(&PLAYER, DIR_CRAWL_L, 0);
                }
            }
        } else if (joy & J_RIGHT) {
            Launch = FALSE;
            launchDelay = 0;
            if (!(joy & (J_DOWN))) {
                if (PLAYER.SpdX < MAX_WALK_SPEED)
                    PLAYER.SpdX += WALK_VELOCITY;
                else
                    PLAYER.SpdX = MAX_WALK_SPEED;
                if (!Jump) {
                    SetActorDirection(&PLAYER, DIR_RIGHT, PLAYER.animation_phase);
                }
            } else if (joy & (J_DOWN)) {
                if (PLAYER.SpdX < MAX_CRAWL_SPEED)
                    PLAYER.SpdX += WALK_VELOCITY;
                else
                    PLAYER.SpdX = MAX_CRAWL_SPEED;
                if (!Jump) {
                    SetActorDirection(&PLAYER, DIR_CRAWL_R, 0);
                }
            }
        }
        //DOWN while standing still
        if ((joy & J_DOWN) && !(joy & J_LEFT) && !(joy & J_RIGHT) && (!Jump)) {
            if (!Launch) {
                launchDelay++;
            }
            if (launchDelay == 50) {
                Launch = TRUE;
                launchDelay = 0;
            }
            switch (PLAYER.direction) {
                case DIR_LEFT:
                    SetActorDirection(&PLAYER, DIR_DOWN_L, 0);
                    break;
                case DIR_IDLE_L:
                    SetActorDirection(&PLAYER, DIR_DOWN_L, 0);
                    break;
                case DIR_DOWN_L:
                    SetActorDirection(&PLAYER, DIR_DOWN_L, 0);
                    break;
                case DIR_CRAWL_L:
                    SetActorDirection(&PLAYER, DIR_DOWN_L, 0);
                    break;
                case DIR_RIGHT:
                    SetActorDirection(&PLAYER, DIR_DOWN_R, 0);
                    break;
                case DIR_IDLE_R:
                    SetActorDirection(&PLAYER, DIR_DOWN_R, 0);
                    break;
                case DIR_DOWN_R:
                    SetActorDirection(&PLAYER, DIR_DOWN_R, 0);
                    break;
                case DIR_CRAWL_R:
                    SetActorDirection(&PLAYER, DIR_DOWN_R, 0);
                    break;
            }
        }
        //Launch
        if (Launch) {
            OBP0_REG = 0xE1;
            OBP1_REG = 0xE1;
            if (!(joy & J_DOWN)) {
                OBP0_REG = 0xE4;
                PLAYER.SpdY = LAUNCH_IMPULSE;
                Jump = TRUE;
                Launch = FALSE;
                if (PLAYER.direction == DIR_DOWN_L) {
                    SetActorDirection(&PLAYER, DIR_JUMP_L, 0);
                } else if (PLAYER.direction == DIR_DOWN_R) {
                    SetActorDirection(&PLAYER, DIR_JUMP_R, 0);
                }
            }
        }
        if (Shooting) {
            shooting_counter--;
            if (shooting_counter <= 0) {
                Shooting = FALSE;
            }
        }

        if ((CHANGED_BUTTONS & J_A) && (joy & J_A)) {
            Launch = FALSE;
            launchDelay = 0;
            if (!Jump) {
                if (joy & J_LEFT) {
                    SetActorDirection(&PLAYER, DIR_JUMP_L, 0);
                } else if (joy & J_RIGHT) {
                    SetActorDirection(&PLAYER, DIR_JUMP_R, 0);
                } else {
                    switch (PLAYER.direction) {
                        case DIR_IDLE_L:
                            SetActorDirection(&PLAYER, DIR_JUMP_L, 0);
                            break;
                        case DIR_IDLE_R:
                            SetActorDirection(&PLAYER, DIR_JUMP_R, 0);
                            break;
                        case DIR_LEFT:
                            SetActorDirection(&PLAYER, DIR_JUMP_L, 0);
                            break;
                        case DIR_RIGHT:
                            SetActorDirection(&PLAYER, DIR_JUMP_R, 0);
                            break;
                        case DIR_DOWN_L:
                            SetActorDirection(&PLAYER, DIR_JUMP_L, 0);
                            break;
                        case DIR_DOWN_R:
                            SetActorDirection(&PLAYER, DIR_JUMP_R, 0);
                            break;
                    }
                }
                PLAYER.SpdY = JUMP_IMPULSE;
                Jump = TRUE;
            }
        }
        if ((CHANGED_BUTTONS & J_B) && (joy & J_B) && (!Jump)) {
            Shooting = TRUE;
            shooting_counter = 20;
        }
        // else if ((CHANGED_BUTTONS & J_B) && (joy & J_B) && (Jump)) {
        // }

#ifdef DEBUG
        //DEBUG DETECTIVE Y COORDS
        if (joy & J_B) {
            printf("Y=%u\n", TO_PIXELS(PLAYER.y));
        }
#endif

        // Load stages on button press
        if (joy & J_START) {
            load_level(&level2);
        } else if (joy & J_SELECT) {
            load_level(&level1);
        }
        // no joy checking below this line
        // ---------------------------------------------

        // ---------------------------------------------
        // WORLD PHYSICS:
        // GRAVITY
        PLAYER.SpdY += GRAVITY;
        if ((canplayermoveBL(TO_PIXELS(PLAYER.x), TO_PIXELS(PLAYER.y) + 1))) {
            if (PLAYER.SpdY > MAX_FALL_SPEED) PLAYER.SpdY = MAX_FALL_SPEED;
        } else {  //if you touch the floor
            if (PLAYER.SpdY > 0) {
                PLAYER.SpdY = 0;
                SetActorDirection(&PLAYER, PLAYER.direction, 5);
                Jump = FALSE;
                //set player idle direction when touching ground
                if (PLAYER.direction == DIR_JUMP_R) {
                    SetActorDirection(&PLAYER, DIR_IDLE_R, 0);
                } else if (PLAYER.direction == DIR_JUMP_L) {
                    SetActorDirection(&PLAYER, DIR_IDLE_L, 0);
                }
            }
        }

        // if (TO_PIXELS(PLAYER.y) < floorYposition) {  //if you are above the floorYposition

        // if (PLAYER.y > TO_COORDS(floorYposition)) {
        //     PLAYER.y = TO_COORDS(floorYposition);  // if we "sunk into the ground" because of high speed, then float up
        // }
        // FRICTION
        // THIS IS CURRENTLY THE MINIMUM AMOUNT FRICTION. REDUCE FURTHER BY ADDING A PHYSICS FRAME COUNTER
        if (PLAYER.SpdX != 0) {
            if (PLAYER.SpdX < 0)
                PLAYER.SpdX += FRICTION;
            else
                PLAYER.SpdX -= FRICTION;
        }

        //TURN DIRECTION MIDAIR
        if (Jump) {
            if (PLAYER.direction == DIR_JUMP_L) {
                if (joy & J_RIGHT) {
                    SetActorDirection(&PLAYER, DIR_JUMP_R, PLAYER.animation_phase);
                }
            }
            if (PLAYER.direction == DIR_JUMP_R) {
                if (joy & J_LEFT) {
                    SetActorDirection(&PLAYER, DIR_JUMP_L, PLAYER.animation_phase);
                }
            }
        }
        // Change to IDLE state when not moving
        if ((!Jump) && !(joy & J_DOWN)) {
            if ((PLAYER.SpdX == 0) && (PLAYER.SpdY == 0)) {
                switch (PLAYER.last_direction) {
                    case DIR_LEFT:
                        if ((PLAYER.direction == DIR_RIGHT) || (PLAYER.direction == DIR_JUMP_R) || (PLAYER.direction == DIR_CRAWL_R)) {
                            SetActorDirection(&PLAYER, DIR_IDLE_R, 0);
                        } else {
                            SetActorDirection(&PLAYER, DIR_IDLE_L, 0);
                        }
                        break;
                    case DIR_IDLE_L:
                        if ((PLAYER.direction == DIR_RIGHT) || (PLAYER.direction == DIR_JUMP_R) || (PLAYER.direction == DIR_CRAWL_R)) {
                            SetActorDirection(&PLAYER, DIR_IDLE_R, 0);
                        } else {
                            SetActorDirection(&PLAYER, DIR_IDLE_L, 0);
                        }
                        break;
                    case DIR_JUMP_L:
                        if ((PLAYER.direction == DIR_RIGHT) || (PLAYER.direction == DIR_JUMP_R) || (PLAYER.direction == DIR_CRAWL_R)) {
                            SetActorDirection(&PLAYER, DIR_IDLE_R, 0);
                        } else {
                            SetActorDirection(&PLAYER, DIR_IDLE_L, 0);
                        }
                        break;
                    case DIR_DOWN_L:
                        if ((PLAYER.direction == DIR_RIGHT) || (PLAYER.direction == DIR_JUMP_R) || (PLAYER.direction == DIR_CRAWL_R)) {
                            SetActorDirection(&PLAYER, DIR_IDLE_R, 0);
                        } else {
                            SetActorDirection(&PLAYER, DIR_IDLE_L, 0);
                        }
                        break;
                    case DIR_CRAWL_L:
                        if ((PLAYER.direction == DIR_RIGHT) || (PLAYER.direction == DIR_JUMP_R) || (PLAYER.direction == DIR_CRAWL_R)) {
                            SetActorDirection(&PLAYER, DIR_IDLE_R, 0);
                        } else {
                            SetActorDirection(&PLAYER, DIR_IDLE_L, 0);
                        }
                        break;

                    case DIR_RIGHT:
                        if ((PLAYER.direction == DIR_LEFT) || (PLAYER.direction == DIR_JUMP_L) || (PLAYER.direction == DIR_CRAWL_L)) {
                            SetActorDirection(&PLAYER, DIR_IDLE_L, 0);
                        } else {
                            SetActorDirection(&PLAYER, DIR_IDLE_R, 0);
                        }
                        break;
                    case DIR_IDLE_R:
                        if ((PLAYER.direction == DIR_LEFT) || (PLAYER.direction == DIR_JUMP_L) || (PLAYER.direction == DIR_CRAWL_L)) {
                            SetActorDirection(&PLAYER, DIR_IDLE_L, 0);
                        } else {
                            SetActorDirection(&PLAYER, DIR_IDLE_R, 0);
                        }
                        break;
                    case DIR_JUMP_R:
                        if ((PLAYER.direction == DIR_LEFT) || (PLAYER.direction == DIR_JUMP_L) || (PLAYER.direction == DIR_CRAWL_L)) {
                            SetActorDirection(&PLAYER, DIR_IDLE_L, 0);
                        } else {
                            SetActorDirection(&PLAYER, DIR_IDLE_R, 0);
                        }
                        break;
                    case DIR_DOWN_R:
                        if ((PLAYER.direction == DIR_LEFT) || (PLAYER.direction == DIR_JUMP_L) || (PLAYER.direction == DIR_CRAWL_L)) {
                            SetActorDirection(&PLAYER, DIR_IDLE_L, 0);
                        } else {
                            SetActorDirection(&PLAYER, DIR_IDLE_R, 0);
                        }
                        break;
                    case DIR_CRAWL_R:
                        if ((PLAYER.direction == DIR_LEFT) || (PLAYER.direction == DIR_JUMP_L) || (PLAYER.direction == DIR_CRAWL_L)) {
                            SetActorDirection(&PLAYER, DIR_IDLE_L, 0);
                        } else {
                            SetActorDirection(&PLAYER, DIR_IDLE_R, 0);
                        }
                        break;

                        // default:
                        //     SetActorDirection(&PLAYER, DIR_IDLE_L, 0);
                        //     break;
                }
            }
        }

        // SWITCH HERE ^

        // update PLAYER absolute posiiton
        PLAYER.y += PLAYER.SpdY;
        PLAYER.x += PLAYER.SpdX;

        // call level animation hook (if any), that makes other actors move (and interact in future)
        if (animate_level) animate_level();

        // render all actors on screen
        render_actors();

        // wait for VBlank
        wait_vbl_done();
    }
}
