#include "level2.h"

#include <gb/gb.h>
#include <stdlib.h>

#include "../res/tiles/detective_large.h"
#include "../res/tiles/enemy_arrow.h"
#include "camera.h"
#include "scene.h"

extern Variables bkg;
void animate_level2();

const actor_t level2_actors[4] = {
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
     .direction = DIR_IDLE_L,
     .tile_count = (sizeof(detective_large_data) >> 4),
     .tile_index = 0,
     .tile_data = detective_large_data,
     .animations = {detective_walk_left, detective_walk_left, detective_crouch, detective_crouch, detective_crawl_left, detective_crawl_left, detective_stand, detective_stand, detective_jump, detective_jump},
     .animations_props = {ANIM_LOOP, ANIM_LOOP, ANIM_ONCE, ANIM_ONCE, ANIM_LOOP, ANIM_LOOP, ANIM_ONCE, ANIM_ONCE, ANIM_ONCE, ANIM_ONCE},
     .animation_phase = 0,
     .copy = FALSE},
    //1 NPC PISTOL
    {.x = TO_COORDS(-233),
     .y = TO_COORDS(136),
     .SpdX = 0,
     .SpdY = 0,
     .w = enemy_arrow_WIDTH,
     .h = enemy_arrow_HEIGHT / 2,
     .w_offset = 10,
     .x_offset = -2,
     .y_offset = 8,
     .direction = DIR_LEFT,
     .NPC_type = PISTOL,
     .tile_count = (sizeof(enemy_arrow_data) >> 4),
     .tile_index = 0,
     .tile_data = enemy_arrow_data,
     .animations = {enemy_arrow_left, enemy_arrow_left, NULL, NULL, NULL, NULL, NULL, NULL, NULL},
     .animations_props = {ANIM_LOOP, ANIM_LOOP, ANIM_ONCE, ANIM_LOOP, ANIM_LOOP, ANIM_ONCE, ANIM_ONCE, ANIM_ONCE, ANIM_ONCE},
     .animation_phase = 0,
     .copy = FALSE},
    //2 NPC PATROL
    {.x = TO_COORDS(-360),
     .y = TO_COORDS(112),
     .SpdX = 12,
     .SpdY = 0,
     .w = enemy_arrow_WIDTH,
     .h = enemy_arrow_HEIGHT / 2,
     .w_offset = 10,
     .x_offset = -2,
     .y_offset = 8,
     .direction = DIR_RIGHT,
     .NPC_type = PATROL,
     .tile_count = (sizeof(enemy_arrow_data) >> 4),
     .tile_index = 0,
     .tile_data = enemy_arrow_data,
     .patrol_timer = 45,
     .patrol_reset = 45,
     .animations = {enemy_arrow_left, enemy_arrow_right, NULL, NULL, NULL, NULL, NULL, NULL, NULL},
     .animations_props = {ANIM_LOOP, ANIM_LOOP, ANIM_ONCE, ANIM_LOOP, ANIM_LOOP, ANIM_ONCE, ANIM_ONCE, ANIM_ONCE, ANIM_ONCE},
     .animation_phase = 0,
     .copy = FALSE},
    //3 NPC WALK
    {.x = TO_COORDS(-185),
     .y = TO_COORDS(136),
     .SpdX = 7,
     .SpdY = 0,
     .w = enemy_arrow_WIDTH,
     .h = enemy_arrow_HEIGHT / 2,
     .w_offset = 10,
     .x_offset = -2,
     .y_offset = 8,
     .direction = DIR_RIGHT,
     .NPC_type = WALK,
     .tile_count = (sizeof(enemy_arrow_data) >> 4),
     .tile_index = 0,
     .tile_data = enemy_arrow_data,
     .animations = {enemy_arrow_left, enemy_arrow_right, NULL, NULL, NULL, NULL, NULL, NULL, NULL},
     .animations_props = {ANIM_LOOP, ANIM_LOOP, ANIM_ONCE, ANIM_LOOP, ANIM_LOOP, ANIM_ONCE, ANIM_ONCE, ANIM_ONCE, ANIM_ONCE},
     .animation_phase = 0,
     .copy = TRUE}};

const level_t level2 = {
    .actors = level2_actors,
    .actor_count = 4,
    .animate_hook = anim_level2};

void anim_level2() {
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
            if (TO_PIXELS(current_actor->x) >= 0)
                current_actor->x += current_actor->SpdX;
        }
        current_actor++;
    }
}

void init_level2() {
    HIDE_BKG;
    bkg.redraw = TRUE;
    bkg.sliding = FALSE;
    bkg.camera_max_y = (BRICK_SMALL_MAPHeight - 18) * 8;
    bkg.camera_max_x = (BRICK_SMALL_MAPWidth - 20) * 8;
    bkg.camera_tiles_x = BRICK_SMALL_MAPWidth * 8;
    bkg.camera_tiles_y = BRICK_SMALL_MAPHeight * 8;
    bkg.camera_x = TO_COORDS(bkg.camera_max_x);
    bkg.camera_y = 0;
    bkg.old_camera_x = bkg.camera_x;
    bkg.old_camera_y = bkg.camera_y;
    bkg.map_pos_x = (UBYTE)(bkg.camera_x >> 7u);
    bkg.map_pos_y = (UBYTE)(bkg.camera_y >> 3u);
    //CHANGE THE TILE COUNT AS YOU ADD TILES TO THE BKG TILE_SET
    set_bkg_data_nonbanked(0, 4, BRICK_WIDE_TILES, BRICK_WIDE_TILESBank);
    bkg.old_map_pos_x = bkg.old_map_pos_y = 255;

    set_bkg_submap_nonbanked(bkg.map_pos_x, bkg.map_pos_y, 20, 18, BRICK_SMALL_MAP, BRICK_SMALL_MAPWidth, BANK(BRICK_SMALL_MAP));
    set_level(BRICK_SMALL_MAPWidth, BRICK_SMALL_MAPHeight, BRICK_SMALL_MAP, BANK(BRICK_SMALL_MAP));

    bkg.old_camera_x = bkg.camera_x;
    bkg.old_camera_y = bkg.camera_y;

    bkg.camera_style = horizontal_cam;

    SCX_REG = bkg.camera_x;
    SCY_REG = bkg.camera_y;

    SHOW_BKG;
}