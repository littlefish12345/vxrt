#include "extern_includes.hpp"
#include "defines.hpp"
#include "structs.hpp"
#include "block.hpp"

section global_section_group[4096];
section_group_map global_section_group_map;

void build_test_box();

void section_group_init() {
    for (int u = 0; u < 16; ++u) {
        for (int v = 0; v < 16; ++v) {
            for (int w = 0; w < 16; ++w) {
                global_section_group_map.section_map[u][v][w] = u + v*16 + w*256;
                global_section_group[u + v*16 + w*256].start_pos_x = u<<4;
                global_section_group[u + v*16 + w*256].start_pos_y = v<<4;
                global_section_group[u + v*16 + w*256].start_pos_z = w<<4;
                global_section_group[u + v*16 + w*256].light_source_num = 0;
                for (int x = 0; x < 16; ++x) {
                    for (int y = 0; y < 16; ++y) {
                        for (int z = 0; z < 16; ++z) {
                            global_section_group[u + v*16 + w*256].blocks[x][y][z].block_id = 0;
                            global_section_group[u + v*16 + w*256].blocks[x][y][z].status = 0;
                            global_section_group[u + v*16 + w*256].light_pos[x + y*16 + z*256] = glm::uvec3(0, 0, 0);
                        }
                    }
                }
            }
        }
    }
    build_test_box();
}

void section_update_light_source(unsigned int index) {
    global_section_group[index].light_source_num = 0;
    for (int x = 0; x < 16; ++x) {
        for (int y = 0; y < 16; ++y) {
            for (int z = 0; z < 16; ++z) {
                if (block_info_get(global_section_group[index].blocks[x][y][z].block_id).brightness != 0) {
                    global_section_group[index].light_pos[global_section_group[index].light_source_num] = glm::uvec3(x, y, z);
                    ++global_section_group[index].light_source_num;
                }
            }
        }
    }
}

section *section_group_get() {
    return global_section_group;
}

section_group_map *section_group_map_get() {
    return &global_section_group_map;
}

void build_test_box() {
    for (int x = 0; x < 7; ++x) {
        for (int y = 0; y < 7; ++y) {
            global_section_group[0].blocks[x][y][0].block_id = 1;
        }
    }
    for (int x = 0; x < 7; ++x) {
        for (int y = 0; y < 7; ++y) {
            global_section_group[0].blocks[x][y][6].block_id = 1;
        }
    }
    for (int x = 0; x < 7; ++x) {
        for (int z = 0; z < 7; ++z) {
            global_section_group[0].blocks[x][0][z].block_id = 3;
        }
    }
    for (int x = 0; x < 7; ++x) {
        for (int z = 0; z < 7; ++z) {
            global_section_group[0].blocks[x][6][z].block_id = 5;
        }
    }
    for (int y = 0; y < 7; ++y) {
        for (int z = 0; z < 7; ++z) {
            global_section_group[0].blocks[0][y][z].block_id = 1;
        }
    }
    for (int y = 0; y < 7; ++y) {
        for (int z = 0; z < 7; ++z) {
            global_section_group[0].blocks[6][y][z].block_id = 1;
        }
    }
    /*
    for (int x = 2; x < 5; ++x) {
        for (int y = 2; y < 5; ++y) {
            global_section_group[0].blocks[x][y][6].block_id = 6;
        }
    }
    */
   global_section_group[0].blocks[2][2][1].block_id = 6;
   global_section_group[0].blocks[3][3][1].block_id = 2;
    section_update_light_source(0);
}