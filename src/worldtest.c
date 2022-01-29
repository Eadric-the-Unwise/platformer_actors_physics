#pragma bank 100
#include "worldtest.h"

#include <gb/gb.h>
#include <stdlib.h>

#include "../res/tiles/NPC_electric.h"
#include "../res/tiles/detective_large.h"
#include "../res/tiles/elevator.h"

extern Variables bkg;
extern WORLDCAM CAM;
extern UINT8 px, py;
extern UINT8 joy, last_joy;
extern UINT8 GAMESTATE;
extern const level_t *current_stage;
extern BYTE ATTACH, x_Collide, y_Collide;
extern UINT8 current_elevator;
extern UINT8 render_actors_count; // the amount of actors in 160px window, the first actor to load current_actor pointer
extern UINT8 bullet_timer;
extern UINT8 *cam_ptr;

const level_t worldtest = {
    .bank = worldtest_BANK,
    .submap_hook = init_submap_worldtest, // call this in collision
    .actors = worldtest_actors,
    .bullets = NULL,
    .actor_count = 5,
    .animate_hook = anim_worldtest, // function that put life into the scene
    .collide_hook = npc_collisions_worldtest};

// CURRENTLY, LOADING FROM THE RIGHT FORCES YOU TO CALC (X COORD MINUS THE TO_PIXELS(CAM.X)). IS THERE A WAY TO AUTOMATICALLY CAL THIS VALUE UPON LOAD?
//.w and .h are adjusted for COLLISION functions
const actor_t worldtest_actors[5] = {
    // 0 PLAYER
    {.x = TO_COORDS(88),
     .y = TO_COORDS(88),
     .SpdX = 0,
     .SpdY = 0,
     .w = detective_16_WIDTH,
     .h = detective_16_HEIGHT,
     .h_offset = 2,
     .x_offset = 2,
     .y_offset = 8,
     .direction = DIR_LEFT,
     .facing = LEFT,
     .patrol_timer = 12,
     .patrol_max = 12,
     .tile_count = (sizeof(detective_16_data) >> 4),
     .tile_index = 0,
     .tile_data = detective_16_data,
     .bank = detective_16_Bank,
     .animations = {detective_16_WALK, detective_16_WALK, detective_16_UP, detective_16_UP, detective_16_DOWN, detective_16_DOWN, NULL, NULL, detective_16_IDLE, detective_16_IDLE},
     .animations_props = {ANIM_LOOP, ANIM_LOOP, ANIM_LOOP, ANIM_LOOP, ANIM_LOOP, ANIM_LOOP, NULL, NULL, ANIM_ONCE, ANIM_ONCE},
     .animation_phase = 0,
     .copy = FALSE,
     .RENDER = TRUE,
     .ON = TRUE},
    // 1 PATROL
    {.x = TO_COORDS(72),
     .y = TO_COORDS(48),
     .SpdX = 8,
     .SpdY = 0,
     .w = NPC_electric_WIDTH,
     .h = NPC_electric_HEIGHT,
     .x_pivot = NPC_electric_PIVOT_X,
     .y_pivot = NPC_electric_PIVOT_Y,
     .x_offset = 6,
     .y_offset = 6,
     .direction = DIR_UP_L,
     .NPC_type = PATROL,
     .patrol_timer = 78,
     .patrol_reset = 78,
     .patrol_max = 156,
     .tile_count = (sizeof(NPC_electric_data) >> 4),
     .tile_index = 0,
     .tile_data = NPC_electric_data,
     .bank = NPC_electric_data_Bank,
     .animations = {NULL, NULL, NPC_electric_animation, NULL, NPC_electric_animation},
     .animations_props = {NULL, NULL, ANIM_LOOP, NULL, ANIM_LOOP},
     .animation_phase = 0,
     .copy = FALSE},
    // 2 PISTOL
    {.x = TO_COORDS(104),
     .y = TO_COORDS(32),
     .SpdX = -8,
     .SpdY = 0,
     .w = NPC_electric_WIDTH,
     .h = NPC_electric_HEIGHT,
     .x_pivot = NPC_electric_PIVOT_X,
     .y_pivot = NPC_electric_PIVOT_Y,
     .x_offset = 6,
     .y_offset = 6,
     .direction = DIR_LEFT,
     .NPC_type = PISTOL,
     .patrol_timer = 78,
     .patrol_reset = 78,
     .patrol_max = 156,
     .tile_count = (sizeof(NPC_electric_data) >> 4),
     .tile_index = 0,
     .tile_data = NPC_electric_data,
     .bank = NPC_electric_data_Bank,
     .animations = {NPC_electric_animation, NPC_electric_animation},
     .animations_props = {ANIM_LOOP, ANIM_LOOP},
     .animation_phase = 0,
     .copy = TRUE},
    // 3 WALK
    {.x = TO_COORDS(36),
     .y = TO_COORDS(140),
     .SpdX = 8,
     .SpdY = 0,
     .w = NPC_electric_WIDTH,
     .h = NPC_electric_HEIGHT,
     .x_pivot = NPC_electric_PIVOT_X,
     .y_pivot = NPC_electric_PIVOT_Y,
     .x_offset = 6,
     .y_offset = 6,
     .direction = DIR_LEFT,
     .NPC_type = PISTOL,
     .tile_count = (sizeof(NPC_electric_data) >> 4),
     .tile_index = 0,
     .tile_data = NPC_electric_data,
     .bank = NPC_electric_data_Bank,
     .animations = {NPC_electric_animation, NPC_electric_animation},
     .animations_props = {ANIM_LOOP, ANIM_LOOP},
     .animation_phase = 0,
     .copy = TRUE},
    // 4 ELEVATOR
    {.x = TO_COORDS(40),
     .y = TO_COORDS(168),
     .SpdX = 0,
     .SpdY = 8,
     .w = elevator_WIDTH,
     .h = elevator_HEIGHT,
     .x_pivot = elevator_PIVOT_X,
     .y_pivot = elevator_PIVOT_Y,
     .x_offset = elevator_PIVOT_X,
     .y_offset = elevator_PIVOT_Y,
     .direction = DIR_DOWN_L,
     .NPC_type = ELEVATOR,
     .tile_count = (sizeof(elevator_data) >> 4),
     .tile_index = 0,
     .tile_data = elevator_data,
     .bank = elevator_data_Bank,
     .patrol_timer = 1,
     .patrol_reset = 1,
     .patrol_max = 160,
     .animations = {NULL, NULL, elevator_frame, NULL, elevator_frame},
     .animations_props = {NULL, NULL, NULL, NULL, NULL},
     .animation_phase = 0,
     .copy = FALSE}};

const actor_t worldtest_bullets[1] = {
    // 0 BULLET
    {.SpdX = 48,
     .SpdY = 0,
     .w = bullet_WIDTH,
     .h = bullet_HEIGHT,
     .h_offset = bullet_HEIGHT,
     .x_offset = 6,
     .y_offset = 6,
     .NPC_type = BULLET,
     .tile_count = (sizeof(bullet_data) >> 4),
     .animations = {bullet_scroll, bullet_scroll},
     .tile_data = bullet_data,
     .bank = bullet_Bank,
     .copy = FALSE,
     .RENDER = FALSE,
     .ON = FALSE}};

UINT8 worldtest_cam1[2] = {0, 1};
UINT8 worldtest_cam2[3] = {0, 2, 4};
UINT8 worldtest_cam3[1] = {0};
UINT8 worldtest_cam4[2] = {0, 3};

// finds the size of the array by dividing the total sizeof the array by the sizeof the array's first element
UINT8 worldtest_CAM1_NPCs = sizeof(worldtest_cam1) / sizeof(worldtest_cam1[0]) - 1;
UINT8 worldtest_CAM2_NPCs = sizeof(worldtest_cam2) / sizeof(worldtest_cam2[0]) - 1;
UINT8 worldtest_CAM3_NPCs = sizeof(worldtest_cam3) / sizeof(worldtest_cam3[0]) - 1;
UINT8 worldtest_CAM4_NPCs = sizeof(worldtest_cam4) / sizeof(worldtest_cam4[0]) - 1;
// CURRENTLY, WHEN RETURNING TO A STANDING NPC, THEY ARE SHIFTED IF YOU REACH THE END OF THE STAGE AND THEN GO BACK. WE EITHER NEED TO PREVENT PLAYERS FROM RETURNING TO A PREVIOUS POINT, OR *FIX THIS*
// CURRENTLY, IF YOU ARE ABLE TO RETURN TO A PREVIOUS POINT, AND ONE OR MORE NPCS WERE TURNED OFF THEN TURNED BACK ON, THEIR X POSITION WILL BE SHIFTED TO THE LEFT
// void NPC_adjust(UINT8 x, UINT8 y)
// {
//     actor_t *adjust_actor = active_actors; // The Detective is currently active_actors[0], so active_actors[1] and above are enemies
//     adjust_actor++;
//     for (UINT8 i = render_actors_count - 1; i != 0; i--)
//     {
//         adjust_actor->x += TO_COORDS(x);
//         adjust_actor->y += TO_COORDS(y);
//     }
// }

void anim_worldtest()
{
    UINT8 *ptr = NULL;  // pointer // simply = NULL to bypass compiler error lol
    UINT8 *pptr = NULL; // previous pointer
    // UINT8 *nptr = NULL; // next pointer

    // INT16 camera_x = TO_PIXELS(bkg.camera_x);
    // UINT8 player_x = TO_PIXELS(PLAYER.x);
    // UINT16 playerx = TO_PIXELS(PLAYER.x);
    UINT8 active_NPC_count = NULL;  // the amount of actors in 160px window, the first actor to load current_actor pointer
    UINT8 prev_actors_count = NULL; // previous array of sprites to turn off
    UINT8 next_actors_count = NULL; // next array of sprite to turn off (in case you move back to a previous position)

    // TRY LOADING THIS AS A FUNCTION INSTEAD OF A CONSITENT SWITCH CASE
    // if (!ANIMATIONLOCK)
    // {
    switch (CAM)
    {
    case CAM1:
        cam_ptr = worldtest_cam1;
        active_NPC_count = worldtest_CAM1_NPCs;
        ptr = worldtest_cam1;
        if (bkg.slider)
        {
            if (bkg.slide_dir == SLIDEDOWN)
            {
                CAM = CAM4;
                cam_ptr = worldtest_cam4;
                active_NPC_count = worldtest_CAM4_NPCs;
                ptr = worldtest_cam4;
                NPC_reset(worldtest.actor_count);
                reload_NPC_actors(worldtest.actors, worldtest.actor_count);
            }
            else if (bkg.slide_dir == SLIDELEFT) //&&!ANIMATIONLOCK
            {
                CAM = CAM2;
                cam_ptr = worldtest_cam2;
                active_NPC_count = worldtest_CAM2_NPCs;
                ptr = worldtest_cam2;
                NPC_reset(worldtest.actor_count);
                reload_NPC_actors(worldtest.actors, worldtest.actor_count);
            }
        }
        break;
    case CAM2:
        cam_ptr = worldtest_cam2;
        active_NPC_count = worldtest_CAM2_NPCs;
        ptr = worldtest_cam2;
        if (bkg.slider)
        {
            if (bkg.slide_dir == SLIDEDOWN)
            {
                CAM = CAM3;
                cam_ptr = worldtest_cam3;
                active_NPC_count = worldtest_CAM3_NPCs;
                ptr = worldtest_cam3;
                NPC_reset(worldtest.actor_count);
                reload_NPC_actors(worldtest.actors, worldtest.actor_count);
            }
            else if (bkg.slide_dir == SLIDERIGHT)
            {
                CAM = CAM1;
                cam_ptr = worldtest_cam1;
                active_NPC_count = worldtest_CAM1_NPCs;
                ptr = worldtest_cam1;
                NPC_reset(worldtest.actor_count);
                reload_NPC_actors(worldtest.actors, worldtest.actor_count);
            }
        }
        break;
    case CAM3:
        cam_ptr = worldtest_cam3;
        active_NPC_count = worldtest_CAM3_NPCs;
        ptr = worldtest_cam3;
        if (bkg.slider)
        {
            if (bkg.slide_dir == SLIDEUP)
            {
                CAM = CAM2;
                cam_ptr = worldtest_cam2;
                active_NPC_count = worldtest_CAM2_NPCs;
                ptr = worldtest_cam2;
                NPC_reset(worldtest.actor_count);
                reload_NPC_actors(worldtest.actors, worldtest.actor_count);
            }
            else if (bkg.slide_dir == SLIDERIGHT)
            {
                CAM = CAM4;
                cam_ptr = worldtest_cam4;
                active_NPC_count = worldtest_CAM4_NPCs;
                ptr = worldtest_cam4;
                NPC_reset(worldtest.actor_count);
                reload_NPC_actors(worldtest.actors, worldtest.actor_count);
            }
        }
        break;
    case CAM4:
        cam_ptr = worldtest_cam4;
        active_NPC_count = worldtest_CAM4_NPCs;
        ptr = worldtest_cam4;
        if (bkg.slider)
        {
            if (bkg.slide_dir == SLIDEUP)
            {
                CAM = CAM1;
                cam_ptr = worldtest_cam1;
                active_NPC_count = worldtest_CAM1_NPCs;
                ptr = worldtest_cam1;
                NPC_reset(worldtest.actor_count);
                reload_NPC_actors(worldtest.actors, worldtest.actor_count);
            }
            else if (bkg.slide_dir == SLIDELEFT)
            {
                CAM = CAM3;
                cam_ptr = worldtest_cam3;
                active_NPC_count = worldtest_CAM3_NPCs;
                ptr = worldtest_cam3;
                NPC_reset(worldtest.actor_count);
                reload_NPC_actors(worldtest.actors, worldtest.actor_count);
            }
        }
        break;
    }
    // }

    render_actors_count = active_NPC_count + 1;
    ptr++;
    actor_t *current_actor = &active_actors[*ptr]; // The Detective is currently active_actors[0], so active_actors[1] and above are enemies
    actor_t *current_bullet = active_bullets;
    PLAYER.x += PLAYER.SpdX;

    INT16 camx = TO_PIXELS(bkg.camera_x);

    for (UINT8 i = active_NPC_count; i != 0; i--)
    { // TURN ON CURRENT SET OF NPC SPRITES

        if (!ANIMATIONLOCK)
        {
            switch (bkg.slide_dir)
            {
            case SLIDELEFT:
                // current_actor->x += 64;
                // if (TO_PIXELS(current_actor->x) >= 0)
                // {
                current_actor->RENDER = TRUE;
                current_actor->ON = TRUE;
                // }
                break;
            case SLIDERIGHT:
                // current_actor->x -= 64;
                // if (TO_PIXELS(current_actor->x) <= 160)
                // {
                current_actor->RENDER = TRUE;
                current_actor->ON = TRUE;
                // }
                break;
            case SLIDEUP:
                // current_actor->y += 64;
                current_actor->RENDER = TRUE;
                current_actor->ON = TRUE;
                break;
            case SLIDEDOWN:
                // current_actor->y -= 64;
                current_actor->RENDER = TRUE;
                current_actor->ON = TRUE;
                break;
            }

            // else if (!ANIMATIONLOCK)
            // {
            // current_actor->RENDER = TRUE;
            // current_actor->ON = TRUE;
            // current_actor->RENDER = TRUE;
            // current_actor->ON = TRUE;
            if (current_actor->RENDER == TRUE && current_actor->KILL == NULL)
            { // AI RULES FOR ALL NPCS ON THIS PARTICULAR STAGE
                if (current_actor->NPC_type == PATROL)
                { // PATROL NPCS
                    current_actor->patrol_timer--;
                    current_actor->x += current_actor->SpdX;
                    if ((current_actor->direction == DIR_DOWN_L) && (current_actor->patrol_timer == 0))
                    {
                        SetActorDirection(current_actor, DIR_UP_L, 0);
                        current_actor->SpdX = abs(current_actor->SpdX);
                        current_actor->patrol_timer = current_actor->patrol_max;
                    }
                    else if ((current_actor->direction == DIR_UP_L) && (current_actor->patrol_timer == 0))
                    {
                        SetActorDirection(current_actor, DIR_DOWN_L, 0);
                        current_actor->SpdX = -abs(current_actor->SpdX);
                        current_actor->patrol_timer = current_actor->patrol_max;
                    }
                }
                else if (current_actor->NPC_type == ELEVATOR)
                { // ELEVATORS
                    current_actor->patrol_timer--;
                    current_actor->y += current_actor->SpdY;

                    if ((current_actor->direction == DIR_UP_L) && (current_actor->patrol_timer == 0))
                    {
                        SetActorDirection(current_actor, DIR_DOWN_L, 0);
                        current_actor->SpdY = abs(current_actor->SpdY);
                        current_actor->patrol_timer = current_actor->patrol_max;
                    }
                    else if ((current_actor->direction == DIR_DOWN_L) && (current_actor->patrol_timer == 0))
                    {
                        SetActorDirection(current_actor, DIR_UP_L, 0);
                        current_actor->SpdY = -abs(current_actor->SpdY);
                        current_actor->patrol_timer = current_actor->patrol_max;
                    }
                }
                else if (current_actor->NPC_type == WALK)
                { // WALK NPCS WALK STRAIGHT AHEAD
                    INT16 actor_x = TO_PIXELS(current_actor->x);

                    if ((actor_x >= -32) && (actor_x <= 200))
                    {
                        current_actor->x += current_actor->SpdX;
                    }
                    if (actor_x > 196)
                    {
                        current_actor->KILL = TRUE; // KILL NPC IS HE GOES OFF SCREEN TO THE RIGHT TOO FAR
                    }
                }
            }
        }
        // }
        ptr++;
        current_actor = &active_actors[*ptr];
    }
    // for (UINT8 i = MAX_BULLETS; i != 0; i--) {
    //     if (current_bullet->RENDER == TRUE) {
    //         INT16 bullet_x = TO_PIXELS(current_bullet->x);
    //         if ((bullet_x < 0) || (bullet_x > 168)) {
    //             current_bullet->KILL = TRUE;
    //             current_bullet->RENDER = FALSE;
    //         }
    //         if ((camx > 0) && (camx < bkg.camera_max_x)) {  // IF CAM IS NOT IN SPAWN OR END POSITION (ie it's moving)
    //             current_bullet->x -= PLAYER.SpdX;
    //         }
    //         if (current_bullet->facing == LEFT) {
    //             current_bullet->x -= current_bullet->SpdX;
    //         } else if (current_bullet->facing == RIGHT) {
    //             current_bullet->x += current_bullet->SpdX;
    //         }
    //     }
    //     current_bullet++;
    // }
    // if (bullet_timer != 0) {
    //     bullet_timer -= 1;
    // }
}

void spawn_bullets_worldtest()
{
    actor_t *spawn_bullet = active_bullets;
    for (UINT8 i = MAX_BULLETS; i != 0; i--)
    {
        if (spawn_bullet->RENDER == TRUE)
        {
            spawn_bullet++;
        }
        else if (bullet_timer == 0)
        {
            spawn_bullet->RENDER = TRUE;
            spawn_bullet->ON = TRUE;
            if (PLAYER.facing == LEFT)
            { // BULLET IS VISIBLE BEFORE ITS X AXIS IS LESS THAN DETECTIVE
                if ((LADDER) && (PLAYER.direction == DIR_LADDER_R))
                {
                    PLAYER.direction = DIR_LADDER_L;
                }
                spawn_bullet->facing = LEFT;
                spawn_bullet->x = PLAYER.x - TO_COORDS(6);
            }
            else
            {
                if ((LADDER) && (PLAYER.direction == DIR_LADDER_L))
                {
                    PLAYER.direction = DIR_LADDER_R;
                }
                spawn_bullet->facing = RIGHT;
                spawn_bullet->x = PLAYER.x + TO_COORDS(6);
            }
            if (CROUCH)
            {
                spawn_bullet->y = PLAYER.y + TO_COORDS(4);
            }
            else if (LADDER)
            {
                spawn_bullet->y = PLAYER.y - TO_COORDS(4);
            }
            else if (JUMP)
            {
                if (PLAYER.SpdY < 0)
                {
                    spawn_bullet->y = PLAYER.y - TO_COORDS(8);
                }
                else
                {
                    spawn_bullet->y = PLAYER.y - TO_COORDS(4);
                }
            }
            else
            {
                spawn_bullet->y = PLAYER.y;
            }
            bullet_timer = 90;
            break;
        }
    }
    // if (spawn_bullet->RENDER == FALSE) {
    // }
}
void npc_collisions_worldtest()
{
    for (UINT8 i = ACTOR_FIRST_NPC; i != (total_actors_count); i++)
    {
        //[y][x]
        UINT16 PTR_y, PTR_x, PBL_y, PBL_x, NTR_y, NTR_x, NBL_y, NBL_x;
        UINT8 ax, ay;
        ax = TO_PIXELS(active_actors[i].x);
        ay = TO_PIXELS(active_actors[i].y);
        INT16 NPC_PLAYER_Offset = px - (ax - active_actors[i].x_pivot);
        // THE PIVOT IS THE LITERAL CENTER OF THE METASPRITE. NOT A PIXEL, BUT THE CROSSHAIRS IN THE MIDDLE OF THE DESGIN
        PTR_y = py - PLAYER.h_offset;           // TR y
        PTR_x = px + PLAYER.x_offset;           // TR x
        PBL_y = py + PLAYER.y_offset;           // BL y
        PBL_x = px - PLAYER.x_offset;           // BL x
        NTR_y = ay - active_actors[i].y_offset; // TR y
        NTR_x = ax + active_actors[i].x_offset; // TR x
        NBL_y = ay + active_actors[i].y_offset; // BL y
        NBL_x = ax - active_actors[i].x_offset; // BL x
        if (active_actors[i].ON && active_actors[i].KILL == NULL)
        {
            if (overlap(PTR_y, PTR_x, PBL_y, PBL_x, NTR_y, NTR_x, NBL_y, NBL_x) == 0x01U)
            {
                if (active_actors[i].NPC_type != ELEVATOR)
                {
                    GAMEOVER = TRUE;
                }
                else if (active_actors[i].NPC_type == ELEVATOR)
                {
                    if (!ATTACH)
                    {
                        if ((PBL_x > NTR_x - 2) || (PTR_x < NBL_x + 2)) // is not on top of elevator
                        {
                            x_Collide = TRUE;
                        }
                        else if ((PBL_y > NTR_y) && (PBL_y < NBL_y))
                        {
                            ATTACH = TRUE;
                            Gravity = FALSE;
                            current_elevator = i;
                        }
                        else if ((PTR_y < NBL_y) && (PTR_y > NTR_y))
                        {
                            y_Collide = TRUE;
                        }
                    }
                }
            }
            else if (overlap(PTR_y, PTR_x, PBL_y, PBL_x, NTR_y, NTR_x, NBL_y, NBL_x) == 0x00U)
            {
                if (x_Collide)
                {
                    x_Collide = FALSE;
                }
            }
        }
        else
        {
        }
        if (ATTACH)
        {
            if (i == current_elevator)
            {
                if ((PBL_x > NTR_x) || (PTR_x < NBL_x))
                {
                    ATTACH = FALSE;
                    Gravity = TRUE;
                }
                else
                {
                    PLAYER.SpdY = 0;
                    PLAYER.y = TO_COORDS(NTR_y - (PLAYER.h / 2));
                    if (JUMP)
                    {
                        switch_land();
                    }
                    else if (!(joy & J_LEFT) && !(joy & J_RIGHT) && (!CROUCH))
                    {
                        switch_idle();
                    }
                    SPAWN = Gravity = JUMP = FALSE;
                }
            }
        }
    }
}

void init_submap_worldtest()
{
    HIDE_BKG;
    bkg.redraw = TRUE;
    bkg.slider = FALSE;
    bkg.camera_max_x = (WORLD1_MAPWidth - 20) * 8;
    bkg.camera_max_y = (WORLD1_MAPHeight - 18) * 8;
    bkg.camera_tiles_x = WORLD1_MAPWidth * 8;
    bkg.camera_tiles_y = WORLD1_MAPHeight * 8;
    bkg.camera_x = TO_COORDS(bkg.camera_max_x);
    bkg.camera_y = TO_COORDS(0);
    ;
    bkg.old_camera_x = bkg.camera_x;
    bkg.old_camera_y = bkg.camera_y;
    bkg.map_pos_x = (UINT8)(bkg.camera_x >> 7u);
    bkg.map_pos_y = (UINT8)(bkg.camera_y >> 7u);
    // CHANGE THE TILE COUNT AS YOU ADD TILES TO THE BKG TILE_SET
    set_bkg_data_nonbanked(0, WORLD1_TILESLen, WORLD1_TILES, WORLD1_TILESBank);
    bkg.old_map_pos_x = bkg.old_map_pos_y = 255;

    set_bkg_submap_nonbanked(bkg.map_pos_x, bkg.map_pos_y, 20, 18, WORLD1_MAP, WORLD1_MAPWidth, WORLD1_MAPBank);
    set_level(WORLD1_MAPWidth, WORLD1_MAPHeight, WORLD1_MAP, WORLD1_MAPBank);

    bkg.old_camera_x = bkg.camera_x;
    bkg.old_camera_y = bkg.camera_y;

    bkg.camera_style = horizontal_cam;
    // bkg.slide_dir = SLIDERIGHT;

    COLLISION_WIDTH = WORLD1_COLLISIONWidth;
    COLLISION_DATA = WORLD1_COLLISION;
    COLLISION_BANK = WORLD1_COLLISIONBank;
    CAM = CAM1;

    shadow_scx = bkg.camera_x;
    shadow_scy = bkg.camera_y;

    SHOW_BKG;
}

void setup_worldtest()
{
    DISPLAY_OFF;
    SPAWN = FALSE;
    GAMEOVER = JOYLOCK = ANIMATIONLOCK = L_LEFT = L_RIGHT = LADDER = CROUCH = canCROUCH = DROP = FALSE;
    JUMP = LADDER_Release = TRUE;
    load_level(&worldtest);
    render_platform_actors();
    DISPLAY_ON;
    // current_stage = &worldtest;
}

void enter_worldtest()
{
    // load_level(&worldtest);
    // if (load_submap) load_submap();
    setup_worldtest();
    while (GAMESTATE == 4)
    {
        last_joy = joy;
        joy = joypad();
        if (!SPAWN)
        {
            // UINT8 px, py;
            px = TO_PIXELS(PLAYER.x);
            py = TO_PIXELS(PLAYER.y);
            if (!JOYLOCK)
            {
                if (joy & J_LEFT)
                {
                    SetActorDirection(&PLAYER, DIR_LEFT, PLAYER.animation_phase);
                    if (joy & J_UP || joy & J_DOWN)
                    {
                        if (PLAYER.SpdX > -ANGLED_WALK_SPEED)
                        {
                            PLAYER.SpdX -= ANGLED_VELOCITY;
                        }
                        else
                        {
                            PLAYER.SpdX = -ANGLED_WALK_SPEED;
                        }
                    }
                    else
                    {
                        if (PLAYER.SpdX > -MAX_WALK_SPEED)
                        {
                            PLAYER.SpdX -= WALK_VELOCITY;
                        }
                        else
                        {
                            PLAYER.SpdX = -MAX_WALK_SPEED;
                        }
                    }
                }
                else if (joy & J_RIGHT)
                {
                    SetActorDirection(&PLAYER, DIR_RIGHT, PLAYER.animation_phase);
                    if (joy & J_UP || joy & J_DOWN)
                    {
                        if (PLAYER.SpdX < ANGLED_WALK_SPEED)
                        {
                            PLAYER.SpdX += ANGLED_VELOCITY;
                        }
                        else
                        {
                            PLAYER.SpdX = ANGLED_WALK_SPEED;
                        }
                    }
                    else
                    {
                        if (PLAYER.SpdX < MAX_WALK_SPEED)
                        {
                            PLAYER.SpdX += WALK_VELOCITY;
                        }
                        else
                        {
                            PLAYER.SpdX = MAX_WALK_SPEED;
                        }
                    }
                }
                if (joy & J_DOWN)
                {
                    SetActorDirection(&PLAYER, DIR_DOWN_L, PLAYER.animation_phase);
                    if (joy & J_LEFT || joy & J_RIGHT)
                    {
                        if (PLAYER.SpdY < ANGLED_WALK_SPEED)
                        {
                            PLAYER.SpdY += ANGLED_VELOCITY;
                        }
                        else
                            PLAYER.SpdY = ANGLED_WALK_SPEED;
                    }
                    else
                    {
                        if (PLAYER.SpdY < MAX_WALK_SPEED)
                        {
                            PLAYER.SpdY += WALK_VELOCITY;
                        }
                        else
                            PLAYER.SpdY = MAX_WALK_SPEED;
                    }
                }
                else if (joy & J_UP)
                {
                    SetActorDirection(&PLAYER, DIR_UP_L, PLAYER.animation_phase);
                    if (joy & J_LEFT || joy & J_RIGHT)
                    {
                        if (PLAYER.SpdY > ANGLED_WALK_SPEED)
                        {
                            PLAYER.SpdY -= ANGLED_VELOCITY;
                        }
                        else
                            PLAYER.SpdY = -ANGLED_WALK_SPEED;
                    }
                    else
                    {
                        if (PLAYER.SpdY > MAX_WALK_SPEED)
                        {
                            PLAYER.SpdY -= WALK_VELOCITY;
                        }
                        else
                            PLAYER.SpdY = -MAX_WALK_SPEED;
                    }
                }

                //SLIDE CAMERA
                if (px <= 2 && TO_PIXELS(bkg.camera_x) > 0)
                { // if player walks off edge of screen LEFT
                    bkg.slider = TRUE;
                    bkg.slide_dir = SLIDELEFT;
                    JOYLOCK = TRUE;
                    ANIMATIONLOCK = TRUE;
                }
                else if (px >= 174 && TO_PIXELS(bkg.camera_x) < bkg.camera_max_x)
                {
                    bkg.slider = TRUE;
                    JOYLOCK = TRUE;
                    ANIMATIONLOCK = TRUE;
                    bkg.slide_dir = SLIDERIGHT;
                }
                else if (py >= 168 && TO_PIXELS(bkg.camera_y) < bkg.camera_max_y)
                {
                    bkg.slider = TRUE;
                    bkg.slide_dir = SLIDEDOWN;
                    JOYLOCK = TRUE;
                    ANIMATIONLOCK = TRUE;
                }
                else if (py <= 8 && TO_PIXELS(bkg.camera_y) > 0)
                {
                    bkg.slider = TRUE;
                    bkg.slide_dir = SLIDEUP;
                    JOYLOCK = TRUE;
                    ANIMATIONLOCK = TRUE;
                }
            }
        }
        if (bkg.slider)
        {
            PLAYER.SpdX = 0;
            PLAYER.SpdY = 0;
            // If the camera and slide is inside the map, slide, otherwise cancel slide
            switch (bkg.slide_dir)
            {
            case SLIDELEFT:
                bkg.camera_x -= 64; // Move as much as slide in X direction
                PLAYER.x = TO_COORDS(174);
                bkg.redraw = TRUE; // Flag for redraw
                break;
            case SLIDERIGHT:
                bkg.camera_x += 64; // Move as much as slide in X direction
                PLAYER.x = TO_COORDS(0);
                bkg.redraw = TRUE; // Flag for redraw
                break;
            case SLIDEUP:
                bkg.camera_y -= 64; // Move as much as slide in X direction
                PLAYER.y = TO_COORDS(178);
                bkg.redraw = TRUE; // Flag for redraw
                break;
            case SLIDEDOWN:
                bkg.camera_y += 64; // Move as much as slide in X direction
                PLAYER.y = TO_COORDS(0);
                bkg.redraw = TRUE; // Flag for redraw
                break;
            }
            // If camera is at the end of the slide, stop slider
            if (TO_PIXELS(bkg.camera_x) % 160 == 0 && TO_PIXELS(bkg.camera_y) % 144 == 0)
            {
                bkg.slider = FALSE;
                ANIMATIONLOCK = FALSE;
                WALKSTATE = TRUE;
            }
        }
        if (WALKSTATE)
        {
            if (TO_PIXELS(PLAYER.x) > 160)
            {
                PLAYER.SpdX = -MAX_WALK_SPEED;
                SetActorDirection(&PLAYER, DIR_LEFT, PLAYER.animation_phase);
            }
            else if (TO_PIXELS(PLAYER.x) < 16)
            {
                PLAYER.SpdX = MAX_WALK_SPEED;
                SetActorDirection(&PLAYER, DIR_RIGHT, PLAYER.animation_phase);
            }
            else if (TO_PIXELS(PLAYER.y) > 148)
            {
                PLAYER.SpdY = -MAX_WALK_SPEED;
                SetActorDirection(&PLAYER, DIR_UP_L, PLAYER.animation_phase);
            }
            else if (TO_PIXELS(PLAYER.y) < 28)
            {
                PLAYER.SpdY = MAX_WALK_SPEED;
                SetActorDirection(&PLAYER, DIR_DOWN_L, PLAYER.animation_phase);
            }
            else
            {
                JOYLOCK = FALSE;
                WALKSTATE = FALSE;
            }
        }

        // ---------------------------------------------
        // WORLD PHYSICS:
        // FRICTION

        px = TO_PIXELS(PLAYER.x);
        py = TO_PIXELS(PLAYER.y);

        if (PLAYER.SpdY > 0)
        {
            check_world_UD(px, py + 1, TO_PIXELS(bkg.camera_x), TO_PIXELS(bkg.camera_y)); // IF MOVING RIGHT
        }
        else if (PLAYER.SpdY < 0)
        {
            check_world_UD(px, py - 1, TO_PIXELS(bkg.camera_x), TO_PIXELS(bkg.camera_y)); // IF MOVING LEFT
        }

        if (PLAYER.SpdX > 0)
        {
            check_world_LR(px + 1, py, TO_PIXELS(bkg.camera_x), TO_PIXELS(bkg.camera_y)); // IF MOVING RIGHT
        }
        else if (PLAYER.SpdX < 0)
        {
            check_world_LR(px - 1, py, TO_PIXELS(bkg.camera_x), TO_PIXELS(bkg.camera_y)); // IF MOVING LEFT
        }
        else if (ATTACH)
        {
            check_world_UD(px, py, TO_PIXELS(bkg.camera_x), TO_PIXELS(bkg.camera_y)); // IF MOVING RIGHT
            check_world_LR(px, py, TO_PIXELS(bkg.camera_x), TO_PIXELS(bkg.camera_y)); // IF MOVING RIGHT
        }

        UINT8 FRICTIONY;
        UINT8 modY;
        modY = PLAYER.SpdY % 2;
        FRICTIONY = (modY == 0) ? 2 : 1; //if modx == 0, then 1 TRUE
        if (PLAYER.SpdY < 0)
        {
            if (PLAYER.SpdY != -MAX_WALK_SPEED || PLAYER.SpdY <= -MAX_WALK_SPEED && !(joy & J_UP))
            {
                PLAYER.SpdY += FRICTIONY;
            }
        }
        if (PLAYER.SpdY > 0)
        {
            if (PLAYER.SpdY != MAX_WALK_SPEED || PLAYER.SpdY >= MAX_WALK_SPEED && !(joy & J_DOWN))
            {
                PLAYER.SpdY -= FRICTIONY;
            }
        }
        UINT8 FRICTIONX;
        UINT8 modX;
        modX = PLAYER.SpdX % 2;
        FRICTIONX = (modX == 0) ? 2 : 1; //if modx == 0, then 1 TRUE
        if (PLAYER.SpdX < 0)
        {
            if (PLAYER.SpdX != -MAX_WALK_SPEED || PLAYER.SpdX <= -MAX_WALK_SPEED && !(joy & J_LEFT)) //IF NOT MAX SPEED
            {
                PLAYER.SpdX += FRICTIONX;
            }
        }
        if (PLAYER.SpdX > 0)
        {
            if (PLAYER.SpdX != MAX_WALK_SPEED || PLAYER.SpdX >= MAX_WALK_SPEED && !(joy & J_RIGHT)) //IF NOT MAX SPEED
            {
                PLAYER.SpdX -= FRICTIONX;
            }
        }

        // update PLAYER absolute posiiton
        if (!ATTACH)
        {
            PLAYER.y += PLAYER.SpdY;
        }
        px = TO_PIXELS(PLAYER.x);
        py = TO_PIXELS(PLAYER.y);
        // // Change to IDLE state when not moving
        if ((PLAYER.SpdX == 0) && (PLAYER.SpdY == 0))
        {
            if (!(joy & J_LEFT) && !(joy & J_RIGHT) && !(ANIMATIONLOCK))
            {
                switch_idle();
            }
        }

        // UPDATE THE CAMERA POSITION SETTINGS
        // render_camera(px, TO_PIXELS(bkg.camera_x));

        // UPDATE CURRENT LEVEL NPC ANIMATIONS AND X/Y POSITIONS
        // npc_collisions_worldtest();
        if (animate_level)
            animate_level(); // call level animation hook (if any)
        // // CHECK FOR NPC COLLISIONS
        if (collide_level)
            collide_level();
        // RENDER ALL CURRENT ACTORS ON SCREEN
        render_world_actors();

        if (bkg.redraw)
        {
            set_world_camera();
            wait_vbl_done();
            refresh_OAM();
            SCX_REG = shadow_scx;
            SCY_REG = shadow_scy;
            bkg.redraw = FALSE;
        }
        else
        {
            wait_vbl_done();
            refresh_OAM();
        }

        if (GAMEOVER)
        {
            enter_worldtest();
        }
        else if (EXIT1)
        { // GO TO LEVEL1
            EXIT1 = FALSE;
            GAMESTATE = 1;
        }
        else if (EXIT2)
        { // GO TO LEVEL2
            EXIT2 = FALSE;
            GAMESTATE = 2;
        }
    }
}