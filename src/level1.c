#pragma bank 255
#include "level1.h"

#include <gb/gb.h>
#include <stdlib.h>

#include "../res/tiles/NPC_electric.h"
#include "../res/tiles/detective_large.h"
#include "../res/tiles/enemy_arrow.h"
#include "../res/tiles/vertical_platform_V1.h"
extern Variables bkg;
extern UINT8 px, py;
extern UINT8 joy, last_joy;
UINT8 Attach, x_Collide, y_Collide;
UINT8 current_elevator;

// CURRENTLY, LOADING FROM THE RIGHT FORCES YOU TO CALC (X COORD MINUS THE TO_PIXELS(CAM.X)). IS THERE A WAY TO AUTOMATICALLY CAL THIS VALUE UPON LOAD?
//.w and .h are adjusted for COLLISION functions
const actor_t level1_actors[6] = {
    // 0 PLAYER
    {.x = TO_COORDS(136),
     .y = TO_COORDS(-8),
     .SpdX = 0,
     .SpdY = -16,
     .w = detective_large_WIDTH,
     .h = detective_large_HEIGHT,
     .h_offset = 7,
     .x_offset = 6,
     .y_offset = 16,
     .direction = DIR_JUMP_L,
     .tile_count = (sizeof(detective_large_data) >> 4),
     .tile_index = 0,
     .tile_data = detective_large_data,
     .animations = {detective_walk_left, detective_walk_left, detective_crouch, detective_crouch, detective_crawl_left, detective_crawl_left, detective_stand, detective_stand, detective_jump, detective_jump, detective_land, detective_land, detective_drop, detective_drop},
     .animations_props = {ANIM_LOOP, ANIM_LOOP, ANIM_ONCE, ANIM_ONCE, ANIM_LOOP, ANIM_LOOP, ANIM_ONCE, ANIM_ONCE, ANIM_ONCE, ANIM_ONCE, ANIM_ONCE, ANIM_ONCE, ANIM_ONCE, ANIM_ONCE},
     .animation_phase = 0,
     .copy = FALSE,
     .RENDER = TRUE,
     .ON = TRUE},
    // 1 NPC
    // {.x = TO_COORDS(-24),
    //  .y = TO_COORDS(148),
    //  .SpdX = 7,
    //  .SpdY = 0,
    //  .w = NPC_electric_WIDTH,
    //  .h = NPC_electric_HEIGHT,
    //  .x_pivot = NPC_electric_PIVOT_X,
    //  .y_pivot = NPC_electric_PIVOT_Y,
    //  .x_offset = 6,
    //  .y_offset = 6,
    //  .direction = DIR_RIGHT,
    //  .NPC_type = PISTOL,
    //  .tile_count = (sizeof(NPC_electric_data) >> 4),
    //  .tile_index = 0,
    //  .tile_data = NPC_electric_data,
    //  .animations = {NPC_electric_animation, NPC_electric_animation},
    //  .animations_props = {ANIM_LOOP, ANIM_LOOP},
    //  .animation_phase = 0,
    //  .copy = FALSE},

    // 1 TOP PATROL
    {.x = TO_COORDS(56),
     .y = TO_COORDS(68),
     .SpdX = -12,
     .SpdY = 0,
     .w = NPC_electric_WIDTH,
     .h = NPC_electric_HEIGHT,
     .x_pivot = NPC_electric_PIVOT_X,
     .y_pivot = NPC_electric_PIVOT_Y,
     .x_offset = 6,
     .y_offset = 6,
     .direction = DIR_LEFT,
     .NPC_type = PATROL,
     .patrol_timer = 60,
     .patrol_reset = 120,
     .tile_count = (sizeof(NPC_electric_data) >> 4),
     .tile_index = 0,
     .tile_data = NPC_electric_data,
     .animations = {NPC_electric_animation, NPC_electric_animation},
     .animations_props = {ANIM_LOOP, ANIM_LOOP},
     .animation_phase = 0,
     .copy = FALSE},
    // 2 BOTTOM PATROL
    {.x = TO_COORDS(56),
     .y = TO_COORDS(132),
     .SpdX = 12,
     .SpdY = 0,
     .w = NPC_electric_WIDTH,
     .h = NPC_electric_HEIGHT,
     .x_pivot = NPC_electric_PIVOT_X,
     .y_pivot = NPC_electric_PIVOT_Y,
     .x_offset = 6,
     .y_offset = 6,
     .direction = DIR_RIGHT,
     .NPC_type = PATROL,
     .patrol_timer = 60,
     .patrol_reset = 120,
     .tile_count = (sizeof(NPC_electric_data) >> 4),
     .tile_index = 0,
     .tile_data = NPC_electric_data,
     .animations = {NPC_electric_animation, NPC_electric_animation},
     .animations_props = {ANIM_LOOP, ANIM_LOOP},
     .animation_phase = 0,
     .copy = TRUE},
    // 3 WALK
    {.x = TO_COORDS(-144),
     .y = TO_COORDS(116),
     .SpdX = 8,
     .SpdY = 0,
     .w = NPC_electric_WIDTH,
     .h = NPC_electric_HEIGHT,
     .x_pivot = NPC_electric_PIVOT_X,
     .y_pivot = NPC_electric_PIVOT_Y,
     .x_offset = 6,
     .y_offset = 6,
     .direction = DIR_RIGHT,
     .NPC_type = WALK,
     .tile_count = (sizeof(NPC_electric_data) >> 4),
     .tile_index = 0,
     .tile_data = NPC_electric_data,
     .animations = {NPC_electric_animation, NPC_electric_animation},
     .animations_props = {ANIM_LOOP, ANIM_LOOP},
     .animation_phase = 0,
     .copy = TRUE},
    // 4 ELEVATOR RIGHT
    {.x = TO_COORDS(-60),
     .y = TO_COORDS(104),
     .SpdX = 0,
     .SpdY = 16,
     .w = vertical_platform_V1_WIDTH,
     .h = vertical_platform_V1_HEIGHT,
     .x_pivot = vertical_platform_V1_PIVOT_X,
     .y_pivot = vertical_platform_V1_PIVOT_Y,
     .x_offset = vertical_platform_V1_PIVOT_X,
     .y_offset = vertical_platform_V1_PIVOT_Y,
     .direction = DIR_DOWN,
     .NPC_type = ELEVATOR,
     .tile_count = (sizeof(vertical_platform_V1_data) >> 4),
     .tile_index = 0,
     .tile_data = vertical_platform_V1_data,
     .patrol_timer = 64,
     .patrol_reset = 64,
     .animations = {elevator_frame, elevator_frame, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, elevator_frame, elevator_frame},
     .animations_props = {ANIM_ONCE, ANIM_ONCE, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL},
     .animation_phase = 0,
     .copy = FALSE},
    // 5 ELEVATOR LEFT
    {.x = TO_COORDS(-140),
     .y = TO_COORDS(136),
     .SpdX = 0,
     .SpdY = 12,
     .w = vertical_platform_V1_WIDTH,
     .h = vertical_platform_V1_HEIGHT,
     .x_pivot = vertical_platform_V1_PIVOT_X,
     .y_pivot = vertical_platform_V1_PIVOT_Y,
     .x_offset = vertical_platform_V1_PIVOT_X,
     .y_offset = vertical_platform_V1_PIVOT_Y,
     .direction = DIR_DOWN,
     .NPC_type = ELEVATOR,
     .tile_count = (sizeof(vertical_platform_V1_data) >> 4),
     .tile_index = 0,
     .tile_data = vertical_platform_V1_data,
     .patrol_timer = 1,
     .patrol_reset = 96,
     .animations = {elevator_frame, elevator_frame, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, elevator_frame, elevator_frame},
     .animations_props = {ANIM_ONCE, ANIM_ONCE, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL},
     .animation_phase = 0,
     .copy = TRUE}};

const level_t level1 = {
    .actors = level1_actors,
    .actor_count = 6,
    .animate_hook = anim_level1  // function that put life into the scene
};

void anim_level1() {
    actor_t *current_actor = &active_actors[ACTOR_FIRST_NPC];  // The Detective is currently active_actors[0], so active_actors[1] and above are enemies
    UINT16 camera_x = TO_PIXELS(bkg.camera_x);
    for (UINT8 i = active_actors_count - 1; i != 0; i--) {
        if ((camera_x > 0) && (camera_x < bkg.camera_max_x)) {  // IF CAM IS NOT IN SPAWN OR END POSITION (ie it's moving)
            if (current_actor->RENDER) {
                current_actor->x -= PLAYER.SpdX;
            }
        }
        if (current_actor->RENDER == TRUE && current_actor->KILL != TRUE) {
            if (current_actor->NPC_type == PATROL) {
                current_actor->patrol_timer--;
                current_actor->x += current_actor->SpdX;
                if ((current_actor->direction == DIR_LEFT) && (current_actor->patrol_timer == 0)) {
                    SetActorDirection(current_actor, DIR_RIGHT, 0);
                    current_actor->SpdX = abs(current_actor->SpdX);
                    current_actor->patrol_timer = current_actor->patrol_reset;
                } else if ((current_actor->direction == DIR_RIGHT) && (current_actor->patrol_timer == 0)) {
                    SetActorDirection(current_actor, DIR_LEFT, 0);
                    current_actor->SpdX = -abs(current_actor->SpdX);
                    current_actor->patrol_timer = current_actor->patrol_reset;
                }

            } else if (current_actor->NPC_type == ELEVATOR) {
                current_actor->patrol_timer--;
                current_actor->y += current_actor->SpdY;

                if ((current_actor->direction == DIR_UP) && (current_actor->patrol_timer == 0)) {
                    SetActorDirection(current_actor, DIR_DOWN, 0);
                    current_actor->SpdY = abs(current_actor->SpdY);
                    current_actor->patrol_timer = current_actor->patrol_reset;
                } else if ((current_actor->direction == DIR_DOWN) && (current_actor->patrol_timer == 0)) {
                    SetActorDirection(current_actor, DIR_UP, 0);
                    current_actor->SpdY = -abs(current_actor->SpdY);
                    current_actor->patrol_timer = current_actor->patrol_reset;
                }
            } else if (current_actor->NPC_type == WALK) {
                INT16 actor_x = TO_PIXELS(current_actor->x);

                if ((actor_x >= -32) && (actor_x <= 200)) {
                    current_actor->x += current_actor->SpdX;
                }
                if (actor_x > 196) {
                    current_actor->KILL = TRUE;
                }
            }
        } else if (current_actor->ON == FALSE) {
        }
        if ((camera_x <= bkg.camera_max_x) && (camera_x > 480)) {
            active_actors[1].RENDER = TRUE;
            active_actors[2].RENDER = TRUE;
            active_actors[3].RENDER = TRUE;
            active_actors[4].RENDER = FALSE;
            active_actors[5].RENDER = FALSE;

        } else if ((camera_x > 320) && (camera_x < 480)) {
            active_actors[1].RENDER = FALSE;
            active_actors[2].RENDER = FALSE;
            active_actors[3].RENDER = TRUE;
            active_actors[4].RENDER = TRUE;
            active_actors[5].RENDER = TRUE;

        } else if ((camera_x > 160) && (camera_x < 320)) {
            active_actors[1].RENDER = FALSE;
            active_actors[2].RENDER = FALSE;
            active_actors[3].RENDER = FALSE;
            active_actors[4].RENDER = TRUE;
            active_actors[5].RENDER = TRUE;

        } else if ((camera_x > 0) && (camera_x < 160)) {
            active_actors[1].RENDER = FALSE;
            active_actors[2].RENDER = FALSE;
            active_actors[3].RENDER = FALSE;
            active_actors[4].RENDER = FALSE;
            active_actors[5].RENDER = TRUE;
        }
        current_actor++;
    }
    // WE SHOULD ONLY NEED TO CHECK FOR CROUCH OR JUMP, BECAUSE BOTH WALK AND LAND HAVE THE SAME HITBOXES. SET THE VALUES FOR EACH BOX HERE
    if (Crouch) {  // CROUCH HITBOX
        PLAYER.h_offset = -2;
        PLAYER.x_offset = 8;
        PLAYER.y_offset = 16;
    } else if (PLAYER.SpdY != 0) {  // JUMP AND FALLING HITBOX
        PLAYER.h_offset = 11;
        PLAYER.x_offset = 6;
        PLAYER.y_offset = 6;
    } else if ((!Crouch) && (!Jump)) {  // STAND AND WALKING HITBOX
        PLAYER.h_offset = 7;
        PLAYER.x_offset = 6;
        PLAYER.y_offset = 16;
    }
}

void npc_collisions_level1() {
    // CHECK LANDING HOTBOX TIMING
    for (UINT8 i = ACTOR_FIRST_NPC; i != (active_actors_count); i++) {
        //[y][x]
        UINT16 PTR_y, PTR_x, PBL_y, PBL_x, NTR_y, NTR_x, NBL_y, NBL_x;
        UINT8 ax, ay;
        ax = TO_PIXELS(active_actors[i].x);
        ay = TO_PIXELS(active_actors[i].y);
        INT16 NPC_PLAYER_Offset = px - (ax - active_actors[i].x_pivot);
        // THE PIVOT IS THE LITERAL CENTER OF THE METASPRITE. NOT A PIXEL, BUT THE CROSSHAIRS IN THE MIDDLE OF THE DESGIN
        PTR_y = py - PLAYER.h_offset;            // TR y
        PTR_x = px + PLAYER.x_offset;            // TR x
        PBL_y = py + PLAYER.y_offset;            // BL y
        PBL_x = px - PLAYER.x_offset;            // BL x
        NTR_y = ay - active_actors[i].y_offset;  // TR y
        NTR_x = ax + active_actors[i].x_offset;  // TR x
        NBL_y = ay + active_actors[i].y_offset;  // BL y
        NBL_x = ax - active_actors[i].x_offset;  // BL x
        if (active_actors[i].ON && active_actors[i].KILL != TRUE) {
            if (overlap(PTR_y, PTR_x, PBL_y, PBL_x, NTR_y, NTR_x, NBL_y, NBL_x) == 0x01U) {
                if (active_actors[i].NPC_type != ELEVATOR) {
                    DISPLAY_OFF;
                    Spawn = TRUE;
                    init_submap();
                    load_level(&level1);
                    DISPLAY_ON;

                } else if (active_actors[i].NPC_type == ELEVATOR) {
                    if ((PBL_x > NTR_x - 2) || (PTR_x < NBL_x + 2))  // is not on top of elevator
                    {
                        x_Collide = TRUE;
                    } else if ((PBL_y > NTR_y) && (PBL_y < NBL_y)) {
                        Attach = TRUE;
                        Gravity = FALSE;
                        current_elevator = i;
                    } else if ((PTR_y < NBL_y) && (PTR_y > NTR_y)) {
                        y_Collide = TRUE;
                    }
                }
            } else if (overlap(PTR_y, PTR_x, PBL_y, PBL_x, NTR_y, NTR_x, NBL_y, NBL_x) == 0x00U) {
                if (x_Collide) {
                    x_Collide = FALSE;
                }
            }

        } else {
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
}