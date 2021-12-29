#include <gb/gb.h>
#include <gbdk/metasprites.h>
#include <stdio.h>

#include "scene.h"

UINT8 joy, last_joy;
UINT8 px, py;
UINT8 GAMEOVER;
extern Variables bkg;
extern uint8_t animation_timer;
extern UINT8 ATTACH;
const level_t *current_stage;
uint8_t shadow_scx = 0, shadow_scy = 0;
// WE NEED TO ADD A STATE OF LOCKING ALL BUTTONS. FOR EXAMPLE: IF ONTO_LADDER {LOCK BUTTONS}

/******************************/
// Define your OBJ and BGP palettes, show SPRITES, turn on DISPLAY
/******************************/
void main() {
    DISABLE_VBL_TRANSFER;
    DISPLAY_OFF;
    BGP_REG = 0xE4;
    OBP0_REG = 0xE4;
    OBP1_REG = 0xE1;
    SPRITES_8x16;  // MUST be 8x16 or 8x8. Can change in different scenes only
    SHOW_BKG;
    SHOW_SPRITES;
    current_stage = &level1;
    GAMEOVER = Gravity = LEFT = RIGHT = ATTACH = LADDER = ONTO_Ladder = DOWN_LADDER = DOWN_LADDER_F = UP_LADDER = OFF_LADDER = CROUCH = canCROUCH = DROP = FALSE;
    SPAWN = JUMP = LADDER_Release = TRUE;
    DROP_timer = 16;
    canCROUCH_timer = 10;  // LEFT AND RIGHT BUTTON PRESS TIME DELAY TO AUTO CROUCH
    canCROUCH_Ftimer = 8;  // TURN canCROUCH TO FALSE WHEN REACH COUNTDOWN
    load_level(current_stage);
    if (load_submap) load_submap();
    actor_t *current_actor = &active_actors[ACTOR_FIRST_NPC];
    // switch on display after everything is ready
    DISPLAY_ON;

    last_joy = joy = 0;
    while (TRUE) {  // main loop runs at 60fps
        // ---------------------------------------------
        last_joy = joy;

        joy = joypad();
        if (!SPAWN) {
            UINT8 px, py;
            px = TO_PIXELS(PLAYER.x);
            py = TO_PIXELS(PLAYER.y);
            if (joy & J_LEFT) {
                if (PLAYER.SpdX == 0) {
                    if (joy & J_DOWN) {
                        canCROUCH_timer = 1;
                    }
                }
                if ((!JUMP) && !(joy & J_DOWN) && (!CROUCH) && (!ONTO_Ladder)) {
                    if (canCROUCH) {
                        SetActorDirection(&PLAYER, DIR_CRAWL_L, PLAYER.animation_phase);
                    } else {
                        SetActorDirection(&PLAYER, DIR_LEFT, PLAYER.animation_phase);
                    }

                } else if (CROUCH) {
                    if (!JUMP) {
                        SetActorDirection(&PLAYER, DIR_CRAWL_L, 0);
                    }
                }
                if (CROUCH) {
                    if (PLAYER.SpdX > -MAX_CRAWL_SPEED) {
                        PLAYER.SpdX -= WALK_VELOCITY;
                    } else
                        PLAYER.SpdX = -MAX_CRAWL_SPEED;
                } else if ((!CROUCH) && (!LADDER)) {
                    if (PLAYER.SpdX > -MAX_WALK_SPEED) {
                        PLAYER.SpdX -= WALK_VELOCITY;
                    } else
                        PLAYER.SpdX = -MAX_WALK_SPEED;
                }

            } else if (joy & J_RIGHT) {
                if (PLAYER.SpdX == 0) {
                    if (joy & J_DOWN) {
                        canCROUCH_timer = 1;
                    }
                }
                if ((!JUMP) && !(joy & J_DOWN) && (!CROUCH) && (!ONTO_Ladder)) {
                    if (canCROUCH) {
                        SetActorDirection(&PLAYER, DIR_CRAWL_R, PLAYER.animation_phase);
                    } else {
                        SetActorDirection(&PLAYER, DIR_RIGHT, PLAYER.animation_phase);
                    }
                } else if (CROUCH) {
                    if (!JUMP) {
                        SetActorDirection(&PLAYER, DIR_CRAWL_R, 0);
                    }
                }
                if (CROUCH) {
                    if (PLAYER.SpdX < MAX_CRAWL_SPEED) {
                        PLAYER.SpdX += WALK_VELOCITY;
                    } else
                        PLAYER.SpdX = MAX_CRAWL_SPEED;
                } else if ((!CROUCH) && (!LADDER)) {
                    if (PLAYER.SpdX < MAX_WALK_SPEED) {
                        PLAYER.SpdX += WALK_VELOCITY;
                    } else
                        PLAYER.SpdX = MAX_WALK_SPEED;
                }
            }
        }
        if ((joy & J_DOWN) && (!JUMP) && (!LADDER) && (!ONTO_Ladder)) {
            CROUCH = TRUE;
        }

        if ((CHANGED_BUTTONS & J_A) && (joy & J_A) && (!ONTO_Ladder) && (!OFF_LADDER)) {
            jump();
        }

        // IF PLAYER IS FREE FALLING FOR ANY REASON
        if (PLAYER.SpdY != 0) {
            JUMP = Gravity = TRUE;
            CROUCH = FALSE;
            if (!LADDER) {
                switch_jump();
            }
        }
        if (DROP) {
            DROP_timer -= 1;
            if (DROP_timer == 0) {
                DROP = FALSE;
                DROP_timer = 16;
            }
        }
        if ((CROUCH) && (canCROUCH)) {
            if ((PLAYER.direction == DIR_CRAWL_L) || (PLAYER.direction == DIR_DOWN_L)) {
                PLAYER.SpdX = -MAX_CRAWL_SPEED;
            } else if ((PLAYER.direction == DIR_CRAWL_R) || (PLAYER.direction == DIR_DOWN_R)) {
                PLAYER.SpdX = MAX_CRAWL_SPEED;
            }
            canCROUCH_Ftimer -= 1;
            if (canCROUCH_Ftimer == 1) {
                canCROUCH = FALSE;
                canCROUCH_Ftimer = 8;
            }
        }
        if (!(joy & J_LEFT) && !(joy & J_RIGHT)) {
            canCROUCH_timer = 10;
            // Release_timer = 20;
        }

        // ---------------------------------------------
        // WORLD PHYSICS:
        // GRAVITY

        if (Gravity) {
            if (LADDER) {
                if (PLAYER.SpdY < 0) {
                    PLAYER.SpdY += GRAVITY;
                } else if (PLAYER.SpdY > 0) {
                    PLAYER.SpdY -= GRAVITY;
                }
            } else {
                PLAYER.SpdY += GRAVITY;
                if (PLAYER.SpdY > MAX_FALL_SPEED) {
                    PLAYER.SpdY = MAX_FALL_SPEED;
                }
            }
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

        if (PLAYER.SpdX != 0) {
            UINT8 px, py;
            px = TO_PIXELS(PLAYER.x);
            py = TO_PIXELS(PLAYER.y);

            if (PLAYER.SpdX > 0) {
                check_LR(px + 1, py, TO_PIXELS(bkg.camera_x));  // IF MOVING RIGHT

            } else if (PLAYER.SpdX < 0) {
                check_LR(px - 1, py, TO_PIXELS(bkg.camera_x));  // IF MOVING LEFT
            }
        }
        px = TO_PIXELS(PLAYER.x);
        py = TO_PIXELS(PLAYER.y);

        // Y-AXIS COLLISION CHECK / /LADDER CHECK
        check_UD(px, py, TO_PIXELS(bkg.camera_x));

        if ((CROUCH) && (!canCROUCH)) {
            if (!(joy & J_DOWN)) {
                check_C(px, py, TO_PIXELS(bkg.camera_x));
            }
        }

        // update PLAYER absolute posiiton
        if (!ATTACH) {
            PLAYER.y += PLAYER.SpdY;
        }
        px = TO_PIXELS(PLAYER.x);
        py = TO_PIXELS(PLAYER.y);
        // // Change to IDLE state when not moving
        if ((!JUMP) && (!CROUCH) && (PLAYER.direction != DIR_LAND_L) && (PLAYER.direction != DIR_LAND_R) && (!LADDER)) {
            if ((PLAYER.SpdX == 0) && (PLAYER.SpdY == 0)) {
                if (!(joy & J_LEFT) && !(joy & J_RIGHT)) {
                    switch_idle();
                    check_C(px, py, TO_PIXELS(bkg.camera_x));
                }
            }
        }
        // DOWN while standing still
        if ((CROUCH) && (!canCROUCH) && (!(joy & J_LEFT) && !(joy & J_RIGHT)) && (PLAYER.SpdY == 0)) {
            switch_down();
        }

        // UPDATE THE CAMERA POSITION SETTINGS
        // render_camera(px, TO_PIXELS(bkg.camera_x));
        // UPDATE CURRENT LEVEL NPC ANIMATIONS AND X/Y POSITIONS
        if (animate_level) animate_level();  // call level animation hook (if any)
        // CHECK FOR NPC COLLISIONS
        if (collide_level) collide_level();
        // RENDER ALL CURRENT ACTORS ON SCREEN
        render_actors();

        if (bkg.redraw) {
            set_camera();
            wait_vbl_done();
            refresh_OAM();
            SCX_REG = shadow_scx;
            SCY_REG = shadow_scy;
            bkg.redraw = FALSE;

        } else {
            wait_vbl_done();
            refresh_OAM();
        }

        if (GAMEOVER) {
            gameover();
        }
    }
}