#include <gb/gb.h>

#include "level.h"
#include "scene.h"

extern const struct actor_t level1_actors[5];
extern const struct level_t level1;
extern UINT8 active_actors_count;
void load_bullet_data(UINT8 hiwater);
void npc_collisions_level1();
void anim_level1();
