#include "level2.h"

#include <gb/gb.h>
#include <stdlib.h>

#include "../res/tiles/detective_large.h"
#include "../res/tiles/enemy_arrow.h"

void animate_level2();

const actor_t level2_actors[4] = {
    {.x = TO_COORDS(140),
     .y = TO_COORDS(36),
     .SpdX = 0,
     .SpdY = 0,
     .direction = DIR_IDLE_L,
     .tile_count = (sizeof(detective_large_data) >> 4),
     .tile_index = 0,
     .tile_data = detective_large_data,
     .frame_delay = 7,
     .animations = {detective_walk_left, detective_walk_left, NULL, detective_stand, detective_stand, detective_jump, detective_jump},
     .animations_props = {ANIM_LOOP, ANIM_LOOP, ANIM_ONCE, ANIM_ONCE, ANIM_ONCE, ANIM_ONCE, ANIM_ONCE},
     .animation_phase = 0},
    {.x = TO_COORDS(160),
     .y = TO_COORDS(140),
     .SpdX = -8,
     .SpdY = 0,
     .direction = DIR_LEFT,
     .tile_count = (sizeof(enemy_arrow_data) >> 4),
     .tile_index = 0,
     .tile_data = enemy_arrow_data,
     .animations = {enemy_arrow_left, enemy_arrow_right, NULL, NULL, NULL},
     .animations_props = {ANIM_LOOP, ANIM_LOOP, ANIM_ONCE, ANIM_ONCE, ANIM_ONCE},
     .animation_phase = 0},
    {.x = TO_COORDS(120),
     .y = TO_COORDS(50),
     .SpdX = -8,
     .SpdY = 0,
     .direction = DIR_LEFT,
     .tile_count = (sizeof(enemy_arrow_data) >> 4),
     .tile_index = 0,
     .tile_data = enemy_arrow_data,
     .animations = {enemy_arrow_left, enemy_arrow_right, NULL, NULL, NULL},
     .animations_props = {ANIM_LOOP, ANIM_LOOP, ANIM_ONCE, ANIM_ONCE, ANIM_ONCE},
     .animation_phase = 0},
    {.x = TO_COORDS(140),
     .y = TO_COORDS(80),
     .SpdX = -8,
     .SpdY = 0,
     .direction = DIR_LEFT,
     .tile_count = (sizeof(enemy_arrow_data) >> 4),
     .tile_index = 0,
     .tile_data = enemy_arrow_data,
     .animations = {enemy_arrow_left, enemy_arrow_right, NULL, NULL, NULL},
     .animations_props = {ANIM_LOOP, ANIM_LOOP, ANIM_ONCE, ANIM_ONCE, ANIM_ONCE},
     .animation_phase = 0}};

const level_t level2 = {
    .actors = level2_actors,
    .actor_count = 4,
    .animate_hook = animate_level2};

void animate_level2() {
    actor_t *current_actor = &active_actors[ACTOR_FIRST_NPC];  //The Detective is currently active_actors[0], so active_actors[1] and above are enemies

    for (UINT8 i = active_actors_count - 1; i != 0; i--) {
        current_actor->x += current_actor->SpdX;
        current_actor->y += current_actor->SpdY;

        if (current_actor->x < TO_COORDS(17)) {
            SetActorDirection(current_actor, DIR_RIGHT, 0);
            current_actor->SpdX = abs(current_actor->SpdX);
        } else if (current_actor->x > TO_COORDS(159)) {
            SetActorDirection(current_actor, DIR_LEFT, 0);
            current_actor->SpdX = -abs(current_actor->SpdX);
        }
        current_actor++;
    }
}