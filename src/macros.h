#ifndef __MACROS_H__
// prevents the includes from being called twice
#define __MACROS_H__


#include <gb/gb.h>
#include <gb/metasprites.h>
#include <stdint.h>
#include <stdio.h>

#include "submap.h"
#include "level.h"

void set_camera();
void performantdelay(UINT8 numloops);
inline void set_level(UINT8, UINT8, const UINT8 *, UINT8 bank);
void set_bkg_data_nonbanked(UINT8 first_tile, UINT8 nb_tiles, const UINT8 *tile_data, UINT8 bank);
void set_bkg_submap_nonbanked(UINT8 x, UINT8 y, UINT8 w, UINT8 h, const UINT8 *map_data, UINT8 map_w, UINT8 bank);

#define MIN(A, B) ((A) < (B) ? (A) : (B))
//##joins the __bank and 'A' variable name together
#define __BANK_PREFIX(A) __bank_##A
#define BANK(A) (UBYTE) & (__BANK_PREFIX(A))
// #define __SIZE_PREFIX(A) __size_##A
// #define SIZE(A) (UWORD) & (__SIZE_PREFIX(A))
// #define OFFSET(A) (void *)&(A)

#endif