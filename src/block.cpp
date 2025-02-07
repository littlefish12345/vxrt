#include "extern_includes.hpp"
#include "defines.hpp"
#include "structs.hpp"

block_info global_block_info_list[BLOCK_NUM];

void block_info_list_init() {
    global_block_info_list[1].roughness = 1; //white rough block
    global_block_info_list[1].brightness = 0;
    global_block_info_list[1].diffuse_color = glm::vec3(1, 1, 1);
    global_block_info_list[1].reflect_color = glm::vec3(0.21, 0.21, 0.21);
    global_block_info_list[2].roughness = 1; //red rough block
    global_block_info_list[2].brightness = 0;
    global_block_info_list[2].diffuse_color = glm::vec3(1, 0, 0);
    global_block_info_list[2].reflect_color = glm::vec3(0.21, 0, 0);
    global_block_info_list[3].roughness = 1; //green rough block
    global_block_info_list[3].brightness = 0;
    global_block_info_list[3].diffuse_color = glm::vec3(0, 1, 0);
    global_block_info_list[3].reflect_color = glm::vec3(0, 0.21, 0);
    global_block_info_list[4].roughness = 0.3; //shiny copper block
    global_block_info_list[4].brightness = 0;
    global_block_info_list[4].diffuse_color = glm::vec3(0.98, 0.82, 0.76);
    global_block_info_list[4].reflect_color = glm::vec3(0.98, 0.82, 0.76);
    global_block_info_list[5].roughness = 0.1; //silver mirror block
    global_block_info_list[5].brightness = 0;
    global_block_info_list[5].diffuse_color = glm::vec3(0.988, 0.980, 0.960);
    global_block_info_list[5].reflect_color = glm::vec3(0.988, 0.980, 0.960);
    global_block_info_list[6].roughness = 2; //white light block
    global_block_info_list[6].brightness = 1;
    global_block_info_list[6].diffuse_color = glm::vec3(1, 1, 1);
    global_block_info_list[6].reflect_color = glm::vec3(0.21, 0.21, 0.21);
    global_block_info_list[7].roughness = 1; //red light block
    global_block_info_list[7].brightness = 1;
    global_block_info_list[7].diffuse_color = glm::vec3(1, 0, 0);
    global_block_info_list[7].reflect_color = glm::vec3(0.21, 0, 0);
    global_block_info_list[8].roughness = 1; //green light block
    global_block_info_list[8].brightness = 1;
    global_block_info_list[8].diffuse_color = glm::vec3(0, 1, 0);
    global_block_info_list[8].reflect_color = glm::vec3(0, 0.21, 0);
}

block_info *block_info_list_get() {
    return global_block_info_list;
}

block_info block_info_get(unsigned int block_id) {
    return global_block_info_list[block_id];
}