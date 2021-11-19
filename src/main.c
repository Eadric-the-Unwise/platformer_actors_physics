#include <gb/gb.h>
#include <gbdk/metasprites.h>
#include <stdio.h>

#include "../res/tiles/brick_wide_map.h"
#include "../res/tiles/brick_wide_tiles.h"
#include "../res/tiles/collision_wide_map.h"
#include "../res/tiles/detective_large.h"
#include "../res/tiles/enemy_arrow.h"
#include "collisions.h"
#include "level1.h"
#include "level2.h"
#include "macros.h"
#include "scene.h"

UBYTE joy, last_joy;
extern Variables bkg;
// uint8_t shadow_scx = 0, shadow_scy = 0;

/******************************/
// Define your OBJ and BGP palettes, show SPRITES, turn on DISPLAY
/******************************/
void main() {
    DISPLAY_OFF;
    BGP_REG = 0xE4;
    OBP0_REG = 0xE4;
    OBP1_REG = 0xE1;
    SPRITES_8x16;
    SHOW_BKG;
    SHOW_SPRITES;

    Jump = Crouch = canCrouch = Drop = x_Adjust = Launch = Shooting = FALSE;
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
        // process joystic input
        last_joy = joy;
        joy = joypad();
        if (!Spawn) {
            if (joy & J_LEFT) {
                if (PLAYER.SpdX == 0) {
                    if (joy & J_DOWN) {
                        canCrouch_timer = 1;
                    }
                    check_LR(TO_PIXELS(PLAYER.x) - 1, TO_PIXELS(PLAYER.y), TO_PIXELS(bkg.camera_x));
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
                    check_LR(TO_PIXELS(PLAYER.x) + 1, TO_PIXELS(PLAYER.y), TO_PIXELS(bkg.camera_x));
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
        if ((joy & J_DOWN) && !(Jump)) {
            Crouch = TRUE;
        }
        if (Crouch) {
            PLAYER.h_offset = 24;
        }
        // DOWN while standing still
        if ((Crouch) && (!canCrouch) && (!(joy & J_LEFT) && !(joy & J_RIGHT)) && (!Jump)) {
            switch_down();
        }
        //IF PLAYER IS FREE FALLING FOR ANY REASON
        if (PLAYER.SpdY != 0) {
            Jump = TRUE;
            Crouch = FALSE;
            switch_jump();
        }

        if ((CHANGED_BUTTONS & J_A) && (joy & J_A)) {
            UBYTE px, py, camx;
            px = TO_PIXELS(PLAYER.x);
            py = TO_PIXELS(PLAYER.y);
            camx = TO_PIXELS(bkg.camera_x);
            if (Crouch) {
                check_Drop(px, py + 1, TO_PIXELS(bkg.camera_x));
            }
            //CHECK WHETHER CAN JUMP (NO COLLISION ABOVE PLAYER)
            check_J(px, py - 25, TO_PIXELS(bkg.camera_x));
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
        // ---------------------------------------------
        // ---------------------------------------------
        // WORLD PHYSICS:
        // GRAVITY

        PLAYER.SpdY += GRAVITY;
        if (PLAYER.SpdY > MAX_FALL_SPEED) {
            PLAYER.SpdY = MAX_FALL_SPEED;
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

        //Y-AXIS COLLISION CHECK (ADD NEGATIVE AND POSITIVE IFS SO THE LOOP ONLY CHECKS 1 FOR Y AND 1 FOR X MOVEMENT)
        if (PLAYER.SpdY > 0) {
            check_UD(TO_PIXELS(PLAYER.x), TO_PIXELS(PLAYER.y) + 1, TO_PIXELS(bkg.camera_x));

        } else if (PLAYER.SpdY < 0) {
            check_UD(TO_PIXELS(PLAYER.x), TO_PIXELS(PLAYER.y) - 25, TO_PIXELS(bkg.camera_x));
        }
        //IF MOVING RIGHT
        if (PLAYER.SpdX > 0) {
            check_LR(TO_PIXELS(PLAYER.x) + 1, TO_PIXELS(PLAYER.y), TO_PIXELS(bkg.camera_x));
            //IF MOVING LEFT
        } else if (PLAYER.SpdX < 0) {
            check_LR(TO_PIXELS(PLAYER.x) - 1, TO_PIXELS(PLAYER.y), TO_PIXELS(bkg.camera_x));
        }

        //PERHAPS REPLACE THIS WITH A NEW SEPARATE FUNCTION CALLED check_C();
        if ((Crouch) && (!canCrouch)) {
            if (!(joy & J_DOWN)) {
                check_C(TO_PIXELS(PLAYER.x), TO_PIXELS(PLAYER.y), TO_PIXELS(bkg.camera_x));
            }
        }
        // Change to IDLE state when not moving
        if ((!Jump) && (!Crouch)) {
            if ((PLAYER.SpdX == 0) && (PLAYER.SpdY == 0)) {
                switch_idle();
                check_C(TO_PIXELS(PLAYER.x), TO_PIXELS(PLAYER.y), TO_PIXELS(bkg.camera_x));
            }
        }
        // update PLAYER absolute posiiton
        PLAYER.y += PLAYER.SpdY;
        //THIS IS ASSUMING PLAYER IS WALKING LEFT TO RIGHT. PERHAPS ADD A STAGE_LEFT AND STAGE_RIGHT VARIABLE IN THE STAGE STRUCT SO HE IS ON THE LEFT SIDE WHEN STAGE_RIGHT//
        if ((TO_PIXELS(bkg.camera_x) > 0) && (TO_PIXELS(bkg.camera_x) < bkg.camera_max_x)) {
            bkg.camera_x += PLAYER.SpdX;
            bkg.redraw = TRUE;
        } else
            PLAYER.x += PLAYER.SpdX;
        if (TO_PIXELS(bkg.camera_x) - 1 <= 0) {
            if ((joy & J_RIGHT) && (PLAYER.SpdX > 0) && (TO_PIXELS(PLAYER.x) >= 118)) {
                bkg.camera_x += PLAYER.SpdX;
                bkg.redraw = TRUE;
            }
        } else if (TO_PIXELS(bkg.camera_x) + 1 >= bkg.camera_max_x) {
            if ((joy & J_LEFT) && (PLAYER.SpdX < 0) && (TO_PIXELS(PLAYER.x) <= 118)) {
                bkg.camera_x += PLAYER.SpdX;
                bkg.redraw = TRUE;
            }
        }
        // if (joy & J_B) {
        //     printf("SpdX=%d\n", PLAYER.SpdX);
        // }
        //LATER CHANGE THIS TO COLLISION TILE RESET/DEATH
        if ((TO_PIXELS(PLAYER.y) > 241) && (TO_PIXELS(PLAYER.y) < 249)) {
            PLAYER.y = TO_COORDS(50);
        }
        // COPIED FROM DINO COLLISIONS
        for (UBYTE i = ACTOR_FIRST_NPC; i != (active_actors_count); i++) {
            //[y][x]
            UINT16 PTR_y, PTR_x, PBL_y, PBL_x, NTR_y, NTR_x, NBL_y, NBL_x;
            UBYTE px, py, ax, ay;
            px = TO_PIXELS(PLAYER.x);
            py = TO_PIXELS(PLAYER.y);
            ax = TO_PIXELS(active_actors[i].x);
            ay = TO_PIXELS(active_actors[i].y);
            
            PTR_y = py - (PLAYER.h - PLAYER.h_offset);  //TR y the top tile of PLAYER is 16 but only 8 or so are actually visible pixels, hence we subtract
            PTR_x = px + PLAYER.x_offset;               //TR x
            PBL_y = py + PLAYER.y_offset;               //BL y
            PBL_x = px - (PLAYER.w - PLAYER.x_offset);  //BL x
            NTR_y = ay - active_actors[i].h;
            NTR_x = ax + active_actors[i].x_offset;
            NBL_y = ay + active_actors[i].y_offset;
            NBL_x = ax - (active_actors[i].w - active_actors[i].w_offset);

            if (overlap(PTR_y, PTR_x, PBL_y, PBL_x, NTR_y, NTR_x, NBL_y, NBL_x) == 0x01U) {
                if (active_actors[i].ON == TRUE) {
            DISPLAY_OFF;
            // Spawn = TRUE;
            // init_submap();
            // load_level(&level1);
            // DISPLAY_ON;
                }
            }
        }

        // call level animation hook (if any), that makes other actors move (and interact in future)
        if (animate_level) animate_level();

        // render all actors on screen
        render_actors();
        if (bkg.redraw) {
            wait_vbl_done();
            bkg.redraw = FALSE;
            refresh_OAM();
            set_camera();

        } else {
            wait_vbl_done();
        }
        refresh_OAM();
    }
}