#include <gb/gb.h>
#include <gbdk/metasprites.h>
#include <stdio.h>

#include "../res/tiles/stage_drop_collision.h"
#include "scene.h"

UINT8 joy, last_joy;
UINT8 px, py;
UINT8 GAMEOVER;
UINT8 EXIT1, EXIT2;
UINT8 JOYLOCK;
UINT8 ANIMATIONLOCK;
// UINT8 NPC_RESET;
UINT8 WALKSTATE;
UINT8 GAMESTATE;
extern Variables bkg;
extern uint8_t animation_timer;
// extern UINT8 ATTACH;
const level_t *current_stage;
uint8_t shadow_scx = 0, shadow_scy = 0;
UINT8 *cam_ptr = NULL;  // pointer // simply = NULL to bypass compiler error lol
BYTE ATTACH, x_Collide, y_Collide;
UINT8 current_elevator;
UINT8 render_actors_count = NULL;  // the amount of actors in 160px window, the first actor to load current_actor pointer
UINT8 render_bullet_count = NULL;
UINT8 PLAYER_bullet_timer = 0;
// actor_t *current_actor = &active_actors[ACTOR_FIRST_NPC];
/*****************************/
// Define your OBJ and BGP palettes, show SPRITES, turn on DISPLAY
/*****************************/
void main() {
    // DISPLAY_OFF;
    DISABLE_VBL_TRANSFER;
    BGP_REG = 0xE4;
    OBP0_REG = 0xE4;
    OBP1_REG = 0xE1;
    SPRITES_8x16;  // MUST be 8x16 or 8x8. Can change in different scenes only
    SHOW_BKG;
    SHOW_SPRITES;
    JOYLOCK = ANIMATIONLOCK = WALKSTATE = GAMEOVER = EXIT1 = EXIT2 = Gravity = L_LEFT = L_RIGHT = ATTACH = LADDER = ONTO_Ladder = DOWN_LADDER = DOWN_LADDER_F = UP_LADDER = OFF_LADDER = CROUCH = canCROUCH = DROP = FALSE;
    SPAWN = JUMP = LADDER_Release = TRUE;
    DROP_timer = 16;
    canCROUCH_timer = 10;  // LEFT AND RIGHT BUTTON PRESS TIME DELAY TO AUTO CROUCH
    canCROUCH_Ftimer = 8;  // TURN canCROUCH TO FALSE WHEN REACH COUNTDOWN
    GAMESTATE = 2;

    last_joy = joy = 0;
    while (TRUE) {  // main loop runs at 60fps
        // ---------------------------------------------
        switch (GAMESTATE) {
            case 1:
                SWITCH_ROM(LEVEL1_BANK);
                enter_lvl1();
                break;
            case 2:
                SWITCH_ROM(LEVEL2_BANK);
                enter_lvl2();
                break;
            // case 3:
            //     SWITCH_ROM(WORLD1_BANK);
            //     enter_world1();
            //     break;
            case 4:
                SWITCH_ROM(worldtest_BANK);
                enter_worldtest();
                break;
        }
    }
}