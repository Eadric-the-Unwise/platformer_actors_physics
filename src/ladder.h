#include "collisions.h"
#include "scene.h"

void check_LADDER(UINT8 newplayerx, UINT8 newplayery, INT16 camera_x, const unsigned char *COLLISION_DATA, UINT8 COLLISION_DATAWidth);
// extern UINT8 ONTO_Ladder_timer, DOWN_LADDER_timer, UP_LADDER_timer, OFF_LADDER_timer;
extern UINT8 LADDER, ONTO_Ladder, DOWN_LADDER, DOWN_LADDER_F, UP_LADDER, OFF_LADDER, LADDER_Release;
void ladder();
// HERE IS SOME CODE//