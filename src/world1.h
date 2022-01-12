#define WORLD1_BANK 100
#include <gb/gb.h>

// #include "../res/tiles/detective_16.h"
#include "scene.h"

extern const struct actor_t world1_actors[1];
extern UINT8 world1_cam1_render[1];
// extern UINT8 world1_cam2_render[3];
// extern UINT8 world1_cam3_render[3];
// extern UINT8 world1_cam4_render[2];
// extern UINT8 world1_cam1[3];
// extern UINT8 world1_cam2[2];
// extern UINT8 world1_cam3[2];
// extern UINT8 world1_cam4[1];
extern const struct level_t world1;
extern UINT8 render_actors_count;
void npc_collisions_world1();
void anim_world1();
// void spawn_bullets_world1();
void init_submap_world1();
void init_submap_example();
void setup_world1();
void enter_world1();