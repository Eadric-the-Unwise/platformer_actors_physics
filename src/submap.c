#include "submap.h"

#include "camera.h"
#include "scene.h"

// extern const void __bank_submap_map;
// extern const void __bank_submap_tiles;

extern Variables bkg;


void init_submap() {
    HIDE_BKG;
    bkg.redraw = TRUE;
    bkg.sliding = FALSE;
    bkg.camera_max_y = (BRICK_WIDE_MAPHeight - 18) * 8;
    bkg.camera_max_x = (BRICK_WIDE_MAPWidth - 20) * 8;
    bkg.camera_tiles_x = BRICK_WIDE_MAPWidth * 8;
    bkg.camera_tiles_y = BRICK_WIDE_MAPHeight * 8;
    bkg.camera_x = TO_COORDS(bkg.camera_max_x);
    bkg.camera_y = 0;
    bkg.old_camera_x = bkg.camera_x;
    bkg.old_camera_y = bkg.camera_y;
    bkg.map_pos_x = (UBYTE)(bkg.camera_x >> 7u);
    bkg.map_pos_y = (UBYTE)(bkg.camera_y >> 3u);
//CHANGE THE TILE COUNT AS YOU ADD TILES TO THE BKG TILE_SET
    set_bkg_data_nonbanked(0, 4, BRICK_WIDE_TILES, BRICK_WIDE_TILESBank);
    bkg.old_map_pos_x = bkg.old_map_pos_y = 255;

    set_bkg_submap_nonbanked(bkg.map_pos_x, bkg.map_pos_y, 20, 18, BRICK_WIDE_MAP, BRICK_WIDE_MAPWidth, BRICK_WIDE_MAPBank);
    set_level(BRICK_WIDE_MAPWidth, BRICK_WIDE_MAPHeight, BRICK_WIDE_MAP, BRICK_WIDE_MAPBank);

    bkg.old_camera_x = bkg.camera_x;
    bkg.old_camera_y = bkg.camera_y;

    bkg.camera_style = horizontal_cam;

    SCX_REG = bkg.camera_x;
    SCY_REG = bkg.camera_y;

    SHOW_BKG;
}