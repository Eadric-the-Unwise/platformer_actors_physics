  #pragma bank 5
#include "submap.h"

#include "camera.h"
#include "scene.h"

extern Variables bkg;

void init_submap() {
    HIDE_BKG;
    bkg.redraw = TRUE;
    bkg.sliding = FALSE;
    bkg.camera_max_y = (STAGE_DROP_MAPHeight - 18) * 8;
    bkg.camera_max_x = (STAGE_DROP_MAPWidth - 20) * 8;
    bkg.camera_tiles_x = STAGE_DROP_MAPWidth * 8;
    bkg.camera_tiles_y = STAGE_DROP_MAPHeight * 8;
    bkg.camera_x = TO_COORDS(bkg.camera_max_x);
    bkg.camera_y = 0;
    bkg.old_camera_x = bkg.camera_x;
    bkg.old_camera_y = bkg.camera_y;
    bkg.map_pos_x = (UINT8)(bkg.camera_x >> 7u);
    bkg.map_pos_y = (UINT8)(bkg.camera_y >> 3u);
    // CHANGE THE TILE COUNT AS YOU ADD TILES TO THE BKG TILE_SET
    set_bkg_data_nonbanked(0, 11, STAGE_DROP_TILES, STAGE_DROP_TILESBank);
    bkg.old_map_pos_x = bkg.old_map_pos_y = 255;

    set_bkg_submap_nonbanked(bkg.map_pos_x, bkg.map_pos_y, 20, 18, STAGE_DROP_MAP, STAGE_DROP_MAPWidth, STAGE_DROP_MAPBank);
    set_level(STAGE_DROP_MAPWidth, STAGE_DROP_MAPHeight, STAGE_DROP_MAP, STAGE_DROP_MAPBank);

    bkg.old_camera_x = bkg.camera_x;
    bkg.old_camera_y = bkg.camera_y;

    bkg.camera_style = horizontal_cam;

    shadow_scx = bkg.camera_x;
    shadow_scy = bkg.camera_y;

    SHOW_BKG;
}