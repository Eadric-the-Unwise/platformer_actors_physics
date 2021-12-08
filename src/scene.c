#include "scene.h"

#include <gb/gb.h>
#include <string.h>

#include "level.h"
extern Variables bkg;
UINT8 dir, last_dir;

// array of avaliable actors
actor_t active_actors[MAX_ACTIVE_ACTORS];  // active_actors[] is your working structures in WRAM
UINT8 total_actors_count;                  // amount of actors that are currently active

animate_level_t animate_level = NULL;  // level animation function
collide_level_t collide_level = NULL;  // level animation function
load_submap_t load_submap = NULL;

/******************************/
// Load enemies sequencially up to MAX_ACTIVE_ACTORS
/******************************/
void load_scene_actors(const actor_t *actor, uint8_t actors_count) {
    actor_t *current_actor = active_actors;

    UINT8 hiwater = 0;
    for (UINT8 i = actors_count; i != 0; i--) {  // counter direction does not matter, because pointer is moved. only number of iterations matter.
        if (actor->copy == TRUE) {
            hiwater -= actor->tile_count;
            current_actor->tile_index = hiwater;
        } else if (actor->copy == FALSE) {
            current_actor->tile_index = hiwater;
            set_sprite_data(hiwater, actor->tile_count, actor->tile_data);
        }
        current_actor->x = actor->x;
        current_actor->y = actor->y;
        current_actor->SpdX = actor->SpdX;
        current_actor->SpdY = actor->SpdY;
        current_actor->w = actor->w;
        current_actor->h = actor->h;
        current_actor->x_pivot = actor->x_pivot;
        current_actor->y_pivot = actor->y_pivot;
        current_actor->h_offset = actor->h_offset;
        current_actor->x_offset = actor->x_offset;
        current_actor->y_offset = actor->y_offset;
        current_actor->last_direction = current_actor->direction = actor->direction;
        current_actor->NPC_type = actor->NPC_type;
        current_actor->patrol_timer = actor->patrol_timer;
        current_actor->patrol_reset = actor->patrol_reset;
        memcpy(current_actor->animations, actor->animations, sizeof(current_actor->animations));  // copy array of 5 pointers to animation phases
        memcpy(current_actor->animations_props, actor->animations_props, sizeof(actor->animations_props));
        current_actor->animation_phase = actor->animation_phase;
        current_actor->RENDER = actor->RENDER;
        current_actor->ON = actor->ON;
        current_actor->KILL = actor->KILL;
        hiwater += actor->tile_count;
        current_actor++;
        actor++;
    }
    total_actors_count = actors_count;  // copies from ROM to RAM
}

void load_level(const level_t *level) {
    if (level == NULL) return;
    load_scene_actors(level->actors, level->actor_count);  // Loads level1.c actors
    load_submap = level->submap_hook;
    animate_level = level->animate_hook;
    collide_level = level->collide_hook;
}

// calls move_metasprite();, increases hiwater, and clears unnecessary Sprites in OAM after the hiwater's value
uint8_t animation_timer = 6;
void render_actors() {
    actor_t *current_actor = active_actors;
    last_dir = dir;
    dir = PLAYER.direction;
    if (dir != last_dir || animation_timer == 0) {
        if (PLAYER.direction == 10 || PLAYER.direction == 11) {
            animation_timer = 6;
        } else {
            animation_timer = 6;
        }
    }
    animation_timer -= 1;

    // draw each metasprite
    direction_e current_direction;
    UINT8 hiwater = 0;  // OAM Sprite hiwater
    for (UINT8 i = total_actors_count; i != (ACTOR_FIRST_NPC - 1); i--) {
        UINT16 camera_x = TO_PIXELS(bkg.camera_x);
        INT16 actor_x = TO_PIXELS(current_actor->x);
        INT16 NPC_xOffset = actor_x - (current_actor->x_pivot + 8);
        current_direction = current_actor->direction;
        const metasprite_t **current_animation = current_actor->animations[current_direction];
        if (current_animation != NULL) {
            if (current_actor->RENDER == TRUE) {
                if (current_animation[current_actor->animation_phase] != NULL) {
                    if (NPC_xOffset <= 160 && NPC_xOffset >= -48 && current_actor->KILL != TRUE) {
                        current_actor->ON = TRUE;
                        if ((current_direction == DIR_RIGHT) || (current_direction == DIR_JUMP_R) || (current_direction == DIR_IDLE_R) || (current_direction == DIR_DOWN_R) || (current_direction == DIR_CRAWL_R) || (current_direction == DIR_LAND_R) || (current_direction == DIR_DROP_R)) {
                            hiwater += move_metasprite_vflip(
                                current_animation[current_actor->animation_phase],
                                current_actor->tile_index,
                                hiwater,
                                TO_PIXELS(current_actor->x), TO_PIXELS(current_actor->y));
                        } else {
                            hiwater += move_metasprite(
                                current_animation[current_actor->animation_phase],
                                current_actor->tile_index,
                                hiwater,
                                TO_PIXELS(current_actor->x), TO_PIXELS(current_actor->y));
                        }
                    } else {
                        current_actor->ON = FALSE;
                        // current_actor->RENDER = FALSE;
                        hide_metasprite(
                            current_animation[current_actor->animation_phase],
                            hiwater);
                    }
                }
                // process actor animation
                if (animation_timer == 1) {
                    current_actor->animation_phase++;
                    // check for the last animation frame
                    if (current_animation[current_actor->animation_phase] == NULL) {
                        // check for loop of animation
                        if (current_actor->animations_props[current_actor->direction] == ANIM_LOOP) {
                            current_actor->animation_phase = 0;  // loop animation back from frame[0]
                        } else {
                            current_actor->animation_phase--;  // stick to last animation frame
                        }
                    }
                }
            } else {
                current_actor->ON = FALSE;
            }
        }
        current_actor++;
    }
    // hide rest of the hardware sprites
    for (UINT8 i = hiwater; i < 40u; i++) shadow_OAM[i].y = 0;
}
void switch_down() {
    if (PLAYER.direction == DIR_LEFT || PLAYER.direction == DIR_IDLE_L || PLAYER.direction == DIR_DOWN_L || PLAYER.direction == DIR_CRAWL_L || PLAYER.direction == DIR_JUMP_L || PLAYER.direction == DIR_LAND_L) {
        SetActorDirection(&PLAYER, DIR_DOWN_L, 0);
    } else {
        SetActorDirection(&PLAYER, DIR_DOWN_R, 0);
    }
}
void switch_jump() {
    if (PLAYER.direction == DIR_LEFT || PLAYER.direction == DIR_IDLE_L || PLAYER.direction == DIR_JUMP_L || PLAYER.direction == DIR_DOWN_L || PLAYER.direction == DIR_CRAWL_L || PLAYER.direction == DIR_LAND_L || PLAYER.direction == DIR_DROP_L) {
        if (!Drop) {
            SetActorDirection(&PLAYER, DIR_JUMP_L, 0);
        } else {
            SetActorDirection(&PLAYER, DIR_DROP_L, 0);
        }
    } else {
        if (!Drop) {
            SetActorDirection(&PLAYER, DIR_JUMP_R, 0);
        } else {
            SetActorDirection(&PLAYER, DIR_DROP_R, 0);
        }
    }
}
void jump() {
    UINT8 px, py;
    px = TO_PIXELS(PLAYER.x);
    py = TO_PIXELS(PLAYER.y);
    if (Crouch) {
        check_Drop(px, py + 1, TO_PIXELS(bkg.camera_x));
    }
    // CHECK WHETHER CAN JUMP (NO COLLISION ABOVE PLAYER)
    check_J(px, py - 25, TO_PIXELS(bkg.camera_x));
    //     if (Ladder) {
    //     Ladder = FALSE;
    //     // Ladder_Release = TRUE;
    // }
}
void switch_idle() {
    if (PLAYER.direction == DIR_LEFT || PLAYER.direction == DIR_IDLE_L || PLAYER.direction == DIR_DOWN_L || PLAYER.direction == DIR_CRAWL_L || PLAYER.direction == DIR_LAND_L) {
        SetActorDirection(&PLAYER, DIR_IDLE_L, 0);
    } else {
        SetActorDirection(&PLAYER, DIR_IDLE_R, 0);
    }
}
void switch_land() {
    if (PLAYER.direction == DIR_JUMP_R) {
        SetActorDirection(&PLAYER, DIR_LAND_R, 0);
    } else if (PLAYER.direction == DIR_JUMP_L) {
        SetActorDirection(&PLAYER, DIR_LAND_L, 0);
    }
}

void switch_crawl() {
    if (PLAYER.direction == DIR_CRAWL_R) {
        SetActorDirection(&PLAYER, DIR_DOWN_R, 0);
    } else if (PLAYER.direction == DIR_CRAWL_L) {
        SetActorDirection(&PLAYER, DIR_DOWN_L, 0);
    }
}

void gameover() {
    DISPLAY_OFF;
    Spawn = TRUE;
    load_level(current_stage);
    if (load_submap) load_submap();
    render_actors();
    GAMEOVER = FALSE;
    DISPLAY_ON;
}