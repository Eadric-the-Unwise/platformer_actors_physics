#include <gb/gb.h>
#include <gbdk/metasprites.h>
#include <stdio.h>

#include "scene.h"

UBYTE joy, last_joy;
UBYTE current_elevator;
UBYTE Attach, x_Collide, y_Collide;
UBYTE px, py;
extern Variables bkg;
extern uint8_t animation_timer;

/******************************/
// Define your OBJ and BGP palettes, show SPRITES, turn on DISPLAY
/******************************/
void main() {
    DISPLAY_OFF;
    BGP_REG = 0xE4;
    OBP0_REG = 0xE4;
    OBP1_REG = 0xE1;
    SPRITES_8x16;  //MUST be 8x16 or 8x8. Can change in different scenes only
    SHOW_BKG;
    SHOW_SPRITES;

    Attach = Jump = Gravity = Crouch = canCrouch = Drop = x_Adjust = x_Collide = y_Collide = Launch = Shooting = FALSE;
    Spawn = TRUE;
    Drop_timer = 16;
    canCrouch_timer = 10;  //LEFT AND RIGHT BUTTON PRESS TIME DELAY TO AUTO CROUCH
    canCrouch_Ftimer = 8;  //TURN canCrouch TO FALSE WHEN REACH COUNTDOWN
    init_submap();
    load_level(&level1);
    actor_t *current_actor = &active_actors[ACTOR_FIRST_NPC];
    // switch on display after everything is ready
    DISPLAY_ON;
    last_joy = joy = 0;
    while (TRUE) {  // main loop runs at 60fps
        // ---------------------------------------------
        last_joy = joy;

        joy = joypad();
        if (!Spawn) {
            UBYTE px, py;
            px = TO_PIXELS(PLAYER.x);
            py = TO_PIXELS(PLAYER.y);
            if (joy & J_LEFT) {
                if (PLAYER.SpdX == 0) {
                    if (joy & J_DOWN) {
                        canCrouch_timer = 1;
                    }
                }
                if ((!Jump) && !(joy & (J_DOWN)) && (!Crouch)) {
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
                } else if (!(Crouch)) {
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
                if ((!Jump) && !(joy & (J_DOWN)) && !(Crouch)) {
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
                } else if (!(Crouch)) {
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
        //IF PLAYER IS FREE FALLING FOR ANY REASON
        if (PLAYER.SpdY != 0) {
            Jump = Gravity = TRUE;
            Crouch = FALSE;
            switch_jump();
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
            PLAYER.SpdY += GRAVITY;
            if (PLAYER.SpdY > MAX_FALL_SPEED) {
                PLAYER.SpdY = MAX_FALL_SPEED;
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
            UBYTE px, py;
            px = TO_PIXELS(PLAYER.x);
            py = TO_PIXELS(PLAYER.y);
            //Y-AXIS COLLISION CHECK
            if (PLAYER.SpdY > 0) {
                check_UD(px, py + 1, TO_PIXELS(bkg.camera_x));

            } else if (PLAYER.SpdY < 0) {
                check_UD(px, py - 25, TO_PIXELS(bkg.camera_x));
            }
        }
        if (PLAYER.SpdX != 0) {
            UBYTE px, py;
            px = TO_PIXELS(PLAYER.x);
            py = TO_PIXELS(PLAYER.y);
            //IF MOVING RIGHT
            if (PLAYER.SpdX > 0) {
                check_LR(px + 1, py, TO_PIXELS(bkg.camera_x));
                //IF MOVING LEFT
            } else if (PLAYER.SpdX < 0) {
                check_LR(px - 1, py, TO_PIXELS(bkg.camera_x));
            }
        }

        if ((Crouch) && (!canCrouch)) {
            if (!(joy & J_DOWN)) {
                check_C(TO_PIXELS(PLAYER.x), TO_PIXELS(PLAYER.y), TO_PIXELS(bkg.camera_x));
            }
        }

        // update PLAYER absolute posiiton
        if (!Attach) {
            PLAYER.y += PLAYER.SpdY;
        }
        //+- PLAYER.SpdX
        render_camera(TO_PIXELS(PLAYER.x), TO_PIXELS(bkg.camera_x));

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
                    check_C(TO_PIXELS(PLAYER.x), TO_PIXELS(PLAYER.y), TO_PIXELS(bkg.camera_x));
                }
            }
        }

        //CHECK LANDING HOTBOX TIMING
        for (UBYTE i = ACTOR_FIRST_NPC; i != (active_actors_count); i++) {
            //[y][x]
            UINT16 PTR_y, PTR_x, PBL_y, PBL_x, NTR_y, NTR_x, NBL_y, NBL_x;
            UBYTE ax, ay;
            ax = TO_PIXELS(active_actors[i].x);
            ay = TO_PIXELS(active_actors[i].y);
            //THE PIVOT IS THE LITERAL CENTER OF THE METASPRITE. NOT A PIXEL, BUT THE CROSSHAIRS IN THE MIDDLE OF THE DESGIN
            PTR_y = py - PLAYER.h_offset;            //TR y
            PTR_x = px + PLAYER.x_offset;            //TR x
            PBL_y = py + PLAYER.y_offset;            //BL y
            PBL_x = px - PLAYER.x_offset;            //BL x
            NTR_y = ay - active_actors[i].y_offset;  //TR y
            NTR_x = ax + active_actors[i].x_offset;  //TR x
            NBL_y = ay + active_actors[i].y_offset;  //BL y
            NBL_x = ax - active_actors[i].x_offset;  //BL x

            if (overlap(PTR_y, PTR_x, PBL_y, PBL_x, NTR_y, NTR_x, NBL_y, NBL_x) == 0x01U) {
                if (active_actors[i].NPC_type != ELEVATOR) {
                    if (active_actors[i].ON == TRUE) {
                        DISPLAY_OFF;
                        Spawn = TRUE;
                        init_submap();
                        load_level(&level1);
                        DISPLAY_ON;
                    }
                } else if (active_actors[i].NPC_type == ELEVATOR) {
                    if (active_actors[i].ON == TRUE) {
                        if ((PBL_x > NTR_x - 2) || (PTR_x < NBL_x + 2))  //is not on top of elevator
                        {
                            x_Collide = TRUE;
                        }
                        // else if (PTR_y > NBL_y - 8) {
                        //     y_Collide = TRUE;
                        // }
                        else if (PBL_y < NTR_y + 8) {
                            Attach = TRUE;
                            Gravity = FALSE;
                            current_elevator = i;
                        }
                    }
                }
            } else if (overlap(PTR_y, PTR_x, PBL_y, PBL_x, NTR_y, NTR_x, NBL_y, NBL_x) == 0x00U) {
                if (x_Collide) {
                    x_Collide = FALSE;
                }
            }
            if (Attach) {
                if (i == current_elevator) {
                    if ((PBL_x > NTR_x) || (PTR_x < NBL_x)) {
                        Attach = FALSE;
                        Gravity = TRUE;
                    } else {
                        PLAYER.SpdY = 0;
                        PLAYER.y = TO_COORDS(NTR_y - (PLAYER.h / 2));
                        if (Jump) {
                            switch_land();
                        } else if (!(joy & J_LEFT) && !(joy & J_RIGHT)) {
                            switch_idle();
                        }
                        Gravity = Spawn = Jump = FALSE;
                    }
                }
                if ((CHANGED_BUTTONS & J_A) && (joy & J_A)) {
                    jump();
                }
            }
        }
        // DOWN while standing still
        // if ((Crouch) && (!canCrouch) && (!(joy & J_LEFT) && !(joy & J_RIGHT)) && (PLAYER.SpdY == 0)) {
        //     switch_down();
        // }
        // render all actors on screen
        render_actors();
    }
}