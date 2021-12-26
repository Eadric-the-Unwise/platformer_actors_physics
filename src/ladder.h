#include "collisions.h"
#include "scene.h"

void check_LADDER(UINT8 newplayerx, UINT8 newplayery, INT16 camera_x);
extern UINT8 ONTO_Ladder_timer, DOWN_LADDER_timer;
extern UINT8 LADDER, ONTO_Ladder, UP_LADDER, DOWN_LADDER, DOWN_LADDER_F, LADDER_Release;
void ladder();
// HERE IS SOME CODE//