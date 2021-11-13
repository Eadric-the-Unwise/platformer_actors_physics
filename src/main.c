#include <gb/gb.h>
#include <gbdk/metasprites.h>

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
#include "../res/tiles/detective_large.h"
#include "../res/tiles/enemy_arrow.h"
#include "level1.h"
#include "level2.h"
#include "macros.h"
#include "scene.h"

UBYTE joy, last_joy;

UBYTE floorYposition;
UBYTE Spawn, Jump, Crouch, Launch, Shooting;
UBYTE launchDelay = 0;
UBYTE shooting_counter = 0;
const unsigned char blankmap[2] = {0x00, 0x01};
extern Variables bkg;
uint8_t shadow_scx = 0, shadow_scy = 0;
BOOLEAN overlap(INT16, INT16, INT16, INT16, INT16, INT16, INT16, INT16);

// TIME TO DELETE THE ORIGINAL CHECKCOLLISION FUNCTIONS
//CHECKS WHETHER OR NOT THE OFFSET OF PLAYER POSITION COLLIDES WITH A COLLISION TILE
//BOTTOM LEFT PIXEL
UBYTE checkcollisionBL(UBYTE newplayerx, UBYTE newplayery, INT16 camera_x) {
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
UBYTE checkcollisionBR(UBYTE newplayerx, UBYTE newplayery, INT16 camera_x) {
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
UBYTE checkcollisionBC(UBYTE newplayerx, UBYTE newplayery, INT16 camera_x) {
    UINT16 indexBLx, indexBLy, indexCamx, tileindexBL;
    UBYTE result;

    indexCamx = camera_x;
    indexBLx = ((newplayerx - 9) + indexCamx) / 8;
    indexBLy = (newplayery - 1) / 8;

    tileindexBL = COLLISION_WIDE_MAPWidth * indexBLy + indexBLx;  //MULTIPLY THE WIDTH BY THE Y TILE TO FIND THE Y ROW. THEN ADD THE X TILE TO SHIFT THE COLUMN. FINDS THE TILE YOU'RE LOOKING FOR

    result = COLLISION_WIDE_MAP[tileindexBL] == blankmap[1];

    return result;
}
void check_D(UBYTE newplayerx, UBYTE newplayery, INT16 camera_x) {
    UINT16 indexDLx, indexDCx, indexDRx, index_y, indexCamx, tileindexDL, tileindexDC, tileindexDR;

    indexCamx = camera_x;
    indexDLx = ((newplayerx - 17) + indexCamx) / 8;
    indexDCx = ((newplayerx - 9) + indexCamx) / 8;
    indexDRx = ((newplayerx) + indexCamx) / 8;
    index_y = (newplayery - 1) / 8;

    tileindexDL = COLLISION_WIDE_MAPWidth * index_y + indexDLx;  //MULTIPLY THE WIDTH BY THE Y TILE TO FIND THE Y ROW. THEN ADD THE X TILE TO SHIFT THE COLUMN. FINDS THE TILE YOU'RE LOOKING FOR
    tileindexDC = COLLISION_WIDE_MAPWidth * index_y + indexDCx;
    tileindexDR = COLLISION_WIDE_MAPWidth * index_y + indexDRx;

    if ((COLLISION_WIDE_MAP[tileindexDL] == 0x01) || (COLLISION_WIDE_MAP[tileindexDC] == 0x01) || (COLLISION_WIDE_MAP[tileindexDR] == 0x01)) {
        UBYTE ty = (TO_PIXELS(PLAYER.y) / 8);
        PLAYER.y = TO_COORDS(ty * 8);
        PLAYER.SpdY = 0;
        Spawn = Jump = FALSE;
        switch_idle_jump();
    } else if (COLLISION_WIDE_MAP[tileindexDL] == 0x00) {
    }
}

//LATER MOVE THIS TO A RENDER PORTION OF THE GAME AND REMOVE THE TILE #INCLUDES //
BOOLEAN overlap(INT16 r1_y, INT16 r1_x, INT16 l1_y, INT16 l1_x, INT16 r2_y, INT16 r2_x, INT16 l2_y, INT16 l2_x) {
    // Standard rectangle-to-rectangle collision check

    if (l1_x == r1_x || l1_y == r1_y || l2_x == r2_x || l2_y == r2_y) {
        // the line cannot have positive overlap
        return 0x00U;
    }

    if ((l1_x >= r2_x) || (l2_x >= r1_x)) {
        return 0x00U;
    }
    if ((r1_y >= l2_y) || (r2_y >= l1_y)) {
        return 0X00U;
    }

    return 0x01U;
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
    Spawn = TRUE;
    init_submap();
    load_level(&level1);
    actor_t *current_actor = &active_actors[ACTOR_FIRST_NPC];
    // shadow_scx = (UBYTE)(bkg.camera_x >> 4u);
    // shadow_scy = bkg.camera_y;
    // DISABLE_VBL_TRANSFER;
    // switch on display after everything is ready
    DISPLAY_ON;
    last_joy = joy = 0;
    while (TRUE) {  // main loop runs at 60fps
        // ---------------------------------------------
        // process joystic input
        last_joy = joy;
        joy = joypad();
        if (!Spawn) {
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
        }
        if ((joy & J_DOWN) && !(Jump)) {
            Crouch = TRUE;
            PLAYER.h_offset = 24;
        }
        // DOWN while standing still
        if ((Crouch) && (!(joy & J_LEFT) && !(joy & J_RIGHT)) && (!Jump)) {
            switch_down();
        }
        //IF PLAYER IS FREE FALLING FOR ANY REASON
        if (PLAYER.SpdY != 0) {
            Jump = TRUE;
            Crouch = FALSE;
            switch_jump();
        }

        if ((CHANGED_BUTTONS & J_A) && (joy & J_A)) {
            Crouch = Launch = FALSE;
            if (!Jump) {
                PLAYER.SpdY = JUMP_IMPULSE;
                Jump = TRUE;
                switch_jump();
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
            check_D(TO_PIXELS(PLAYER.x), TO_PIXELS(PLAYER.y) + 1, TO_PIXELS(bkg.camera_x));

        } else if (PLAYER.SpdY < 0) {
            if ((checkcollisionBL(TO_PIXELS(PLAYER.x), TO_PIXELS(PLAYER.y) - 25, TO_PIXELS(bkg.camera_x))) || (checkcollisionBR(TO_PIXELS(PLAYER.x), TO_PIXELS(PLAYER.y) - 25, TO_PIXELS(bkg.camera_x))) || (checkcollisionBC(TO_PIXELS(PLAYER.x), TO_PIXELS(PLAYER.y) - 25, TO_PIXELS(bkg.camera_x)))) {
                UBYTE ty = (TO_PIXELS(PLAYER.y) / 8);
                PLAYER.y = TO_COORDS(ty * 8);
                PLAYER.SpdY = 0;
                Jump = FALSE;
                switch_idle_jump();
            }
        }
        if (PLAYER.SpdX > 0) {
            if (Crouch) {
                if ((checkcollisionBR(TO_PIXELS(PLAYER.x) + 1, TO_PIXELS(PLAYER.y), TO_PIXELS(bkg.camera_x))) || (checkcollisionBR(TO_PIXELS(PLAYER.x) + 1, TO_PIXELS(PLAYER.y) - 15, TO_PIXELS(bkg.camera_x)))) {
                    PLAYER.SpdX = 0;
                    //set player idle direction when touching ground
                    if (!Jump) {
                        switch_crawl();
                    }
                }

            } else {
                if ((checkcollisionBR(TO_PIXELS(PLAYER.x) + 1, TO_PIXELS(PLAYER.y), TO_PIXELS(bkg.camera_x))) || (checkcollisionBR(TO_PIXELS(PLAYER.x) + 1, TO_PIXELS(PLAYER.y) - 12, TO_PIXELS(bkg.camera_x))) || (checkcollisionBR(TO_PIXELS(PLAYER.x), TO_PIXELS(PLAYER.y) - 23, TO_PIXELS(bkg.camera_x)))) {
                    PLAYER.SpdX = 0;
                    switch_idle_jump();  //NOT SURE IF NEEDED
                }
            }
        } else if (PLAYER.SpdX < 0) {
            if (Crouch) {
                if ((checkcollisionBL(TO_PIXELS(PLAYER.x) - 1, TO_PIXELS(PLAYER.y), TO_PIXELS(bkg.camera_x))) || (checkcollisionBL(TO_PIXELS(PLAYER.x) - 1, TO_PIXELS(PLAYER.y) - 15, TO_PIXELS(bkg.camera_x)))) {
                    PLAYER.SpdX = 0;
                    if (!Jump) {
                        switch_crawl();
                    }
                }
            } else {
                if ((checkcollisionBL(TO_PIXELS(PLAYER.x) - 1, TO_PIXELS(PLAYER.y), TO_PIXELS(bkg.camera_x))) || (checkcollisionBL(TO_PIXELS(PLAYER.x) - 1, TO_PIXELS(PLAYER.y) - 15, TO_PIXELS(bkg.camera_x))) || (checkcollisionBL(TO_PIXELS(PLAYER.x), TO_PIXELS(PLAYER.y) - 23, TO_PIXELS(bkg.camera_x)))) {
                    PLAYER.SpdX = 0;
                    switch_idle_jump();  //NOT SURE IF NEEDED
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
        if ((!Jump) && (!Crouch)) {
            if ((PLAYER.SpdX == 0) && (PLAYER.SpdY == 0)) {
                switch_idle();
            }
        }
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
        if (joy & J_B) {
            printf("CamxD=-%u\n", (bkg.camera_max_x - TO_PIXELS(bkg.camera_x)));
        }
        //LATER CHANGE THIS TO COLLISION TILE RESET/DEATH
        if ((TO_PIXELS(PLAYER.y) > 241) && (TO_PIXELS(PLAYER.y) < 249)) {
            // DISPLAY_OFF;
            // Spawn = TRUE;
            // init_submap();
            // load_level(&level1);
            // DISPLAY_ON;
            PLAYER.y = TO_COORDS(40);
        }
        // COPIED FROM DINO COLLISIONS
        for (UBYTE i = ACTOR_FIRST_NPC; i != (active_actors_count); i++) {
            //[y][x]
            UINT16 PTR_y, PTR_x, PBL_y, PBL_x, NTR_y, NTR_x, NBL_y, NBL_x;
            PTR_y = TO_PIXELS(PLAYER.y) - (PLAYER.h - PLAYER.h_offset);  //TR y the top tile of PLAYER is 16 but only 8 or so are actually visible pixels, hence we subtract
            PTR_x = TO_PIXELS(PLAYER.x) + PLAYER.x_offset;               //TR x
            PBL_y = TO_PIXELS(PLAYER.y) + PLAYER.y_offset;               //BL y
            PBL_x = TO_PIXELS(PLAYER.x) - (PLAYER.w - PLAYER.x_offset);  //BL x
            NTR_y = TO_PIXELS(active_actors[i].y) - active_actors[i].h;
            NTR_x = TO_PIXELS(active_actors[i].x) + active_actors[i].x_offset;
            NBL_y = TO_PIXELS(active_actors[i].y) + active_actors[i].y_offset;
            NBL_x = TO_PIXELS(active_actors[i].x) - (active_actors[i].w - active_actors[i].w_offset);

            if (overlap(PTR_y, PTR_x, PBL_y, PBL_x, NTR_y, NTR_x, NBL_y, NBL_x) == 0x01U) {
                if (active_actors[i].ON == TRUE) {
                    printf("GAME OVER\n");
                }
            }
        }
        // call level animation hook (if any), that makes other actors move (and interact in future)
        if (animate_level) animate_level();

        // render all actors on screen
        render_actors();
        if (bkg.redraw) {
            wait_vbl_done();
            bkg.redraw = FALSE;
            refresh_OAM();
            set_camera();

        } else {
            wait_vbl_done();
        }
        refresh_OAM();
    }
}
