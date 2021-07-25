#include <gb/gb.h>
#include <gb/metasprites.h>

// uncomment the line below to switch on debug features
//#define DEBUG
#ifdef DEBUG
#include <stdio.h>
#endif

// Include your scene and map header files
#include "level1.h"
#include "level2.h"
#include "scene.h"

UINT8 joy, last_joy;

UINT8 floorYposition;
UINT8 Jump;

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

    // game assumes floor is at the level of 100px. that is a temporary workaround, should use collision maps instead
    floorYposition = 100;
    Jump = FALSE;

    load_level(&level1);

    // switch on display after everything is ready
    DISPLAY_ON;
    last_joy = joy = 0;
    while (TRUE) {  //main loop runs at 60fps
        // ---------------------------------------------
        // process joystic input
        last_joy = joy;
        joy = joypad();
        if (joy & J_LEFT) {
            if (PLAYER.SpdX > -MAX_WALK_SPEED)
                PLAYER.SpdX -= WALK_VELOCITY;
            else
                PLAYER.SpdX = -MAX_WALK_SPEED;
            if (!Jump) SetActorDirection(&PLAYER, DIR_LEFT, 0);
        } else if (joy & J_RIGHT) {
            if (PLAYER.SpdX < MAX_WALK_SPEED)
                PLAYER.SpdX += WALK_VELOCITY;
            else
                PLAYER.SpdX = MAX_WALK_SPEED;
            if (!Jump) SetActorDirection(&PLAYER, DIR_RIGHT, 0);
        }
        if (joy & J_DOWN) {
            ;  // add code here for duck

        } else if ((CHANGED_BUTTONS & J_A) && (joy & J_A)) {
            if (!Jump) {
                SetActorDirection(&PLAYER, DIR_UP, 0);
                PLAYER.SpdY = JUMP_IMPULSE;
                Jump = TRUE;
            }
        }

#ifdef DEBUG
        //DEBUG DETECTIVE Y COORDS
        if (joy & J_B) {
            printf("Y=%u\n", TO_PIXELS(PLAYER.y));
        }
#endif

        // Load stages on button press
        if (joy & J_START) {
            load_level(&level2);
        } else if (joy & J_SELECT) {
            load_level(&level1);
        }
        // no joy checking below this line
        // ---------------------------------------------

        // ---------------------------------------------
        // WORLD PHYSICS:
        // GRAVITY
        PLAYER.SpdY += GRAVITY;
        if (TO_PIXELS(PLAYER.y) < floorYposition) {  //if you are above the floorYposition
            if (PLAYER.SpdY > MAX_FALL_SPEED) PLAYER.SpdY = MAX_FALL_SPEED;
        } else {  //if you touch the floor
            if (PLAYER.SpdY > 0) {
                PLAYER.SpdY = 0;
                Jump = FALSE;
            }
            if (PLAYER.y > TO_COORDS(floorYposition)) PLAYER.y = TO_COORDS(floorYposition);  // if we "sunk into the ground" because of high speed, then float up
        }
        // FRICTION
        // THIS IS CURRENTLY THE MINIMUM AMOUNT FRICTION. REDUCE FURTHER BY ADDING A PHYSICS FRAME COUNTER
        if (PLAYER.SpdX != 0) {
            if (PLAYER.SpdX < 0)
                PLAYER.SpdX += FRICTION;
            else
                PLAYER.SpdX -= FRICTION;
            // set idle animation when standing
            //if you want the character to "slide",
            // then you should make variables and put all those constants into level struct
            // and load those when load level
            //add counter there
            // counter++; counter &= 3;
            //if (counter == 0) { //friction }
            //call is physics_counter
            // counter++;
            // counter &= 3; (&3 make is tick 012301230123)
        }
        // ---------------------------------------------

        // Change to IDLE state when not moving
        if (!Jump)
            if ((PLAYER.SpdX == 0) && (PLAYER.SpdY == 0)) {
                SetActorDirection(&PLAYER, DIR_NONE, 0);
            }

        // update PLAYER absolute posiiton
        PLAYER.y += PLAYER.SpdY;
        PLAYER.x += PLAYER.SpdX;

        // call level animation hook (if any), that makes other actors move (and interact in future)
        if (animate_level) animate_level();

        // render all actors on screen
        render_actors();

        // wait for VBlank
        wait_vbl_done();
    }
}
