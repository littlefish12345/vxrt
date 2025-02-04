#pragma once

#include "extern_includes.hpp"
#include "structs.hpp"

void camera_init();
camera *camera_get();
void camera_absulute_move(glm::vec3 dir, float dist);
void camera_move_front(float dist);
void camera_move_back(float dist);
void camera_move_left(float dist);
void camera_move_right(float dist);
void camera_move_up(float dist);
void camera_move_down(float dist);
void camera_set_front(glm::vec3 front);
void camera_set_seed(unsigned int seed);
void camera_set_fixed_frame_count(unsigned int fixed_frame_count);
bool camera_get_moved();
void camera_reset_moved();