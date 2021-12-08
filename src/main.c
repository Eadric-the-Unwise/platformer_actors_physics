#include <gb/gb.h>
#include <gbdk/metasprites.h>
#include <stdio.h>

#include "scene.h"

UINT8 joy, last_joy;
UINT8 px, py;
UINT8 GAMEOVER;
extern Variables bkg;
extern uint8_t animation_timer;
extern UINT8 Attach;
const level_t *current_stage;

/******************************/
// Define your OBJ and BGP palettes, show SPRITES, turn on DISPLAY
/******************************/
void main() {
    DISPLAY_OFF;
    BGP_REG = 0xE4;
    OBP0_REG = 0xE4;
    OBP1_REG = 0xE1;
    SPRITES_8x16;  // MUST be 8x16 or 8x8. Can change in different scenes only
    SHOW_BKG;
    SHOW_SPRITES;
    current_stage = &level1;

    GAMEOVER = Attach = Ladder = Ladder_Release = Jump = Gravity = Crouch = canCrouch = Drop = Launch = Shooting = FALSE;
    Drop_timer = 16;
    canCrouch_timer = 10;  // LEFT AND RIGHT BUTTON PRESS TIME DELAY TO AUTO CROUCH
    canCrouch_Ftimer = 8;  // TURN canCrouch TO FALSE WHEN REACH COUNTDOWN
    load_level(current_stage);
    if (load_submap) load_submap();
    actor_t *current_actor = &active_actors[ACTOR_FIRST_NPC];
    // switch on display after everything is ready
    DISPLAY_ON;
    Spawn = TRUE;
    last_joy = joy = 0;
    while (TRUE) {  // main loop runs at 60fps
        // ---------------------------------------------
        last_joy = joy;

        joy = joypad();
        if (!Spawn) {
            UINT8 px, py;
            px = TO_PIXELS(PLAYER.x);
            py = TO_PIXELS(PLAYER.y);
            if (joy & J_LEFT) {
                if (PLAYER.SpdX == 0) {
                    if (joy & J_DOWN) {
                        canCrouch_timer = 1;
                    }
                }
                if ((!Jump) && !(joy & J_DOWN) && (!Crouch)) {
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
                } else if ((!Crouch) && (!Ladder)) {
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
                }
                if ((!Jump) && !(joy & J_DOWN) && !(Crouch)) {
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
                } else if ((!Crouch) && (!Ladder)) {
                    if (PLAYER.SpdX < MAX_WALK_SPEED) {
                        PLAYER.SpdX += WALK_VELOCITY;
                    } else
                        PLAYER.SpdX = MAX_WALK_SPEED;
                }
            }
        }
        if ((joy & J_DOWN) && (PLAYER.SpdY == 0)) {
            Crouch = TRUE;
        }

        if ((CHANGED_BUTTONS & J_A) && (joy & J_A)) {
            jump();
        }

        // IF PLAYER IS FREE FALLING FOR ANY REASON
        if (PLAYER.SpdY != 0) {
            Jump = Gravity = TRUE;
            Crouch = FALSE;
            if (!Ladder){
            switch_jump();
            }
        }
        if (Drop) {
            Drop_timer -= 1;
            if (Drop_timer == 0) {
                Drop = FALSE;
                Drop_timer = 16;
            }
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
        if (PLAYER.SpdY == 0) {
            check_UD(px, py + 1, TO_PIXELS(bkg.camera_x));
        }

        // ---------------------------------------------
        // WORLD PHYSICS:
        // GRAVITY

        if (Gravity) {
            if (Ladder) {
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

        if (PLAYER.SpdY != 0) {
            UINT8 px, py;
            px = TO_PIXELS(PLAYER.x);
            py = TO_PIXELS(PLAYER.y);
            // Y-AXIS COLLISION CHECK
            if (PLAYER.SpdY > 0) {
                check_UD(px, py + 1, TO_PIXELS(bkg.camera_x));

            } else if (PLAYER.SpdY < 0) {
                check_UD(px, py - 25, TO_PIXELS(bkg.camera_x));
            }
        }
        if (PLAYER.SpdX != 0) {
            UINT8 px, py;
            px = TO_PIXELS(PLAYER.x);
            py = TO_PIXELS(PLAYER.y);

            if (PLAYER.SpdX > 0) {
                check_LR(px + 1, py, TO_PIXELS(bkg.camera_x));    // IF MOVING RIGHT
              
            } else if (PLAYER.SpdX < 0) {
                check_LR(px - 1, py, TO_PIXELS(bkg.camera_x));   // IF MOVING LEFT
            }   
        }
        px = TO_PIXELS(PLAYER.x);
        py = TO_PIXELS(PLAYER.y);

//LADDER CHECK
//WE NEED THIS TO ONLY BE CALLED WHEN STANDING NEAR A LADDER, OTHERWISE IT GETS CALLED EACH LOOP AND SLOWS GAME DOWN
            if ((joy & J_UP) || (joy & J_DOWN) && (!Crouch)) { //? AND (!Jump) ??? This would allow for it to only be called when standing still, 
                        if (!(joy & J_LEFT) && !(joy & J_RIGHT)) { // and when you jump or move L/R, the check will be handled inside of there.
                        check_UD(px, TO_PIXELS(PLAYER.y), TO_PIXELS(bkg.camera_x)); //Once we add Y_Offsets, we may no longer need +1 and -1 in func calls
                        }                                                            //we likely can just adjust each offset scenario to be +1 or -1. Let's see, though...
            }
        

        if ((Crouch) && (!canCrouch)) {
            if (!(joy & J_DOWN)) {
                check_C(px, py, TO_PIXELS(bkg.camera_x));
            }
        }

        // update PLAYER absolute posiiton
        if (!Attach) {
            PLAYER.y += PLAYER.SpdY;
        }
        //+- PLAYER.SpdX
        render_camera(px, TO_PIXELS(bkg.camera_x));

        // call level animation hook (if any)
        if (animate_level) animate_level();

        if (bkg.redraw) {
            wait_vbl_done();
            bkg.redraw = FALSE;
            refresh_OAM();
            set_camera();

        } else {
            wait_vbl_done();
            refresh_OAM();
        }
        px = TO_PIXELS(PLAYER.x);
        py = TO_PIXELS(PLAYER.y);

        // Change to IDLE state when not moving
        if ((!Jump) && (!Crouch) && (PLAYER.direction != DIR_LAND_L) && (PLAYER.direction != DIR_LAND_R)) {
            if ((PLAYER.SpdX == 0) && (PLAYER.SpdY == 0)) {
                if (!(joy & J_LEFT) && !(joy & J_RIGHT)) {
                    switch_idle();
                    check_C(px, py, TO_PIXELS(bkg.camera_x));
                }
            }
        }
        if (collide_level) collide_level();

        // DOWN while standing still
        if ((Crouch) && (!canCrouch) && (!(joy & J_LEFT) && !(joy & J_RIGHT)) && (PLAYER.SpdY == 0)) {
            switch_down();
        }
        // render all actors on screen
        render_actors();
        if (GAMEOVER) {
            gameover();
        }
    }
}