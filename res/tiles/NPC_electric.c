  #pragma bank 5
// AUTOGENERATED FILE FROM png2mtspr

#include <gb/gb.h>
#include <gb/metasprites.h>

const UINT8 NPC_electric_data[128] = {
    0x03, 0x00, 0x0c, 0x03, 0x13, 0x0f, 0x2e, 0x17, 0x7e, 0x01, 0x54, 0x2b, 0xb2, 0x7d, 0xa0, 0x7f, 0xa0, 0x7f, 0xbc, 0x73, 0x58, 0x27, 0x5c, 0x33, 0x2e, 0x17, 0x13, 0x0f, 0x0c, 0x03, 0x03, 0x00,
    0xc0, 0x00, 0x30, 0xc0, 0xc8, 0xf0, 0x74, 0xc8, 0x7a, 0x9c, 0x3a, 0xcc, 0x6d, 0x9e, 0x05, 0xfe, 0x05, 0xfe, 0x0d, 0xfe, 0x5a, 0xac, 0x7a, 0x84, 0x74, 0xd8, 0xc8, 0xf0, 0x30, 0xc0, 0xc0, 0x00,
    0x00, 0x00, 0x03, 0x00, 0x0c, 0x03, 0x10, 0x0f, 0x26, 0x19, 0x24, 0x1b, 0x40, 0x3f, 0x40, 0x3f, 0x40, 0x3f, 0x40, 0x3f, 0x20, 0x1f, 0x20, 0x1f, 0x10, 0x0f, 0x0c, 0x03, 0x03, 0x00, 0x00, 0x00,
    0x00, 0x00, 0xc0, 0x00, 0x30, 0xc0, 0x08, 0xf0, 0x04, 0xf8, 0x04, 0xf8, 0x02, 0xfc, 0x02, 0xfc, 0x02, 0xfc, 0x02, 0xfc, 0x04, 0xf8, 0x04, 0xf8, 0x08, 0xf0, 0x30, 0xc0, 0xc0, 0x00, 0x00, 0x00};

const metasprite_t NPC_electric_metasprite0[] = {
    {-8, -8, 0, 0}, {0, 8, 2, 0}, {metasprite_end}};

const metasprite_t NPC_electric_metasprite1[] = {
    {-8, -8, 4, 0}, {0, 8, 6, 0}, {metasprite_end}};

const metasprite_t* const NPC_electric_metasprites[2] = {
    NPC_electric_metasprite0, NPC_electric_metasprite1};
const metasprite_t* const NPC_electric_animation[] = {
    NPC_electric_metasprite0, NPC_electric_metasprite1, NULL};