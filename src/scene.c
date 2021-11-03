#include "scene.h"

#include <gb/gb.h>
#include <string.h>

#include "level.h"
extern Variables bkg;

// array of avaliable actors
actor_t active_actors[MAX_ACTIVE_ACTORS];  // active_actors[] is your working structures in WRAM
UINT8 active_actors_count;                 // amount of actors that are currently active

animate_level_t animate_level = NULL;  // level animation function

/******************************/
// Load enemies sequencially up to MAX_ACTIVE_ACTORS
/******************************/
void load_scene_actors(const actor_t *actor, uint8_t actors_count) {
    actor_t *current_actor = active_actors;

    UINT8 hiwater = 0;
    for (UINT8 i = actors_count; i != 0; i--) {  //counter direction does not matter, because pointer is moved. only number of iterations matter.
        current_actor->tile_index = hiwater;
        set_sprite_data(hiwater, actor->tile_count, actor->tile_data);
        current_actor->x = actor->x;
        current_actor->y = actor->y;
        current_actor->SpdX = actor->SpdX;
        current_actor->SpdY = actor->SpdY;
        current_actor->last_direction = current_actor->direction = actor->direction;
        current_actor->frame_delay = actor->frame_delay;
        memcpy(current_actor->animations, actor->animations, sizeof(current_actor->animations));  // copy array of 5 pointers to animation phases
        memcpy(current_actor->animations_props, actor->animations_props, sizeof(actor->animations_props));
        current_actor->animation_phase = actor->animation_phase;
        hiwater += actor->tile_count;
        current_actor++;
        actor++;
    }
    active_actors_count = actors_count;  //copies from ROM to RAM
}

void load_level(const level_t *level) {
    if (level == NULL) return;
    load_scene_actors(level->actors, level->actor_count);  //Loads level1.c actors
    animate_level = level->animate_hook;
}

//calls move_metasprite();, increases hiwater, and clears unnecessary Sprites in OAM after the hiwater's value
static uint8_t animation_timer = 6;
void render_actors() {
    actor_t *current_actor = active_actors;
    animation_timer -= 1;
    if (animation_timer == 0) {
        animation_timer = 6;
    }
    // draw each metasprite
    direction_e current_direction;
    UINT8 hiwater = 0;
    for (UINT8 i = active_actors_count; i != (ACTOR_FIRST_NPC - 1); i--) {
        UINT16 NPC_PLAYER_Offset = (TO_PIXELS(PLAYER.x) - TO_PIXELS(current_actor->x));
        current_direction = current_actor->direction;
        const metasprite_t **current_animation = current_actor->animations[current_direction];
        if (current_animation != NULL) {
            if (current_animation[current_actor->animation_phase] != NULL) {
                //HERE MUST BE ALL NPC BUT THE PLAYER//
                if (NPC_PLAYER_Offset <= 160 || NPC_PLAYER_Offset >= -100) {
                    if ((current_direction == DIR_RIGHT) || (current_direction == DIR_JUMP_R) || (current_direction == DIR_IDLE_R) || (current_direction == DIR_DOWN_R) || (current_direction == DIR_CRAWL_R)) {
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
                }
                else { 
                    // hiwater -= current_actor->metasprite_count;
                    hide_metasprite(
                    current_animation[current_actor->animation_phase],
                    hiwater);}
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
        }
        current_actor++;
    }
    // hide rest of the hardware sprites
    for (UINT8 i = hiwater; i < 40u; i++) shadow_OAM[i].y = 0;
}
