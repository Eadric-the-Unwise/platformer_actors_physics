#include <gb/gb.h>

#include "level.h"
#include "scene.h"

extern const struct actor_t level1_actors[5];
extern UINT8 cam1[4];
extern UINT8 cam2[3];
extern UINT8 cam3[3];
extern UINT8 cam4[2];
extern const struct level_t level1;
extern UINT8 active_actors_count;
void npc_collisions_level1();
void anim_level1();
