#include "../res/tiles/brick_wide_map.h"
#include "../res/tiles/collision_wide_map.h"
#include "scene.h"

extern UBYTE Spawn, Jump, Gravity, Crouch, canCrouch, Drop, x_Adjust, Launch, Shooting;
extern UBYTE canCrouch_timer, canCrouch_Ftimer, Drop_timer;

void check_LR(UBYTE newplayerx, UBYTE newplayery, INT16 camera_x);
void check_UD(UBYTE newplayerx, UBYTE newplayery, INT16 camera_x);
void check_J(UBYTE newplayerx, UBYTE newplayery, INT16 camera_x);
void check_Drop(UBYTE newplayerx, UBYTE newplayery, INT16 camera_x);
void check_C(UBYTE newplayerx, UBYTE newplayery, INT16 camera_x);
BOOLEAN overlap(INT16 r1_y, INT16 r1_x, INT16 l1_y, INT16 l1_x, INT16 r2_y, INT16 r2_x, INT16 l2_y, INT16 l2_x);