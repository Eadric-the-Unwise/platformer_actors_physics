#include "collisions.h"

UBYTE Spawn, Jump, Crouch, canCrouch, Drop, x_Adjust, Launch, Shooting;
UBYTE canCrouch_timer, canCrouch_Ftimer, Drop_timer;

extern UBYTE joy;

void check_LR(UBYTE newplayerx, UBYTE newplayery, INT16 camera_x, UBYTE *collision_map, UBYTE mapWidth) {
    UINT16 indexDy, indexCy, indexTy, index_Lx, index_Rx, indexCamx, tileindexLD, tileindexLC, tileindexLT, tileindexRD, tileindexRC, tileindexRT;

    indexCamx = camera_x;
    indexDy = (newplayery - 1) / 8;
    indexCy = (newplayery - 16) / 8;
    indexTy = (newplayery - 24) / 8;
    index_Lx = ((newplayerx - 16) + indexCamx) / 8;
    index_Rx = ((newplayerx - 1) + indexCamx) / 8;
    //LEFT INDEX
    tileindexLD = mapWidth * indexDy + index_Lx;  //MULTIPLY THE WIDTH BY THE Y TILE TO FIND THE Y ROW. THEN ADD THE X TILE TO SHIFT THE COLUMN. FINDS THE TILE YOU'RE LOOKING FOR
    tileindexLC = mapWidth * indexCy + index_Lx;
    tileindexLT = mapWidth * indexTy + index_Lx;
    //RIGHT INDEX
    tileindexRD = mapWidth * indexDy + index_Rx;  //MULTIPLY THE WIDTH BY THE Y TILE TO FIND THE Y ROW. THEN ADD THE X TILE TO SHIFT THE COLUMN. FINDS THE TILE YOU'RE LOOKING FOR
    tileindexRC = mapWidth * indexCy + index_Rx;
    tileindexRT = mapWidth * indexTy + index_Rx;

    if (Crouch) {
        if ((collision_map[tileindexLD] == 0x01) || (collision_map[tileindexLC] == 0x01) || (collision_map[tileindexRD] == 0x01) || (collision_map[tileindexRC] == 0x01)) {
            if (!Jump) {
                PLAYER.SpdX = 0;
                switch_crawl();
            }
        }
        if ((collision_map[tileindexLD] == 0x00) && (collision_map[tileindexLC] == 0x00) && (collision_map[tileindexLT] == 0x01) || (collision_map[tileindexRD] == 0x00) && (collision_map[tileindexRC] == 0x00) && (collision_map[tileindexRT] == 0x01)) {
            if (canCrouch_timer == 1) {
                canCrouch = TRUE;
                Crouch = TRUE;
                // canCrouch = FALSE;
                canCrouch_timer = 10;
            }
            canCrouch_timer -= 1;
        }
    } else if (!Crouch) {
        if ((collision_map[tileindexLD] == 0x01) || (collision_map[tileindexLC] == 0x01) || (collision_map[tileindexLT] == 0x01) || (collision_map[tileindexRD] == 0x01) || (collision_map[tileindexRC] == 0x01) || (collision_map[tileindexRT] == 0x01) || (collision_map[tileindexLD] == 0x02) || (collision_map[tileindexLC] == 0x02) || (collision_map[tileindexLT] == 0x02) || (collision_map[tileindexRD] == 0x02) || (collision_map[tileindexRC] == 0x02) || (collision_map[tileindexRT] == 0x02)) {
            if (!x_Adjust) {
                PLAYER.SpdX = 0;
                if (!Jump) {
                    switch_idle_jump();  //NOT SURE IF NEEDED
                }
            }
        }
        if ((collision_map[tileindexLD] == 0x00) && (collision_map[tileindexLC] == 0x00) && (collision_map[tileindexLT] == 0x01) || (collision_map[tileindexRD] == 0x00) && (collision_map[tileindexRC] == 0x00) && (collision_map[tileindexRT] == 0x01)) {
            if (canCrouch_timer == 1) {
                canCrouch = TRUE;
                Crouch = TRUE;
                // canCrouch = FALSE;
                canCrouch_timer = 10;
            }
            canCrouch_timer -= 1;
        }
    }
}
//TRY COMBINING THIS WITH CHECK_J BY ADDING A SWITCH WHEN PRESSING A BUTTON, TURNS OFF AFTER CHECK_J IN BOTH IF AND ELSE IF SECNARIOS
void check_UD(UBYTE newplayerx, UBYTE newplayery, INT16 camera_x, UBYTE *collision_map, UBYTE mapWidth) {
    UINT16 indexLx, indexCx, indexRx, index_y, indexCamx, tileindexL, tileindexC, tileindexR;
    indexCamx = camera_x;
    indexLx = ((newplayerx - 16) + indexCamx) / 8;
    indexCx = ((newplayerx - 8) + indexCamx) / 8;
    indexRx = ((newplayerx - 1) + indexCamx) / 8;
    index_y = (newplayery - 1) / 8;

    tileindexL = mapWidth * index_y + indexLx;  //MULTIPLY THE WIDTH BY THE Y TILE TO FIND THE Y ROW. THEN ADD THE X TILE TO SHIFT THE COLUMN. FINDS THE TILE YOU'RE LOOKING FOR
    tileindexC = mapWidth * index_y + indexCx;
    tileindexR = mapWidth * index_y + indexRx;
    //IF THE CHARACTER IS STANDING UNDER AN 0x02 COLLISION CORNER AND CLIPS, ADJUST HIS X UNTIL HE IS SAFELY OUTSIDE OF THE COLLISION WALL OF TILES
    if (x_Adjust) {
        if ((PLAYER.SpdX < MAX_WALK_SPEED) && (PLAYER.SpdX > -MAX_WALK_SPEED)) {
            if ((collision_map[tileindexL] != 0x02) && (collision_map[tileindexR] == 0x02)) {
                PLAYER.SpdX = -MAX_CRAWL_SPEED;
            } else if ((collision_map[tileindexL] == 0x02) && (collision_map[tileindexR] != 0x02)) {
                PLAYER.SpdX = MAX_CRAWL_SPEED;
            }
        }
        if ((collision_map[tileindexL] == 0x00) && (collision_map[tileindexR] == 0x00)) {
            if (PLAYER.SpdX != MAX_WALK_SPEED && PLAYER.SpdX != -MAX_WALK_SPEED && (!(joy & J_LEFT)) && (!(joy & J_RIGHT))) {
                PLAYER.SpdX = 0;
            }
            x_Adjust = FALSE;
        }
    }
    if (PLAYER.SpdY > 0) {
        if ((collision_map[tileindexL] == 0x01) || (collision_map[tileindexC] == 0x01) || (collision_map[tileindexR] == 0x01) || (collision_map[tileindexL] == 0x03) || (collision_map[tileindexC] == 0x03) || (collision_map[tileindexR] == 0x03)) {
            if (!Drop) {
                UBYTE ty = (TO_PIXELS(PLAYER.y) / 8);
                PLAYER.y = TO_COORDS(ty * 8);
                PLAYER.SpdY = 0;
                Spawn = Jump = FALSE;
                switch_idle_jump();
            }
        }
    } else if (PLAYER.SpdY < 0) {
        if ((collision_map[tileindexL] == 0x01) || (collision_map[tileindexC] == 0x01) || (collision_map[tileindexR] == 0x01)) {
            PLAYER.SpdY = 0;
        }
    }
}
//TRY COMBINING THIS WITH CHECK_J BY ADDING A SWITCH WHEN PRESSING A BUTTON, TURNS OFF AFTER CHECK_J IN BOTH IF AND ELSE IF SECNARIOS
void check_J(UBYTE newplayerx, UBYTE newplayery, INT16 camera_x, UBYTE *collision_map, UBYTE mapWidth) {
    UINT16 indexLx, indexCx, indexRx, indexSLx, indexSCx, indexSRx, index_y, index_Cy, indexCamx, tileindexL, tileindexC, tileindexR, tileindexCL, tileindexCC, tileindexCR, tileindexSL, tileindexSC, tileindexSR;
    //CL = Crouch Left CC = Crouch Center CR = Crouch Right
    indexCamx = camera_x;

    indexLx = ((newplayerx - 16) + indexCamx) / 8;
    indexCx = ((newplayerx - 8) + indexCamx) / 8;
    indexRx = ((newplayerx - 1) + indexCamx) / 8;
    //STANDING x with a few pixels of forgiveness for 0x02 collision checks
    indexSLx = ((newplayerx - 10) + indexCamx) / 8;
    indexSCx = ((newplayerx - 9) + indexCamx) / 8;
    indexSRx = ((newplayerx - 5) + indexCamx) / 8;

    index_y = (newplayery - 1) / 8;
    index_Cy = (newplayery + 7) / 8;

    tileindexL = mapWidth * index_y + indexLx;  //MULTIPLY THE WIDTH BY THE Y TILE TO FIND THE Y ROW. THEN ADD THE X TILE TO SHIFT THE COLUMN. FINDS THE TILE YOU'RE LOOKING FOR
    tileindexC = mapWidth * index_y + indexCx;
    tileindexR = mapWidth * index_y + indexRx;

    tileindexSL = mapWidth * index_y + indexSLx;
    tileindexSC = mapWidth * index_y + indexSCx;
    tileindexSR = mapWidth * index_y + indexSRx;

    tileindexCL = mapWidth * index_Cy + indexLx;
    tileindexCC = mapWidth * index_Cy + indexCx;
    tileindexCR = mapWidth * index_Cy + indexRx;

    if (Crouch) {
        if (((collision_map[tileindexC] == 0x02) && (collision_map[tileindexR] == 0x02)) || ((collision_map[tileindexSC] == 0x02) && (collision_map[tileindexL] == 0x02)) || (collision_map[tileindexL] == 0x01) || (collision_map[tileindexC] == 0x01) || (collision_map[tileindexR] == 0x01) || (collision_map[tileindexCL] == 0x01) || (collision_map[tileindexCC] == 0x01) || (collision_map[tileindexCR] == 0x01)) {
        } else {
            if (!Drop) {
                Crouch = Launch = FALSE;
                if (!Jump) {
                    PLAYER.SpdY = JUMP_IMPULSE;
                    Jump = x_Adjust = TRUE;
                    switch_jump();
                }
            }
        }
    } else if (!Crouch) {
        //IF WALK SPEED IS LESS THAN MAX, MAKE HIS JUMP ABILITY ON CORNERS A BIT MORE RESTRICTED
        if ((PLAYER.SpdX < MAX_WALK_SPEED) && (PLAYER.SpdX > -MAX_WALK_SPEED)) {
            if (((collision_map[tileindexC] == 0x02) && (collision_map[tileindexR] == 0x02)) || ((collision_map[tileindexSC] == 0x02) && (collision_map[tileindexL] == 0x02)) || (collision_map[tileindexL] == 0x01) || (collision_map[tileindexC] == 0x01) || (collision_map[tileindexR] == 0x01)) {
            } else {
                if (!Drop) {
                    Crouch = Launch = FALSE;
                    if (!Jump) {
                        PLAYER.SpdY = JUMP_IMPULSE;
                        Jump = TRUE;
                        switch_jump();
                    }
                }
            }
            if ((collision_map[tileindexL] == 0x02) || (collision_map[tileindexR] == 0x02)) {
                x_Adjust = TRUE;
            }
        } else {  //IF WALK SPEED MAX, THEN ALLOW SOME LEEWAY ON WHEN HE IS ABLE TO JUMP OUT OF A CORNER (EXCEPT INTO THE CORNER FROM OUTSIDE)
            if (PLAYER.direction == DIR_RIGHT) {
                if (((collision_map[tileindexC] == 0x02) && (collision_map[tileindexR] == 0x02)) || ((collision_map[tileindexR] == 0x02) || (collision_map[tileindexL] == 0x01) || (collision_map[tileindexC] == 0x01) || (collision_map[tileindexR] == 0x01))) {
                } else {
                    if (!Drop) {
                        Crouch = Launch = FALSE;
                        if (!Jump) {
                            PLAYER.SpdY = JUMP_IMPULSE;
                            Jump = TRUE;
                            switch_jump();
                        }
                    }
                }
            }
            if (PLAYER.direction == DIR_LEFT) {
                if (((collision_map[tileindexC] == 0x02) && (collision_map[tileindexL] == 0x02)) || ((collision_map[tileindexL] == 0x02) || (collision_map[tileindexL] == 0x01) || (collision_map[tileindexC] == 0x01) || (collision_map[tileindexR] == 0x01))) {
                } else {
                    if (!Drop) {
                        Crouch = Launch = FALSE;
                        if (!Jump) {
                            PLAYER.SpdY = JUMP_IMPULSE;
                            Jump = TRUE;
                            switch_jump();
                        }
                    }
                }
            }
            if ((collision_map[tileindexL] == 0x02) || (collision_map[tileindexR] == 0x02)) {
                x_Adjust = TRUE;
            }
        }
    }
}

void check_Drop(UBYTE newplayerx, UBYTE newplayery, INT16 camera_x, UBYTE *collision_map, UBYTE mapWidth) {
    UINT16 indexLx, indexCx, indexRx, index_y, indexCamx, tileindexL, tileindexC, tileindexR;
    indexCamx = camera_x;
    indexLx = ((newplayerx - 16) + indexCamx) / 8;
    indexCx = ((newplayerx - 8) + indexCamx) / 8;
    indexRx = ((newplayerx - 1) + indexCamx) / 8;
    index_y = (newplayery - 1) / 8;

    tileindexL = mapWidth * index_y + indexLx;  //MULTIPLY THE WIDTH BY THE Y TILE TO FIND THE Y ROW. THEN ADD THE X TILE TO SHIFT THE COLUMN. FINDS THE TILE YOU'RE LOOKING FOR
    tileindexC = mapWidth * index_y + indexCx;
    tileindexR = mapWidth * index_y + indexRx;

    if ((collision_map[tileindexL] == 0x03) || (collision_map[tileindexC] == 0x03) || (collision_map[tileindexR] == 0x03)) {
        Drop = TRUE;
    }
}
//CHECK CROUCH
void check_C(UBYTE newplayerx, UBYTE newplayery, INT16 camera_x, UBYTE *collision_map, UBYTE mapWidth) {
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

    tileindexCL = mapWidth * index_y + indexCLx;  //MULTIPLY THE WIDTH BY THE Y TILE TO FIND THE Y ROW. THEN ADD THE X TILE TO SHIFT THE COLUMN. FINDS THE TILE YOU'RE LOOKING FOR
    tileindexCC = mapWidth * index_y + indexCCx;
    tileindexCT = mapWidth * index_y + indexCRx;
    //STANDING
    tileindexSL = mapWidth * index_y + indexSLx;
    tileindexSR = mapWidth * index_y + indexSRx;

    if (Crouch) {
        if ((collision_map[tileindexCL] == 0x01) || (collision_map[tileindexCC] == 0x01) || (collision_map[tileindexCT] == 0x01)) {
        } else {
            Crouch = FALSE;
        }
    } else if (!Crouch) {
        if (collision_map[tileindexSR] == 0x01) {
            PLAYER.SpdX -= MAX_CRAWL_SPEED;
        } else if (collision_map[tileindexSL] == 0x01) {
            PLAYER.SpdX += MAX_CRAWL_SPEED;
        }
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