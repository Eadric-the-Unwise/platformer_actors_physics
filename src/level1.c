#pragma bank 255
#include "level1.h"

#include <gb/gb.h>
#include <stdlib.h>

#include "../res/tiles/NPC_electric.h"
#include "../res/tiles/detective_large.h"
#include "../res/tiles/elevator.h"
#include "../res/tiles/enemy_arrow.h"
extern Variables bkg;
extern UINT8 px, py;
extern UINT8 joy, last_joy;
UINT8 ATTACH, x_Collide, y_Collide;
UINT8 current_elevator;

// CURRENTLY, LOADING FROM THE RIGHT FORCES YOU TO CALC (X COORD MINUS THE TO_PIXELS(CAM.X)). IS THERE A WAY TO AUTOMATICALLY CAL THIS VALUE UPON LOAD?
//.w and .h are adjusted for COLLISION functions
const actor_t level1_actors[5] = {
    // 0 PLAYER
    {.x = TO_COORDS(144),
     .y = TO_COORDS(16),
     .SpdX = 0,
     .SpdY = 16,
     .w = detective_large_WIDTH,
     .h = detective_large_HEIGHT,
     .h_offset = 7,
     .x_offset = 6,
     .y_offset = 16,
     .direction = DIR_JUMP_L,
     .patrol_timer = 12,
     .patrol_reset = 12,
     .tile_count = (sizeof(detective_large_data) >> 4),
     .tile_index = 0,
     .tile_data = detective_large_data,
     .animations = {detective_walk_left, detective_walk_left, detective_CROUCH, detective_CROUCH, detective_crawl_left, detective_crawl_left, detective_stand, detective_stand, detective_JUMP, detective_JUMP, detective_land, detective_land, detective_drop, detective_drop, detective_LADDER, detective_LADDER, NULL, NULL, detective_ONTOLADDER, detective_ONTOLADDER, detective_OFFLADDER, detective_OFFLADDER},
     .animations_props = {ANIM_LOOP, ANIM_LOOP, ANIM_ONCE, ANIM_ONCE, ANIM_LOOP, ANIM_LOOP, ANIM_ONCE, ANIM_ONCE, ANIM_ONCE, ANIM_ONCE, ANIM_ONCE, ANIM_ONCE, ANIM_ONCE, ANIM_ONCE, NULL, NULL, NULL, NULL, ANIM_ONCE, ANIM_ONCE, ANIM_ONCE},
     .animation_phase = 3,
     .copy = FALSE,
     .RENDER = TRUE,
     .ON = TRUE},
    // 1 TOP PATROL
    {.x = TO_COORDS(56),
     .y = TO_COORDS(68),
     .SpdX = 8,
     .SpdY = 0,
     .w = NPC_electric_WIDTH,
     .h = NPC_electric_HEIGHT,
     .x_pivot = NPC_electric_PIVOT_X,
     .y_pivot = NPC_electric_PIVOT_Y,
     .x_offset = 6,
     .y_offset = 6,
     .direction = DIR_DOWN,
     .NPC_type = PATROL,
     .patrol_timer = 78,
     .patrol_reset = 156,
     .tile_count = (sizeof(NPC_electric_data) >> 4),
     .tile_index = 0,
     .tile_data = NPC_electric_data,
     .animations = {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NPC_electric_animation, NPC_electric_animation},
     .animations_props = {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, ANIM_LOOP, ANIM_LOOP},
     .animation_phase = 0,
     .copy = FALSE,
     .ON = TRUE},
    // 2 BOTTOM PATROL
    {.x = TO_COORDS(56),
     .y = TO_COORDS(132),
     .SpdX = -8,
     .SpdY = 0,
     .w = NPC_electric_WIDTH,
     .h = NPC_electric_HEIGHT,
     .x_pivot = NPC_electric_PIVOT_X,
     .y_pivot = NPC_electric_PIVOT_Y,
     .x_offset = 6,
     .y_offset = 6,
     .direction = DIR_UP,
     .NPC_type = PATROL,
     .patrol_timer = 78,
     .patrol_reset = 156,
     .tile_count = (sizeof(NPC_electric_data) >> 4),
     .tile_index = 0,
     .tile_data = NPC_electric_data,
     .animations = {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NPC_electric_animation, NPC_electric_animation},
     .animations_props = {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, ANIM_LOOP, ANIM_LOOP},
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
     .direction = DIR_LEFT,
     .NPC_type = WALK,
     .tile_count = (sizeof(NPC_electric_data) >> 4),
     .tile_index = 0,
     .tile_data = NPC_electric_data,
     .animations = {NPC_electric_animation, NPC_electric_animation},
     .animations_props = {ANIM_LOOP, ANIM_LOOP},
     .animation_phase = 0,
     .copy = TRUE},
    // 4 ELEVATOR
    {.x = TO_COORDS(-148),
     .y = TO_COORDS(136),
     .SpdX = 0,
     .SpdY = 8,
     .w = elevator_WIDTH,
     .h = elevator_HEIGHT,
     .x_pivot = elevator_PIVOT_X,
     .y_pivot = elevator_PIVOT_Y,
     .x_offset = elevator_PIVOT_X,
     .y_offset = elevator_PIVOT_Y,
     .direction = DIR_DOWN,
     .NPC_type = ELEVATOR,
     .tile_count = (sizeof(elevator_data) >> 4),
     .tile_index = 0,
     .tile_data = elevator_data,
     .patrol_timer = 1,
     .patrol_reset = 160,
     .animations = {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, elevator_frame, elevator_frame},
     .animations_props = {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL},
     .animation_phase = 0,
     .copy = FALSE}};

const level_t level1 = {
    .submap_hook = init_submap,  // call this in collision
    .actors = level1_actors,
    .actor_count = 5,
    .animate_hook = anim_level1,  // function that put life into the scene
    .collide_hook = npc_collisions_level1};

UINT8 cam1[3] = {1, 2, 3};
UINT8 cam2[3] = {3, 4};
UINT8 cam3[2] = {4};
UINT8 cam4[1] = {4};
#define CAM1_COUNT 3
#define CAM2_COUNT 2
#define CAM3_COUNT 1
#define CAM4_COUNT 1

void anim_level1() {
    UINT8 *ptr = NULL;   // pointer // simply = NULL to bypass compiler error lol
    UINT8 *pptr = NULL;  // previous pointer
    UINT8 *nptr = NULL;  // next pointer

    UINT16 camera_x = TO_PIXELS(bkg.camera_x);
    UINT8 active_actors_count = NULL;  // the amount of actors in 160px window, the first actor to load current_actor pointer
    UINT8 prev_actors_count = NULL;    // previous array of sprites to turn off
    UINT8 next_actors_count = NULL;    // next array of sprite to turn off (in case you move back to a previous position)

    if ((camera_x >= 480) && (camera_x <= bkg.camera_max_x)) {  // CAM1
        active_actors_count = CAM1_COUNT;
        next_actors_count = CAM2_COUNT;
        ptr = cam1;
        nptr = cam2;
    } else if ((camera_x >= 320) && (camera_x < 480)) {  // CAM2
        prev_actors_count = CAM1_COUNT;
        active_actors_count = CAM2_COUNT;
        next_actors_count = CAM3_COUNT;
        pptr = cam1;
        ptr = cam2;
        nptr = cam3;
    } else if ((camera_x >= 160) && (camera_x < 320)) {  // CAM3
        prev_actors_count = CAM2_COUNT;
        active_actors_count = CAM3_COUNT;
        next_actors_count = CAM4_COUNT;
        pptr = cam2;
        ptr = cam3;
        nptr = cam4;
    } else if (camera_x < 160) {  // CAM4
        prev_actors_count = CAM3_COUNT;
        active_actors_count = CAM4_COUNT;
        pptr = cam3;
        ptr = cam4;
    }
    actor_t *current_actor = &active_actors[*ptr];  // The Detective is currently active_actors[0], so active_actors[1] and above are enemies
    actor_t *prev_actor = &active_actors[*pptr];
    actor_t *next_actor = &active_actors[*nptr];

    for (UINT8 x = prev_actors_count; x != 0; x--) {  // TURN OFF PREVIOUS SET OF SPRITES
        prev_actor->RENDER = FALSE;
        // prev_actor->ON = FALSE;
        pptr++;
        prev_actor = &active_actors[*pptr];
    }
    for (UINT8 x = next_actors_count; x != 0; x--) {  // TURN OFF NEXT SET OF SPRITES
        next_actor->RENDER = FALSE;
        // next_actor->ON = FALSE;
        nptr++;
        next_actor = &active_actors[*nptr];
    }
    for (UINT8 i = active_actors_count; i != 0; i--) {  // TURN ON CURRENT SET OF NPC SPRITES
        current_actor->RENDER = TRUE;

        if ((camera_x > 0) && (camera_x < bkg.camera_max_x)) {  // IF CAM IS NOT IN SPAWN OR END POSITION (ie it's moving)
            current_actor->x -= PLAYER.SpdX;
        }
        if (current_actor->RENDER == TRUE && current_actor->KILL == NULL) {  // AI RULES FOR ALL NPCS ON THIS PARTICULAR STAGE
            if (current_actor->NPC_type == PATROL) {                         // PATROL NPCS
                current_actor->patrol_timer--;
                current_actor->x += current_actor->SpdX;
                if ((current_actor->direction == DIR_UP) && (current_actor->patrol_timer == 0)) {
                    SetActorDirection(current_actor, DIR_DOWN, 0);
                    current_actor->SpdX = abs(current_actor->SpdX);
                    current_actor->patrol_timer = current_actor->patrol_reset;
                } else if ((current_actor->direction == DIR_DOWN) && (current_actor->patrol_timer == 0)) {
                    SetActorDirection(current_actor, DIR_UP, 0);
                    current_actor->SpdX = -abs(current_actor->SpdX);
                    current_actor->patrol_timer = current_actor->patrol_reset;
                }

            } else if (current_actor->NPC_type == ELEVATOR) {  // ELEVATORS
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
            } else if (current_actor->NPC_type == WALK) {  // WALK NPCS WALK STRAIGHT AHEAD
                INT16 actor_x = TO_PIXELS(current_actor->x);

                if ((actor_x >= -32) && (actor_x <= 200)) {
                    current_actor->x += current_actor->SpdX;
                }
                if (actor_x > 196) {
                    current_actor->KILL = TRUE;  // KILL NPC IS HE GOES OFF SCREEN TO THE RIGHT TOO FAR
                }
            }
        }
        ptr++;
        current_actor = &active_actors[*ptr];
    }
}

void npc_collisions_level1() {
    // CHECK LANDING HOTBOX TIMING
    // WE SHOULD ONLY NEED TO CHECK FOR CROUCH OR JUMP, BECAUSE BOTH WALK AND LAND HAVE THE SAME HITBOXES. SET THE VALUES FOR EACH BOX HERE
    if (CROUCH) {  // CROUCH HITBOX
        PLAYER.h_offset = -2;
        PLAYER.x_offset = 8;
        PLAYER.y_offset = 16;
    } else if (PLAYER.SpdY != 0) {  // JUMP AND FALLING HITBOX
        PLAYER.h_offset = 11;
        PLAYER.x_offset = 6;
        PLAYER.y_offset = 6;
    } else if ((!CROUCH) && (!JUMP)) {  // STAND AND WALKING HITBOX
        PLAYER.h_offset = 7;
        PLAYER.x_offset = 6;
        PLAYER.y_offset = 16;
    }
    for (UINT8 i = ACTOR_FIRST_NPC; i != (total_actors_count); i++) {
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
        if (active_actors[i].ON && active_actors[i].KILL == NULL) {
            if (overlap(PTR_y, PTR_x, PBL_y, PBL_x, NTR_y, NTR_x, NBL_y, NBL_x) == 0x01U) {
                if (active_actors[i].NPC_type != ELEVATOR) {
                    GAMEOVER = TRUE;
                } else if (active_actors[i].NPC_type == ELEVATOR) {
                    if ((PBL_x > NTR_x - 2) || (PTR_x < NBL_x + 2))  // is not on top of elevator
                    {
                        x_Collide = TRUE;
                    } else if ((PBL_y > NTR_y) && (PBL_y < NBL_y)) {
                        ATTACH = TRUE;
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
        if (ATTACH) {
            if (i == current_elevator) {
                if ((PBL_x > NTR_x) || (PTR_x < NBL_x)) {
                    ATTACH = FALSE;
                    Gravity = TRUE;
                } else {
                    PLAYER.SpdY = 0;
                    PLAYER.y = TO_COORDS(NTR_y - (PLAYER.h / 2));
                    if (JUMP) {
                        switch_land();
                    } else if (!(joy & J_LEFT) && !(joy & J_RIGHT)) {
                        switch_idle();
                    }
                    SPAWN = Gravity = JUMP = FALSE;
                }
            }
            if ((CHANGED_BUTTONS & J_A) && (joy & J_A)) {
                jump();
            }
        }
    }
}