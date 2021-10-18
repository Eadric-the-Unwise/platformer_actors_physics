#include "submap.h"

#include "../../macros.h"

// extern const void __bank_submap_map;
// extern const void __bank_submap_tiles;

extern Variables bkg;

void init_submap() {
    fadeout_white();
    HIDE_BKG;
    bkg.sliding = FALSE;
    bkg.camera_x = 320;
    bkg.camera_y = 144;
    bkg.old_camera_x = bkg.camera_x;
    bkg.old_camera_y = bkg.camera_y;
    bkg.map_pos_x = (UBYTE)(bkg.camera_x >> 3u);
    bkg.map_pos_y = (UBYTE)(bkg.camera_y >> 3u);

    set_bkg_data_nonbanked(0, SUBMAP_TILE_COUNT, SUBMAP_TILE_DATA, BANK(SUBMAP_TILE_DATA));
    bkg.old_map_pos_x = bkg.old_map_pos_y = 255;
    bkg.camera_max_y = (SUBMAP_MAP_HEIGHT - 18) * 8;
    bkg.camera_max_x = (SUBMAP_MAP_WIDTH - 20) * 8;
    set_bkg_submap_nonbanked(bkg.map_pos_x, bkg.map_pos_y, 20, 18, SUBMAP_MAP_DATA, SUBMAP_MAP_WIDTH, BANK(SUBMAP_MAP_DATA));
    set_level(SUBMAP_MAP_WIDTH, SUBMAP_MAP_HEIGHT, SUBMAP_MAP_DATA, BANK(SUBMAP_MAP_DATA));

    bkg.old_camera_x = bkg.camera_x;
    bkg.old_camera_y = bkg.camera_y;

    bkg.redraw = FALSE;

    bkg.camera_style = scroll_cam;

    SCX_REG = bkg.camera_x;
    SCY_REG = bkg.camera_y;
    SHOW_BKG;

    fadein_white();
}