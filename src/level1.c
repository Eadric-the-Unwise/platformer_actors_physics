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
const actor_t level1_actors[5] = {
    //0 PLAYER
    {.x = TO_COORDS(136),
     .y = TO_COORDS(-8),
     .SpdX = 0,
     .SpdY = -16,
     .w = detective_large_WIDTH,
     .h = detective_large_HEIGHT,
     .h_offset = 8,
     .x_offset = 8,
     .y_offset = 8,
     .direction = DIR_JUMP_L,
     .tile_count = (sizeof(detective_large_data) >> 4),
     .tile_index = 0,
     .tile_data = detective_large_data,
     .animations = {detective_walk_left, detective_walk_left, detective_crouch, detective_crouch, detective_crawl_left, detective_crawl_left, detective_stand, detective_stand, detective_jump, detective_jump, detective_land, detective_land, detective_drop, detective_drop},
     .animations_props = {ANIM_LOOP, ANIM_LOOP, ANIM_ONCE, ANIM_ONCE, ANIM_LOOP, ANIM_LOOP, ANIM_ONCE, ANIM_ONCE, ANIM_ONCE, ANIM_ONCE, ANIM_ONCE, ANIM_ONCE, ANIM_ONCE, ANIM_ONCE},
     .animation_phase = 0,
     .copy = FALSE},
    //1 NPC
    {.x = TO_COORDS(-44),
     .y = TO_COORDS(148),
     .SpdX = 7,
     .SpdY = 0,
     .w = NPC_electric_WIDTH,
     .h = NPC_electric_HEIGHT / 2,
     .w_offset = 10,
     .x_offset = -2,
     .y_offset = 8,
     .direction = DIR_RIGHT,
     .NPC_type = WALK,
     .tile_count = (sizeof(NPC_electric_data) >> 4),
     .tile_index = 0,
     .tile_data = NPC_electric_data,
     .animations = {NPC_electric_animation, NPC_electric_animation},
     .animations_props = {ANIM_LOOP, ANIM_LOOP},
     .animation_phase = 0,
     .copy = FALSE},
    //2 NPC
    {.x = TO_COORDS(-80),
     .y = TO_COORDS(116),
     .SpdX = 7,
     .SpdY = 0,
     .w = NPC_electric_WIDTH,
     .h = NPC_electric_HEIGHT / 2,
     .w_offset = 10,
     .x_offset = -2,
     .y_offset = 8,
     .direction = DIR_RIGHT,
     .NPC_type = WALK,
     .tile_count = (sizeof(NPC_electric_data) >> 4),
     .tile_index = 0,
     .tile_data = NPC_electric_data,
     .animations = {NPC_electric_animation, NPC_electric_animation},
     .animations_props = {ANIM_LOOP, ANIM_LOOP},
     .animation_phase = 0,
     .copy = TRUE},
    //3
    {.x = TO_COORDS(-140),
     .y = TO_COORDS(116),
     .SpdX = 7,
     .SpdY = 0,
     .w = NPC_electric_WIDTH,
     .h = NPC_electric_HEIGHT / 2,
     .w_offset = 10,
     .x_offset = -2,
     .y_offset = 8,
     .direction = DIR_RIGHT,
     .NPC_type = WALK,
     .tile_count = (sizeof(NPC_electric_data) >> 4),
     .tile_index = 0,
     .tile_data = NPC_electric_data,
     .animations = {NPC_electric_animation, NPC_electric_animation},
     .animations_props = {ANIM_LOOP, ANIM_LOOP},
     .animation_phase = 0,
     .copy = TRUE},
    //4
    {.x = TO_COORDS(-140),
     .y = TO_COORDS(52),
     .SpdX = 7,
     .SpdY = 0,
     .w = NPC_electric_WIDTH,
     .h = NPC_electric_HEIGHT / 2,
     .w_offset = 10,
     .x_offset = -2,
     .y_offset = 8,
     .direction = DIR_RIGHT,
     .NPC_type = WALK,
     .tile_count = (sizeof(NPC_electric_data) >> 4),
     .tile_index = 0,
     .tile_data = NPC_electric_data,
     .animations = {NPC_electric_animation, NPC_electric_animation},
     .animations_props = {ANIM_LOOP, ANIM_LOOP},
     .animation_phase = 0,
     .copy = TRUE}};

const level_t level1 = {
    .actors = level1_actors,
    .actor_count = 5,
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
            if (TO_PIXELS(current_actor->x) >= -40)
                current_actor->x += current_actor->SpdX;
        }
        current_actor++;
    }
}