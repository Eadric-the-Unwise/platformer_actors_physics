#include <gb/gb.h>
#include <gbdk/metasprites.h>
#include <stdio.h>

#include "scene.h"

UINT8 joy, last_joy;
UINT8 px, py;
UINT8 GAMEOVER;
UINT8 gamestate;
extern Variables bkg;
extern uint8_t animation_timer;
extern UINT8 ATTACH;
const level_t *current_stage;
uint8_t shadow_scx = 0, shadow_scy = 0;
// WE NEED TO ADD A STATE OF LOCKING ALL BUTTONS. FOR EXAMPLE: IF ONTO_LADDER {LOCK BUTTONS}

/*****************************/
// Define your OBJ and BGP palettes, show SPRITES, turn on DISPLAY
/*****************************/
void main() {
    DISABLE_VBL_TRANSFER;
    DISPLAY_OFF;
    BGP_REG = 0xE4;
    OBP0_REG = 0xE4;
    OBP1_REG = 0xE1;
    SPRITES_8x16;  // MUST be 8x16 or 8x8. Can change in different scenes only
    SHOW_BKG;
    SHOW_SPRITES;
    GAMEOVER = Gravity = L_LEFT = L_RIGHT = ATTACH = LADDER = ONTO_Ladder = DOWN_LADDER = DOWN_LADDER_F = UP_LADDER = OFF_LADDER = CROUCH = canCROUCH = DROP = FALSE;
    SPAWN = JUMP = LADDER_Release = TRUE;
    DROP_timer = 16;
    canCROUCH_timer = 10;  // LEFT AND RIGHT BUTTON PRESS TIME DELAY TO AUTO CROUCH
    canCROUCH_Ftimer = 8;  // TURN canCROUCH TO FALSE WHEN REACH COUNTDOWN
    gamestate = 1;
    // load_bullet_data(hiwater);
    actor_t *current_actor = &active_actors[ACTOR_FIRST_NPC];

    // switch on display after everything is ready
    DISPLAY_ON;

    last_joy = joy = 0;
    while (TRUE) {  // main loop runs at 60fps
        // ---------------------------------------------
        switch(gamestate){
            case 1:
            SWITCH_ROM_MBC1(LEVEL1_BANK);
            enter_lvl1();
            break;
            case 2:
            //level 2 here
            break;
        }

    }
}