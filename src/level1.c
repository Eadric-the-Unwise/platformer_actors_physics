#include "level1.h"

#include <gb/gb.h>
#include <stdlib.h>

#include "../res/tiles/detective_large.h"
#include "../res/tiles/enemy_arrow.h"
extern Variables bkg;
void move_arrows();
void move_x();

const actor_t level1_actors[3] = {
    {.x = TO_COORDS(128),
     .y = TO_COORDS(40),
     .SpdX = 0,
     .SpdY = 0,
     .direction = DIR_IDLE_L,
     .tile_count = (sizeof(detective_large_data) >> 4),
     .tile_index = 0,
     .tile_data = detective_large_data,
     .frame_delay = 7,
     .animations = {detective_walk_left, detective_walk_left, detective_crouch, detective_crouch, detective_crawl_left, detective_crawl_left, detective_stand, detective_stand, detective_jump, detective_jump},
     .animations_props = {ANIM_LOOP, ANIM_LOOP, ANIM_ONCE, ANIM_ONCE, ANIM_LOOP, ANIM_LOOP, ANIM_ONCE, ANIM_ONCE, ANIM_ONCE, ANIM_ONCE},
     .animation_phase = 0},
    {.x = TO_COORDS(400),
     .y = TO_COORDS(136),
     .SpdX = -20,
     .SpdY = 0,
     .direction = DIR_LEFT,
     .tile_count = (sizeof(enemy_arrow_data) >> 4),
     .tile_index = 0,
     .tile_data = enemy_arrow_data,
     .animations = {enemy_arrow_left, enemy_arrow_left, NULL, NULL, NULL, NULL, NULL, NULL, NULL},
     .animations_props = {ANIM_LOOP, ANIM_LOOP, ANIM_ONCE, ANIM_LOOP, ANIM_LOOP, ANIM_ONCE, ANIM_ONCE, ANIM_ONCE, ANIM_ONCE},
     .animation_phase = 0},
    {.x = TO_COORDS(160),
     .y = TO_COORDS(136),
     .SpdX = 12,
     .SpdY = 0,
     .direction = DIR_RIGHT,
     .tile_count = (sizeof(enemy_arrow_data) >> 4),
     .tile_index = 0,
     .tile_data = enemy_arrow_data,
     .animations = {enemy_arrow_left, enemy_arrow_right, NULL, NULL, NULL, NULL, NULL, NULL, NULL},
     .animations_props = {ANIM_LOOP, ANIM_LOOP, ANIM_ONCE, ANIM_LOOP, ANIM_LOOP, ANIM_ONCE, ANIM_ONCE, ANIM_ONCE, ANIM_ONCE},
     .animation_phase = 0}};

const level_t level1 = {
    .actors = level1_actors,
    .actor_count = 3,
    .animate_hook = move_x  // function that put life into the scene
};

void move_arrows() {
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
void move_x() {
    actor_t *current_actor = &active_actors[ACTOR_FIRST_NPC];  //The Detective is currently active_actors[0], so active_actors[1] and above are enemies
    
    for (UINT8 i = active_actors_count - 1; i != 0; i--) {
    UINT16 NPC_Cam_Offset = (TO_PIXELS(current_actor->x) - TO_PIXELS(bkg.camera_x));
    UINT16 Cam_NPC_Offset = (TO_PIXELS(bkg.camera_x) - TO_PIXELS(current_actor->x));
        if (NPC_Cam_Offset < 255){
        current_actor->x -= PLAYER.SpdX;
        } else if (Cam_NPC_Offset < 255){
        current_actor->x -= PLAYER.SpdX;
        } else {}
        current_actor++;
    }
}