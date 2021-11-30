#include "../res/tiles/brick_wide_map.h"
#include "../res/tiles/collision_wide_map.h"
#include "scene.h"

extern UINT8 Spawn, Jump, Gravity, Crouch, canCrouch, Drop, x_Adjust, Launch, Shooting;
extern UINT8 canCrouch_timer, canCrouch_Ftimer, Drop_timer;

void check_LR(UINT8 newplayerx, UINT8 newplayery, INT16 camera_x);
void check_UD(UINT8 newplayerx, UINT8 newplayery, INT16 camera_x);
void check_J(UINT8 newplayerx, UINT8 newplayery, INT16 camera_x);
void check_Drop(UINT8 newplayerx, UINT8 newplayery, INT16 camera_x);
void check_C(UINT8 newplayerx, UINT8 newplayery, INT16 camera_x);
BOOLEAN overlap(INT16 r1_y, INT16 r1_x, INT16 l1_y, INT16 l1_x, INT16 r2_y, INT16 r2_x, INT16 l2_y, INT16 l2_x);
void jump();