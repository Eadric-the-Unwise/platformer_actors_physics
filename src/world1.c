#pragma bank 100
#include "world1.h"

#include <gb/gb.h>
#include <stdlib.h>

#include "../res/tiles/detective_16.h"

extern Variables bkg;
extern UINT8 px, py;
extern UINT8 joy, last_joy;
extern UINT8 gamestate;
extern const level_t *current_stage;
extern BYTE ATTACH, x_Collide, y_Collide;
extern UINT8 current_elevator;
extern UINT8 render_actors_count;  // the amount of actors in 160px window, the first actor to load current_actor pointer
extern UINT8 bullet_timer;
extern UINT8 *cam_ptr;

const level_t world1 = {
    .bank = WORLD1_BANK,
    .submap_hook = init_submap_example,  // call this in collision
    .actors = world1_actors,
    // .bullets = world1_bullets,
    .actor_count = 1,
    .animate_hook = anim_world1,  // function that put life into the scene
    .collide_hook = npc_collisions_world1};

// CURRENTLY, LOADING FROM THE RIGHT FORCES YOU TO CALC (X COORD MINUS THE TO_PIXELS(CAM.X)). IS THERE A WAY TO AUTOMATICALLY CAL THIS VALUE UPON LOAD?
//.w and .h are adjusted for COLLISION functions
const actor_t world1_actors[1] = {
    // 0 PLAYER
    {.x = TO_COORDS(144),
     .y = TO_COORDS(16),
     .SpdX = 0,
     .SpdY = 0,
     .w = detective_16_WIDTH,
     .h = detective_16_HEIGHT,
     .h_offset = 7,
     .x_offset = 6,
     .y_offset = 16,
     .direction = DIR_JUMP_L,
     .facing = LEFT,
     .patrol_timer = 12,
     .patrol_reset = 12,
     .tile_count = (sizeof(detective_16_data) >> 4),
     .tile_index = 0,
     .tile_data = detective_16_data,
     .bank = detective_16_Bank,
     .animations = {detective_16_WALK, detective_16_WALK, detective_16_UP, NULL, detective_16_DOWN, NULL},
     .animations_props = {ANIM_LOOP, ANIM_LOOP, ANIM_LOOP, NULL, ANIM_LOOP, NULL},
     .animation_phase = 3,
     .copy = FALSE,
     .RENDER = TRUE,
     .ON = TRUE}};

// const actor_t world1_bullets[1] = {
//     // 0 BULLET
//     {.SpdX = 48,
//      .SpdY = 0,
//      .w = bullet_WIDTH,
//      .h = bullet_HEIGHT,
//      .h_offset = bullet_HEIGHT,
//      .x_offset = 6,
//      .y_offset = 6,
//      .NPC_type = BULLET,
//      .tile_count = (sizeof(bullet_data) >> 4),
//      .animations = {bullet_scroll, bullet_scroll},
//      .tile_data = bullet_data,
//      .bank = bullet_Bank,
//      .copy = FALSE,
//      .RENDER = FALSE,
//      .ON = FALSE}};

// current_actor = cam1 + 1
//  for loop
// aka don't need double arrays here
UINT8 world1_cam1_render[1] = {0};
// UINT8 world1_cam2_render[3] = {0, 3, 4};
// UINT8 world1_cam3_render[3] = {0, 3, 4};
// UINT8 world1_cam4_render[2] = {0, 4};
// UINT8 world1_cam1[3] = {1, 2, 3};
// UINT8 world1_cam2[2] = {3, 4};
// UINT8 world1_cam3[2] = {3, 4};
// UINT8 world1_cam4[1] = {4};

#define world1_CAM1_COUNT 0
// #define world1_CAM2_COUNT 2
// #define world1_CAM3_COUNT 2
// #define world1_CAM4_COUNT 1  // CURRENTLY, WHEN RETURNING TO A STANDING NPC, THEY ARE SHIFTED IF YOU REACH THE END OF THE STAGE AND THEN GO BACK. WE EITHER NEED TO PREVENT PLAYERS FROM RETURNING TO A PREVIOUS POINT, OR *FIX THIS*
// CURRENTLY, IF YOU ARE ABLE TO RETURN TO A PREVIOUS POINT, AND ONE OR MORE NPCS WERE TURNED OFF THEN TURNED BACK ON, THEIR X POSITION WILL BE SHIFTED TO THE LEFT
void anim_world1() {
    UINT8 *ptr = NULL;   // pointer // simply = NULL to bypass compiler error lol
    UINT8 *pptr = NULL;  // previous pointer
    UINT8 *nptr = NULL;  // next pointer

    INT16 camera_x = TO_PIXELS(bkg.camera_x);
    UINT8 player_x = TO_PIXELS(PLAYER.x);
    // UINT16 playerx = TO_PIXELS(PLAYER.x);
    UINT8 active_actors_count = NULL;  // the amount of actors in 160px window, the first actor to load current_actor pointer
    UINT8 prev_actors_count = NULL;    // previous array of sprites to turn off
    UINT8 next_actors_count = NULL;    // next array of sprite to turn off (in case you move back to a previous position)

    // if ((camera_x >= 480) && (camera_x <= bkg.camera_max_x)) {  // CAM1
   
    //     cam_ptr = world1_cam1_render;
        active_actors_count = world1_CAM1_COUNT;
    //     next_actors_count = world1_CAM2_COUNT;
    //     ptr = world1_cam1;
    //     nptr = world1_cam2;
    // } else if ((camera_x >= 320) && (camera_x < 480)) {  // CAM2
    //                                                  
    //     cam_ptr = world1_cam2_render;
    //     prev_actors_count = world1_CAM1_COUNT;
    //     active_actors_count = world1_CAM2_COUNT;
    //     next_actors_count = world1_CAM3_COUNT;
    //     pptr = world1_cam1;
    //     ptr = world1_cam2;
    //     nptr = world1_cam3;
    // } else if ((camera_x >= 160) && (camera_x < 320)) {  // CAM3
    //                                                     
    //     cam_ptr = world1_cam3_render;
    //     prev_actors_count = world1_CAM2_COUNT;
    //     active_actors_count = world1_CAM3_COUNT;
    //     next_actors_count = world1_CAM4_COUNT;
    //     pptr = world1_cam2;
    //     ptr = world1_cam3;
    //     nptr = world1_cam4;
    // } else if (camera_x < 160) {  // CAM4
    //     cam_ptr = world1_cam4_render;
    //     
    //     prev_actors_count = world1_CAM3_COUNT;
    //     active_actors_count = world1_CAM4_COUNT;
    //     pptr = world1_cam3;
    //     ptr = world1_cam4;
    // }
    cam_ptr = world1_cam1_render;
    render_actors_count = active_actors_count + 1;
    actor_t *current_actor = &active_actors[*ptr];  // The Detective is currently active_actors[0], so active_actors[1] and above are enemies
    actor_t *prev_actor = &active_actors[*pptr];
    actor_t *next_actor = &active_actors[*nptr];
    actor_t *current_bullet = active_bullets;

    // SIDE SCROLLING CAMERA LOCKED AT 118
    // if ((camera_x > 0) && (camera_x < bkg.camera_max_x)) {
    //     bkg.camera_x += PLAYER.SpdX;
        // bkg.redraw = TRUE;

    // } else
    //     PLAYER.x += PLAYER.SpdX;
    // if ((camera_x - 1) <= 0) {
    //     if ((joy & J_RIGHT) && (player_x >= 118)) {
    //         bkg.camera_x += PLAYER.SpdX;
    //         bkg.redraw = TRUE;
    //     }
    // } else if ((camera_x + 1) >= bkg.camera_max_x) {
    //     if ((joy & J_LEFT) && (player_x <= 118)) {
    //         bkg.camera_x += PLAYER.SpdX;
    //         bkg.redraw = TRUE;
    //     }
    // }

    // for (UINT8 x = prev_actors_count; x != 0; x--) {  // TURN OFF PREVIOUS SET OF SPRITES
    //     prev_actor->RENDER = FALSE;
    //     // prev_actor++;
    //     pptr++;
    //     prev_actor = &active_actors[*pptr];
    // }
    // for (UINT8 x = next_actors_count; x != 0; x--) {  // TURN OFF NEXT SET OF SPRITES
    //     next_actor->RENDER = FALSE;
    //     // next_actor++;
    //     nptr++;
    //     next_actor = &active_actors[*nptr];
    // }

    //     INT16 camx = TO_PIXELS(bkg.camera_x);
    //     for (UINT8 i = active_actors_count; i != 0; i--) {  // TURN ON CURRENT SET OF SPRITES
    //         current_actor->RENDER = TRUE;

    //         if ((camx > 0) && (camx < bkg.camera_max_x)) {  // IF CAM IS NOT IN SPAWN OR END POSITION (ie it's moving)
    //             current_actor->x -= PLAYER.SpdX;
    //         }

    //         if (current_actor->RENDER == TRUE && current_actor->KILL == NULL) {  // AI RULES FOR ALL NPCS ON THIS PARTICULAR STAGE
    //             if (current_actor->NPC_type == PATROL) {                         // PATROL NPCS
    //                 current_actor->patrol_timer--;
    //                 current_actor->x += current_actor->SpdX;
    //                 if ((current_actor->direction == DIR_UP_L) && (current_actor->patrol_timer == 0)) {
    //                     SetActorDirection(current_actor, DIR_UP_R, 0);
    //                     current_actor->SpdX = abs(current_actor->SpdX);
    //                     current_actor->patrol_timer = current_actor->patrol_reset;
    //                 } else if ((current_actor->direction == DIR_UP_R) && (current_actor->patrol_timer == 0)) {
    //                     SetActorDirection(current_actor, DIR_UP_L, 0);
    //                     current_actor->SpdX = -abs(current_actor->SpdX);
    //                     current_actor->patrol_timer = current_actor->patrol_reset;
    //                 }

    //             } else if (current_actor->NPC_type == ELEVATOR) {  // ELEVATORS
    //                 current_actor->patrol_timer--;
    //                 current_actor->y += current_actor->SpdY;

    //                 if ((current_actor->direction == DIR_UP_L) && (current_actor->patrol_timer == 0)) {
    //                     SetActorDirection(current_actor, DIR_UP_R, 0);
    //                     current_actor->SpdY = abs(current_actor->SpdY);
    //                     current_actor->patrol_timer = current_actor->patrol_reset;
    //                 } else if ((current_actor->direction == DIR_UP_R) && (current_actor->patrol_timer == 0)) {
    //                     SetActorDirection(current_actor, DIR_UP_L, 0);
    //                     current_actor->SpdY = -abs(current_actor->SpdY);
    //                     current_actor->patrol_timer = current_actor->patrol_reset;
    //                 }
    //             } else if (current_actor->NPC_type == WALK) {  // WALK NPCS WALK STRAIGHT AHEAD
    //                 INT16 actor_x = TO_PIXELS(current_actor->x);

    //                 if ((actor_x >= -32) && (actor_x <= 200)) {
    //                     current_actor->x += current_actor->SpdX;
    //                 }
    //                 if (actor_x > 196) {
    //                     current_actor->KILL = TRUE;  // KILL NPC IS HE GOES OFF SCREEN TO THE RIGHT TOO FAR
    //                 }
    //             }
    //         }
    //         ptr++;
    //         current_actor = &active_actors[*ptr];
    //         // current_actor++;
    //     }
    //     for (UINT8 i = MAX_BULLETS; i != 0; i--) {
    //         if (current_bullet->RENDER == TRUE) {
    //             INT16 bullet_x = TO_PIXELS(current_bullet->x);
    //             if ((bullet_x < 0) || (bullet_x > 168)) {
    //                 current_bullet->KILL = TRUE;
    //                 current_bullet->RENDER = FALSE;
    //             }
    //             if ((camx > 0) && (camx < bkg.camera_max_x)) {  // IF CAM IS NOT IN SPAWN OR END POSITION (ie it's moving)
    //                 current_bullet->x -= PLAYER.SpdX;
    //             }
    //             if (current_bullet->facing == LEFT) {
    //                 current_bullet->x -= current_bullet->SpdX;
    //             } else if (current_bullet->facing == RIGHT) {
    //                 current_bullet->x += current_bullet->SpdX;
    //             }
    //         }
    //         current_bullet++;
    //     }
    //     if (bullet_timer != 0) {
    //         bullet_timer -= 1;
    //     }
}

// void spawn_bullets_world1() {
//     actor_t *spawn_bullet = active_bullets;
//     for (UINT8 i = MAX_BULLETS; i != 0; i--) {
//         if (spawn_bullet->RENDER == TRUE) {
//             spawn_bullet++;
//         } else if (bullet_timer == 0) {
//             spawn_bullet->RENDER = TRUE;
//             spawn_bullet->ON = TRUE;
//             if (PLAYER.facing == LEFT) {  // BULLET IS VISIBLE BEFORE ITS X AXIS IS LESS THAN DETECTIVE
//                 if ((LADDER) && (PLAYER.direction == DIR_LADDER_R)) {
//                     PLAYER.direction = DIR_LADDER_L;
//                 }
//                 spawn_bullet->facing = LEFT;
//                 spawn_bullet->x = PLAYER.x - TO_COORDS(6);
//             } else {
//                 if ((LADDER) && (PLAYER.direction == DIR_LADDER_L)) {
//                     PLAYER.direction = DIR_LADDER_R;
//                 }
//                 spawn_bullet->facing = RIGHT;
//                 spawn_bullet->x = PLAYER.x + TO_COORDS(6);
//             }
//             if (CROUCH) {
//                 spawn_bullet->y = PLAYER.y + TO_COORDS(4);
//             } else if (LADDER) {
//                 spawn_bullet->y = PLAYER.y - TO_COORDS(4);
//             } else if (JUMP) {
//                 if (PLAYER.SpdY < 0) {
//                     spawn_bullet->y = PLAYER.y - TO_COORDS(8);
//                 } else {
//                     spawn_bullet->y = PLAYER.y - TO_COORDS(4);
//                 }
//             } else {
//                 spawn_bullet->y = PLAYER.y;
//             }
//             bullet_timer = 90;
//             break;
//         }
//     }
//     // if (spawn_bullet->RENDER == FALSE) {
//     // }
// }
void npc_collisions_world1() {
    // CHECK LANDING HOTBOX TIMING
    // WE SHOULD ONLY NEED TO CHECK FOR CROUCH OR JUMP, BECAUSE BOTH WALK AND LAND HAVE THE SAME HITBOXES. SET THE VALUES FOR EACH BOX HERE
    if (CROUCH) {  // CROUCH HITBOX
        PLAYER.h_offset = -2;
        PLAYER.x_offset = 8;
        PLAYER.y_offset = 16;
    } else if (PLAYER.SpdY != 0) {  // JUMP AND FALLING HITBOX
        PLAYER.h_offset = 11;
        PLAYER.x_offset = 6;
        PLAYER.y_offset = 6;
    } else if ((!CROUCH) && (!JUMP)) {  // STAND AND WALKING HITBOX
        PLAYER.h_offset = 7;
        PLAYER.x_offset = 6;
        PLAYER.y_offset = 16;
    }
    for (UINT8 i = ACTOR_FIRST_NPC; i != (total_actors_count); i++) {
        //[y][x]
        UINT16 PTR_y, PTR_x, PBL_y, PBL_x, NTR_y, NTR_x, NBL_y, NBL_x;
        UINT8 ax, ay;
        ax = TO_PIXELS(active_actors[i].x);
        ay = TO_PIXELS(active_actors[i].y);
        INT16 NPC_PLAYER_Offset = px - (ax - active_actors[i].x_pivot);
        // THE PIVOT IS THE LITERAL CENTER OF THE METASPRITE. NOT A PIXEL, BUT THE CROSSHAIRS IN THE MIDDLE OF THE DESGIN
        PTR_y = py - PLAYER.h_offset;            // TR y
        PTR_x = px + PLAYER.x_offset;            // TR x
        PBL_y = py + PLAYER.y_offset;            // BL y
        PBL_x = px - PLAYER.x_offset;            // BL x
        NTR_y = ay - active_actors[i].y_offset;  // TR y
        NTR_x = ax + active_actors[i].x_offset;  // TR x
        NBL_y = ay + active_actors[i].y_offset;  // BL y
        NBL_x = ax - active_actors[i].x_offset;  // BL x
        if (active_actors[i].ON && active_actors[i].KILL == NULL) {
            if (overlap(PTR_y, PTR_x, PBL_y, PBL_x, NTR_y, NTR_x, NBL_y, NBL_x) == 0x01U) {
                if (active_actors[i].NPC_type != ELEVATOR) {
                    GAMEOVER = TRUE;
                } else if (active_actors[i].NPC_type == ELEVATOR) {
                    if (!ATTACH) {
                        if ((PBL_x > NTR_x - 2) || (PTR_x < NBL_x + 2))  // is not on top of elevator
                        {
                            x_Collide = TRUE;
                        } else if ((PBL_y > NTR_y) && (PBL_y < NBL_y)) {
                            ATTACH = TRUE;
                            Gravity = FALSE;
                            current_elevator = i;
                        } else if ((PTR_y < NBL_y) && (PTR_y > NTR_y)) {
                            y_Collide = TRUE;
                        }
                    }
                }
            } else if (overlap(PTR_y, PTR_x, PBL_y, PBL_x, NTR_y, NTR_x, NBL_y, NBL_x) == 0x00U) {
                if (x_Collide) {
                    x_Collide = FALSE;
                }
            }

        } else {
        }
        if (ATTACH) {
            if (i == current_elevator) {
                if ((PBL_x > NTR_x) || (PTR_x < NBL_x)) {
                    ATTACH = FALSE;
                    Gravity = TRUE;
                } else {
                    PLAYER.SpdY = 0;
                    PLAYER.y = TO_COORDS(NTR_y - (PLAYER.h / 2));
                    if (JUMP) {
                        switch_land();
                    } else if (!(joy & J_LEFT) && !(joy & J_RIGHT) && (!CROUCH)) {
                        switch_idle();
                    }
                    SPAWN = Gravity = JUMP = FALSE;
                }
            }
            if ((CHANGED_BUTTONS & J_A) && (joy & J_A)) {
                jump();
            }
        }
    }
}
void init_submap_example() {
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
    set_bkg_data_nonbanked(0, STAGE_DROP_TILESLen, STAGE_DROP_TILES, STAGE_DROP_TILESBank);
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

void init_submap_world1() {
    // fadeout_white();
    HIDE_BKG;
    bkg.redraw = TRUE;
    bkg.sliding = FALSE;
    bkg.camera_max_y = (WORLD1_MAPHeight - 18) * 8;
    bkg.camera_max_x = (WORLD1_MAPWidth - 20) * 8;
    bkg.camera_tiles_x = WORLD1_MAPWidth * 8;
    bkg.camera_tiles_y = WORLD1_MAPHeight * 8;
    bkg.camera_x = 0;
    bkg.camera_y = 80;
    bkg.old_camera_x = bkg.camera_x;
    bkg.old_camera_y = bkg.camera_y;
    bkg.map_pos_x = (UINT8)(bkg.camera_x >> 3u);
    bkg.map_pos_y = (UINT8)(bkg.camera_y >> 3u);
    // CHANGE THE TILE COUNT AS YOU ADD TILES TO THE BKG TILE_SET
    set_bkg_data_nonbanked(0, WORLD1_TILESLen, WORLD1_TILES, WORLD1_TILESBank);
    bkg.old_map_pos_x = bkg.old_map_pos_y = 255;

    set_bkg_submap_nonbanked(bkg.map_pos_x, bkg.map_pos_y, 20, 18, WORLD1_MAP, WORLD1_MAPWidth, WORLD1_MAPBank);
    set_level(WORLD1_MAPWidth, WORLD1_MAPHeight, WORLD1_MAP, WORLD1_MAPBank);

    bkg.old_camera_x = bkg.camera_x;
    bkg.old_camera_y = bkg.camera_y;

    bkg.camera_style = scroll_cam;

    shadow_scx = bkg.camera_x;
    shadow_scy = bkg.camera_y;

    SHOW_BKG;
    // fadein_white();
}
// void init_submap() {
//     fadeout_white();
//     HIDE_BKG;
//     bkg.sliding = FALSE;
//     bkg.camera_x = 320;
//     bkg.camera_y = 144;
//     bkg.old_camera_x = bkg.camera_x;
//     bkg.old_camera_y = bkg.camera_y;
//     bkg.map_pos_x = (UBYTE)(bkg.camera_x >> 3u);
//     bkg.map_pos_y = (UBYTE)(bkg.camera_y >> 3u);

//     set_bkg_data_nonbanked(0, SUBMAP_TILE_COUNT, SUBMAP_TILE_DATA, BANK(SUBMAP_TILE_DATA));
//     bkg.old_map_pos_x = bkg.old_map_pos_y = 255;
//     bkg.camera_max_y = (SUBMAP_MAP_HEIGHT - 18) * 8;
//     bkg.camera_max_x = (SUBMAP_MAP_WIDTH - 20) * 8;
//     set_bkg_submap_nonbanked(bkg.map_pos_x, bkg.map_pos_y, 20, 18, SUBMAP_MAP_DATA, SUBMAP_MAP_WIDTH, BANK(SUBMAP_MAP_DATA));
//     set_level(SUBMAP_MAP_WIDTH, SUBMAP_MAP_HEIGHT, SUBMAP_MAP_DATA, BANK(SUBMAP_MAP_DATA));

//     bkg.old_camera_x = bkg.camera_x;
//     bkg.old_camera_y = bkg.camera_y;

//     bkg.redraw = FALSE;

//     bkg.camera_style = scroll_cam;

//     SCX_REG = bkg.camera_x;
//     SCY_REG = bkg.camera_y;
//     SHOW_BKG;

//     fadein_white();
// }

void setup_world1() {
    DISPLAY_OFF;
    SPAWN = TRUE;
    GAMEOVER = L_LEFT = L_RIGHT = LADDER = CROUCH = canCROUCH = DROP = FALSE;
    JUMP = LADDER_Release = TRUE;
    load_level(&world1);
    // render_actors_platform();
    DISPLAY_ON;
    current_stage = &world1;
}

void enter_world1() {
    setup_world1();
    while (gamestate == 3) {
        last_joy = joy;
        joy = joypad();
            if (joy & J_LEFT)
            {
                if (bkg.camera_x)
                {
                    bkg.camera_x -= TO_COORDS(1);
                    bkg.redraw = TRUE;
                }
            }
            else if (joy & J_RIGHT)
            {
                if (bkg.camera_x < bkg.camera_max_x)
                {
                    bkg.camera_x += TO_COORDS(1);
                    bkg.redraw = TRUE;
                }
            }
        if (!SPAWN) {
            UINT8 px, py;
            px = TO_PIXELS(PLAYER.x);
            py = TO_PIXELS(PLAYER.y);
            if (joy & J_LEFT) {
                if (PLAYER.SpdX == 0) {
                    if (joy & J_DOWN) {
                        canCROUCH_timer = 1;
                    }
                }
                if ((!JUMP) && !(joy & J_DOWN) && (!CROUCH) && (!ONTO_Ladder)) {
                    if (canCROUCH) {
                        SetActorDirection(&PLAYER, DIR_CRAWL_L, PLAYER.animation_phase);
                    } else {
                        SetActorDirection(&PLAYER, DIR_LEFT, PLAYER.animation_phase);
                    }

                } else if (CROUCH) {
                    if (!JUMP) {
                        SetActorDirection(&PLAYER, DIR_CRAWL_L, 0);
                    }
                }
                if (CROUCH) {
                    if (PLAYER.SpdX > -MAX_CRAWL_SPEED) {
                        PLAYER.SpdX -= WALK_VELOCITY;
                    } else
                        PLAYER.SpdX = -MAX_CRAWL_SPEED;
                } else if ((!CROUCH) && (!LADDER)) {
                    if (PLAYER.SpdX > -MAX_WALK_SPEED) {
                        PLAYER.SpdX -= WALK_VELOCITY;
                    } else
                        PLAYER.SpdX = -MAX_WALK_SPEED;
                }

            } else if (joy & J_RIGHT) {
                if (PLAYER.SpdX == 0) {
                    if (joy & J_DOWN) {
                        canCROUCH_timer = 1;
                    }
                }
                if ((!JUMP) && !(joy & J_DOWN) && (!CROUCH) && (!ONTO_Ladder)) {
                    if (canCROUCH) {
                        SetActorDirection(&PLAYER, DIR_CRAWL_R, PLAYER.animation_phase);
                    } else {
                        SetActorDirection(&PLAYER, DIR_RIGHT, PLAYER.animation_phase);
                    }
                } else if (CROUCH) {
                    if (!JUMP) {
                        SetActorDirection(&PLAYER, DIR_CRAWL_R, 0);
                    }
                }
                if (CROUCH) {
                    if (PLAYER.SpdX < MAX_CRAWL_SPEED) {
                        PLAYER.SpdX += WALK_VELOCITY;
                    } else
                        PLAYER.SpdX = MAX_CRAWL_SPEED;
                } else if ((!CROUCH) && (!LADDER)) {
                    if (PLAYER.SpdX < MAX_WALK_SPEED) {
                        PLAYER.SpdX += WALK_VELOCITY;
                    } else
                        PLAYER.SpdX = MAX_WALK_SPEED;
                }
            }

            if ((joy & J_DOWN) && (!JUMP) && (!LADDER) && (!ONTO_Ladder)) {
                CROUCH = TRUE;
            }

            if ((CHANGED_BUTTONS & J_A) && (joy & J_A) && (!ONTO_Ladder) && (!OFF_LADDER)) {
                jump();
            }
            if ((CHANGED_BUTTONS & J_B) && (joy & J_B)) {
                spawn_bullets();
            }
        }

        // IF PLAYER IS FREE FALLING FOR ANY REASON
        if (PLAYER.SpdY != 0) {
            JUMP = Gravity = TRUE;
            CROUCH = FALSE;
            if (!LADDER) {
                switch_jump();
            }
        }
        if (DROP) {
            DROP_timer -= 1;
            if (DROP_timer == 0) {
                DROP = FALSE;
                DROP_timer = 16;
            }
        }
        if ((CROUCH) && (canCROUCH)) {
            if ((PLAYER.direction == DIR_CRAWL_L) || (PLAYER.direction == DIR_DOWN_L)) {
                PLAYER.SpdX = -MAX_CRAWL_SPEED;
            } else if ((PLAYER.direction == DIR_CRAWL_R) || (PLAYER.direction == DIR_DOWN_R)) {
                PLAYER.SpdX = MAX_CRAWL_SPEED;
            }
            canCROUCH_Ftimer -= 1;
            if (canCROUCH_Ftimer == 1) {
                canCROUCH = FALSE;
                canCROUCH_Ftimer = 8;
            }
        }
        if (!(joy & J_LEFT) && !(joy & J_RIGHT)) {
            canCROUCH_timer = 10;
            // Release_timer = 20;
        }

        // ---------------------------------------------
        // WORLD PHYSICS:
        // GRAVITY

        if (Gravity) {
            if (LADDER) {
                if (PLAYER.SpdY < 0) {
                    PLAYER.SpdY += GRAVITY;
                } else if (PLAYER.SpdY > 0) {
                    PLAYER.SpdY -= GRAVITY;
                }
            } else {
                PLAYER.SpdY += GRAVITY;
                if (PLAYER.SpdY > MAX_FALL_SPEED) {
                    PLAYER.SpdY = MAX_FALL_SPEED;
                }
            }
        }
        px = TO_PIXELS(PLAYER.x);
        py = TO_PIXELS(PLAYER.y);

        // Y-AXIS COLLISION CHECK / /LADDER CHECK
        check_UD(px, py, TO_PIXELS(bkg.camera_x));

        if ((CROUCH) && (!canCROUCH)) {
            if (!(joy & J_DOWN)) {
                check_C(px, py, TO_PIXELS(bkg.camera_x));
            }
        }

        if (PLAYER.SpdX < 0) {
            if (PLAYER.SpdX != -MAX_WALK_SPEED) {
                PLAYER.SpdX += FRICTION;
            } else if ((PLAYER.SpdX <= -MAX_WALK_SPEED) && !(joy & J_LEFT)) {
                PLAYER.SpdX += FRICTION;
            }
        }
        if (PLAYER.SpdX > 0) {
            if (PLAYER.SpdX != MAX_WALK_SPEED) {
                PLAYER.SpdX -= FRICTION;
            } else if ((PLAYER.SpdX >= MAX_WALK_SPEED) && !(joy & J_RIGHT)) {
                PLAYER.SpdX -= FRICTION;
            }
        }

        if (PLAYER.SpdX != 0) {
            UINT8 px, py;
            px = TO_PIXELS(PLAYER.x);
            py = TO_PIXELS(PLAYER.y);

            if (PLAYER.SpdX > 0) {
                check_LR(px + 1, py, TO_PIXELS(bkg.camera_x));  // IF MOVING RIGHT

            } else if (PLAYER.SpdX < 0) {
                check_LR(px - 1, py, TO_PIXELS(bkg.camera_x));  // IF MOVING LEFT
            }
        }

        // update PLAYER absolute posiiton
        if (!ATTACH) {
            PLAYER.y += PLAYER.SpdY;
        }
        px = TO_PIXELS(PLAYER.x);
        py = TO_PIXELS(PLAYER.y);
        // // Change to IDLE state when not moving
        if ((!JUMP) && (!CROUCH) && (PLAYER.direction != DIR_LAND_L) && (PLAYER.direction != DIR_LAND_R) && (!LADDER)) {
            if ((PLAYER.SpdX == 0) && (PLAYER.SpdY == 0)) {
                if (!(joy & J_LEFT) && !(joy & J_RIGHT)) {
                    switch_idle();
                    check_C(px, py, TO_PIXELS(bkg.camera_x));
                }
            }
        }
        // DOWN while standing still
        if ((CROUCH) && (!canCROUCH) && (!(joy & J_LEFT) && !(joy & J_RIGHT)) && (PLAYER.SpdY == 0)) {
            switch_down();
        }

        // UPDATE THE CAMERA POSITION SETTINGS
        render_camera(px, TO_PIXELS(bkg.camera_x));
        // UPDATE CURRENT LEVEL NPC ANIMATIONS AND X/Y POSITIONS
        if (animate_level) animate_level();  // call level animation hook (if any)
        // CHECK FOR NPC COLLISIONS
        if (collide_level) collide_level();
        // RENDER ALL CURRENT ACTORS ON SCREEN
        render_actors_platform();

        if (bkg.redraw) {
            set_world_camera();
            wait_vbl_done();
            refresh_OAM();
            SCX_REG = shadow_scx;
            SCY_REG = shadow_scy;
            bkg.redraw = FALSE;

        } else {
            wait_vbl_done();
            refresh_OAM();
        }

    //     if (GAMEOVER) {
    //         // gamestate = 1;
    //         enter_world1();
    //         // gameover();
    //         // TRY LOADING A SECOND STAGE HERE?
    //     } else if (WINNER) {
    //         WINNER = FALSE;
    //         gamestate = 1;
    //     }
    }
}