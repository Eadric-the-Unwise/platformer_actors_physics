#define worldtest_BANK 100
#include <gb/gb.h>

#include "../res/tiles/stage_drop_map.h"
#include "../res/tiles/stage_drop_tiles.h"
#include "scene.h"

extern const struct actor_t worldtest_actors[5];
extern const struct level_t worldtest;
extern UINT8 render_actors_count;
void npc_collisions_worldtest();
void anim_worldtest();
void spawn_bullets_worldtest();
void init_submap_worldtest();
void setup_worldtest();
void enter_worldtest();