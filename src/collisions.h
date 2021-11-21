#include "../res/tiles/brick_wide_collision.h"
#include "../res/tiles/brick_wide_map.h"
#include "scene.h"

extern UBYTE Spawn, Jump, Crouch, canCrouch, Drop, x_Adjust, Launch, Shooting;
extern UBYTE canCrouch_timer, canCrouch_Ftimer, Drop_timer;

void check_LR(UBYTE newplayerx, UBYTE newplayery, INT16 camera_x, UBYTE *collision_map, UBYTE mapWidth);
void check_UD(UBYTE newplayerx, UBYTE newplayery, INT16 camera_x, UBYTE *collision_map, UBYTE mapWidth);
void check_J(UBYTE newplayerx, UBYTE newplayery, INT16 camera_x, UBYTE *collision_map, UBYTE mapWidth);
void check_Drop(UBYTE newplayerx, UBYTE newplayery, INT16 camera_x, UBYTE *collision_map, UBYTE mapWidth);
void check_C(UBYTE newplayerx, UBYTE newplayery, INT16 camera_x, UBYTE *collision_map, UBYTE mapWidth);
BOOLEAN overlap(INT16 r1_y, INT16 r1_x, INT16 l1_y, INT16 l1_x, INT16 r2_y, INT16 r2_x, INT16 l2_y, INT16 l2_x);