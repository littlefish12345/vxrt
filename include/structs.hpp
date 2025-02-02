#pragma once

#include "defines.hpp"

struct block {
    unsigned int block_id;
    unsigned int status;
};

struct section {
    unsigned int start_pos_x;
    unsigned int start_pos_y;
    unsigned int start_pos_z;
    block blocks[16][16][16];
};

struct octree_node {
    //unsigned int width: 3; //2^width
    //unsigned int empty: 1;
    //unsigned int index: 28;
    unsigned int metadata;
};

struct octree {
    unsigned int start_pos_x; //min coord
    unsigned int start_pos_y;
    unsigned int start_pos_z;
    octree_node nodes[OCTREE_SIZE];
};

struct section_group_map {
    unsigned int section_map[16][16][16];
};

struct section_group_update_map {
    unsigned int update_data[128];
};

struct camera {
    glm::vec3 pos;
    float _pad1;
    glm::vec3 front;
    float _pad2;
    glm::vec3 up;
    float _pad3;
    glm::vec3 right;
    float _pad4;
    glm::vec3 coord_up;
    float _pad5;
    glm::uvec2 width_height;
    float aspect_ratio;
    float fov;
};
