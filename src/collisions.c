#include "collisions.h"
// FOR SOME REASNON, SETTING PLAYER.direction or SetActorDirection causes inaccessible VRAM bug.
// Use switch(); functions instead
UINT8 Spawn, Ladder, Ladder_Release, Gravity, Jump, Crouch, canCrouch, Drop, x_Adjust, Launch, Shooting;
UINT8 canCrouch_timer, canCrouch_Ftimer, Drop_timer;

extern UINT8 joy, last_joy;
extern UINT8 Attach, x_Collide, y_Collide;
UINT8 LR_Offset_X, LR_Offset_kX;
UINT8 UD_Offset_Y, UD_Offset_kY, UD_Offset_LY;

// THESE COLLISIONS ARE SET ON SINGLE PIXELS, MEANING FINDING THE CENTER IS A CHALLENGE (AS OPPOSED TO NPC COLLISIONS WHICH ARE PERFECTLY CENTRED)
void check_LR(UINT8 newplayerx, UINT8 newplayery, INT16 camera_x) {
    UINT16 indexDy, indexCy, indexTy, index_x, index_kx, indexCamx, tileindexD, tileindexC, tileindexT, tileindexkD, tileindexkC, tileindexkT;
    ;
    if (joy & J_LEFT) {
        LR_Offset_X = 16;
        LR_Offset_kX = 14;
    } else if (joy & J_RIGHT) {
        LR_Offset_X = 1;
        LR_Offset_kX = 3;
    }
    // REPLACE THESE HARD CODED INDEXES WITH OFFSETS SIMILAR TO THE NPC COLLISION FUNC
    indexCamx = camera_x;
    indexTy = (newplayery - 24) / 8;  // TOP Y AXIS
    indexCy = (newplayery - 16) / 8;  // CENTER Y AXIS
    indexDy = (newplayery - 1) / 8;   // BOTTOM Y AXIS

    index_x = ((newplayerx - LR_Offset_X) + indexCamx) / 8;
    index_kx = ((newplayerx - LR_Offset_kX) + indexCamx) / 8;  // SPIKE KILL CHECK

    // REGULAR COLLISION INDEX
    tileindexT = COLLISION_WIDE_MAPWidth * indexTy + index_x;  // MULTIPLY THE WIDTH BY THE Y TILE TO FIND THE Y ROW. THEN ADD THE X TILE TO SHIFT THE COLUMN. FINDS THE TILE YOU'RE LOOKING FOR
    tileindexC = COLLISION_WIDE_MAPWidth * indexCy + index_x;
    tileindexD = COLLISION_WIDE_MAPWidth * indexDy + index_x;

    // KILL COLLISION INDEX
    tileindexkT = COLLISION_WIDE_MAPWidth * indexTy + index_kx;  // MULTIPLY THE WIDTH BY THE Y TILE TO FIND THE Y ROW. THEN ADD THE X TILE TO SHIFT THE COLUMN. FINDS THE TILE YOU'RE LOOKING FOR
    tileindexkC = COLLISION_WIDE_MAPWidth * indexCy + index_kx;
    tileindexkD = COLLISION_WIDE_MAPWidth * indexDy + index_kx;

    if (Crouch) {
        if ((COLLISION_WIDE_MAP[tileindexD] == 0x01) || (COLLISION_WIDE_MAP[tileindexC] == 0x01) || (COLLISION_WIDE_MAP[tileindexD] == 0x02) || (COLLISION_WIDE_MAP[tileindexC] == 0x02)) {
            if (!Jump) {
                PLAYER.SpdX = 0;
                switch_crawl();
            }
        }
        if ((COLLISION_WIDE_MAP[tileindexD] == 0x00) && (COLLISION_WIDE_MAP[tileindexC] == 0x00) && (COLLISION_WIDE_MAP[tileindexT] == 0x01)) {
            if (canCrouch_timer == 1) {
                canCrouch = TRUE;
                Crouch = TRUE;
                // canCrouch = FALSE;
                canCrouch_timer = 10;
            }
            canCrouch_timer -= 1;
        }
    } else if (!Crouch) {
        if ((COLLISION_WIDE_MAP[tileindexD] == 0x01) || (COLLISION_WIDE_MAP[tileindexC] == 0x01) || (COLLISION_WIDE_MAP[tileindexT] == 0x01) || (COLLISION_WIDE_MAP[tileindexD] == 0x02) || (COLLISION_WIDE_MAP[tileindexC] == 0x02) || (COLLISION_WIDE_MAP[tileindexT] == 0x02) || (x_Collide)) {
            if (!x_Adjust) {
                PLAYER.SpdX = 0;
                if (!Jump) {
                    switch_idle();
                }
            }
        }
        if ((COLLISION_WIDE_MAP[tileindexD] == 0x00) && (COLLISION_WIDE_MAP[tileindexC] == 0x00) && (COLLISION_WIDE_MAP[tileindexT] == 0x01)) {
            if (canCrouch_timer == 1) {
                canCrouch = TRUE;
                Crouch = TRUE;
                // canCrouch = FALSE;
                canCrouch_timer = 10;
            }
            canCrouch_timer -= 1;
        }
    }
    if ((COLLISION_WIDE_MAP[tileindexkD] == 0x04) || (COLLISION_WIDE_MAP[tileindexkC] == 0x04) || (COLLISION_WIDE_MAP[tileindexkT] == 0x04)) {
        GAMEOVER = TRUE;
    }
}
// TRY COMBINING THIS WITH CHECK_J BY ADDING A SWITCH WHEN PRESSING A BUTTON, TURNS OFF AFTER CHECK_J IN BOTH IF AND ELSE IF SECNARIOS
void check_UD(UINT8 newplayerx, UINT8 newplayery, INT16 camera_x) {
    UINT16 indexLx, indexCx, indexRx, index6, index10, index_y, index_Ty, index_By, index_ky, index_Ly, indexCamx, tileindexL, tileindexC, tileindexR, tileindexLT, tileindexCT, tileindexRT, tileindexLB, tileindexCB, tileindexRB, tileindexLL, tileindexCL, tileindexRL, tileindex6, tileindex10, tileindexkL, tileindexkC, tileindexkR;
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
    index_Ty = (newplayery - 26) / 8;
    index_By = (newplayery) / 8;
    index_ky = (newplayery - UD_Offset_kY) / 8;  // KILL SPIKE CHECK
    index_Ly = (newplayery - UD_Offset_LY) / 8;  // LADDER COLLISION CHECK
    // MULTIPLY THE WIDTH BY THE Y TILE TO FIND THE Y ROW. THEN ADD THE X TILE TO SHIFT THE COLUMN. FINDS THE TILE YOU'RE LOOKING FOR
    tileindexL = COLLISION_WIDE_MAPWidth * index_y + indexLx;  // THESE ARE USED MOSTLY FOR BASIC COLLISION CHECKS (NOT LADDERS)
    tileindexC = COLLISION_WIDE_MAPWidth * index_y + indexCx;  // AS THEY FLUCTUATE DEPENDING ON THE SPD OF THE PLAYER
    tileindexR = COLLISION_WIDE_MAPWidth * index_y + indexRx;  // LADDERS REQUIRE HARDSET TOP AND BOTTOM AND CENTER ROWS OF X, SEE BELOW

    tileindexLL = COLLISION_WIDE_MAPWidth * index_Ly + indexLx;  // LADDER CENTER Y
    tileindexCL = COLLISION_WIDE_MAPWidth * index_Ly + indexCx;  // LADDER CENTER Y
    tileindexRL = COLLISION_WIDE_MAPWidth * index_Ly + indexRx;  // LADDER CENTER Y

    tileindexLT = COLLISION_WIDE_MAPWidth * index_Ty + indexLx;  // TOP Y
    tileindexCT = COLLISION_WIDE_MAPWidth * index_Ty + indexCx;  // TOP Y
    tileindexRT = COLLISION_WIDE_MAPWidth * index_Ty + indexRx;  // TOP Y

    tileindexLB = COLLISION_WIDE_MAPWidth * index_By + indexLx;  // BOT Y
    tileindexCB = COLLISION_WIDE_MAPWidth * index_By + indexCx;  // BOT Y
    tileindexRB = COLLISION_WIDE_MAPWidth * index_By + indexRx;  // BOT Y

    tileindex6 = COLLISION_WIDE_MAPWidth * index_Ly + index6;    // OFFSET FOR LADDER X "CENTER"
    tileindex10 = COLLISION_WIDE_MAPWidth * index_Ly + index10;  // OFFSET FOR LADDER X "CENTER"

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
                Spawn = Jump = Ladder = y_Collide = Gravity = FALSE;
                Ladder_Release = TRUE;
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
    // ********** ALL LADDER CHECKS BELOW THIS LINE **********
    // CHECK IF PLAYER CAN SNAP TO THE LADDER WHEN PRESSING U/L U/R etc
    if (Ladder_Release) {
        if ((CHANGED_BUTTONS & J_UP) && (joy & J_UP) || (CHANGED_BUTTONS & J_DOWN) && (joy & J_DOWN) && (!Jump) || (COLLISION_WIDE_MAP[tileindexCL] != 0x05)) {
            Ladder_Release = FALSE;
        }
    }
    if (!Spawn) {
        // if ((!Ladder) && (!Ladder_Release)) {
        //     if ((COLLISION_WIDE_MAP[tileindexLB] == 0x05) && (COLLISION_WIDE_MAP[tileindexLT] != 0x05)) {  // IF YOU ARE STANDING ON TOP OF A LADDER (NOT JUMP UPWARDS FROM UNDERNEATH)
        //         if (joy & J_DOWN) {                                                                        // IF PRESS DOWN WHILE STANDING ON TOP OF A LADDER
        //             Ladder = TRUE;
        //         } else {
        //             UINT8 ty = (TO_PIXELS(PLAYER.y) / 8);
        //             PLAYER.y = TO_COORDS(ty * 8);
        //             PLAYER.SpdY = 0;
        //             Spawn = Jump = y_Collide = Gravity = FALSE;
        //             switch_land();
        //         }
        //     }
        // }

        if ((COLLISION_WIDE_MAP[tileindex6] == 0x05) && (COLLISION_WIDE_MAP[tileindexLL] == 0x05) || (COLLISION_WIDE_MAP[tileindex10] == 0x05) && (COLLISION_WIDE_MAP[tileindexRL] == 0x05) || (COLLISION_WIDE_MAP[tileindexC] == 0x05)) {  // LADDER VERTICAL MOVEMENT
            if ((joy & J_UP) || (joy & J_DOWN) && (!Jump) && (!Crouch)) {
                if (!Ladder) {
                    if (PLAYER.SpdX == 0) {  // prevents looping Ladder when you release with J_A
                        if ((CHANGED_BUTTONS & J_UP) && (joy & J_UP)) {
                            Ladder = TRUE;
                        }
                    } else {
                        if (!Ladder_Release) {
                            Ladder = TRUE;  // allows you to Ladder when walking up to the ladder while holding J_UP
                        }
                    }
                }
            } else {
                // Ladder = FALSE;
            }
        }
        if (Ladder) {
            // Crouch = FALSE;
            switch_idle();
            if ((COLLISION_WIDE_MAP[tileindexLL] == 0x05)) {
                UINT8 tx = (TO_PIXELS(PLAYER.x) / 8);
                PLAYER.x = TO_COORDS(tx * 8);
            } else if ((COLLISION_WIDE_MAP[tileindexRL] == 0x05)) {
                UINT8 tx = (TO_PIXELS(PLAYER.x) / 8);
                PLAYER.x = TO_COORDS((tx * 8) + 8);  // if on left tile of ladder
            }

            if (joy & J_UP) {
                PLAYER.SpdY = -12;
            } else if (joy & J_DOWN) {
                PLAYER.SpdY = 12;
            }
            PLAYER.SpdX = 0;
            // if ((COLLISION_WIDE_MAP[tileindexLT] != 0x05) && (COLLISION_WIDE_MAP[tileindexLB] != 0x05)) {
            //     UINT8 ty = ((TO_PIXELS(PLAYER.y) + 8) / 8);
            //     PLAYER.y = TO_COORDS(ty * 8);
            //     PLAYER.SpdY = 0;
            //     Spawn = Ladder = Jump = y_Collide = Gravity = FALSE;
            //     switch_idle();
            // } else if ((COLLISION_WIDE_MAP[tileindexLL] != 0x05) && (COLLISION_WIDE_MAP[tileindexLB] != 0x05)) {
            //     Ladder = FALSE;
            // }
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

    if (Ladder) {
        if (joy & J_LEFT) {
            PLAYER.SpdX = -MAX_WALK_SPEED;
            PLAYER.SpdY = -48;
        } else if (joy & J_RIGHT) {
            PLAYER.SpdX = MAX_WALK_SPEED;
            PLAYER.SpdY = -48;
        }
        Ladder = FALSE;
        Jump = Ladder_Release = TRUE;
        switch_jump();
    }

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