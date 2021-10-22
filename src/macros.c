#include "macros.h"

#include "scene.h"

Variables bkg;

void set_camera() {
    // update hardware scroll position
    SCY_REG = bkg.camera_y;
    SCX_REG = (UBYTE) (bkg.camera_x >> 4u);
    // up or down
    bkg.map_pos_y = (UBYTE)(bkg.camera_y >> 3u);
    if (bkg.map_pos_y != bkg.old_map_pos_y)
    {
        if (bkg.camera_y < bkg.old_camera_y)
        {
            set_bkg_submap_nonbanked(bkg.map_pos_x, bkg.map_pos_y, MIN(21u, bkg.level_map_width - bkg.map_pos_x), 1, bkg.level_map_data, bkg.level_map_width, bkg.level_map_bank);
        }
        else
        {
            if ((bkg.level_map_height - 18u) > bkg.map_pos_y)
                set_bkg_submap_nonbanked(bkg.map_pos_x, bkg.map_pos_y + 18u, MIN(21u, bkg.level_map_width - bkg.map_pos_x), 1, bkg.level_map_data, bkg.level_map_width, bkg.level_map_bank);
        }
        bkg.old_map_pos_y = bkg.map_pos_y;
    }
    // left or right
    bkg.map_pos_x = (UBYTE)(bkg.camera_x >> 7u);
    if (bkg.map_pos_x != bkg.old_map_pos_x)
    {
        if (bkg.camera_x < bkg.old_camera_x)
        {
            set_bkg_submap_nonbanked(bkg.map_pos_x, bkg.map_pos_y, 1, MIN(19u, bkg.level_map_height - bkg.map_pos_y), bkg.level_map_data, bkg.level_map_width, bkg.level_map_bank);
        }
        else
        {
            if ((bkg.level_map_width - 20u) > bkg.map_pos_x)
                set_bkg_submap_nonbanked(bkg.map_pos_x + 20u, bkg.map_pos_y, 1, MIN(19u, bkg.level_map_height - bkg.map_pos_y), bkg.level_map_data, bkg.level_map_width, bkg.level_map_bank);
        }
        bkg.old_map_pos_x = bkg.map_pos_x;
    }
    // set old camera position to current camera position
    bkg.old_camera_x = bkg.camera_x, bkg.old_camera_y = bkg.camera_y;
    bkg.camera_x_p = TO_PIXELS(bkg.camera_x);
}

inline void set_level(UINT8 map_width, UINT8 map_height, const UINT8 *map_data, UINT8 bank) {
    bkg.level_map_width = map_width;
    bkg.level_map_height = map_height;
    bkg.level_map_data = map_data;
    bkg.camera_max_y = (map_height - 18) * 8;
    bkg.camera_max_x = (map_width - 20) * 8;
    bkg.level_map_bank = bank;
}

void set_bkg_data_nonbanked(UINT8 first_tile, UINT8 nb_tiles, const UINT8 *tile_data, UINT8 bank)
#ifndef __INTELLISENSE__
    NONBANKED
#endif
{
    UINT8 __save = _current_bank;
    SWITCH_ROM_MBC1(bank);
    set_bkg_data(first_tile, nb_tiles, tile_data);
    SWITCH_ROM_MBC1(__save);
}

void set_bkg_submap_nonbanked(UINT8 x, UINT8 y, UINT8 w, UINT8 h, const UINT8 *map_data, UINT8 map_w, UINT8 bank)
#ifndef __INTELLISENSE__
    NONBANKED
#endif
{
    UINT8 __save = _current_bank;
    SWITCH_ROM_MBC1(bank);
    set_bkg_submap(x, y, w, h, map_data, map_w);
    SWITCH_ROM_MBC1(__save);
}

void performantdelay(UINT8 numloops) {
    UINT8 i;
    for (i = 0; i < numloops; i++) {
        wait_vbl_done();
    }
}
