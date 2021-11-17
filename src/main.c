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
UBYTE Spawn, Jump, Crouch, canCrouch, x_Adjust, Launch, Shooting;
UBYTE launchDelay = 0;
UBYTE canCrouch_timer, canCrouch_Ftimer;
UBYTE shooting_counter = 0;
const unsigned char blankmap[3] = {0x00, 0x01, 0x02};
extern Variables bkg;
uint8_t shadow_scx = 0, shadow_scy = 0;
BOOLEAN overlap(INT16, INT16, INT16, INT16, INT16, INT16, INT16, INT16);

void check_LR(UBYTE newplayerx, UBYTE newplayery, INT16 camera_x) {
    UINT16 indexDy, indexCy, indexTy, index_Lx, index_Rx, indexCamx, tileindexLD, tileindexLC, tileindexLT, tileindexRD, tileindexRC, tileindexRT;

    indexCamx = camera_x;
    indexDy = (newplayery - 1) / 8;
    indexCy = (newplayery - 16) / 8;
    indexTy = (newplayery - 24) / 8;
    index_Lx = ((newplayerx - 16) + indexCamx) / 8;
    index_Rx = ((newplayerx - 1) + indexCamx) / 8;
    //LEFT INDEX
    tileindexLD = COLLISION_WIDE_MAPWidth * indexDy + index_Lx;  //MULTIPLY THE WIDTH BY THE Y TILE TO FIND THE Y ROW. THEN ADD THE X TILE TO SHIFT THE COLUMN. FINDS THE TILE YOU'RE LOOKING FOR
    tileindexLC = COLLISION_WIDE_MAPWidth * indexCy + index_Lx;
    tileindexLT = COLLISION_WIDE_MAPWidth * indexTy + index_Lx;
    //RIGHT INDEX
    tileindexRD = COLLISION_WIDE_MAPWidth * indexDy + index_Rx;  //MULTIPLY THE WIDTH BY THE Y TILE TO FIND THE Y ROW. THEN ADD THE X TILE TO SHIFT THE COLUMN. FINDS THE TILE YOU'RE LOOKING FOR
    tileindexRC = COLLISION_WIDE_MAPWidth * indexCy + index_Rx;
    tileindexRT = COLLISION_WIDE_MAPWidth * indexTy + index_Rx;

    if (Crouch) {
        if ((COLLISION_WIDE_MAP[tileindexLD] == 0x01) || (COLLISION_WIDE_MAP[tileindexLC] == 0x01) || (COLLISION_WIDE_MAP[tileindexRD] == 0x01) || (COLLISION_WIDE_MAP[tileindexRC] == 0x01)) {
            if (!Jump) {
                PLAYER.SpdX = 0;
                switch_crawl();
            }
        }
        if ((COLLISION_WIDE_MAP[tileindexLD] == 0x00) && (COLLISION_WIDE_MAP[tileindexLC] == 0x00) && (COLLISION_WIDE_MAP[tileindexLT] == 0x01) || (COLLISION_WIDE_MAP[tileindexRD] == 0x00) && (COLLISION_WIDE_MAP[tileindexRC] == 0x00) && (COLLISION_WIDE_MAP[tileindexRT] == 0x01)) {
            if (canCrouch_timer == 1) {
                canCrouch = TRUE;
                Crouch = TRUE;
                // canCrouch = FALSE;
                canCrouch_timer = 10;
            }
            canCrouch_timer -= 1;
        }
    } else if (!Crouch) {
        if ((COLLISION_WIDE_MAP[tileindexLD] == 0x01) || (COLLISION_WIDE_MAP[tileindexLC] == 0x01) || (COLLISION_WIDE_MAP[tileindexLT] == 0x01) || (COLLISION_WIDE_MAP[tileindexRD] == 0x01) || (COLLISION_WIDE_MAP[tileindexRC] == 0x01) || (COLLISION_WIDE_MAP[tileindexRT] == 0x01)) {
            PLAYER.SpdX = 0;
            if (!Jump) {
                switch_idle_jump();  //NOT SURE IF NEEDED
            }
        }
        if ((COLLISION_WIDE_MAP[tileindexLD] == 0x00) && (COLLISION_WIDE_MAP[tileindexLC] == 0x00) && (COLLISION_WIDE_MAP[tileindexLT] == 0x01) || (COLLISION_WIDE_MAP[tileindexRD] == 0x00) && (COLLISION_WIDE_MAP[tileindexRC] == 0x00) && (COLLISION_WIDE_MAP[tileindexRT] == 0x01)) {
            if (canCrouch_timer == 1) {
                canCrouch = TRUE;
                Crouch = TRUE;
                // canCrouch = FALSE;
                canCrouch_timer = 10;
            }
            canCrouch_timer -= 1;
        }
        // else if (COLLISION_WIDE_MAP[tileindexLD] == 0x00) {
        // }
    }
}
//TRY COMBINING THIS WITH CHECK_J BY ADDING A SWITCH WHEN PRESSING A BUTTON, TURNS OFF AFTER CHECK_J IN BOTH IF AND ELSE IF SECNARIOS
void check_UD(UBYTE newplayerx, UBYTE newplayery, INT16 camera_x) {
    UINT16 indexLx, indexCx, indexRx, index_y, indexCamx, tileindexL, tileindexC, tileindexR;
    indexCamx = camera_x;
    indexLx = ((newplayerx - 16) + indexCamx) / 8;
    indexCx = ((newplayerx - 8) + indexCamx) / 8;
    indexRx = ((newplayerx - 1) + indexCamx) / 8;
    index_y = (newplayery - 1) / 8;

    tileindexL = COLLISION_WIDE_MAPWidth * index_y + indexLx;  //MULTIPLY THE WIDTH BY THE Y TILE TO FIND THE Y ROW. THEN ADD THE X TILE TO SHIFT THE COLUMN. FINDS THE TILE YOU'RE LOOKING FOR
    tileindexC = COLLISION_WIDE_MAPWidth * index_y + indexCx;
    tileindexR = COLLISION_WIDE_MAPWidth * index_y + indexRx;
//IF THE CHARACTER IS STANDING UNDER AN 0x02 COLLISION CORNER AND CLIPS, ADJUST HIS X UNTIL HE IS SAFELY OUTSIDE OF THE COLLISION WALL OF TILES
    if (x_Adjust) {
    if ((COLLISION_WIDE_MAP[tileindexL] != 0x02) && (COLLISION_WIDE_MAP[tileindexR] == 0x02)){
            PLAYER.SpdX = -MAX_WALK_SPEED;
        }
    else if ((COLLISION_WIDE_MAP[tileindexL] == 0x02) && (COLLISION_WIDE_MAP[tileindexR] != 0x02)){
            PLAYER.SpdX = MAX_WALK_SPEED;
        }
    else if ((COLLISION_WIDE_MAP[tileindexL] != 0x02) && (COLLISION_WIDE_MAP[tileindexR] != 0x02)){
        x_Adjust = FALSE;
    }
    }
    if (PLAYER.SpdY > 0){
    if ((COLLISION_WIDE_MAP[tileindexL] == 0x01) || (COLLISION_WIDE_MAP[tileindexC] == 0x01) || (COLLISION_WIDE_MAP[tileindexR] == 0x01)) {
        UBYTE ty = (TO_PIXELS(PLAYER.y) / 8);
        PLAYER.y = TO_COORDS(ty * 8);
        PLAYER.SpdY = 0;
        Spawn = Jump = FALSE;
        switch_idle_jump();
    }
    } else if (PLAYER.SpdY < 0){
    if ((COLLISION_WIDE_MAP[tileindexL] == 0x01) || (COLLISION_WIDE_MAP[tileindexC] == 0x01) || (COLLISION_WIDE_MAP[tileindexR] == 0x01)) {
        PLAYER.SpdY = 0;
        }
    }
    // else if ((COLLISION_WIDE_MAP[tileindexDL] == 0x00) && (COLLISION_WIDE_MAP[tileindexDC] == 0x00) && (COLLISION_WIDE_MAP[tileindexDT] == 0x00)) {
    // }
}
//TRY COMBINING THIS WITH CHECK_J BY ADDING A SWITCH WHEN PRESSING A BUTTON, TURNS OFF AFTER CHECK_J IN BOTH IF AND ELSE IF SECNARIOS
void check_J(UBYTE newplayerx, UBYTE newplayery, INT16 camera_x) {
    UINT16 indexLx, indexCx, indexRx, indexSLx, indexSRx, index_y, index_Cy, indexCamx, tileindexL, tileindexC, tileindexR, tileindexCL, tileindexCC, tileindexCR, tileindexSL, tileindexSR;
    //CL = Crouch Left CC = Crouch Center CR = Crouch Right
    indexCamx = camera_x;
  
    indexLx = ((newplayerx - 16) + indexCamx) / 8;
    indexCx = ((newplayerx - 8) + indexCamx) / 8;
    indexRx = ((newplayerx - 1) + indexCamx) / 8;
 //STANDING x with a few pixels of forgiveness for 0x02 collision checks
    indexSLx = ((newplayerx - 10) + indexCamx) / 8;
    indexSRx = ((newplayerx - 7) + indexCamx) / 8;
    
    index_y = (newplayery - 1) / 8;
    index_Cy = (newplayery + 7) / 8;

    tileindexL = COLLISION_WIDE_MAPWidth * index_y + indexLx;  //MULTIPLY THE WIDTH BY THE Y TILE TO FIND THE Y ROW. THEN ADD THE X TILE TO SHIFT THE COLUMN. FINDS THE TILE YOU'RE LOOKING FOR
    tileindexC = COLLISION_WIDE_MAPWidth * index_y + indexCx;
    tileindexR = COLLISION_WIDE_MAPWidth * index_y + indexRx;

    tileindexSL = COLLISION_WIDE_MAPWidth * index_y + indexSLx;  //MULTIPLY THE WIDTH BY THE Y TILE TO FIND THE Y ROW. THEN ADD THE X TILE TO SHIFT THE COLUMN. FINDS THE TILE YOU'RE LOOKING FOR
    tileindexSR = COLLISION_WIDE_MAPWidth * index_y + indexSRx;

    tileindexCL = COLLISION_WIDE_MAPWidth * index_Cy + indexLx;
    tileindexCC = COLLISION_WIDE_MAPWidth * index_Cy + indexCx;
    tileindexCR = COLLISION_WIDE_MAPWidth * index_Cy + indexRx;

    if (Crouch) {
        if ((COLLISION_WIDE_MAP[tileindexCL] == 0x01) || (COLLISION_WIDE_MAP[tileindexCC] == 0x01) || (COLLISION_WIDE_MAP[tileindexCR] == 0x01)) {
        } else {
            Crouch = Launch = FALSE;
            if (!Jump) {
                PLAYER.SpdY = JUMP_IMPULSE;
                Jump = TRUE;
                switch_jump();
            }
        }
    } else if (!Crouch) {
        //CHECKING 0X01 here prevents him from "trying" to jump aka glitched animation 1 frame
        }
        if (PLAYER.SpdX < 0){
        if ((COLLISION_WIDE_MAP[tileindexL] != 0x02) && (COLLISION_WIDE_MAP[tileindexR] == 0x02)){
            x_Adjust = TRUE;
            }
        }
        else if (PLAYER.SpdX > 0){
        if ((COLLISION_WIDE_MAP[tileindexL] == 0x02) && (COLLISION_WIDE_MAP[tileindexR] != 0x02)){
            x_Adjust = TRUE;
            }
        }
        else if (PLAYER.SpdX == 0) {
                    if ((COLLISION_WIDE_MAP[tileindexSL] != 0x02) && (COLLISION_WIDE_MAP[tileindexSR] == 0x02)){
            x_Adjust = TRUE;
            } else if ((COLLISION_WIDE_MAP[tileindexSL] == 0x02) && (COLLISION_WIDE_MAP[tileindexSR] != 0x02)){
            x_Adjust = TRUE;
            }
        }
        if (PLAYER.SpdX != 0){
           if (((COLLISION_WIDE_MAP[tileindexL] == 0x02) && (COLLISION_WIDE_MAP[tileindexR] == 0x02)) || ((COLLISION_WIDE_MAP[tileindexL] == 0x01) || (COLLISION_WIDE_MAP[tileindexC] == 0x01) || (COLLISION_WIDE_MAP[tileindexR] == 0x01))){
           } else {
            Crouch = Launch = FALSE;
            if (!Jump) {
                PLAYER.SpdY = JUMP_IMPULSE;
                Jump = TRUE;
                switch_jump();
            }
           }
        }
        else if (PLAYER.SpdX == 0){
           if (((COLLISION_WIDE_MAP[tileindexSL] == 0x02) && (COLLISION_WIDE_MAP[tileindexSR] == 0x02)) || ((COLLISION_WIDE_MAP[tileindexL] == 0x01) || (COLLISION_WIDE_MAP[tileindexC] == 0x01) || (COLLISION_WIDE_MAP[tileindexR] == 0x01))) {
           } else {
            Crouch = Launch = FALSE;
            if (!Jump) {
                PLAYER.SpdY = JUMP_IMPULSE;
                Jump = TRUE;
                switch_jump();
            }
           }
        }

}

//CHECK CROUCH
void check_C(UBYTE newplayerx, UBYTE newplayery, INT16 camera_x) {
    UINT16 indexCLx, indexCCx, indexCRx, indexSLx, indexSRx, index_y, indexCamx, tileindexCL, tileindexCC, tileindexCT, tileindexSL, tileindexSR;

    indexCamx = camera_x;
    //CROUCH VALUES
    indexCLx = ((newplayerx - 14) + indexCamx) / 8;
    indexCCx = ((newplayerx - 8) + indexCamx) / 8;
    indexCRx = ((newplayerx - 3) + indexCamx) / 8;
    //STANDING VALUES (CHECK TO PUSH PLAYER LEFT OR RIGHT IF HEAD IS IN A COLLISION)
    indexSLx = ((newplayerx - 16) + indexCamx) / 8;
    indexSRx = ((newplayerx - 1) + indexCamx) / 8;
    index_y = (newplayery - 20) / 8;

    tileindexCL = COLLISION_WIDE_MAPWidth * index_y + indexCLx;  //MULTIPLY THE WIDTH BY THE Y TILE TO FIND THE Y ROW. THEN ADD THE X TILE TO SHIFT THE COLUMN. FINDS THE TILE YOU'RE LOOKING FOR
    tileindexCC = COLLISION_WIDE_MAPWidth * index_y + indexCCx;
    tileindexCT = COLLISION_WIDE_MAPWidth * index_y + indexCRx;
    //STANDING
    tileindexSL = COLLISION_WIDE_MAPWidth * index_y + indexSLx;
    tileindexSR = COLLISION_WIDE_MAPWidth * index_y + indexSRx;

    if (Crouch) {
        if ((COLLISION_WIDE_MAP[tileindexCL] == 0x01) || (COLLISION_WIDE_MAP[tileindexCC] == 0x01) || (COLLISION_WIDE_MAP[tileindexCT] == 0x01)) {
        } else {
            Crouch = FALSE;
        }
    } else if (!Crouch) {
        if (COLLISION_WIDE_MAP[tileindexSR] == 0x01) {
            PLAYER.SpdX -= MAX_CRAWL_SPEED;
        } else if (COLLISION_WIDE_MAP[tileindexSL] == 0x01) {
            PLAYER.SpdX += MAX_CRAWL_SPEED;
        }

        // else if ((COLLISION_WIDE_MAP[tileindexDL] == 0x00) && (COLLISION_WIDE_MAP[tileindexDC] == 0x00) && (COLLISION_WIDE_MAP[tileindexDT] == 0x00)) {
        // }
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

    Jump = Crouch = canCrouch = x_Adjust = Launch = Shooting = FALSE;
    Spawn = TRUE;
    canCrouch_timer = 10;  //LEFT AND RIGHT BUTTON PRESS TIME DELAY TO AUTO CROUCH
    canCrouch_Ftimer = 8;  //TURN canCrouch TO FALSE WHEN REACH COUNTDOWN
    init_submap();
    load_level(&level1);
    actor_t *current_actor = &active_actors[ACTOR_FIRST_NPC];
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
                if (PLAYER.SpdX == 0) {
                    if (joy & J_DOWN) {
                        canCrouch_timer = 1;
                    }
                    check_LR(TO_PIXELS(PLAYER.x) - 1, TO_PIXELS(PLAYER.y), TO_PIXELS(bkg.camera_x));
                }
                if ((!Jump) && !(joy & (J_DOWN)) && (!Crouch)) {
                    if (canCrouch) {
                        SetActorDirection(&PLAYER, DIR_CRAWL_L, PLAYER.animation_phase);
                    } else {
                        SetActorDirection(&PLAYER, DIR_LEFT, PLAYER.animation_phase);
                    }

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
                if (PLAYER.SpdX == 0) {
                    if (joy & J_DOWN) {
                        canCrouch_timer = 1;
                    }
                    check_LR(TO_PIXELS(PLAYER.x) + 1, TO_PIXELS(PLAYER.y), TO_PIXELS(bkg.camera_x));
                }
                if ((!Jump) && !(joy & (J_DOWN)) && !(Crouch)) {
                    if (canCrouch) {
                        SetActorDirection(&PLAYER, DIR_CRAWL_R, PLAYER.animation_phase);
                    } else {
                        SetActorDirection(&PLAYER, DIR_RIGHT, PLAYER.animation_phase);
                    }
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
        }
        if (Crouch) {
            PLAYER.h_offset = 24;
        }
        // DOWN while standing still
        if ((Crouch) && (!canCrouch) && (!(joy & J_LEFT) && !(joy & J_RIGHT)) && (!Jump)) {
            switch_down();
        }
        //IF PLAYER IS FREE FALLING FOR ANY REASON
        if (PLAYER.SpdY != 0) {
            Jump = TRUE;
            Crouch = FALSE;
            switch_jump();
        }

        if ((CHANGED_BUTTONS & J_A) && (joy & J_A)) {
            //CHECK WHETHER CAN JUMP (NO COLLISION ABOVE PLAYER)
            check_J(TO_PIXELS(PLAYER.x), TO_PIXELS(PLAYER.y) - 25, TO_PIXELS(bkg.camera_x));
        }
        if ((Crouch) && (canCrouch)) {
            if ((PLAYER.direction == DIR_CRAWL_L) || (PLAYER.direction == DIR_DOWN_L)) {
                PLAYER.SpdX = -MAX_CRAWL_SPEED;
            } else if ((PLAYER.direction == DIR_CRAWL_R) || (PLAYER.direction == DIR_DOWN_R)) {
                PLAYER.SpdX = MAX_CRAWL_SPEED;
            }
            canCrouch_Ftimer -= 1;
            if (canCrouch_Ftimer == 1) {
                canCrouch = FALSE;
                canCrouch_Ftimer = 8;
            }
        }
        if (!(joy & J_LEFT) && !(joy & J_RIGHT)) {
            canCrouch_timer = 10;
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
            check_UD(TO_PIXELS(PLAYER.x), TO_PIXELS(PLAYER.y) + 1, TO_PIXELS(bkg.camera_x));

        } else if (PLAYER.SpdY < 0) {
            check_UD(TO_PIXELS(PLAYER.x), TO_PIXELS(PLAYER.y) - 25, TO_PIXELS(bkg.camera_x));
        }
        //IF MOVING RIGHT
        if (PLAYER.SpdX > 0) {
            check_LR(TO_PIXELS(PLAYER.x) + 1, TO_PIXELS(PLAYER.y), TO_PIXELS(bkg.camera_x));
            //IF MOVING LEFT
        } else if (PLAYER.SpdX < 0) {
            check_LR(TO_PIXELS(PLAYER.x) - 1, TO_PIXELS(PLAYER.y), TO_PIXELS(bkg.camera_x));
        }

        //PERHAPS REPLACE THIS WITH A NEW SEPARATE FUNCTION CALLED check_C();
        if ((Crouch) && (!canCrouch)) {
            if (!(joy & J_DOWN)) {
                check_C(TO_PIXELS(PLAYER.x), TO_PIXELS(PLAYER.y), TO_PIXELS(bkg.camera_x));
            }
        }
        // Change to IDLE state when not moving
        if ((!Jump) && (!Crouch)) {
            if ((PLAYER.SpdX == 0) && (PLAYER.SpdY == 0)) {
                switch_idle();
                check_C(TO_PIXELS(PLAYER.x), TO_PIXELS(PLAYER.y), TO_PIXELS(bkg.camera_x));
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
        // if (joy & J_B) {
        //     printf("CamxD=-%u\n", (bkg.camera_max_x - TO_PIXELS(bkg.camera_x)));
        // }
        //LATER CHANGE THIS TO COLLISION TILE RESET/DEATH
        if ((TO_PIXELS(PLAYER.y) > 241) && (TO_PIXELS(PLAYER.y) < 249)) {
            // DISPLAY_OFF;
            // Spawn = TRUE;
            // init_submap();
            // load_level(&level1);
            // DISPLAY_ON;
            PLAYER.y = TO_COORDS(50);
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

            // if (overlap(PTR_y, PTR_x, PBL_y, PBL_x, NTR_y, NTR_x, NBL_y, NBL_x) == 0x01U) {
            //     if (active_actors[i].ON == TRUE) {
            //         printf("GAME OVER\n");
            //     }
            // }
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
