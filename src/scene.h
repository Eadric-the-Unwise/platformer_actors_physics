#ifndef __SCENE_H__
//prevents the includes from being called twice
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
#define TO_TILES(A) ((A) >> (4 + 3))  //4 is "subpixels to pixels" and 3 is "pixels to tiles".

#define GRAVITY 4
#define FRICTION 2
#define JUMP_IMPULSE -64
#define LAUNCH_IMPULSE -94
#define WALK_VELOCITY 4
#define MAX_WALK_SPEED 16
#define MAX_CRAWL_SPEED 10
#define MAX_FALL_SPEED 64
//if last_joy and J_A both equal 1, XOR = 0.
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
    INT8 w_offset;  //x - value (only needed if sprite hitbox is not evenly centered, otherwise just use x_offset for both + and -)
    INT8 h_offset;  //y - value
    INT8 x_offset;
    INT8 y_offset;  //y + value
    direction_e direction;
    direction_e last_direction;
    NPC_type_e NPC_type;
    // tiledata related
    UINT8 tile_count;
    UINT8 tile_index;
    UINT16 patrol_timer;
    UINT16 patrol_reset;
    const UINT8 *tile_data;

    // animation description
    const metasprite_t **animations[14];
    anim_loop_e animations_props[14];
    UINT8 animation_phase;
    UBYTE copy;
    UBYTE ON;
} actor_t;

typedef void (*animate_level_t)();

typedef struct level_t {
    const actor_t *actors;
    UINT8 actor_count;
    animate_level_t animate_hook;
} level_t;

extern actor_t active_actors[MAX_ACTIVE_ACTORS];
extern animate_level_t animate_level;

extern UINT8 active_actors_count;

void load_level(const level_t *level);
void render_actors();
void switch_down();
void switch_jump();
void switch_idle();
void switch_land();
void switch_crawl();
void jump();

//fuction body is inlined into the code
inline void SetActorDirection(actor_t *actor, direction_e dir, UBYTE phase) {
    if (actor->direction != dir) {
        actor->last_direction = actor->direction;
        actor->direction = dir;
        actor->animation_phase = phase;
    }
}

#endif