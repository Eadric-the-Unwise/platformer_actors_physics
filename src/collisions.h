#include "../res/tiles/stage_drop_collision.h"
#include "../res/tiles/stage_drop_map.h"
#include "scene.h"

extern BYTE SPAWN, JUMP, Gravity, CROUCH, canCROUCH, DROP, x_Adjust;
extern BYTE L_LEFT, L_RIGHT;
extern BYTE canCROUCH_timer, canCROUCH_Ftimer, DROP_timer;
extern UINT8 ONTO_Ladder_timer, DOWN_LADDER_timer, UP_LADDER_timer, OFF_LADDER_timer;
extern UINT8 LADDER, ONTO_Ladder, DOWN_LADDER, DOWN_LADDER_F, UP_LADDER, OFF_LADDER, LADDER_Release;
extern BYTE LR_Offset_X, LR_Offset_kX, UD_Offset_Y, UD_Offset_kY, UD_Offset_LY;
extern const UINT8 *COLLISION_DATA;
extern unsigned char COLLISION_WIDTH;
extern UINT8 COLLISION_BANK;

// Release_timer
void check_LR(UINT8 newplayerx, UINT8 newplayery, INT16 camera_x);
void check_UD(UINT8 newplayerx, UINT8 newplayery, INT16 camera_x);
void check_J(UINT8 newplayerx, UINT8 newplayery, INT16 camera_x);
void check_Drop(UINT8 newplayerx, UINT8 newplayery, INT16 camera_x);
void check_C(UINT8 newplayerx, UINT8 newplayery, INT16 camera_x);
BYTE overlap(INT16 r1_y, INT16 r1_x, INT16 l1_y, INT16 l1_x, INT16 r2_y, INT16 r2_x, INT16 l2_y, INT16 l2_x);
void check_world_LR(UINT8 newplayerx, UINT8 newplayery, INT16 camera_x, INT16 camera_y);
void check_world_UD(UINT8 newplayerx, UINT8 newplayery, INT16 camera_x, INT16 camera_y);
