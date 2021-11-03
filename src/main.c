#include <gb/gb.h>
#include <gb/metasprites.h>

// uncomment the line below to switch on debug features
//#define DEBUG
#ifdef DEBUG
#include <stdio.h>
#endif
// SUBMAP-3 BRANCH
//  Include your scene and map header files
#include "../res/tiles/brick_wide_map.h"
#include "../res/tiles/brick_wide_tiles.h"
#include "../res/tiles/collision_wide_map.h"
#include "level1.h"
#include "level2.h"
#include "macros.h"
#include "scene.h"

UINT8 joy, last_joy;

UINT8 floorYposition;
UINT8 Jump, Crouch, Launch, Shooting;
UBYTE launchDelay = 0;
UBYTE shooting_counter = 0;
const unsigned char blankmap[2] = {0x00, 0x01};
extern Variables bkg;

//CHECKS WHETHER OR NOT THE OFFSET OF PLAYER POSITION COLLIDES WITH A COLLISION TILE
//BOTTOM LEFT PIXEL
UBYTE checkcollisionBL(UINT8 newplayerx, UINT8 newplayery, INT16 camera_x) {
    UINT16 indexBLx, indexBLy, indexCamx, tileindexBL;
    UBYTE result;

    indexCamx = camera_x;
    indexBLx = ((newplayerx - 17) + indexCamx) / 8;
    indexBLy = (newplayery - 1) / 8;

    tileindexBL = COLLISION_WIDE_MAPWidth * indexBLy + indexBLx;  //MULTIPLY THE WIDTH BY THE Y TILE TO FIND THE Y ROW. THEN ADD THE X TILE TO SHIFT THE COLUMN. FINDS THE TILE YOU'RE LOOKING FOR

    result = COLLISION_WIDE_MAP[tileindexBL] == blankmap[1];

    return result;
}
//BOTTOM RIGHT PIXEL
UBYTE checkcollisionBR(UINT8 newplayerx, UINT8 newplayery, INT16 camera_x) {
    UINT16 indexBLx, indexBLy, indexCamx, tileindexBL;
    UBYTE result;

    indexCamx = camera_x;
    indexBLx = ((newplayerx) + indexCamx) / 8;
    indexBLy = (newplayery - 1) / 8;

    tileindexBL = COLLISION_WIDE_MAPWidth * indexBLy + indexBLx;  //MULTIPLY THE WIDTH BY THE Y TILE TO FIND THE Y ROW. THEN ADD THE X TILE TO SHIFT THE COLUMN. FINDS THE TILE YOU'RE LOOKING FOR

    result = COLLISION_WIDE_MAP[tileindexBL] == blankmap[1];

    return result;
}
//BOTTOM CENTER PIXEL
UBYTE checkcollisionBC(UINT8 newplayerx, UINT8 newplayery, INT16 camera_x) {
    UINT16 indexBLx, indexBLy, indexCamx, tileindexBL;
    UBYTE result;

    indexCamx = camera_x;
    indexBLx = ((newplayerx - 9) + indexCamx) / 8;
    indexBLy = (newplayery - 1) / 8;

    tileindexBL = COLLISION_WIDE_MAPWidth * indexBLy + indexBLx;  //MULTIPLY THE WIDTH BY THE Y TILE TO FIND THE Y ROW. THEN ADD THE X TILE TO SHIFT THE COLUMN. FINDS THE TILE YOU'RE LOOKING FOR

    result = COLLISION_WIDE_MAP[tileindexBL] == blankmap[1];

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

    Jump = Crouch = Launch = Shooting = FALSE;

    init_submap();
    load_level(&level1);

    // switch on display after everything is ready
    DISPLAY_ON;
    last_joy = joy = 0;
    while (TRUE) {  // main loop runs at 60fps
        // ---------------------------------------------
        // process joystic input
        last_joy = joy;
        joy = joypad();

        if (joy & J_LEFT) {
            if ((!Jump) && !(joy & (J_DOWN)) && !(Crouch)) {
                SetActorDirection(&PLAYER, DIR_LEFT, PLAYER.animation_phase);
            } else if (Crouch) {
                if (!Jump) {
                    SetActorDirection(&PLAYER, DIR_CRAWL_L, 0);
                }
            }
            if (Crouch) {
                if (PLAYER.SpdX > -MAX_CRAWL_SPEED) {
                    PLAYER.SpdX -= WALK_VELOCITY;
                } else
                    PLAYER.SpdX = -MAX_CRAWL_SPEED;
            } else if (!(Crouch)) {
                if (PLAYER.SpdX > -MAX_WALK_SPEED) {
                    PLAYER.SpdX -= WALK_VELOCITY;
                } else
                    PLAYER.SpdX = -MAX_WALK_SPEED;
            }
        } else if (joy & J_RIGHT) {
            if ((!Jump) && !(joy & (J_DOWN)) && !(Crouch)) {
                SetActorDirection(&PLAYER, DIR_RIGHT, PLAYER.animation_phase);
            } else if (Crouch) {
                if (!Jump) {
                    SetActorDirection(&PLAYER, DIR_CRAWL_R, 0);
                }
            }
            if (Crouch) {
                if (PLAYER.SpdX < MAX_CRAWL_SPEED) {
                    PLAYER.SpdX += WALK_VELOCITY;
                } else
                    PLAYER.SpdX = MAX_CRAWL_SPEED;
            } else if (!(Crouch)) {
                if (PLAYER.SpdX < MAX_WALK_SPEED) {
                    PLAYER.SpdX += WALK_VELOCITY;
                } else
                    PLAYER.SpdX = MAX_WALK_SPEED;
            }
        }
        if ((joy & J_DOWN) && !(Jump)) {
            Crouch = TRUE;
        }
        // DOWN while standing still
        // if ((Crouch) && (!(joy & J_LEFT) && !(joy & J_RIGHT)) && (!Jump)) {
        //     switch (PLAYER.direction) {
        //         case DIR_LEFT:
        //             SetActorDirection(&PLAYER, DIR_DOWN_L, 0);
        //             break;
        //         case DIR_IDLE_L:
        //             SetActorDirection(&PLAYER, DIR_DOWN_L, 0);
        //             break;
        //         case DIR_DOWN_L:
        //             SetActorDirection(&PLAYER, DIR_DOWN_L, 0);
        //             break;
        //         case DIR_CRAWL_L:
        //             SetActorDirection(&PLAYER, DIR_DOWN_L, 0);
        //             break;
        //         case DIR_RIGHT:
        //             SetActorDirection(&PLAYER, DIR_DOWN_R, 0);
        //             break;
        //         case DIR_IDLE_R:
        //             SetActorDirection(&PLAYER, DIR_DOWN_R, 0);
        //             break;
        //         case DIR_DOWN_R:
        //             SetActorDirection(&PLAYER, DIR_DOWN_R, 0);
        //             break;
        //         case DIR_CRAWL_R:
        //             SetActorDirection(&PLAYER, DIR_DOWN_R, 0);
        //             break;
        //     }
        // }

        //IF PLAYER IS FREE FALLING FOR ANY REASON
        // if (PLAYER.SpdY != 0) {
        //     Jump = TRUE;
        //     Crouch = FALSE;
        //     switch (PLAYER.direction) {
        //         case DIR_IDLE_L:
        //             SetActorDirection(&PLAYER, DIR_JUMP_L, 0);
        //             break;
        //         case DIR_IDLE_R:
        //             SetActorDirection(&PLAYER, DIR_JUMP_R, 0);
        //             break;
        //         case DIR_LEFT:
        //             SetActorDirection(&PLAYER, DIR_JUMP_L, 0);
        //             break;
        //         case DIR_RIGHT:
        //             SetActorDirection(&PLAYER, DIR_JUMP_R, 0);
        //             break;
        //         case DIR_DOWN_L:
        //             SetActorDirection(&PLAYER, DIR_JUMP_L, 0);
        //             break;
        //         case DIR_DOWN_R:
        //             SetActorDirection(&PLAYER, DIR_JUMP_R, 0);
        //             break;
        //         case DIR_CRAWL_L:
        //             SetActorDirection(&PLAYER, DIR_JUMP_L, 0);
        //             break;
        //         case DIR_CRAWL_R:
        //             SetActorDirection(&PLAYER, DIR_JUMP_R, 0);
        //             break;
        //     }
        // }

        if ((CHANGED_BUTTONS & J_A) && (joy & J_A)) {
            Crouch = Launch = FALSE;
            if (!Jump) {
                // switch (PLAYER.direction) {
                //     case DIR_IDLE_L:
                //         SetActorDirection(&PLAYER, DIR_JUMP_L, 0);
                //         break;
                //     case DIR_IDLE_R:
                //         SetActorDirection(&PLAYER, DIR_JUMP_R, 0);
                //         break;
                //     case DIR_LEFT:
                //         SetActorDirection(&PLAYER, DIR_JUMP_L, 0);
                //         break;
                //     case DIR_RIGHT:
                //         SetActorDirection(&PLAYER, DIR_JUMP_R, 0);
                //         break;
                //     case DIR_DOWN_L:
                //         SetActorDirection(&PLAYER, DIR_JUMP_L, 0);
                //         break;
                //     case DIR_DOWN_R:
                //         SetActorDirection(&PLAYER, DIR_JUMP_R, 0);
                //         break;
                //     case DIR_CRAWL_L:
                //         SetActorDirection(&PLAYER, DIR_JUMP_L, 0);
                //         break;
                //     case DIR_CRAWL_R:
                //         SetActorDirection(&PLAYER, DIR_JUMP_R, 0);
                //         break;
                // }
                // }
                PLAYER.SpdY = JUMP_IMPULSE;
                Jump = TRUE;
            }
        }

        // ---------------------------------------------
        // ---------------------------------------------
        // WORLD PHYSICS:
        // GRAVITY

        PLAYER.SpdY += GRAVITY;
        if (PLAYER.SpdY > MAX_FALL_SPEED) {
            PLAYER.SpdY = MAX_FALL_SPEED;
        }

        if (PLAYER.SpdX < 0) {
            if (PLAYER.SpdX != -MAX_WALK_SPEED) {
                PLAYER.SpdX += FRICTION;
            } else if ((PLAYER.SpdX <= -MAX_WALK_SPEED) && !(joy & J_LEFT)) {
                PLAYER.SpdX += FRICTION;
            }
        }
        if (PLAYER.SpdX > 0) {
            if (PLAYER.SpdX != MAX_WALK_SPEED) {
                PLAYER.SpdX -= FRICTION;
            } else if ((PLAYER.SpdX >= MAX_WALK_SPEED) && !(joy & J_RIGHT)) {
                PLAYER.SpdX -= FRICTION;
            }
        }

        //Y-AXIS COLLISION CHECK (ADD NEGATIVE AND POSITIVE IFS SO THE LOOP ONLY CHECKS 1 FOR Y AND 1 FOR X MOVEMENT)
        if (PLAYER.SpdY > 0) {
            if ((checkcollisionBL(TO_PIXELS(PLAYER.x), TO_PIXELS(PLAYER.y) + 1, TO_PIXELS(bkg.camera_x))) || (checkcollisionBR(TO_PIXELS(PLAYER.x), TO_PIXELS(PLAYER.y) + 1, TO_PIXELS(bkg.camera_x))) || (checkcollisionBC(TO_PIXELS(PLAYER.x), TO_PIXELS(PLAYER.y) + 1, TO_PIXELS(bkg.camera_x)))) {
                UBYTE ty = (TO_PIXELS(PLAYER.y) / 8);
                PLAYER.y = TO_COORDS(ty * 8);
                PLAYER.SpdY = 0;
                Jump = FALSE;
                //set player idle direction when touching ground
                // if (PLAYER.direction == DIR_JUMP_R) {
                //     SetActorDirection(&PLAYER, DIR_IDLE_R, 0);
                // } else if (PLAYER.direction == DIR_JUMP_L) {
                //     SetActorDirection(&PLAYER, DIR_IDLE_L, 0);
                // }
            }
        } else if (PLAYER.SpdY < 0) {
            if ((checkcollisionBL(TO_PIXELS(PLAYER.x), TO_PIXELS(PLAYER.y) - 25, TO_PIXELS(bkg.camera_x))) || (checkcollisionBR(TO_PIXELS(PLAYER.x), TO_PIXELS(PLAYER.y) - 25, TO_PIXELS(bkg.camera_x))) || (checkcollisionBC(TO_PIXELS(PLAYER.x), TO_PIXELS(PLAYER.y) - 25, TO_PIXELS(bkg.camera_x)))) {
                UBYTE ty = (TO_PIXELS(PLAYER.y) / 8);
                PLAYER.y = TO_COORDS(ty * 8);
                PLAYER.SpdY = 0;
                Jump = FALSE;
                //set player idle direction when touching ground
                // if (PLAYER.direction == DIR_JUMP_R) {
                //     SetActorDirection(&PLAYER, DIR_IDLE_R, 0);
                // } else if (PLAYER.direction == DIR_JUMP_L) {
                //     SetActorDirection(&PLAYER, DIR_IDLE_L, 0);
                // }
            }
        }
        if (PLAYER.SpdX > 0) {
            if (Crouch) {
                if ((checkcollisionBR(TO_PIXELS(PLAYER.x) + 1, TO_PIXELS(PLAYER.y), TO_PIXELS(bkg.camera_x))) || (checkcollisionBR(TO_PIXELS(PLAYER.x) + 1, TO_PIXELS(PLAYER.y) - 15, TO_PIXELS(bkg.camera_x)))) {
                    // UBYTE tx = ((TO_PIXELS(PLAYER.x) + 1) / 8);
                    // PLAYER.x = TO_COORDS((tx * 8) - 1);
                    PLAYER.SpdX = 0;
                    //set player idle direction when touching ground
                    // if (!Jump) {
                    //     if (PLAYER.direction == DIR_JUMP_R) {
                    //         SetActorDirection(&PLAYER, DIR_IDLE_R, 0);
                    //     } else if (PLAYER.direction == DIR_JUMP_L) {
                    //         SetActorDirection(&PLAYER, DIR_IDLE_L, 0);
                    //     }
                    //     if (PLAYER.direction == DIR_CRAWL_R) {
                    //         SetActorDirection(&PLAYER, DIR_DOWN_R, 0);
                    //     } else if (PLAYER.direction == DIR_CRAWL_L) {
                    //         SetActorDirection(&PLAYER, DIR_DOWN_L, 0);
                    //     }
                    // }
                }

            } else {
                if ((checkcollisionBR(TO_PIXELS(PLAYER.x) + 1, TO_PIXELS(PLAYER.y), TO_PIXELS(bkg.camera_x))) || (checkcollisionBR(TO_PIXELS(PLAYER.x) + 1, TO_PIXELS(PLAYER.y) - 12, TO_PIXELS(bkg.camera_x))) || (checkcollisionBR(TO_PIXELS(PLAYER.x), TO_PIXELS(PLAYER.y) - 23, TO_PIXELS(bkg.camera_x)))) {
                    // UBYTE tx = ((TO_PIXELS(PLAYER.x) + 1) / 8);
                    // PLAYER.x = TO_COORDS((tx * 8) - 1);
                    PLAYER.SpdX = 0;
                    //set player idle direction when touching ground
                    // if (!Jump) {
                    //     if (PLAYER.direction == DIR_JUMP_R) {
                    //         SetActorDirection(&PLAYER, DIR_IDLE_R, 0);
                    //     } else if (PLAYER.direction == DIR_JUMP_L) {
                    //         SetActorDirection(&PLAYER, DIR_IDLE_L, 0);
                    //     }
                    // }
                }
            }
        } else if (PLAYER.SpdX < 0) {
            if (Crouch) {
                if ((checkcollisionBL(TO_PIXELS(PLAYER.x) - 1, TO_PIXELS(PLAYER.y), TO_PIXELS(bkg.camera_x))) || (checkcollisionBL(TO_PIXELS(PLAYER.x) - 1, TO_PIXELS(PLAYER.y) - 15, TO_PIXELS(bkg.camera_x)))) {
                    PLAYER.SpdX = 0;
                    //set player idle direction when touching ground
                    // if (!Jump) {
                    //     if (PLAYER.direction == DIR_JUMP_R) {
                    //         SetActorDirection(&PLAYER, DIR_IDLE_R, 0);
                    //     } else if (PLAYER.direction == DIR_JUMP_L) {
                    //         SetActorDirection(&PLAYER, DIR_IDLE_L, 0);
                    //     }
                    //     if (PLAYER.direction == DIR_CRAWL_R) {
                    //         SetActorDirection(&PLAYER, DIR_DOWN_R, 0);
                    //     } else if (PLAYER.direction == DIR_CRAWL_L) {
                    //         SetActorDirection(&PLAYER, DIR_DOWN_L, 0);
                    //     }
                    // }
                }
            } else {
                if ((checkcollisionBL(TO_PIXELS(PLAYER.x) - 1, TO_PIXELS(PLAYER.y), TO_PIXELS(bkg.camera_x))) || (checkcollisionBL(TO_PIXELS(PLAYER.x) - 1, TO_PIXELS(PLAYER.y) - 15, TO_PIXELS(bkg.camera_x))) || (checkcollisionBL(TO_PIXELS(PLAYER.x), TO_PIXELS(PLAYER.y) - 23, TO_PIXELS(bkg.camera_x)))) {
                    PLAYER.SpdX = 0;
                    //set player idle direction when touching ground
                    // if (!Jump) {
                    //     if (PLAYER.direction == DIR_JUMP_R) {
                    //         SetActorDirection(&PLAYER, DIR_IDLE_R, 0);
                    //     } else if (PLAYER.direction == DIR_JUMP_L) {
                    //         SetActorDirection(&PLAYER, DIR_IDLE_L, 0);
                    //     }
                    // }
                }
            }
        }

        if ((Crouch) && !(joy & J_DOWN)) {
            if (joy & J_LEFT) {
                if ((checkcollisionBL(TO_PIXELS(PLAYER.x), TO_PIXELS(PLAYER.y), TO_PIXELS(bkg.camera_x))) || (checkcollisionBL(TO_PIXELS(PLAYER.x), TO_PIXELS(PLAYER.y) - 15, TO_PIXELS(bkg.camera_x))) || (checkcollisionBL(TO_PIXELS(PLAYER.x), TO_PIXELS(PLAYER.y) - 23, TO_PIXELS(bkg.camera_x))) || (checkcollisionBC(TO_PIXELS(PLAYER.x) + 5, TO_PIXELS(PLAYER.y) - 23, TO_PIXELS(bkg.camera_x)))) {
                } else
                    Crouch = FALSE;
            } else if (joy & J_RIGHT) {
                if ((checkcollisionBR(TO_PIXELS(PLAYER.x), TO_PIXELS(PLAYER.y), TO_PIXELS(bkg.camera_x))) || (checkcollisionBR(TO_PIXELS(PLAYER.x), TO_PIXELS(PLAYER.y) - 15, TO_PIXELS(bkg.camera_x))) || (checkcollisionBR(TO_PIXELS(PLAYER.x), TO_PIXELS(PLAYER.y) - 23, TO_PIXELS(bkg.camera_x))) || (checkcollisionBC(TO_PIXELS(PLAYER.x) - 5, TO_PIXELS(PLAYER.y) - 23, TO_PIXELS(bkg.camera_x)))) {
                } else
                    Crouch = FALSE;
            }
            if (!(joy & J_LEFT) && !(joy & J_RIGHT)) {
                if ((checkcollisionBL(TO_PIXELS(PLAYER.x) + 1, TO_PIXELS(PLAYER.y) - 23, TO_PIXELS(bkg.camera_x))) || (checkcollisionBR(TO_PIXELS(PLAYER.x) - 1, TO_PIXELS(PLAYER.y) - 23, TO_PIXELS(bkg.camera_x))) || (checkcollisionBC(TO_PIXELS(PLAYER.x), TO_PIXELS(PLAYER.y) - 23, TO_PIXELS(bkg.camera_x)))) {
                } else
                    Crouch = FALSE;
            }
        }

        // Change to IDLE state when not moving
        // if ((!Jump) && !(joy & J_DOWN) && !(Crouch)) {
        // if ((PLAYER.SpdX == 0) && (PLAYER.SpdY == 0)) {
        // switch (PLAYER.last_direction) {
        //     case DIR_LEFT:
        //         if ((PLAYER.direction == DIR_RIGHT) || (PLAYER.direction == DIR_JUMP_R) || (PLAYER.direction == DIR_CRAWL_R)) {
        //             SetActorDirection(&PLAYER, DIR_IDLE_R, 0);
        //         } else {
        //             SetActorDirection(&PLAYER, DIR_IDLE_L, 0);
        //         }
        //         break;
        //     case DIR_IDLE_L:
        //         if ((PLAYER.direction == DIR_RIGHT) || (PLAYER.direction == DIR_JUMP_R) || (PLAYER.direction == DIR_CRAWL_R)) {
        //             SetActorDirection(&PLAYER, DIR_IDLE_R, 0);
        //         } else {
        //             SetActorDirection(&PLAYER, DIR_IDLE_L, 0);
        //         }
        //         break;
        //     case DIR_JUMP_L:
        //         if ((PLAYER.direction == DIR_RIGHT) || (PLAYER.direction == DIR_JUMP_R) || (PLAYER.direction == DIR_CRAWL_R)) {
        //             SetActorDirection(&PLAYER, DIR_IDLE_R, 0);
        //         } else {
        //             SetActorDirection(&PLAYER, DIR_IDLE_L, 0);
        //         }
        //         break;
        //     case DIR_DOWN_L:
        //         if ((PLAYER.direction == DIR_RIGHT) || (PLAYER.direction == DIR_JUMP_R) || (PLAYER.direction == DIR_CRAWL_R)) {
        //             SetActorDirection(&PLAYER, DIR_IDLE_R, 0);
        //         } else {
        //             SetActorDirection(&PLAYER, DIR_IDLE_L, 0);
        //         }
        //         break;
        //     case DIR_CRAWL_L:
        //         if ((PLAYER.direction == DIR_RIGHT) || (PLAYER.direction == DIR_JUMP_R) || (PLAYER.direction == DIR_CRAWL_R)) {
        //             SetActorDirection(&PLAYER, DIR_IDLE_R, 0);
        //         } else {
        //             SetActorDirection(&PLAYER, DIR_IDLE_L, 0);
        //         }
        //         break;

        //     case DIR_RIGHT:
        //         if ((PLAYER.direction == DIR_LEFT) || (PLAYER.direction == DIR_JUMP_L) || (PLAYER.direction == DIR_CRAWL_L)) {
        //             SetActorDirection(&PLAYER, DIR_IDLE_L, 0);
        //         } else {
        //             SetActorDirection(&PLAYER, DIR_IDLE_R, 0);
        //         }
        //         break;
        //     case DIR_IDLE_R:
        //         if ((PLAYER.direction == DIR_LEFT) || (PLAYER.direction == DIR_JUMP_L) || (PLAYER.direction == DIR_CRAWL_L)) {
        //             SetActorDirection(&PLAYER, DIR_IDLE_L, 0);
        //         } else {
        //             SetActorDirection(&PLAYER, DIR_IDLE_R, 0);
        //         }
        //         break;
        //     case DIR_JUMP_R:
        //         if ((PLAYER.direction == DIR_LEFT) || (PLAYER.direction == DIR_JUMP_L) || (PLAYER.direction == DIR_CRAWL_L)) {
        //             SetActorDirection(&PLAYER, DIR_IDLE_L, 0);
        //         } else {
        //             SetActorDirection(&PLAYER, DIR_IDLE_R, 0);
        //         }
        //         break;
        //     case DIR_DOWN_R:
        //         if ((PLAYER.direction == DIR_LEFT) || (PLAYER.direction == DIR_JUMP_L) || (PLAYER.direction == DIR_CRAWL_L)) {
        //             SetActorDirection(&PLAYER, DIR_IDLE_L, 0);
        //         } else {
        //             SetActorDirection(&PLAYER, DIR_IDLE_R, 0);
        //         }
        //         break;
        //     case DIR_CRAWL_R:
        //         if ((PLAYER.direction == DIR_LEFT) || (PLAYER.direction == DIR_JUMP_L) || (PLAYER.direction == DIR_CRAWL_L)) {
        //             SetActorDirection(&PLAYER, DIR_IDLE_L, 0);
        //         } else {
        //             SetActorDirection(&PLAYER, DIR_IDLE_R, 0);
        //         }
        //         break;
        // }
        // }
        // }
        // update PLAYER absolute posiiton
        PLAYER.y += PLAYER.SpdY;
        //THIS IS ASSUMING PLAYER IS WALKING LEFT TO RIGHT. PERHAPS ADD A STAGE_LEFT AND STAGE_RIGHT VARIABLE IN THE STAGE STRUCT SO HE IS ON THE LEFT SIDE WHEN STAGE_RIGHT//
        if ((TO_PIXELS(bkg.camera_x) > 0) && (TO_PIXELS(bkg.camera_x) < bkg.camera_max_x)) {
            bkg.camera_x += PLAYER.SpdX;
            bkg.redraw = TRUE;
        } else
            PLAYER.x += PLAYER.SpdX;
        if (TO_PIXELS(bkg.camera_x) - 1 <= 0) {
            if ((joy & J_RIGHT) && (PLAYER.SpdX > 0) && (TO_PIXELS(PLAYER.x) >= 118)) {
                bkg.camera_x += PLAYER.SpdX;
                bkg.redraw = TRUE;
            }
        } else if (TO_PIXELS(bkg.camera_x) + 1 >= bkg.camera_max_x) {
            if ((joy & J_LEFT) && (PLAYER.SpdX < 0) && (TO_PIXELS(PLAYER.x) <= 118)) {
                bkg.camera_x += PLAYER.SpdX;
                bkg.redraw = TRUE;
            }
        }
        // #ifdef DEBUG
        // if (joy & J_B) {
        //     printf("P.X=%u\nOff.x=%u\n", TO_PIXELS(PLAYER.x), (TO_PIXELS(PLAYER.x) - TO_PIXELS(active_actors[ACTOR_FIRST_NPC].x)));
        // }
        // #endif
                if (joy & J_B) {
            printf("Map.x=%u P.y=%u\n", (TO_PIXELS(PLAYER.x) + TO_PIXELS(bkg.camera_x)), TO_PIXELS(PLAYER.y));
        }
       
        //LATER CHANGE THIS TO COLLISION TILE RESET/DEATH
        if ((TO_PIXELS(PLAYER.y) > 241) && (TO_PIXELS(PLAYER.y) < 249)) {
            DISPLAY_OFF;
            PLAYER.y = TO_COORDS(0);
            load_level(&level1);
            init_submap();
            DISPLAY_ON;
        }

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
