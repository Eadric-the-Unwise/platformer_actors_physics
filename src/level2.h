#define LEVEL2_BANK 7
#include <gb/gb.h>

#include "../res/tiles/stage_drop_map.h"
#include "../res/tiles/stage_drop_tiles.h"
#include "scene.h"

extern const struct actor_t level2_actors[5];
extern const struct level_t level2;
extern UINT8 render_actors_count;
void npc_collisions_level2();
void anim_level2();
void spawn_bullets_lvl2(UINT8 bullet_number);
void init_submap_lvl2();
void setup_lvl2();
void enter_lvl2();