#define LEVEL2_BANK 7
#include <gb/gb.h>

#include "../res/tiles/stage_drop_map.h"
#include "../res/tiles/stage_drop_tiles.h"
#include "scene.h"

extern const struct actor_t level2_actors[5];
extern UINT8 lvl2_cam1_render[4];
extern UINT8 lvl2_cam2_render[3];
extern UINT8 lvl2_cam3_render[3];
extern UINT8 lvl2_cam4_render[2];
extern UINT8 lvl2_cam1[3];
extern UINT8 lvl2_cam2[2];
extern UINT8 lvl2_cam3[2];
extern UINT8 lvl2_cam4[1];
extern const struct level_t level2;
extern UINT8 render_actors_count;
void npc_collisions_level2();
void anim_level2();
void spawn_bullets_lvl2();
void init_submap_lvl2();
void setup_lvl2();
void enter_lvl2();