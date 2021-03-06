#ifndef __SCENE_H__
// prevents the includes from being called twice
#define __SCENE_H__

#include <gb/gb.h>
#include <gbdk/metasprites.h>
#include <stdint.h>
#include <stdio.h>

#include "../res/tiles/bullet.h"
#include "../res/tiles/detective_16.h"
#include "../res/tiles/detective_large.h"
#include "../res/tiles/elevator.h"
#include "../res/tiles/level_1_collision.h"
#include "../res/tiles/level_1_map.h"
#include "../res/tiles/level_2_collision.h"
#include "../res/tiles/level_2_map.h"
#include "../res/tiles/level_tileset.h"
#include "../res/tiles/stage_drop_collision.h"
#include "../res/tiles/stage_drop_map.h"
#include "../res/tiles/stage_drop_tiles.h"
#include "../res/tiles/world1_collision.h"
#include "../res/tiles/world1_map.h"
#include "../res/tiles/world1_tiles.h"
#include "camera.h"
#include "collisions.h"
// #include "ladder.h"
#include "level1.h"
#include "level2.h"
// #include "world1.h"
#include "worldtest.h"

// typedef INT8 BOOL; //SAME AS BYTE IN GBDK

#define MAX_ACTIVE_ACTORS 6
#define MAX_BULLETS 2
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
#define WALK_VELOCITY 4
#define MAX_WALK_SPEED 16
#define ANGLED_VELOCITY 3
#define ANGLED_WALK_SPEED 15
#define MAX_CRAWL_SPEED 10
#define MAX_FALL_SPEED 64
// if last_joy and J_A both equal 1, XOR = 0.
#define CHANGED_BUTTONS (last_joy ^ joy)
#define SCROLL_SPD 62

typedef enum {
    DIR_LEFT,
    DIR_RIGHT,
    DIR_UP_L,
    DIR_UP_R,
    DIR_DOWN_L,
    DIR_DOWN_R,
    DIR_CRAWL_L,
    DIR_CRAWL_R,
    DIR_IDLE_L,
    DIR_IDLE_R,
    DIR_IDLE_UP,
    DIR_IDLE_DOWN,
    DIR_JUMP_L,
    DIR_JUMP_R,
    DIR_LAND_L,
    DIR_LAND_R,
    DIR_DROP_L,
    DIR_DROP_R,
    DIR_LADDER_L,
    DIR_LADDER_R,
    DIR_ONTOLADDER_L,
    DIR_ONTOLADDER_R,
    DIR_OFFLADDER_L,
    DIR_OFFLADDER_R,
} direction_e;

typedef enum {
    LEFT,
    RIGHT,
} facing_e;

typedef enum {
    TALK,
    PATROL,
    PISTOL,
    WALK,
    ELEVATOR,
    BULLET,
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
    facing_e facing;
    // NPC type
    NPC_type_e NPC_type;
    // tiledata related
    UINT8 tile_count;
    UINT8 tile_index;
    UINT8 patrol_timer;
    UINT8 patrol_reset;
    UINT8 bullet_timer;
    UINT8 bullet_reset;
    UINT8 patrol_max;
    const UINT8 *tile_data;  // const variables cannot be manipulated. Initialized only ONCE
    UINT8 bank;
    UINT8 actor;
    UINT8 bullet;
    // animation description
    const metasprite_t **animations[24];  // list all DIRs in level's actors struct, up to max of [this value]
    anim_loop_e animations_props[24];     // equivilent to above DIRs to define whether they loop or play ONCE
    UINT8 animation_phase;                // frame of metasprite animation loop
    UINT8 copy;
    UINT8 RENDER;  // if a stage has multiple of an NPC design, this variable will keep hiwater from loading it into tile data more than once
    UINT8 ON;
    UINT8 KILL;  // if disabled, the NPC will hide_metasprite();
} actor_t;

typedef void (*animate_level_t)();
typedef void (*collide_level_t)();
typedef void (*load_submap_t)();

typedef struct level_t {
    UINT8 bank;
    load_submap_t submap_hook;
    const actor_t *actors;
    const actor_t *bullets;
    UINT8 actor_count;
    UINT8 bullet_count;
    animate_level_t animate_hook;
    collide_level_t collide_hook;
} level_t;

// from scene.c
extern actor_t active_actors[MAX_ACTIVE_ACTORS];
extern actor_t active_bullets[MAX_BULLETS];
extern animate_level_t animate_level;
extern collide_level_t collide_level;
extern load_submap_t load_submap;
extern UINT8 total_actors_count;
// extern UINT8 RENDERCAM;
extern UINT8 hiwater;

extern const level_t *current_stage;
void load_level(const level_t *level);
UINT8 load_scene_actors(const actor_t *actor, UINT8 actors_count);
UINT8 reload_NPC_actors(const actor_t *actor, UINT8 actors_count);
void load_bullets(const actor_t *bullet, UINT8 bullet_count, UINT8 hiwater);
void render_platform_actors();
void render_world_actors();
void switch_down();
void switch_jump();
void switch_idle();
void switch_idle_world();
void switch_land();
void switch_crawl();
void switch_onto_ladder();
void switch_off_ladder();
void switch_ladder();
void LEFT_RIGHT();
void jump();
void land();
void setGameBank(UBYTE i);
void gameover();
void NPC_reset(UINT8 actor_count);
extern UINT8 GAMEOVER;
extern UINT8 WALKSTATE;
extern UINT8 EXIT1, EXIT2;
extern UINT8 JOYLOCK;
extern UINT8 ANIMATIONLOCK;
// extern UINT8 NPC_RESET;
extern uint8_t animation_timer;

extern uint8_t shadow_scx, shadow_scy;

// fuction body is inlined into the code
inline void SetActorDirection(actor_t *actor, direction_e dir, UINT8 phase) {
    if (actor->direction != dir) {
        actor->last_direction = actor->direction;
        actor->direction = dir;
        actor->animation_phase = phase;
    }
}

#endif