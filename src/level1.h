#include <gb/gb.h>

#include "level.h"
#include "scene.h"

extern const struct actor_t level1_actors[5];
extern UINT8 cam1[3];
extern UINT8 cam2[2];
extern UINT8 cam3[2];
extern UINT8 cam4[1];
extern UINT8 cam1_render[4];
extern UINT8 cam2_render[3];
extern UINT8 cam3_render[3];
extern UINT8 cam4_render[2];
extern const struct level_t level1;
extern UINT8 render_actors_count;
void npc_collisions_level1();
void anim_level1();
void spawn_bullets();
