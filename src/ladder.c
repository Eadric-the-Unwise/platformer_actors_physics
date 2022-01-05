#pragma bank 255
#include "ladder.h"

extern UINT8 joy, last_joy;
extern UINT8 ATTACH, x_Collide, y_Collide;
UINT8 ONTO_Ladder_timer, DOWN_LADDER_timer, UP_LADDER_timer, OFF_LADDER_timer;
UINT8 LADDER, ONTO_Ladder, DOWN_LADDER, DOWN_LADDER_F, UP_LADDER, OFF_LADDER, LADDER_Release;

void check_LADDER(UINT8 newplayerx, UINT8 newplayery, INT16 camera_x) {
    UINT16 indexLx, indexCx, indexRx, index6, index10, index_y, index_Ty, index_By, index_Ly, indexCamx, tileindexL, tileindexC, tileindexR, tileindexCT, tileindexLB, tileindexCB, tileindexRB, tileindex6B, tileindex10B, tileindexLL, tileindexCL, tileindexRL, tileindex6, tileindex10;
    indexCamx = camera_x;

    if (PLAYER.SpdY >= 0) {
        UD_Offset_Y = 0;
        UD_Offset_kY = 9;
        UD_Offset_LY = 10;

    } else if (PLAYER.SpdY < 0) {
        UD_Offset_Y = 26;
        UD_Offset_kY = 33;
        UD_Offset_LY = 14;
    }

    indexLx = ((newplayerx - 16) + indexCamx) / 8;
    indexCx = ((newplayerx - 8) + indexCamx) / 8;
    index10 = ((newplayerx - 10) + indexCamx) / 8;  // LADDER X CENTER OFFSETS
    index6 = ((newplayerx - 6) + indexCamx) / 8;    // LADDER X CENTER OFFSETS
    indexRx = ((newplayerx - 1) + indexCamx) / 8;
    index_y = (newplayery - UD_Offset_Y) / 8;
    index_Ty = (newplayery - 26) / 8;
    index_By = (newplayery) / 8;
    index_Ly = (newplayery - UD_Offset_LY) / 8;  // LADDER COLLISION CHECKS
    // MULTIPLY THE WIDTH BY THE Y TILE TO FIND THE Y ROW. THEN ADD THE X TILE TO SHIFT THE COLUMN. FINDS THE TILE YOU'RE LOOKING FOR
    tileindexL = STAGE_DROP_COLLISIONWidth * index_y + indexLx;  // THESE ARE USED MOSTLY FOR BASIC COLLISION CHECKS (NOT LADDERS)
    tileindexC = STAGE_DROP_COLLISIONWidth * index_y + indexCx;  // AS THEY FLUCTUATE DEPENDING ON THE SPD OF THE PLAYER
    tileindexR = STAGE_DROP_COLLISIONWidth * index_y + indexRx;  // LADDERS REQUIRE HARDSET TOP AND BOTTOM AND CENTER ROWS OF X, SEE BELOW

    tileindexLL = STAGE_DROP_COLLISIONWidth * index_Ly + indexLx;  // LADDER CENTER Y
    tileindexCL = STAGE_DROP_COLLISIONWidth * index_Ly + indexCx;  // LADDER CENTER Y
    tileindexRL = STAGE_DROP_COLLISIONWidth * index_Ly + indexRx;  // LADDER CENTER Y

    tileindexCT = STAGE_DROP_COLLISIONWidth * index_Ty + indexCx;  // TOP Y

    tileindexLB = STAGE_DROP_COLLISIONWidth * index_By + indexLx;  // BOT Y
    tileindexCB = STAGE_DROP_COLLISIONWidth * index_By + indexCx;  // BOT Y
    tileindexRB = STAGE_DROP_COLLISIONWidth * index_By + indexRx;  // BOT Y
    tileindex6B = STAGE_DROP_COLLISIONWidth * index_By + index6;
    tileindex10B = STAGE_DROP_COLLISIONWidth * index_By + index10;

    tileindex6 = STAGE_DROP_COLLISIONWidth * index_Ly + index6;    // OFFSET FOR LADDER X "CENTER"
    tileindex10 = STAGE_DROP_COLLISIONWidth * index_Ly + index10;  // OFFSET FOR LADDER X "CENTER"

    // ********** ALL LADDER CHECKS BELOW THIS LINE **********
    // CHECK IF PLAYER CAN SNAP TO THE LADDER WHEN PRESSING U/L U/R etc
    if (!SPAWN) {
        if (LADDER_Release) {
            if ((CHANGED_BUTTONS & J_UP) && (joy & J_UP) || (CHANGED_BUTTONS & J_DOWN) && (joy & J_DOWN) && (!JUMP) || (STAGE_DROP_COLLISION[tileindexCL] != 0x05) && (!JUMP)) {
                LADDER_Release = FALSE;
            }
        }

        if (!LADDER) {
            if ((L_LEFT) || (L_RIGHT)) {
                if ((joy & J_LEFT) || (joy & J_RIGHT)) {
                    L_LEFT = FALSE;
                    L_RIGHT = FALSE;
                }
            }
            // LADDER VERTICAL MOVEMENT
            if ((STAGE_DROP_COLLISION[tileindex6] == 0x05) && (STAGE_DROP_COLLISION[tileindexLL] == 0x05) || (STAGE_DROP_COLLISION[tileindex10] == 0x05) && (STAGE_DROP_COLLISION[tileindexRL] == 0x05) || (STAGE_DROP_COLLISION[tileindex6] == 0x07) && (STAGE_DROP_COLLISION[tileindexLL] == 0x07) || (STAGE_DROP_COLLISION[tileindex10] == 0x07) && (STAGE_DROP_COLLISION[tileindexRL] == 0x07)) {
                if (JUMP) {
                    if (PLAYER.SpdX == 0) {
                        if (PLAYER.SpdY == 0 || (CHANGED_BUTTONS & J_UP) && (joy & J_UP)) {
                            if (!LADDER_Release) {
                                ladder();
                            }
                        }
                    } else if (PLAYER.SpdX != 0 && (joy & J_UP)) {
                        if (!LADDER_Release) {
                            if (PLAYER.SpdY < 0) {
                                PLAYER.SpdY = -16;  //-JUMP_IMPULSE / 4
                            } else if (PLAYER.SpdY > 0) {
                                PLAYER.SpdY = 16;  //-JUMP_IMPULSE / 4
                            }
                            ladder();
                        }
                    }
                } else if ((!JUMP) && (!CROUCH)) {
                    if (!LADDER_Release) {
                        if ((STAGE_DROP_COLLISION[tileindexCT] == 0x07) || (STAGE_DROP_COLLISION[tileindexCT] == 0x05) || (STAGE_DROP_COLLISION[tileindexCT] == 0x06) || (STAGE_DROP_COLLISION[tileindexCB] == 0x06)) {
                            if (joy & J_UP) {
                                ladder();
                            }
                        }
                        // else if (STAGE_DROP_COLLISION[tileindexCB] != 0x06) {
                        //     ladder();
                        // }
                    }
                }
            }
            // IF PRESS DOWN WHILE STANDING ON TOP OF LADDER, DESCEND LADDER
            if ((STAGE_DROP_COLLISION[tileindex6B] == 0x06) && (STAGE_DROP_COLLISION[tileindexL] == 0x06) || (STAGE_DROP_COLLISION[tileindex10B] == 0x06) && (STAGE_DROP_COLLISION[tileindexR] == 0x06)) {
                if ((joy & J_DOWN) && (!JUMP)) {
                    // ladder();
                    LEFT_RIGHT();
                    LADDER = TRUE;
                    ONTO_Ladder = TRUE;
                    ONTO_Ladder_timer = 15;  // 3 animation frames (DOWN, then ONTO_LADDER, ONTO_LADDER)
                    // animation_timer = 3;
                }
            }

        } else if (LADDER) {
            UINT8 tx = (TO_PIXELS(PLAYER.x) / 8);
            if ((STAGE_DROP_COLLISION[tileindexLL] == 0x05) || (STAGE_DROP_COLLISION[tileindexLL] == 0x07) || (STAGE_DROP_COLLISION[tileindexLB] == 0x06)) {
                PLAYER.x = TO_COORDS(tx * 8);
            } else if ((STAGE_DROP_COLLISION[tileindexRL] == 0x05) || (STAGE_DROP_COLLISION[tileindexRL] == 0x07) || (STAGE_DROP_COLLISION[tileindexRB] == 0x06)) {
                PLAYER.x = TO_COORDS((tx * 8) + 8);  // if on left side of LADDER
            }

            if (STAGE_DROP_COLLISION[tileindexCL] == 0x06 && STAGE_DROP_COLLISION[tileindexCT] == 0x00) {
                if ((!ONTO_Ladder) && (!DOWN_LADDER)) {
                    UP_LADDER = TRUE;  // ADD OFF LADDER CODE HERE
                    UP_LADDER_timer = 1;
                    PLAYER.patrol_timer = 2;
                }
            }
            if (ONTO_Ladder) {
                switch_onto_ladder();
                CROUCH = FALSE;
                ONTO_Ladder_timer -= 1;
                if (ONTO_Ladder_timer == 9) {
                    PLAYER.y += TO_COORDS(5);
                }

                if (ONTO_Ladder_timer == 1) {
                    PLAYER.y += TO_COORDS(13);
                    ONTO_Ladder = FALSE;
                    DOWN_LADDER = TRUE;
                    DOWN_LADDER_timer = 8;
                    PLAYER.patrol_timer = 4;
                    // switch_ladder();
                }
            }
            if (DOWN_LADDER) {
                PLAYER.SpdY = 10;
                switch_ladder();
                DOWN_LADDER_timer -= 1;

                if (DOWN_LADDER_timer == 0) {
                    ONTO_Ladder = FALSE;
                    DOWN_LADDER = FALSE;

                    DOWN_LADDER_F = TRUE;
                    PLAYER.SpdY = 0;
                }
            }
            if (UP_LADDER) {
                // ONTO_Ladder_timer = 32;
                // UINT8 tiley = ((TO_PIXELS(PLAYER.y)) / 8);
                // PLAYER.y = TO_COORDS((tiley * 8) - 4);
                // PLAYER.SpdY = 0;
                // UP_LADDER = SPAWN = LADDER = JUMP = y_Collide = Gravity = FALSE;
                SPAWN = JUMP = y_Collide = Gravity = FALSE;
                PLAYER.SpdY = -10;
                switch_ladder();
                // PREVENT PLAYER FROM FLICKERING OFF LADDER IF FACING THE WRONG DIRECTION
                // if (LEFT) {
                //     if (PLAYER.direction == DIR_LADDER_R && PLAYER.patrol_timer < 3) {
                //         // PLAYER.patrol_timer = 4;
                //         SetActorDirection(&PLAYER, DIR_LADDER_L, 0);
                //     }
                // } else if (RIGHT) {
                //     SetActorDirection(&PLAYER, DIR_LADDER_R, 0);
                // }

                if (UP_LADDER_timer == 0) {
                    UP_LADDER = FALSE;
                    PLAYER.SpdY = 0;
                    PLAYER.y -= TO_COORDS(8);  // 9
                    // switch_off_ladder();
                    OFF_LADDER = TRUE;
                    OFF_LADDER_timer = 12;
                }
                UP_LADDER_timer -= 1;
            }
            if (OFF_LADDER) {
                OFF_LADDER_timer -= 1;
                switch_off_ladder();
                if (OFF_LADDER_timer == 0) {
                    PLAYER.y -= TO_COORDS(5);
                    OFF_LADDER = LADDER = FALSE;
                    PLAYER.SpdY = -12;
                }
            }

            if ((ONTO_Ladder) || (DOWN_LADDER) || (UP_LADDER) || (OFF_LADDER)) {
            } else {  // CROUCH = FALSE;
                if ((DOWN_LADDER_F) && (CHANGED_BUTTONS & J_DOWN) && (joy & J_DOWN)) {
                    PLAYER.patrol_timer = 1;
                    DOWN_LADDER_F = FALSE;
                }

                if (joy & J_UP) {
                    PLAYER.SpdY = -12;
                    switch_ladder();
                } else if (joy & J_DOWN) {
                    PLAYER.SpdY = 12;
                    switch_ladder();
                }
                if (joy & J_LEFT) {
                    L_LEFT = TRUE;
                    L_RIGHT = FALSE;
                    if (!(joy & J_UP) && !(joy & J_DOWN)) {
                        PLAYER.direction = DIR_LADDER_L;
                        PLAYER.patrol_timer = 1;
                    }
                } else if (joy & J_RIGHT) {
                    L_LEFT = FALSE;
                    L_RIGHT = TRUE;
                    if (!(joy & J_UP) && !(joy & J_DOWN)) {
                        PLAYER.direction = DIR_LADDER_R;
                        PLAYER.patrol_timer = 1;
                    }
                }
                if (PLAYER.SpdY == 0) {  // PREVENTS UP DOWN SPAM OF ANIMATION
                    PLAYER.patrol_timer = 4;
                }
                if ((DOWN_LADDER_F) && (PLAYER.SpdY != 0)) {
                    DOWN_LADDER_F = FALSE;
                }
            }
            PLAYER.SpdX = 0;
            if (PLAYER.SpdY > 0) {
                if (STAGE_DROP_COLLISION[tileindexCB] == 0x00) {
                    LADDER = FALSE;
                    JUMP = Gravity = LADDER_Release = TRUE;
                    switch_jump();
                }
            }
        }
    }
}
void ladder() {
    // PLAYER.SpdX = 0;
    LADDER = TRUE;
    // JUMP = FALSE;
    LEFT_RIGHT();
    switch_ladder();
}