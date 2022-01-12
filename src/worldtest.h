#define worldtest_BANK 101
#include <gb/gb.h>

#include "../res/tiles/stage_drop_map.h"
#include "../res/tiles/stage_drop_tiles.h"
#include "scene.h"

extern const struct actor_t worldtest_actors[5];
extern UINT8 worldtest_cam1_render[4];
extern UINT8 worldtest_cam2_render[3];
extern UINT8 worldtest_cam3_render[3];
extern UINT8 worldtest_cam4_render[2];
extern UINT8 worldtest_cam1[3];
extern UINT8 worldtest_cam2[2];
extern UINT8 worldtest_cam3[2];
extern UINT8 worldtest_cam4[1];
extern const struct level_t worldtest;
extern UINT8 render_actors_count;
void npc_collisions_worldtest();
void anim_worldtest();
void spawn_bullets_worldtest();
void init_submap_worldtest();
void setup_worldtest();
void enter_worldtest();