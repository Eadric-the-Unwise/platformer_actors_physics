#pragma bank 255
#include "ladder.h"

extern UINT8 joy, last_joy;
extern UINT8 ATTACH, x_Collide, y_Collide;

void check_LADDER (UINT8 newplayerx, UINT8 newplayery, INT16 camera_x) {
    UINT16 indexLx, indexCx, indexRx, index6, index10, index_y, index_Ty, index_By, index_ky, index_Ly, indexCamx, tileindexL, tileindexC, tileindexR, tileindexLT, tileindexCT, tileindexRT, tileindexLB, tileindexCB, tileindexRB, tileindex6B, tileindex10B, tileindexLL, tileindexCL, tileindexRL, tileindex6, tileindex10, tileindexkL, tileindexkC, tileindexkR;
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
    tileindex6B = COLLISION_WIDE_MAPWidth * index_By + index6;
    tileindex10B = COLLISION_WIDE_MAPWidth * index_By + index10;

    tileindex6 = COLLISION_WIDE_MAPWidth * index_Ly + index6;    // OFFSET FOR LADDER X "CENTER"
    tileindex10 = COLLISION_WIDE_MAPWidth * index_Ly + index10;  // OFFSET FOR LADDER X "CENTER"

    tileindexkL = COLLISION_WIDE_MAPWidth * index_ky + indexLx;  // MULTIPLY THE WIDTH BY THE Y TILE TO FIND THE Y ROW. THEN ADD THE X TILE TO SHIFT THE COLUMN. FINDS THE TILE YOU'RE LOOKING FOR
    tileindexkC = COLLISION_WIDE_MAPWidth * index_ky + indexCx;
    tileindexkR = COLLISION_WIDE_MAPWidth * index_ky + indexRx;

    // ********** ALL LADDER CHECKS BELOW THIS LINE **********
    // CHECK IF PLAYER CAN SNAP TO THE LADDER WHEN PRESSING U/L U/R etc
    if (!SPAWN) {
    if (LADDER_Release) {
        if ((CHANGED_BUTTONS & J_UP) && (joy & J_UP) || (CHANGED_BUTTONS & J_DOWN) && (joy & J_DOWN) && (!JUMP) || (COLLISION_WIDE_MAP[tileindexCL] != 0x05) && (!JUMP)) {
            LADDER_Release = FALSE;
        }
    }

        if (!LADDER) {

                        // IF PRESS DOWN WHILE STANDING ON TOP OF LADDER, DESCEND LADDER
            if ((COLLISION_WIDE_MAP[tileindex6B] == 0x06) && (COLLISION_WIDE_MAP[tileindexL] == 0x06) || (COLLISION_WIDE_MAP[tileindex10B] == 0x06) && (COLLISION_WIDE_MAP[tileindexR] == 0x06)) {
                if ((joy & J_DOWN) && (!JUMP)) {
                    // ladder();
                    LEFT_RIGHT();
                    LADDER = TRUE;
                    ONTO_Ladder = TRUE;
                    ONTO_Ladder_timer = 12; //2 animation frames (DOWN, then ONTO_LADDER)
                }
            }
            
            // LADDER VERTICAL MOVEMENT
            if ((COLLISION_WIDE_MAP[tileindex6] == 0x05) && (COLLISION_WIDE_MAP[tileindexLL] == 0x05) || (COLLISION_WIDE_MAP[tileindex10] == 0x05) && (COLLISION_WIDE_MAP[tileindexRL] == 0x05)) {
                if (JUMP) {
                    if (PLAYER.SpdX == 0) {
                        if (PLAYER.SpdY == 0 || (CHANGED_BUTTONS & J_UP) && (joy & J_UP)) {
                            if (!LADDER_Release) {
                                ladder();
                            }
                        }
                    } else if (PLAYER.SpdX != 0 || (CHANGED_BUTTONS & J_UP) && (joy & J_UP)) {
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
                        if ((COLLISION_WIDE_MAP[tileindexCT] == 0x07) || (COLLISION_WIDE_MAP[tileindexCT] == 0x06) || (COLLISION_WIDE_MAP[tileindexCB] == 0x06)) {
                            if (joy & J_UP) {
                                ladder();
                            }
                        } else if (COLLISION_WIDE_MAP[tileindexCB] != 0x06) {
                            ladder();
                        }
                    }
                }
            }

        }
        if (LADDER) {
            UINT8 tx = (TO_PIXELS(PLAYER.x) / 8);
            if ((COLLISION_WIDE_MAP[tileindex6] == 0x05) && (COLLISION_WIDE_MAP[tileindexLL] == 0x05) || (COLLISION_WIDE_MAP[tileindexLB] == 0x06)) {
                PLAYER.x = TO_COORDS(tx * 8);
            } else if ((COLLISION_WIDE_MAP[tileindex10] == 0x05) && (COLLISION_WIDE_MAP[tileindexRL] == 0x05) || (COLLISION_WIDE_MAP[tileindexRB] == 0x06)) {
                PLAYER.x = TO_COORDS((tx * 8) + 8);  // if on left side of LADDER
            }

            if (COLLISION_WIDE_MAP[tileindexCB] == 0x06 && COLLISION_WIDE_MAP[tileindexCT] == 0x00) {
                if ((!ONTO_Ladder) && (!DOWN_LADDER)) {
                OFF_Ladder = TRUE; //ADD OFF LADDER CODE HERE
                }
            }
            if (ONTO_Ladder){
                CROUCH = FALSE;
                switch_onto_ladder();
                 ONTO_Ladder_timer -= 1;

                if (ONTO_Ladder_timer == 0){
                    PLAYER.y += TO_COORDS(8);
                    // ONTO_Ladder_timer = 1;
                    // DOWN_LADDER_timer = 8;
                    DOWN_LADDER = TRUE;
                    ONTO_Ladder = FALSE;
                }

            } else if (OFF_Ladder){
                    // ONTO_Ladder_timer = 32;
                    UINT8 tiley = ((TO_PIXELS(PLAYER.y)) / 8);
                    PLAYER.y = TO_COORDS((tiley * 8) - 4);
                    // PLAYER.SpdY = 0;
                    OFF_Ladder = SPAWN = LADDER = JUMP = y_Collide = Gravity = FALSE;
            }  
            if (DOWN_LADDER){
                    PLAYER.SpdY = 12;
                    switch_ladder();
                    // DOWN_LADDER_timer -= 1;
                    // if (DOWN_LADDER_timer == 0){
                    // DOWN_LADDER_timer = 1;
                    // DOWN_LADDER = FALSE;
                    // ladder();
                    // switch_ladder();
                    // }
                if ((COLLISION_WIDE_MAP[tileindexCB] == 0x00) || (COLLISION_WIDE_MAP[tileindexCB] == 0x05)) {
                        ONTO_Ladder = FALSE;
                        DOWN_LADDER = FALSE;
                            // LADDER = TRUE;
                            switch_ladder();
                }
            }
                
            if ((!ONTO_Ladder) && (!DOWN_LADDER)){
                // CROUCH = FALSE;
                if (PLAYER.SpdY == 0 && PLAYER.patrol_timer > 4){ //PREVENTS UP DOWN SPAM OF ANIMATION
                    PLAYER.patrol_timer = 4;
                }
            
                if (joy & J_UP) {
                        PLAYER.SpdY = -12;
                        switch_ladder();
                } else if (joy & J_DOWN) {
                        PLAYER.SpdY = 12;
                        switch_ladder();
                }
                if (joy & J_LEFT) {
                    LEFT = TRUE;
                    RIGHT = FALSE;
                    if (!(joy & J_UP) && !(joy & J_DOWN)) {
                        PLAYER.direction = DIR_LADDER_L;
                        PLAYER.patrol_timer = 1;
                    }
                } else if (joy & J_RIGHT) {
                    LEFT = FALSE;
                    RIGHT = TRUE;
                    if (!(joy & J_UP) && !(joy & J_DOWN)) {
                        PLAYER.direction = DIR_LADDER_R;
                        PLAYER.patrol_timer = 1;
                    }
                }
            }
            
            PLAYER.SpdX = 0;



            // REACH TOP OF LADDER, SWITCH TO STANDING ON TOP OF LADDER
            // if (PLAYER.SpdY <= 0) {    //ADD THE LADDER CLIMB ONTO/OFF HERE
            //     // if (COLLISION_WIDE_MAP[tileindexCB] == 0x06 && COLLISION_WIDE_MAP[tileindexCT] == 0x00) {
            //     //     if (!ONTO_Ladder) {
            //     //     ONTO_Ladder = TRUE;
            //     //     ONTO_Ladder_timer = 1;
            //     //     }
            //     // }
            // //REACH BOTTOM OF LADDER AND DROP OFF OF THE LADDER
            // } else 
            if (PLAYER.SpdY > 0) {
                if (COLLISION_WIDE_MAP[tileindexCB] == 0x00) {
                        LADDER = FALSE;
                        JUMP = Gravity = LADDER_Release = TRUE;
                        switch_jump();
                }
            }
        }
             else if (!LADDER) {
                if ((LEFT) || (RIGHT)) {
                    if ((joy & J_LEFT) || (joy & J_RIGHT)) {
                        LEFT = FALSE;
                        RIGHT = FALSE;
                    }
                }
            }
    }
}