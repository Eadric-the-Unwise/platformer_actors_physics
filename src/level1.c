#include "level1.h"

#include <gb/gb.h>
#include <stdlib.h>

#include "../res/tiles/NPC_electric.h"
#include "../res/tiles/detective_large.h"
#include "../res/tiles/enemy_arrow.h"
#include "../res/tiles/vertical_platform_V1.h"
extern Variables bkg;
// void move_arrows();
void render_level1();

//CURRENTLY, LOADING FROM THE RIGHT FORCES YOU TO CALC (X COORD MINUS THE TO_PIXELS(CAM.X)). IS THERE A WAY TO AUTOMATICALLY CAL THIS VALUE UPON LOAD?
//.w and .h are adjusted for COLLISION functions
const actor_t level1_actors[6] = {
    //0 PLAYER
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
     .copy = FALSE},
    //1 NPC
    {.x = TO_COORDS(-16),
     .y = TO_COORDS(148),
     .SpdX = 7,
     .SpdY = 0,
     .w = NPC_electric_WIDTH,
     .h = NPC_electric_HEIGHT,
     .w_offset = 10,
     .x_offset = 6,
     .y_offset = 6,
     .direction = DIR_RIGHT,
     .NPC_type = PISTOL,
     .tile_count = (sizeof(NPC_electric_data) >> 4),
     .tile_index = 0,
     .tile_data = NPC_electric_data,
     .animations = {NPC_electric_animation, NPC_electric_animation},
     .animations_props = {ANIM_LOOP, ANIM_LOOP},
     .animation_phase = 0,
     .copy = FALSE},
    //2 NPC
    {.x = TO_COORDS(-160),
     .y = TO_COORDS(84),
     .SpdX = 7,
     .SpdY = 0,
     .w = NPC_electric_WIDTH,
     .h = NPC_electric_HEIGHT,
     .w_offset = 10,
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
    //TOP PATROL
    {.x = TO_COORDS(56),
     .y = TO_COORDS(68),
     .SpdX = -12,
     .SpdY = 0,
     .w = NPC_electric_WIDTH,
     .h = NPC_electric_HEIGHT,
     .w_offset = 10,
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
     .copy = TRUE},
    //BOTTOM PATROL
    {.x = TO_COORDS(56),
     .y = TO_COORDS(132),
     .SpdX = 12,
     .SpdY = 0,
     .w = NPC_electric_WIDTH,
     .h = NPC_electric_HEIGHT,
     .w_offset = 10,
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
    // ELEVATOR
    {.x = TO_COORDS(-210),
     .y = TO_COORDS(104),
     .SpdX = 0,
     .SpdY = 16,
     .w = vertical_platform_V1_WIDTH,
     .h = vertical_platform_V1_HEIGHT,
     .w_offset = 16,
     .x_offset = vertical_platform_V1_WIDTH / 2,
     .y_offset = vertical_platform_V1_HEIGHT / 2,
     .direction = DIR_RIGHT,
     .NPC_type = ELEVATOR,
     .tile_count = (sizeof(vertical_platform_V1_data) >> 4),
     .tile_index = 0,
     .tile_data = vertical_platform_V1_data,
     .patrol_timer = 64,
     .patrol_reset = 64,
     .animations = {elevator_frame, elevator_frame},
     .animations_props = {ANIM_LOOP, ANIM_LOOP},
     .animation_phase = 0,
     .copy = FALSE}};

const level_t level1 = {
    .actors = level1_actors,
    .actor_count = 6,
    .animate_hook = render_level1  // function that put life into the scene
};

// void move_arrows() {
//     actor_t *current_actor = &active_actors[ACTOR_FIRST_NPC];  //The Detective is currently active_actors[0], so active_actors[1] and above are enemies

//     for (UINT8 i = active_actors_count - 1; i != 0; i--) {
//         current_actor->x += current_actor->SpdX;
//         current_actor->y += current_actor->SpdY;

//         if (current_actor->x < TO_COORDS(17)) {
//             SetActorDirection(current_actor, DIR_RIGHT, 0);
//             current_actor->SpdX = abs(current_actor->SpdX);
//         } else if (current_actor->x > TO_COORDS(159)) {
//             SetActorDirection(current_actor, DIR_LEFT, 0);
//             current_actor->SpdX = -abs(current_actor->SpdX);
//         }
//         current_actor++;
//     }
// }
void render_level1() {
    actor_t *current_actor = &active_actors[ACTOR_FIRST_NPC];  //The Detective is currently active_actors[0], so active_actors[1] and above are enemies

    for (UINT8 i = active_actors_count - 1; i != 0; i--) {
        if ((TO_PIXELS(bkg.camera_x) > 0) && (TO_PIXELS(bkg.camera_x) < bkg.camera_max_x)) {
            current_actor->x -= PLAYER.SpdX;
        }

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

            if ((current_actor->direction == DIR_LEFT) && (current_actor->patrol_timer == 0)) {
                SetActorDirection(current_actor, DIR_RIGHT, 0);
                current_actor->SpdY = abs(current_actor->SpdY);
                current_actor->patrol_timer = current_actor->patrol_reset;
            } else if ((current_actor->direction == DIR_RIGHT) && (current_actor->patrol_timer == 0)) {
                SetActorDirection(current_actor, DIR_LEFT, 0);
                current_actor->SpdY = -abs(current_actor->SpdY);
                current_actor->patrol_timer = current_actor->patrol_reset;
            }
        } else if (current_actor->NPC_type == WALK) {
            INT16 actor_x = TO_PIXELS(current_actor->x);
            //NEED TO CALCULATE THE BKG.MAPS X POSITION RELATIVE TO THE SPAWN POSITION
            // if (actor_x > 160){
            //         current_actor->x= -60;
            //         current_actor->ON = TRUE;
            //     }
            if (actor_x >= -40) {
                current_actor->x += current_actor->SpdX;
            }
        }
        current_actor++;
    }
    //WE SHOULD ONLY NEED TO CHECK FOR CROUCH OR JUMP, BECAUSE BOTH WALK AND LAND HAVE THE SAME HITBOXES. SET THE VALUES FOR EACH BOX HERE
    if (Crouch) {  //CROUCH HITBOX
        PLAYER.h_offset = -2;
        PLAYER.x_offset = 8;
        PLAYER.y_offset = 16;
    } else if (PLAYER.SpdY != 0) {  //JUMP AND FALLING HITBOX
        PLAYER.h_offset = 11;
        PLAYER.x_offset = 6;
        PLAYER.y_offset = 6;
    } else if ((!Crouch) && (!Jump)) {  //STAND AND WALKING HITBOX
        PLAYER.h_offset = 7;
        PLAYER.x_offset = 6;
        PLAYER.y_offset = 16;
    }
}