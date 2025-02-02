#include "extern_includes.hpp"
#include "defines.hpp"
#include "camera.hpp"
#include "shader.hpp"

void movement_process(bool front, bool back, bool left, bool right, bool up, bool down, float delta_time) {
    glm::vec2 plane_dir = glm::vec2(0);
    plane_dir += front ? glm::vec2(1, 0) : glm::vec2(0);
    plane_dir += back ? glm::vec2(-1, 0) : glm::vec2(0);
    plane_dir += left ? glm::vec2(0, -1) : glm::vec2(0);
    plane_dir += right ? glm::vec2(0, 1) : glm::vec2(0);

    if (plane_dir.x == 1 && plane_dir.y == 0) { //front
        camera_move_front(MOVEMRNT_SPEED * delta_time);
    }
    if (plane_dir.x == 0 && plane_dir.y == 1) { //right
        camera_move_right(MOVEMRNT_SPEED * delta_time);
    }
    if (plane_dir.x == -1 && plane_dir.y == 0) { //back
        camera_move_back(MOVEMRNT_SPEED * delta_time);
    }
    if (plane_dir.x == 0 && plane_dir.y == -1) { //left
        camera_move_left(MOVEMRNT_SPEED * delta_time);
    }
    if (plane_dir.x == 1 && plane_dir.y == 1) { //front right
        camera_move_front(MOVEMRNT_SPEED * delta_time / sqrt(2));
        camera_move_right(MOVEMRNT_SPEED * delta_time / sqrt(2));
    }
    if (plane_dir.x == -1 && plane_dir.y == 1) { //back right
        camera_move_back(MOVEMRNT_SPEED * delta_time / sqrt(2));
        camera_move_right(MOVEMRNT_SPEED * delta_time / sqrt(2));
    }
    if (plane_dir.x == -1 && plane_dir.y == -1) { //back left
        camera_move_back(MOVEMRNT_SPEED * delta_time / sqrt(2));
        camera_move_left(MOVEMRNT_SPEED * delta_time / sqrt(2));
    }
    if (plane_dir.x == 1 && plane_dir.y == -1) { //front left
        camera_move_front(MOVEMRNT_SPEED * delta_time / sqrt(2));
        camera_move_left(MOVEMRNT_SPEED * delta_time / sqrt(2));
    }

    float up_down = 0;
    up_down += up ? 1 : 0;
    up_down += down ? -1 : 0;

    if (up_down == 1) {
        camera_move_up(MOVEMRNT_SPEED * delta_time);
    }
    if (up_down ==-1) {
        camera_move_down(MOVEMRNT_SPEED * delta_time);
    }
}