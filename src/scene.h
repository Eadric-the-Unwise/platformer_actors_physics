#ifndef __SCENE_H__
//prevents the includes from being called twice
#define __SCENE_H__

#include <gb/gb.h>
#include <gb/metasprites.h>

#define MAX_ACTIVE_ACTORS 8
#define ACTOR_DETECTIVE 0
#define ACTOR_FIRST_NPC 1
#define FALL_DELAY 12

#define PLAYER active_actors[ACTOR_DETECTIVE]

#define TO_COORDS(A) ((A) << 4)
#define TO_PIXELS(A) ((A) >> 4)

#define GRAVITY 4
#define FRICTION 1
#define JUMP_IMPULSE -64
#define WALK_VELOCITY 4
#define MAX_WALK_SPEED 16
#define MAX_FALL_SPEED 64

typedef enum {
    DIR_LEFT,
    DIR_RIGHT,
    DIR_UP,
    DIR_DOWN,
    DIR_NONE
} direction_e;

typedef enum {
    ANIM_LOOP,
    ANIM_ONCE
} anim_loop_e;

typedef struct actor_t {
    UINT16 x;
    UINT16 y;
    INT16 SpdX;
    INT16 SpdY;
    direction_e direction;

    // tiledata related
    UINT8 tile_count;
    UINT8 tile_index;
    UINT8 frame_delay;
    const UINT8 *tile_data;

    // animation description
    const metasprite_t **animations[5];
    anim_loop_e animations_props[5];
    UINT8 animation_phase;
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

//fuction body is inlined into the code
inline void SetActorDirection(actor_t *actor, direction_e dir, UBYTE phase) {
    if (actor->direction != dir) {
        actor->direction = dir;
        actor->animation_phase = phase;
    }
}

#endif