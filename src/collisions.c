#pragma bank 0
#include "collisions.h"

BYTE SPAWN, Gravity, JUMP, CROUCH, canCROUCH, DROP, x_Adjust;
BYTE L_LEFT, L_RIGHT;
BYTE canCROUCH_timer, canCROUCH_Ftimer, DROP_timer;
// Release_timer
extern UINT8 joy, last_joy;
extern BYTE ATTACH, x_Collide, y_Collide;
extern Variables bkg;
BYTE LR_Offset_X, LR_Offset_kX, UD_Offset_Y, UD_Offset_kY, UD_Offset_LY;
const UINT8 *COLLISION_DATA;
UINT8 COLLISION_BANK;
unsigned char COLLISION_WIDTH;

// ADD STAGE_DROP_MAP AS A VARIABLE IN THE FUNC CALL

// THESE COLLISIONS ARE SET ON SINGLE PIXELS, MEANING FINDING THE CENTER IS A CHALLENGE (AS OPPOSED TO NPC COLLISIONS WHICH ARE PERFECTLY CENTRED)
void check_LR(UINT8 newplayerx, UINT8 newplayery, INT16 camera_x) {
    UINT8 __save = _current_bank;
    SWITCH_ROM(COLLISION_BANK);
    UINT16 indexDy, indexCy, indexTy, index_x, index_kx, indexCamx, tileindexD, tileindexC, tileindexT, tileindexkD, tileindexkC, tileindexkT;

    if (joy & J_LEFT) {
        LR_Offset_X = 16;
        LR_Offset_kX = 14;
    } else if (joy & J_RIGHT) {
        LR_Offset_X = 1;
        LR_Offset_kX = 3;
    }
    // REPLACE THESE HARD CODED INDEXES WITH OFFSETS SIMILAR TO THE NPC COLLISION FUNCT
    indexCamx = camera_x;
    indexTy = (newplayery - 24) / 8;  // TOP Y AXIS
    indexCy = (newplayery - 16) / 8;  // CENTER Y AXIS
    indexDy = (newplayery - 1) / 8;   // BOTTOM Y AXIS

    index_x = ((newplayerx - LR_Offset_X) + indexCamx) / 8;
    index_kx = ((newplayerx - LR_Offset_kX) + indexCamx) / 8;  // SPIKE KILL CHECK

    // REGULAR COLLISION INDEX
    tileindexT = COLLISION_WIDTH * indexTy + index_x;  // MULTIPLY THE WIDTH BY THE Y TILE TO FIND THE Y ROW. THEN ADD THE X TILE TO SHIFT THE COLUMN. FINDS THE TILE YOU'RE LOOKING FOR
    tileindexC = COLLISION_WIDTH * indexCy + index_x;
    tileindexD = COLLISION_WIDTH * indexDy + index_x;

    // KILL COLLISION INDEX
    tileindexkT = COLLISION_WIDTH * indexTy + index_kx;  // MULTIPLY THE WIDTH BY THE Y TILE TO FIND THE Y ROW. THEN ADD THE X TILE TO SHIFT THE COLUMN. FINDS THE TILE YOU'RE LOOKING FOR
    tileindexkC = COLLISION_WIDTH * indexCy + index_kx;
    tileindexkD = COLLISION_WIDTH * indexDy + index_kx;

    if (CROUCH) {
        if ((COLLISION_DATA[tileindexD] == 0x01) || (COLLISION_DATA[tileindexC] == 0x01) || (COLLISION_DATA[tileindexD] == 0x02) || (COLLISION_DATA[tileindexC] == 0x02)) {
            if (!JUMP) {
                PLAYER.SpdX = 0;
                switch_crawl();
            }
        }
        if ((COLLISION_DATA[tileindexD] == 0x00) && (COLLISION_DATA[tileindexC] == 0x00) && (COLLISION_DATA[tileindexT] == 0x01)) {
            if (canCROUCH_timer == 1) {
                canCROUCH = TRUE;
                CROUCH = TRUE;
                canCROUCH_timer = 10;
            }
            canCROUCH_timer -= 1;
        }
    } else if (!CROUCH) {
        if ((COLLISION_DATA[tileindexD] == 0x01) || (COLLISION_DATA[tileindexC] == 0x01) || (COLLISION_DATA[tileindexT] == 0x01) || (COLLISION_DATA[tileindexD] == 0x02) || (COLLISION_DATA[tileindexC] == 0x02) || (COLLISION_DATA[tileindexT] == 0x02) || (x_Collide)) {
            if (!x_Adjust) {
                PLAYER.SpdX = 0;
                if (!JUMP) {
                    switch_idle();
                }
            }
        }
        if (!JUMP) {
            if ((COLLISION_DATA[tileindexD] == 0x00) && (COLLISION_DATA[tileindexC] == 0x00) && (COLLISION_DATA[tileindexT] == 0x01)) {
                if (canCROUCH_timer == 1) {
                    canCROUCH = TRUE;
                    CROUCH = TRUE;
                    canCROUCH_timer = 10;
                }
                canCROUCH_timer -= 1;
            }
        }
    }
    if ((COLLISION_DATA[tileindexkD] == 0x04) || (COLLISION_DATA[tileindexkC] == 0x04) || (COLLISION_DATA[tileindexkT] == 0x04)) {
        GAMEOVER = TRUE;
    }
    if ((COLLISION_DATA[tileindexC] == 0x08)) {
        EXIT1 = TRUE;
    } else if ((COLLISION_DATA[tileindexC] == 0x09)) {
        EXIT2 = TRUE;
    }
    SWITCH_ROM(__save);
}
// TRY COMBINING THIS WITH CHECK_J BY ADDING A SWITCH WHEN PRESSING A BUTTON, TURNS OFF AFTER CHECK_J IN BOTH IF AND ELSE IF SECNARIOS
void check_J(UINT8 newplayerx, UINT8 newplayery, INT16 camera_x) {
    UINT8 __save = _current_bank;
    SWITCH_ROM(COLLISION_BANK);
    UINT16 indexLx, indexCx, indexRx, indexLLx, indexRLx, indexSLx, indexSCx, indexSRx, index_y, index_Ty, index_Ly, index_Cy, indexCamx, tileindexL, tileindexC, tileindexR, tileindexLLT, tileindexRLT, tileindexLLC, tileindexRLC, tileindexCL, tileindexCC, tileindexCR, tileindexSL, tileindexSC, tileindexSR;
    // CL = CROUCH Left CC = CROUCH Center CR = CROUCH Right
    indexCamx = camera_x;

    indexLx = ((newplayerx - 16) + indexCamx) / 8;
    indexCx = ((newplayerx - 8) + indexCamx) / 8;
    indexRx = ((newplayerx - 1) + indexCamx) / 8;
    // LADDER LEFT/RIGHT CHECKS (1 PIXEL LEFT OF AND RIGHT OF PLAYER VISUAL)
    indexLLx = ((newplayerx - 17) + indexCamx) / 8;
    indexRLx = ((newplayerx) + indexCamx) / 8;
    // STANDING x with a few pixels of forgiveness for 0x02 collision checks
    indexSLx = ((newplayerx - 10) + indexCamx) / 8;
    indexSCx = ((newplayerx - 9) + indexCamx) / 8;
    indexSRx = ((newplayerx - 5) + indexCamx) / 8;

    index_y = (newplayery - 1) / 8;   // CHECKS COLLISION 1 PIXEL ABOVE PLAYER
    index_Ty = (newplayery - 3) / 8;  // CHECKS ABSOLUTE TOP PIXEL OF VISUAL PLAYER
    index_Ly = (newplayery + 4) / 8;  // CHECKS WHEN TO JUMP VERTICALLY WHILE ON LADDER (ie climbing near top and wanting to JUMP out of the LADDER)
    index_Cy = (newplayery + 7) / 8;

    tileindexL = COLLISION_WIDTH * index_y + indexLx;  // MULTIPLY THE WIDTH BY THE Y TILE TO FIND THE Y ROW. THEN ADD THE X TILE TO SHIFT THE COLUMN. FINDS THE TILE YOU'RE LOOKING FOR
    tileindexC = COLLISION_WIDTH * index_y + indexCx;
    tileindexR = COLLISION_WIDTH * index_y + indexRx;
    // LADDER LEFT/RIGHT
    tileindexLLT = COLLISION_WIDTH * index_Ty + indexLLx;
    tileindexRLT = COLLISION_WIDTH * index_Ty + indexRLx;

    tileindexLLC = COLLISION_WIDTH * index_Ly + indexLLx;
    tileindexRLC = COLLISION_WIDTH * index_Ly + indexRLx;
    // STANDING 0X02 FORGIVENESS
    tileindexSL = COLLISION_WIDTH * index_y + indexSLx;
    tileindexSC = COLLISION_WIDTH * index_y + indexSCx;
    tileindexSR = COLLISION_WIDTH * index_y + indexSRx;
    // CROUCH
    tileindexCL = COLLISION_WIDTH * index_Cy + indexLx;
    tileindexCC = COLLISION_WIDTH * index_Cy + indexCx;
    tileindexCR = COLLISION_WIDTH * index_Cy + indexRx;

    if (!LADDER) {  // RESET LADDER RELEASE IF JUMPING FROM THE GROUND TO ALLOW GRABBING WHEN SPDY = 0
        if (LADDER_Release) {
            LADDER_Release = FALSE;
        }
    } else {  // IF LADDER
        if (joy & J_LEFT) {
            if ((COLLISION_DATA[tileindexLLT] == 0x01) || (COLLISION_DATA[tileindexLLC] == 0x01)) {
            } else {
                PLAYER.SpdX = -MAX_WALK_SPEED;
                if (!(joy & J_DOWN)) {
                    PLAYER.SpdY = -52;
                }
            }
        } else if (joy & J_RIGHT) {
            if ((COLLISION_DATA[tileindexRLT] == 0x01) || (COLLISION_DATA[tileindexRLC] == 0x01)) {
            } else {
                PLAYER.SpdX = MAX_WALK_SPEED;
                if (!(joy & J_DOWN)) {
                    PLAYER.SpdY = -52;
                }
            }
        }
        LADDER = UP_LADDER = DOWN_LADDER = FALSE;
        JUMP = Gravity = LADDER_Release = TRUE;
        switch_jump();
    }

    if (CROUCH) {
        // THIS IS CAUSING A BUG WHEN YOU CROUCH JUMP INTO AN 0X01 WALL COLLISION. WE MUST RESTRICT THIS EVEN FURTHER
        if (((COLLISION_DATA[tileindexC] == 0x02) && (COLLISION_DATA[tileindexR] == 0x02)) || ((COLLISION_DATA[tileindexSC] == 0x02) && (COLLISION_DATA[tileindexL] == 0x02)) || (COLLISION_DATA[tileindexL] == 0x01) || (COLLISION_DATA[tileindexC] == 0x01) || (COLLISION_DATA[tileindexR] == 0x01) || (COLLISION_DATA[tileindexCL] == 0x01) || (COLLISION_DATA[tileindexCC] == 0x01) || (COLLISION_DATA[tileindexCR] == 0x01)) {
        } else {
            if (!DROP) {
                CROUCH = canCROUCH = FALSE;
                if (!JUMP) {
                    PLAYER.SpdY = JUMP_IMPULSE;
                    JUMP = x_Adjust = TRUE;
                    ATTACH = FALSE;
                    switch_jump();
                }
            }
        }
    } else if (!CROUCH) {
        // IF WALK SPEED IS LESS THAN MAX, MAKE HIS JUMP ABILITY ON CORNERS A BIT MORE RESTRICTED
        if ((PLAYER.SpdX < MAX_WALK_SPEED) && (PLAYER.SpdX > -MAX_WALK_SPEED)) {
            if (((COLLISION_DATA[tileindexC] == 0x02) && (COLLISION_DATA[tileindexR] == 0x02)) || ((COLLISION_DATA[tileindexSC] == 0x02) && (COLLISION_DATA[tileindexL] == 0x02)) || (COLLISION_DATA[tileindexL] == 0x01) || (COLLISION_DATA[tileindexC] == 0x01) || (COLLISION_DATA[tileindexR] == 0x01)) {
            } else {
                if (!DROP) {
                    CROUCH = canCROUCH = FALSE;
                    if (!JUMP) {
                        PLAYER.SpdY = JUMP_IMPULSE;
                        JUMP = TRUE;
                        ATTACH = FALSE;
                        switch_jump();
                    }
                }
            }
            if ((COLLISION_DATA[tileindexL] == 0x02) || (COLLISION_DATA[tileindexR] == 0x02)) {
                x_Adjust = TRUE;
            }
        } else {  // IF WALK SPEED MAX, THEN ALLOW SOME LEEWAY ON WHEN HE IS ABLE TO JUMP OUT OF A CORNER (EXCEPT INTO THE CORNER FROM OUTSIDE)
            if (PLAYER.direction == DIR_RIGHT) {
                if (((COLLISION_DATA[tileindexC] == 0x02) && (COLLISION_DATA[tileindexR] == 0x02)) || ((COLLISION_DATA[tileindexR] == 0x02) || (COLLISION_DATA[tileindexL] == 0x01) || (COLLISION_DATA[tileindexC] == 0x01) || (COLLISION_DATA[tileindexR] == 0x01))) {
                } else {
                    if (!DROP) {
                        CROUCH = canCROUCH = FALSE;
                        if (!JUMP) {
                            PLAYER.SpdY = JUMP_IMPULSE;
                            JUMP = TRUE;
                            ATTACH = FALSE;
                            switch_jump();
                        }
                    }
                }
            }
            if (PLAYER.direction == DIR_LEFT) {
                if (((COLLISION_DATA[tileindexC] == 0x02) && (COLLISION_DATA[tileindexL] == 0x02)) || ((COLLISION_DATA[tileindexL] == 0x02) || (COLLISION_DATA[tileindexL] == 0x01) || (COLLISION_DATA[tileindexC] == 0x01) || (COLLISION_DATA[tileindexR] == 0x01))) {
                } else {
                    if (!DROP) {
                        CROUCH = canCROUCH = FALSE;
                        if (!JUMP) {
                            PLAYER.SpdY = JUMP_IMPULSE;
                            ATTACH = FALSE;
                            JUMP = TRUE;
                            switch_jump();
                        }
                    }
                }
            }
            if ((COLLISION_DATA[tileindexL] == 0x02) || (COLLISION_DATA[tileindexR] == 0x02)) {
                x_Adjust = TRUE;
            }
        }
    }
    SWITCH_ROM(__save);
}
// TRY COMBINING THIS WITH CHECK_J BY ADDING A SWITCH WHEN PRESSING A BUTTON, TURNS OFF AFTER CHECK_J IN BOTH IF AND ELSE IF SECNARIOS
void check_UD(UINT8 newplayerx, UINT8 newplayery, INT16 camera_x) {
    UINT8 __save = _current_bank;
    SWITCH_ROM(COLLISION_BANK);
    UINT16 indexLx, indexCx, indexRx, index6, index10, index_y, index_By, index_ky, index_Ly, indexCamx, tileindexL, tileindexC, tileindexR, tileindexCB, tileindex6B, tileindex10B, tileindexLL, tileindexCL, tileindexRL, tileindex6, tileindex10, tileindexkL, tileindexkC, tileindexkR;
    indexCamx = camera_x;

    if (PLAYER.SpdY >= 0) {
        UD_Offset_Y = 0;
        UD_Offset_kY = 9;
        UD_Offset_LY = 12;
    } else if (PLAYER.SpdY < 0) {
        UD_Offset_Y = 26;
        UD_Offset_kY = 33;
        UD_Offset_LY = 12;
    }

    indexLx = ((newplayerx - 16) + indexCamx) / 8;
    indexCx = ((newplayerx - 8) + indexCamx) / 8;
    index10 = ((newplayerx - 10) + indexCamx) / 8;  // LADDER X CENTER OFFSETS
    index6 = ((newplayerx - 6) + indexCamx) / 8;    // LADDER X CENTER OFFSETS
    indexRx = ((newplayerx - 1) + indexCamx) / 8;
    index_y = (newplayery - UD_Offset_Y) / 8;
    index_By = (newplayery) / 8;
    index_ky = (newplayery - UD_Offset_kY) / 8;  // KILL SPIKE CHECK
    index_Ly = (newplayery - UD_Offset_LY) / 8;  // LADDER COLLISION CHECK
    UINT16 CWindex_y = COLLISION_WIDTH * index_y;
    // MULTIPLY THE WIDTH BY THE Y TILE TO FIND THE Y ROW. THEN ADD THE X TILE TO SHIFT THE COLUMN. FINDS THE TILE YOU'RE LOOKING FOR
    tileindexL = CWindex_y + indexLx;  // THESE ARE USED MOSTLY FOR BASIC COLLISION CHECKS (NOT LADDERS)
    tileindexC = CWindex_y + indexCx;  // AS THEY FLUCTUATE DEPENDING ON THE SPD OF THE PLAYER
    tileindexR = CWindex_y + indexRx;  // LADDERS REQUIRE HARDSET TOP AND BOTTOM AND CENTER ROWS OF X, SEE BELOW

    UINT16 CWindex_Ly = COLLISION_WIDTH * index_Ly;
    tileindexLL = CWindex_Ly + indexLx;  // LADDER CENTER Y
    tileindexCL = CWindex_Ly + indexCx;  // LADDER CENTER Y
    tileindexRL = CWindex_Ly + indexRx;  // LADDER CENTER Y

    UINT16 CWindex_By = COLLISION_WIDTH * index_By;
    // tileindexLB = CWindex_By + indexLx;  // BOT Y
    tileindexCB = CWindex_By + indexCx;  // BOT Y
    // tileindexRB = CWindex_By + indexRx;  // BOT Y
    tileindex6B = CWindex_By + index6;
    tileindex10B = CWindex_By + index10;

    tileindex6 = CWindex_Ly + index6;    // OFFSET FOR LADDER X "CENTER"
    tileindex10 = CWindex_Ly + index10;  // OFFSET FOR LADDER X "CENTER"

    UINT16 CWindex_ky = COLLISION_WIDTH * index_ky;
    tileindexkL = CWindex_ky + indexLx;  // MULTIPLY THE WIDTH BY THE Y TILE TO FIND THE Y ROW. THEN ADD THE X TILE TO SHIFT THE COLUMN. FINDS THE TILE YOU'RE LOOKING FOR
    tileindexkC = CWindex_ky + indexCx;
    tileindexkR = CWindex_ky + indexRx;
    // IF THE CHARACTER IS STANDING UNDER AN 0x02 COLLISION CORNER AND CLIPS, ADJUST HIS X UNTIL HE IS SAFELY OUTSIDE OF THE COLLISION WALL OF TILES
    if (x_Adjust) {
        if ((PLAYER.SpdX < MAX_WALK_SPEED) && (PLAYER.SpdX > -MAX_WALK_SPEED)) {
            if ((COLLISION_DATA[tileindexL] != 0x02) && (COLLISION_DATA[tileindexR] == 0x02)) {
                PLAYER.SpdX = -MAX_CRAWL_SPEED;
            } else if ((COLLISION_DATA[tileindexL] == 0x02) && (COLLISION_DATA[tileindexR] != 0x02)) {
                PLAYER.SpdX = MAX_CRAWL_SPEED;
            }
        }
        if ((COLLISION_DATA[tileindexL] != 0x02) && (COLLISION_DATA[tileindexR] != 0x02)) {
            if (PLAYER.SpdX != MAX_WALK_SPEED && PLAYER.SpdX != -MAX_WALK_SPEED && (!(joy & J_LEFT)) && (!(joy & J_RIGHT))) {
                PLAYER.SpdX = 0;
            }
            x_Adjust = FALSE;
        }
    }

    if (PLAYER.SpdY > 0) {
        if ((COLLISION_DATA[tileindexL] == 0x01) || (COLLISION_DATA[tileindexC] == 0x01) || (COLLISION_DATA[tileindexR] == 0x01) || (COLLISION_DATA[tileindexL] == 0x03) || (COLLISION_DATA[tileindexC] == 0x03) || (COLLISION_DATA[tileindexR] == 0x03) || (COLLISION_DATA[tileindex10B] == 0x06) && (!SPAWN) && (!LADDER) && (COLLISION_DATA[tileindexCL] != 0x05) || (COLLISION_DATA[tileindexCB] == 0x06) && (!SPAWN) && (!LADDER) && (COLLISION_DATA[tileindexCL] != 0x05) || (COLLISION_DATA[tileindex6B] == 0x06) && (!SPAWN) && (!LADDER) && (COLLISION_DATA[tileindexCL] != 0x05)) {
            if (!DROP) {
                UINT8 ty = (TO_PIXELS(PLAYER.y) / 8);
                PLAYER.y = TO_COORDS(ty * 8);
                PLAYER.SpdY = 0;
                SPAWN = JUMP = y_Collide = Gravity = FALSE;
                switch_land();
            }
        } else if ((COLLISION_DATA[tileindexkL] == 0x04) || (COLLISION_DATA[tileindexkC] == 0x04) || (COLLISION_DATA[tileindexkR] == 0x04)) {
            GAMEOVER = TRUE;
        }
    } else if (PLAYER.SpdY < 0) {
        if ((COLLISION_DATA[tileindexkL] == 0x01) || (COLLISION_DATA[tileindexkC] == 0x01) || (COLLISION_DATA[tileindexkR] == 0x01) || (y_Collide)) {
            PLAYER.SpdY = 0;
            Gravity = TRUE;
        } else if ((COLLISION_DATA[tileindexL] == 0x04) || (COLLISION_DATA[tileindexC] == 0x04) || (COLLISION_DATA[tileindexR] == 0x04)) {
            GAMEOVER = TRUE;
        }
    } else if (PLAYER.SpdY == 0) {
        if ((COLLISION_DATA[tileindexL] == 0x00) || (COLLISION_DATA[tileindexC] == 0x00) || (COLLISION_DATA[tileindexR] == 0x00) || (y_Collide)) {
            Gravity = TRUE;
        }
    }
    // check_LADDER(newplayerx, newplayery, camera_x);
    SWITCH_ROM(__save);
}

void check_Drop(UINT8 newplayerx, UINT8 newplayery, INT16 camera_x) {
    UINT8 __save = _current_bank;
    SWITCH_ROM(COLLISION_BANK);
    UINT16 indexLx, indexCx, indexRx, index_y, indexCamx, tileindexL, tileindexC, tileindexR;
    indexCamx = camera_x;
    indexLx = ((newplayerx - 16) + indexCamx) / 8;
    indexCx = ((newplayerx - 8) + indexCamx) / 8;
    indexRx = ((newplayerx - 1) + indexCamx) / 8;
    index_y = (newplayery - 1) / 8;

    tileindexL = COLLISION_WIDTH * index_y + indexLx;  // MULTIPLY THE WIDTH BY THE Y TILE TO FIND THE Y ROW. THEN ADD THE X TILE TO SHIFT THE COLUMN. FINDS THE TILE YOU'RE LOOKING FOR
    tileindexC = COLLISION_WIDTH * index_y + indexCx;
    tileindexR = COLLISION_WIDTH * index_y + indexRx;

    if ((COLLISION_DATA[tileindexL] == 0x03) && (COLLISION_DATA[tileindexR] != 0x01) || (COLLISION_DATA[tileindexR] == 0x03) && (COLLISION_DATA[tileindexL] != 0x01)) {
        DROP = Gravity = TRUE;
    }
    SWITCH_ROM(__save);
}
// CHECK CROUCH
void check_C(UINT8 newplayerx, UINT8 newplayery, INT16 camera_x) {
    UINT8 __save = _current_bank;
    SWITCH_ROM(COLLISION_BANK);
    UINT16 indexSLx, indexSRx, index_y, indexCamx, tileindexSL, tileindexSR;

    indexCamx = camera_x;

    // STANDING VALUES (CHECK TO PUSH PLAYER LEFT OR RIGHT IF HEAD IS IN A COLLISION)
    indexSLx = ((newplayerx - 16) + indexCamx) / 8;
    indexSRx = ((newplayerx - 1) + indexCamx) / 8;
    index_y = (newplayery - 20) / 8;

    // STANDING
    tileindexSL = COLLISION_WIDTH * index_y + indexSLx;
    tileindexSR = COLLISION_WIDTH * index_y + indexSRx;

    if (CROUCH) {
        if ((COLLISION_DATA[tileindexSL] == 0x01) || (COLLISION_DATA[tileindexSR] == 0x01)) {
        } else {
            CROUCH = FALSE;
        }
    }
    SWITCH_ROM(__save);
}

void check_world_collisions(UINT8 newplayerx, UINT8 newplayery, INT16 camera_x) {
    UINT8 __save = _current_bank;
    SWITCH_ROM(COLLISION_BANK);
    UINT16 indexTy, indexDy, index_Lx, index_Rx, indexCamx, tileindexTR, tileindexTL, tileindexDL, tileindexDR;  //
    // INSTEAD OF CHECKING T C D, CHECK TL, BL, TR, BR ONLY. HARD CODE THESE OFFSETS AND CHECK ALL 4 IN ANY DIRECTION
    // if (joy & J_LEFT) {
    //     LR_Offset_X = 12;
    // } else if (joy & J_RIGHT) {
    //     LR_Offset_X = 4;
    // }
    // REPLACE THESE HARD CODED INDEXES WITH OFFSETS SIMILAR TO THE NPC COLLISION FUNC
    indexCamx = camera_x;
    indexTy = (newplayery - 12) / 8;  // TOP Y AXIS
    indexDy = (newplayery - 10) / 8;  // BOTTOM Y AXIS

    index_Lx = ((newplayerx - 12) + indexCamx) / 8;
    index_Rx = ((newplayerx - 6) + indexCamx) / 8;

    // REGULAR COLLISION INDEX
    tileindexTL = COLLISION_WIDTH * indexTy + index_Lx;  // MULTIPLY THE WIDTH BY THE Y TILE TO FIND THE Y ROW. THEN ADD THE X TILE TO SHIFT THE COLUMN. FINDS THE TILE YOU'RE LOOKING FOR
    tileindexDL = COLLISION_WIDTH * indexDy + index_Lx;
    tileindexTR = COLLISION_WIDTH * indexTy + index_Rx;  // MULTIPLY THE WIDTH BY THE Y TILE TO FIND THE Y ROW. THEN ADD THE X TILE TO SHIFT THE COLUMN. FINDS THE TILE YOU'RE LOOKING FOR
    tileindexDR = COLLISION_WIDTH * indexDy + index_Rx;

    if ((COLLISION_DATA[tileindexDL] == 0x01) || (COLLISION_DATA[tileindexDR] == 0x01)) {
        PLAYER.SpdX = 0;
    }

    if ((COLLISION_DATA[tileindexTL] == 0x08) || (COLLISION_DATA[tileindexDR] == 0x08) || (COLLISION_DATA[tileindexDL] == 0x08) || (COLLISION_DATA[tileindexDR] == 0x08)) {
        EXIT1 = TRUE;
    }
    // else if ((COLLISION_DATA[tileindexT] == 0x09) || (COLLISION_DATA[tileindexC] == 0x09) || (COLLISION_DATA[tileindexD] == 0x09)) {
    //     EXIT2 = TRUE;
    // }
    SWITCH_ROM(__save);
}

// LATER MOVE THIS TO A RENDER PORTION OF THE GAME AND REMOVE THE TILE #INCLUDES //
BYTE overlap(INT16 r1_y, INT16 r1_x, INT16 l1_y, INT16 l1_x, INT16 r2_y, INT16 r2_x, INT16 l2_y, INT16 l2_x) {  // BYTE IS SAME AS BOOLEAN (ONLY SHORTER NAME)
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
// IF YOU COLLIDE WITH AN EXIT DOOR, THEN SET GAMESTATE TO THAT PARTICULAR STAGE
// IF COLLIDE WITH DOOR
// GAMESTATE = NEXT STAGE