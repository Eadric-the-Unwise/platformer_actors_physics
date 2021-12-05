#ifndef __SCENE_H__
// prevents the includes from being called twice
#define __SCENE_H__

#include <gb/gb.h>
#include <gbdk/metasprites.h>

#include "../res/tiles/brick_wide_map.h"
#include "../res/tiles/brick_wide_tiles.h"
#include "../res/tiles/collision_wide_map.h"
#include "../res/tiles/detective_large.h"
#include "../res/tiles/enemy_arrow.h"
#include "../res/tiles/vertical_platform_V1.h"
#include "camera.h"
#include "collisions.h"
#include "collisions.h"  //Drop
#include "level1.h"
#include "level2.h"

#define MAX_ACTIVE_ACTORS 8
#define ACTOR_DETECTIVE 0
#define ACTOR_FIRST_NPC 1
#define FALL_DELAY 12

#define PLAYER active_actors[ACTOR_DETECTIVE]
#define NPC(A) active_actors[A]

#define TO_COORDS(A) ((A) << 4)
#define TO_PIXELS(A) ((A) >> 4)
#define TO_TILES(A) ((A) >> (4 + 3))  // 4 is "subpixels to pixels" and 3 is "pixels to tiles".

#define GRAVITY 4
#define FRICTION 2
#define JUMP_IMPULSE -64
#define LAUNCH_IMPULSE -94
#define WALK_VELOCITY 4
#define MAX_WALK_SPEED 16
#define MAX_CRAWL_SPEED 10
#define MAX_FALL_SPEED 64
// if last_joy and J_A both equal 1, XOR = 0.
#define CHANGED_BUTTONS (last_joy ^ joy)

typedef enum {
    DIR_LEFT,
    DIR_RIGHT,
    DIR_DOWN_L,
    DIR_DOWN_R,
    DIR_CRAWL_L,
    DIR_CRAWL_R,
    DIR_IDLE_L,
    DIR_IDLE_R,
    DIR_JUMP_L,
    DIR_JUMP_R,
    DIR_LAND_L,
    DIR_LAND_R,
    DIR_DROP_L,
    DIR_DROP_R,
    DIR_UP,
    DIR_DOWN,
} direction_e;

typedef enum {
    PATROL,
    PISTOL,
    WALK,
    ELEVATOR,
} NPC_type_e;

typedef enum {
    ANIM_LOOP,
    ANIM_ONCE
} anim_loop_e;

typedef struct actor_t {
    INT16 x;
    INT16 y;
    INT16 SpdX;
    INT16 SpdY;
    INT8 w;
    INT8 h;
    INT8 x_pivot;
    INT8 y_pivot;
    INT8 h_offset;  // y - value
    INT8 x_offset;
    INT8 y_offset;  // y + value

    // direction
    direction_e direction;
    direction_e last_direction;

    // NPC type
    NPC_type_e NPC_type;

    // tiledata related
    UINT8 tile_count;
    UINT8 tile_index;
    UINT8 patrol_timer;
    UINT8 patrol_reset;
    const UINT8 *tile_data;  // const variables cannot be manipulated. Initialized only ONCE

    // animation description
    const metasprite_t **animations[116];  // list all DIRs in level's actors struct, up to max of [this value]
    anim_loop_e animations_props[16];      // equivilent to above DIRs to define whether they loop or play ONCE
    UINT8 animation_phase;                 // frame of metasprite animation loop
    UINT8 copy;
    UINT8 RENDER;  // if a stage has multiple of an NPC design, this variable will keep hiwater from loading it into tile data more than once
    UINT8 ON;
    UINT8 KILL;  // if disabled, the NPC will hide_metasprite();
} actor_t;

typedef void (*animate_level_t)();
typedef void (*collide_level_t)();

typedef struct level_t {
    const actor_t *actors;
    UINT8 actor_count;
    animate_level_t animate_hook;
    collide_level_t collide_hook;
} level_t;

// from scene.c
extern actor_t active_actors[MAX_ACTIVE_ACTORS];
extern animate_level_t animate_level;
extern collide_level_t collide_level;
extern UINT8 total_actors_count;

void load_level(const level_t *level);
void render_actors();
void switch_down();
void switch_jump();
void switch_idle();
void switch_land();
void switch_crawl();
void jump();

// fuction body is inlined into the code
inline void SetActorDirection(actor_t *actor, direction_e dir, UINT8 phase) {
    if (actor->direction != dir) {
        actor->last_direction = actor->direction;
        actor->direction = dir;
        actor->animation_phase = phase;
    }
}

#endif