#include "extern_includes.hpp"
#include "defines.hpp"
#include "structs.hpp"

camera global_camera;
bool global_camera_moved;

void camera_init() {
    global_camera.pos = glm::vec3(1.5f, 1.5f, 1.5f);
    global_camera.front = glm::vec3(0.0f, 1.0f, 0.0f);
    global_camera.up = glm::vec3(0.0f, 0.0f, 1.0f);
    global_camera.right = glm::vec3(1.0f, 0.0f, 0.0f);
    global_camera.coord_up = glm::vec3(0.0f, 0.0f, 1.0f);
    global_camera.width_height = glm::uvec2(WINDOW_WIDTH, WINDOW_HEIGHT);
    global_camera.aspect_ratio = float(WINDOW_WIDTH) / float(WINDOW_HEIGHT);
    global_camera.fov = 45.0f;
    global_camera.seed = 0;
    global_camera_moved = true;
}

camera *camera_get() {
    return &global_camera;
}

void camera_absulute_move(glm::vec3 dir, float dist) {
    global_camera.pos += dir * dist;
    global_camera_moved = true;
}

void camera_move_front(float dist) {
    global_camera.pos += global_camera.front * dist;
    global_camera_moved = true;
}

void camera_move_back(float dist) {
    global_camera.pos -= global_camera.front * dist;
    global_camera_moved = true;
}

void camera_move_left(float dist) {
    global_camera.pos -= global_camera.right * dist;
    global_camera_moved = true;
}

void camera_move_right(float dist) {
    global_camera.pos += global_camera.right * dist;
    global_camera_moved = true;
}

void camera_move_up(float dist) {
    global_camera.pos += global_camera.up * dist;
    global_camera_moved = true;
}

void camera_move_down(float dist) {
    global_camera.pos -= global_camera.up * dist;
    global_camera_moved = true;
}

void camera_set_front(glm::vec3 front) {
    global_camera.front = front;
    global_camera.right = glm::normalize(glm::cross(front, global_camera.up));
    global_camera.coord_up = glm::normalize(glm::cross(global_camera.right, global_camera.front));
    global_camera_moved = true;
}

void camera_set_seed(unsigned int seed) {
    global_camera.seed = seed;
}

void camera_set_fixed_frame_count(unsigned int fixed_frame_count) {
    global_camera.fixed_frame_count = fixed_frame_count;
}

bool camera_get_moved() {
    return global_camera_moved;
}

void camera_reset_moved() {
    global_camera_moved = false;
}