#pragma bank 255
#include "collisions.h"

UINT8 Spawn, Ladder, Gravity, Jump, Crouch, canCrouch, Drop, x_Adjust, Launch, Shooting;
UINT8 canCrouch_timer, canCrouch_Ftimer, Drop_timer;

extern UINT8 joy, last_joy;
extern UINT8 Attach, x_Collide, y_Collide;

// THESE COLLISIONS ARE SET ON SINGLE PIXELS, MEANING FINDING THE CENTER IS A CHALLENGE (AS OPPOSED TO NPC COLLISIONS WHICH ARE PERFECTLY CENTRED)
void check_LR(UINT8 newplayerx, UINT8 newplayery, INT16 camera_x) {
    UINT16 indexDy, indexCy, indexTy, index_Lx, index_Rx, index_kLx, index_kRx, indexCamx, tileindexLD, tileindexLC, tileindexLT, tileindexRD, tileindexRC, tileindexRT, tileindexkLD, tileindexkLC, tileindexkLT, tileindexkRD, tileindexkRC, tileindexkRT;
    // REPLACE THESE HARD CODED INDEXES WITH OFFSETS SIMILAR TO THE NPC COLLISION FUNC
    indexCamx = camera_x;
    indexDy = (newplayery - 1) / 8;
    indexCy = (newplayery - 16) / 8;
    indexTy = (newplayery - 24) / 8;
    index_Lx = ((newplayerx - 16) + indexCamx) / 8;
    index_Rx = ((newplayerx - 1) + indexCamx) / 8;
    index_kLx = ((newplayerx - 14) + indexCamx) / 8;  // SPIKE KILL CHECK
    index_kRx = ((newplayerx - 3) + indexCamx) / 8;   // SPIKE KILL CHECK
    // LEFT INDEX
    tileindexLD = COLLISION_WIDE_MAPWidth * indexDy + index_Lx;  // MULTIPLY THE WIDTH BY THE Y TILE TO FIND THE Y ROW. THEN ADD THE X TILE TO SHIFT THE COLUMN. FINDS THE TILE YOU'RE LOOKING FOR
    tileindexLC = COLLISION_WIDE_MAPWidth * indexCy + index_Lx;
    tileindexLT = COLLISION_WIDE_MAPWidth * indexTy + index_Lx;
    // RIGHT INDEX
    tileindexRD = COLLISION_WIDE_MAPWidth * indexDy + index_Rx;  // MULTIPLY THE WIDTH BY THE Y TILE TO FIND THE Y ROW. THEN ADD THE X TILE TO SHIFT THE COLUMN. FINDS THE TILE YOU'RE LOOKING FOR
    tileindexRC = COLLISION_WIDE_MAPWidth * indexCy + index_Rx;
    tileindexRT = COLLISION_WIDE_MAPWidth * indexTy + index_Rx;
    // KILL LEFT INDEX
    tileindexkLD = COLLISION_WIDE_MAPWidth * indexDy + index_kLx;  // MULTIPLY THE WIDTH BY THE Y TILE TO FIND THE Y ROW. THEN ADD THE X TILE TO SHIFT THE COLUMN. FINDS THE TILE YOU'RE LOOKING FOR
    tileindexkLC = COLLISION_WIDE_MAPWidth * indexCy + index_kLx;
    tileindexkLT = COLLISION_WIDE_MAPWidth * indexTy + index_kLx;
    // KILL RIGHT INDEX
    tileindexkRD = COLLISION_WIDE_MAPWidth * indexDy + index_kRx;  // MULTIPLY THE WIDTH BY THE Y TILE TO FIND THE Y ROW. THEN ADD THE X TILE TO SHIFT THE COLUMN. FINDS THE TILE YOU'RE LOOKING FOR
    tileindexkRC = COLLISION_WIDE_MAPWidth * indexCy + index_kRx;
    tileindexkRT = COLLISION_WIDE_MAPWidth * indexTy + index_kRx;

    if (Crouch) {
        if ((COLLISION_WIDE_MAP[tileindexLD] == 0x01) || (COLLISION_WIDE_MAP[tileindexLC] == 0x01) || (COLLISION_WIDE_MAP[tileindexRD] == 0x01) || (COLLISION_WIDE_MAP[tileindexRC] == 0x01) || (COLLISION_WIDE_MAP[tileindexLD] == 0x02) || (COLLISION_WIDE_MAP[tileindexLC] == 0x02) || (COLLISION_WIDE_MAP[tileindexRD] == 0x02) || (COLLISION_WIDE_MAP[tileindexRC] == 0x02)) {
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
        if ((COLLISION_WIDE_MAP[tileindexLD] == 0x01) || (COLLISION_WIDE_MAP[tileindexLC] == 0x01) || (COLLISION_WIDE_MAP[tileindexLT] == 0x01) || (COLLISION_WIDE_MAP[tileindexRD] == 0x01) || (COLLISION_WIDE_MAP[tileindexRC] == 0x01) || (COLLISION_WIDE_MAP[tileindexRT] == 0x01) || (COLLISION_WIDE_MAP[tileindexLD] == 0x02) || (COLLISION_WIDE_MAP[tileindexLC] == 0x02) || (COLLISION_WIDE_MAP[tileindexLT] == 0x02) || (COLLISION_WIDE_MAP[tileindexRD] == 0x02) || (COLLISION_WIDE_MAP[tileindexRC] == 0x02) || (COLLISION_WIDE_MAP[tileindexRT] == 0x02) || (x_Collide)) {
            if (!x_Adjust) {
                PLAYER.SpdX = 0;
                if (!Jump) {
                    switch_idle();
                }
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
    }
    if ((COLLISION_WIDE_MAP[tileindexkLD] == 0x04) || (COLLISION_WIDE_MAP[tileindexkLC] == 0x04) || (COLLISION_WIDE_MAP[tileindexkLT] == 0x04) || (COLLISION_WIDE_MAP[tileindexkRD] == 0x04) || (COLLISION_WIDE_MAP[tileindexkRC] == 0x04) || (COLLISION_WIDE_MAP[tileindexkRT] == 0x04)) {
        GAMEOVER = TRUE;
    }
}
// TRY COMBINING THIS WITH CHECK_J BY ADDING A SWITCH WHEN PRESSING A BUTTON, TURNS OFF AFTER CHECK_J IN BOTH IF AND ELSE IF SECNARIOS
void check_UD(UINT8 newplayerx, UINT8 newplayery, INT16 camera_x) {
    UINT16 indexLx, indexCx, indexCx9, indexRx, index_y, index_ky, indexCamx, tileindexL, tileindexC, tileindexC9, tileindexR, tileindexkL, tileindexkC, tileindexkR;
    indexCamx = camera_x;
    indexLx = ((newplayerx - 16) + indexCamx) / 8;
    indexCx = ((newplayerx - 8) + indexCamx) / 8;
    indexCx9 = ((newplayerx - 9) + indexCamx) / 8;
    indexRx = ((newplayerx - 1) + indexCamx) / 8;
    index_y = (newplayery - 1) / 8;
    index_ky = (newplayery - 8) / 8;  // KILL SPIKE CHECK

    tileindexL = COLLISION_WIDE_MAPWidth * index_y + indexLx;  // MULTIPLY THE WIDTH BY THE Y TILE TO FIND THE Y ROW. THEN ADD THE X TILE TO SHIFT THE COLUMN. FINDS THE TILE YOU'RE LOOKING FOR
    tileindexC = COLLISION_WIDE_MAPWidth * index_y + indexCx;
    tileindexC9 = COLLISION_WIDE_MAPWidth * index_y + indexCx9;
    tileindexR = COLLISION_WIDE_MAPWidth * index_y + indexRx;
    tileindexkL = COLLISION_WIDE_MAPWidth * index_ky + indexLx;  // MULTIPLY THE WIDTH BY THE Y TILE TO FIND THE Y ROW. THEN ADD THE X TILE TO SHIFT THE COLUMN. FINDS THE TILE YOU'RE LOOKING FOR
    tileindexkC = COLLISION_WIDE_MAPWidth * index_ky + indexCx;
    tileindexkR = COLLISION_WIDE_MAPWidth * index_ky + indexRx;
    // IF THE CHARACTER IS STANDING UNDER AN 0x02 COLLISION CORNER AND CLIPS, ADJUST HIS X UNTIL HE IS SAFELY OUTSIDE OF THE COLLISION WALL OF TILES
    if (x_Adjust) {
        if ((PLAYER.SpdX < MAX_WALK_SPEED) && (PLAYER.SpdX > -MAX_WALK_SPEED)) {
            if ((COLLISION_WIDE_MAP[tileindexL] != 0x02) && (COLLISION_WIDE_MAP[tileindexR] == 0x02)) {
                PLAYER.SpdX = -MAX_CRAWL_SPEED;
            } else if ((COLLISION_WIDE_MAP[tileindexL] == 0x02) && (COLLISION_WIDE_MAP[tileindexR] != 0x02)) {
                PLAYER.SpdX = MAX_CRAWL_SPEED;
            }
        }
        if ((COLLISION_WIDE_MAP[tileindexL] == 0x00) && (COLLISION_WIDE_MAP[tileindexR] == 0x00)) {
            if (PLAYER.SpdX != MAX_WALK_SPEED && PLAYER.SpdX != -MAX_WALK_SPEED && (!(joy & J_LEFT)) && (!(joy & J_RIGHT))) {
                PLAYER.SpdX = 0;
            }
            x_Adjust = FALSE;
        }
    }
    if (PLAYER.SpdY > 0) {
        if ((COLLISION_WIDE_MAP[tileindexL] == 0x01) || (COLLISION_WIDE_MAP[tileindexC] == 0x01) || (COLLISION_WIDE_MAP[tileindexR] == 0x01) || (COLLISION_WIDE_MAP[tileindexL] == 0x03) || (COLLISION_WIDE_MAP[tileindexC] == 0x03) || (COLLISION_WIDE_MAP[tileindexR] == 0x03)) {
            if (!Drop) {
                UINT8 ty = (TO_PIXELS(PLAYER.y) / 8);
                PLAYER.y = TO_COORDS(ty * 8);
                PLAYER.SpdY = 0;
                Spawn = Jump = y_Collide = Gravity = FALSE;
                switch_land();
            }
        } else if ((COLLISION_WIDE_MAP[tileindexkL] == 0x04) || (COLLISION_WIDE_MAP[tileindexkC] == 0x04) || (COLLISION_WIDE_MAP[tileindexkR] == 0x04)) {
            GAMEOVER = TRUE;
        }
    } else if (PLAYER.SpdY < 0) {
        if ((COLLISION_WIDE_MAP[tileindexkL] == 0x01) || (COLLISION_WIDE_MAP[tileindexkC] == 0x01) || (COLLISION_WIDE_MAP[tileindexkR] == 0x01) || (y_Collide)) {
            PLAYER.SpdY = 0;
            Gravity = TRUE;
        } else if ((COLLISION_WIDE_MAP[tileindexL] == 0x04) || (COLLISION_WIDE_MAP[tileindexC] == 0x04) || (COLLISION_WIDE_MAP[tileindexR] == 0x04)) {
            GAMEOVER = TRUE;
        }
    } else if (PLAYER.SpdY == 0) {
        if ((COLLISION_WIDE_MAP[tileindexL] == 0x00) || (COLLISION_WIDE_MAP[tileindexC] == 0x00) || (COLLISION_WIDE_MAP[tileindexR] == 0x00) || (y_Collide)) {
            Gravity = TRUE;
        }
    }
    if ((COLLISION_WIDE_MAP[tileindexC] == 0x05) || (COLLISION_WIDE_MAP[tileindexC9] == 0x05)) {  // LADDER VERTICAL MOVEMENT
        if (((CHANGED_BUTTONS & J_UP) && (joy & J_UP)) || ((joy & J_DOWN) && (!Jump))) {
            if (!Ladder) {
                Ladder = TRUE;
                if ((COLLISION_WIDE_MAP[tileindexL] == 0x05)) {
                    UINT8 tx = (TO_PIXELS(PLAYER.x) / 8);
                    PLAYER.x = TO_COORDS(tx * 8);
                } else if ((COLLISION_WIDE_MAP[tileindexR] == 0x05)) {
                    UINT8 tx = (TO_PIXELS(PLAYER.x) / 8);
                    PLAYER.x = TO_COORDS((tx * 8) + 8);
                }
            }
        }
    } else {
        Ladder = FALSE;
    }
    if (Ladder) {
        if (joy & J_UP) {
            PLAYER.SpdY = -12;
        } else if (joy & J_DOWN) {
            PLAYER.SpdY = 12;
        }
    }
}
// TRY COMBINING THIS WITH CHECK_J BY ADDING A SWITCH WHEN PRESSING A BUTTON, TURNS OFF AFTER CHECK_J IN BOTH IF AND ELSE IF SECNARIOS
void check_J(UINT8 newplayerx, UINT8 newplayery, INT16 camera_x) {
    UINT16 indexLx, indexCx, indexRx, indexSLx, indexSCx, indexSRx, index_y, index_Cy, indexCamx, tileindexL, tileindexC, tileindexR, tileindexCL, tileindexCC, tileindexCR, tileindexSL, tileindexSC, tileindexSR;
    // CL = Crouch Left CC = Crouch Center CR = Crouch Right
    indexCamx = camera_x;

    indexLx = ((newplayerx - 16) + indexCamx) / 8;
    indexCx = ((newplayerx - 8) + indexCamx) / 8;
    indexRx = ((newplayerx - 1) + indexCamx) / 8;
    // STANDING x with a few pixels of forgiveness for 0x02 collision checks
    indexSLx = ((newplayerx - 10) + indexCamx) / 8;
    indexSCx = ((newplayerx - 9) + indexCamx) / 8;
    indexSRx = ((newplayerx - 5) + indexCamx) / 8;

    index_y = (newplayery - 1) / 8;
    index_Cy = (newplayery + 7) / 8;

    tileindexL = COLLISION_WIDE_MAPWidth * index_y + indexLx;  // MULTIPLY THE WIDTH BY THE Y TILE TO FIND THE Y ROW. THEN ADD THE X TILE TO SHIFT THE COLUMN. FINDS THE TILE YOU'RE LOOKING FOR
    tileindexC = COLLISION_WIDE_MAPWidth * index_y + indexCx;
    tileindexR = COLLISION_WIDE_MAPWidth * index_y + indexRx;

    tileindexSL = COLLISION_WIDE_MAPWidth * index_y + indexSLx;
    tileindexSC = COLLISION_WIDE_MAPWidth * index_y + indexSCx;
    tileindexSR = COLLISION_WIDE_MAPWidth * index_y + indexSRx;

    tileindexCL = COLLISION_WIDE_MAPWidth * index_Cy + indexLx;
    tileindexCC = COLLISION_WIDE_MAPWidth * index_Cy + indexCx;
    tileindexCR = COLLISION_WIDE_MAPWidth * index_Cy + indexRx;

    if (Crouch) {
        if (((COLLISION_WIDE_MAP[tileindexC] == 0x02) && (COLLISION_WIDE_MAP[tileindexR] == 0x02)) || ((COLLISION_WIDE_MAP[tileindexSC] == 0x02) && (COLLISION_WIDE_MAP[tileindexL] == 0x02)) || (COLLISION_WIDE_MAP[tileindexL] == 0x01) || (COLLISION_WIDE_MAP[tileindexC] == 0x01) || (COLLISION_WIDE_MAP[tileindexR] == 0x01) || (COLLISION_WIDE_MAP[tileindexCL] == 0x01) || (COLLISION_WIDE_MAP[tileindexCC] == 0x01) || (COLLISION_WIDE_MAP[tileindexCR] == 0x01)) {
        } else {
            if (!Drop) {
                Crouch = canCrouch = Launch = FALSE;
                if (!Jump) {
                    PLAYER.SpdY = JUMP_IMPULSE;
                    Jump = x_Adjust = TRUE;
                    Attach = FALSE;
                    switch_jump();
                }
            }
        }
    } else if (!Crouch) {
        // IF WALK SPEED IS LESS THAN MAX, MAKE HIS JUMP ABILITY ON CORNERS A BIT MORE RESTRICTED
        if ((PLAYER.SpdX < MAX_WALK_SPEED) && (PLAYER.SpdX > -MAX_WALK_SPEED)) {
            if (((COLLISION_WIDE_MAP[tileindexC] == 0x02) && (COLLISION_WIDE_MAP[tileindexR] == 0x02)) || ((COLLISION_WIDE_MAP[tileindexSC] == 0x02) && (COLLISION_WIDE_MAP[tileindexL] == 0x02)) || (COLLISION_WIDE_MAP[tileindexL] == 0x01) || (COLLISION_WIDE_MAP[tileindexC] == 0x01) || (COLLISION_WIDE_MAP[tileindexR] == 0x01)) {
            } else {
                if (!Drop) {
                    Crouch = canCrouch = Launch = FALSE;
                    if (!Jump) {
                        PLAYER.SpdY = JUMP_IMPULSE;
                        Jump = TRUE;
                        Attach = FALSE;
                        switch_jump();
                    }
                }
            }
            if ((COLLISION_WIDE_MAP[tileindexL] == 0x02) || (COLLISION_WIDE_MAP[tileindexR] == 0x02)) {
                x_Adjust = TRUE;
            }
        } else {  // IF WALK SPEED MAX, THEN ALLOW SOME LEEWAY ON WHEN HE IS ABLE TO JUMP OUT OF A CORNER (EXCEPT INTO THE CORNER FROM OUTSIDE)
            if (PLAYER.direction == DIR_RIGHT) {
                if (((COLLISION_WIDE_MAP[tileindexC] == 0x02) && (COLLISION_WIDE_MAP[tileindexR] == 0x02)) || ((COLLISION_WIDE_MAP[tileindexR] == 0x02) || (COLLISION_WIDE_MAP[tileindexL] == 0x01) || (COLLISION_WIDE_MAP[tileindexC] == 0x01) || (COLLISION_WIDE_MAP[tileindexR] == 0x01))) {
                } else {
                    if (!Drop) {
                        Crouch = canCrouch = Launch = FALSE;
                        if (!Jump) {
                            PLAYER.SpdY = JUMP_IMPULSE;
                            Jump = TRUE;
                            Attach = FALSE;
                            switch_jump();
                        }
                    }
                }
            }
            if (PLAYER.direction == DIR_LEFT) {
                if (((COLLISION_WIDE_MAP[tileindexC] == 0x02) && (COLLISION_WIDE_MAP[tileindexL] == 0x02)) || ((COLLISION_WIDE_MAP[tileindexL] == 0x02) || (COLLISION_WIDE_MAP[tileindexL] == 0x01) || (COLLISION_WIDE_MAP[tileindexC] == 0x01) || (COLLISION_WIDE_MAP[tileindexR] == 0x01))) {
                } else {
                    if (!Drop) {
                        Crouch = canCrouch = Launch = FALSE;
                        if (!Jump) {
                            PLAYER.SpdY = JUMP_IMPULSE;
                            Attach = FALSE;
                            Jump = TRUE;
                            switch_jump();
                        }
                    }
                }
            }
            if ((COLLISION_WIDE_MAP[tileindexL] == 0x02) || (COLLISION_WIDE_MAP[tileindexR] == 0x02)) {
                x_Adjust = TRUE;
            }
        }
    }
}

void check_Drop(UINT8 newplayerx, UINT8 newplayery, INT16 camera_x) {
    UINT16 indexLx, indexCx, indexRx, index_y, indexCamx, tileindexL, tileindexC, tileindexR;
    indexCamx = camera_x;
    indexLx = ((newplayerx - 16) + indexCamx) / 8;
    indexCx = ((newplayerx - 8) + indexCamx) / 8;
    indexRx = ((newplayerx - 1) + indexCamx) / 8;
    index_y = (newplayery - 1) / 8;

    tileindexL = COLLISION_WIDE_MAPWidth * index_y + indexLx;  // MULTIPLY THE WIDTH BY THE Y TILE TO FIND THE Y ROW. THEN ADD THE X TILE TO SHIFT THE COLUMN. FINDS THE TILE YOU'RE LOOKING FOR
    tileindexC = COLLISION_WIDE_MAPWidth * index_y + indexCx;
    tileindexR = COLLISION_WIDE_MAPWidth * index_y + indexRx;

    if ((COLLISION_WIDE_MAP[tileindexL] == 0x03) && (COLLISION_WIDE_MAP[tileindexR] != 0x01) || (COLLISION_WIDE_MAP[tileindexR] == 0x03) && (COLLISION_WIDE_MAP[tileindexL] != 0x01)) {
        Drop = Gravity = TRUE;
    }
}
// CHECK CROUCH
void check_C(UINT8 newplayerx, UINT8 newplayery, INT16 camera_x) {
    UINT16 indexSLx, indexSRx, index_y, indexCamx, tileindexSL, tileindexSR;

    indexCamx = camera_x;

    // STANDING VALUES (CHECK TO PUSH PLAYER LEFT OR RIGHT IF HEAD IS IN A COLLISION)
    indexSLx = ((newplayerx - 16) + indexCamx) / 8;
    indexSRx = ((newplayerx - 1) + indexCamx) / 8;
    index_y = (newplayery - 20) / 8;

    // STANDING
    tileindexSL = COLLISION_WIDE_MAPWidth * index_y + indexSLx;
    tileindexSR = COLLISION_WIDE_MAPWidth * index_y + indexSRx;

    if (Crouch) {
        if ((COLLISION_WIDE_MAP[tileindexSL] == 0x01) || (COLLISION_WIDE_MAP[tileindexSR] == 0x01)) {
        } else {
            Crouch = FALSE;
        }
    }
}

// LATER MOVE THIS TO A RENDER PORTION OF THE GAME AND REMOVE THE TILE #INCLUDES //
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