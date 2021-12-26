#include "../res/tiles/brick_wide_map.h"
#include "../res/tiles/collision_wide_map.h"
#include "scene.h"

extern UINT8 SPAWN, JUMP, Gravity, CROUCH, canCROUCH, DROP, x_Adjust;
extern UINT8 LEFT, RIGHT;
extern UINT8 canCROUCH_timer, canCROUCH_Ftimer, DROP_timer;
extern UINT8 LR_Offset_X, LR_Offset_kX, UD_Offset_Y, UD_Offset_kY, UD_Offset_LY;

// Release_timer
void check_LR(UINT8 newplayerx, UINT8 newplayery, INT16 camera_x);
void check_UD(UINT8 newplayerx, UINT8 newplayery, INT16 camera_x);
void check_J(UINT8 newplayerx, UINT8 newplayery, INT16 camera_x);
void check_Drop(UINT8 newplayerx, UINT8 newplayery, INT16 camera_x);
void check_C(UINT8 newplayerx, UINT8 newplayery, INT16 camera_x);
BOOLEAN overlap(INT16 r1_y, INT16 r1_x, INT16 l1_y, INT16 l1_x, INT16 r2_y, INT16 r2_x, INT16 l2_y, INT16 l2_x);
