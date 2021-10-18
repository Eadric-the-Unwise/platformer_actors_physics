#ifndef __MACROS_H__
//prevents the includes from being called twice
#define __MACROS_H__

#include <gb/gb.h>
#include <stdio.h>
#include <stdint.h>
#include <gb/font.h>
#include <gb/console.h>
#include <gb/drawing.h>
#include <gb/metasprites.h>
#include "character.h"
#include "character_smoke.h"
#include "level.h"

#include "tiles/detective/tile_detectivewalk.h"
#include "tiles/detective/cig_shine.h"
#include "tiles/detective/smoke.h"

#include "maps/submap/submap.h"
#include "maps/house/house.h"
#include "maps/house/cellar/cellar.h"
#include "maps/house/cellar/cellar_lvl2/cellar_lvl2.h"

void set_camera();
void performantdelay(UINT8 numloops);
inline void set_level(UINT8, UINT8, const UINT8 *, UINT8 bank);
void set_bkg_data_nonbanked(UINT8 first_tile, UINT8 nb_tiles, const UINT8 *tile_data, UINT8 bank);
void set_bkg_submap_nonbanked(UINT8 x, UINT8 y, UINT8 w, UINT8 h, const UINT8 *map_data, UINT8 map_w, UINT8 bank);
void fadein_black();
void fadeout_black();
void fadein_white();
void fadeout_white();

#define MIN(A, B) ((A) < (B) ? (A) : (B))
//##joins the __bank and 'A' variable name together
#define __BANK_PREFIX(A) __bank_##A
#define BANK(A) (UBYTE) & (__BANK_PREFIX(A))
// #define __SIZE_PREFIX(A) __size_##A
// #define SIZE(A) (UWORD) & (__SIZE_PREFIX(A))
// #define OFFSET(A) (void *)&(A)

#define DEFAULT_BKG_PALETTE 0x1B
#define DEFAULT_OBJ0_PALETTE 0xE4;
#define DEFAULT_OBJ1_PALETTE 0xE1;

#define FADE_DELAY 120

#define STOPPED 0
#define RESTART 1
#define PLAYING 2 // Sprite delays (60 = 1 second)
#define SMOKE_OBJECT_COUNT 4

#define FRAME_DELAY 7
#define SMOKE_DELAY 12            //walking smoke animation frame delay
#define SMOKE_IDLE_DELAY 11       //standing smoke animation frame delay
#define SMOKE_IDLE_START_DELAY 90 //pause time between smoke puffs
#define SMOKE_WALK_START_DELAY 10

//SPRITE INDEX IS NOT RELATED TO THE TILE INDEX. These are used to load the Sprite OBJ's into OAM in order
//0 in the OAM

//amount of sprites
// #define DETECTIVE_BODY_SPRITE_COUNT 4
#define SMOKE_SMOKE_SPRITE_INDEX 0                      //OAM
#define SMOKE_TILE_COUNT (SMOKE_SMOKE_SPRITE_INDEX + 2) //assures ShadowOAM only effects the smoke tiles
#define DETECTIVE_CIG_SHINE_SPRITE_INDEX 2              //OAM
#define DETECTIVE_CIG_SPRITE 3
// #define DETECTIVE_CIG_SHINE_TILE_COUNT (DETECTIVE_CIG_SHINE_SPRITE_INDEX + 2)

#define DETECTIVE_BODY_SPRITE_INDEX 5 //OAM

// DETECTIVE body stand
#define DETECTIVE_BODY_STAND_FRAME 0

#define DETECTIVE_BODY_UP_STAND_FRAME 4
#define DETECTIVE_BODY_DOWN_STAND_FRAME 8

// DETECTIVE body walk animations + smoke animations
#define DETECTIVE_BODY_WALK_FRAME_START 0
#define DETECTIVE_BODY_WALK_FRAME_END 3

#define DETECTIVE_BODY_UP_FRAME_START 4
#define DETECTIVE_BODY_UP_FRAME_END 7

#define DETECTIVE_BODY_DOWN_FRAME_START 8
#define DETECTIVE_BODY_DOWN_FRAME_END 11

#define DETECTIVE_SMOKE_STAND_FRAME_START 0
#define DETECTIVE_SMOKE_STAND_FRAME_END 5

#define DETECTIVE_SMOKE_WALK_LR_FRAME_START 6
#define DETECTIVE_SMOKE_WALK_LR_FRAME_END 8

#define DETECTIVE_SMOKE_WALK_UPDOWN_FRAME_START 9
#define DETECTIVE_SMOKE_WALK_UPDOWN_FRAME_END 11

#define TILE_SIZE 8

#define FACE_UP 0
#define FACE_LEFT 1
#define FACE_RIGHT 2
#define FACE_DOWN 3

#endif