#define LEVEL1_BANK 5
#include <gb/gb.h>

#include "../res/tiles/stage_drop_map.h"
#include "../res/tiles/stage_drop_tiles.h"
#include "scene.h"

extern const struct actor_t level1_actors[5];
extern const struct level_t level1;
extern UINT8 render_actors_count;
void npc_collisions_level1();
void anim_level1();
void spawn_bullets();
void init_submap();
void setup_lvl1();
void enter_lvl1();

// UINT8 init_lvl1_actor_data();
