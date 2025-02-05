#include "extern_includes.hpp"
#include "defines.hpp"
#include "structs.hpp"
#include "shader_source.hpp"
#include "camera.hpp"

#include <vector>

GLuint section_octree_pass1_compute_shader;
GLuint section_octree_pass1_compute_program;
GLuint section_octree_pass2_compute_shader;
GLuint section_octree_pass2_compute_program;
GLuint center_octree_pass1_compute_shader;
GLuint center_octree_pass1_compute_program;
GLuint center_octree_pass2_compute_shader;
GLuint center_octree_pass2_compute_program;
GLuint rt_compute_shader;
GLuint rt_compute_program;

GLuint display_vertex_shader;
GLuint display_fragment_shader;
GLuint display_program;

GLuint camera_ssbo, section_group_ssbo, section_group_map_ssbo, section_group_update_map_ssbo, center_octree_ssbo, section_octree_ssbo, block_info_list_ssbo, output_image;
GLuint vao, vbo, ebo;

section_group_update_map update_map;

section test_section_group[4096];
section_group_map test_section_group_map;

block_info block_info_list[BLOCK_NUM];

float display_vertices[] = {
    -1.0f, 1.0f, 0.0f, 1.0f,
    1.0f, 1.0f, 1.0f, 1.0f,
    -1.0f, -1.0f, 0.0f, 0.0f,
    1.0f, -1.0f, 1.0f, 0.0f
};

unsigned int display_indices[] = {
    0, 1, 2,
    1, 2, 3
};

void check_shader_compile(GLuint shader) {
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cout << "Shader compile failed:\n" << infoLog << std::endl;
        exit(-1);
    }
}

void check_program_link(GLuint program) {
    GLint success;
    int err;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        std::cout << "Program link failed:\n" << infoLog << std::endl;
        exit(-1);
    }
}

void section_group_udpate(section *group_data, section_group_map *group_map) {
    for (int i = 0; i < 128; ++i) {
        update_map.update_data[i] = 0xFFFFFFFF;
    }
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, section_group_ssbo);
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(section)*4096, group_data, GL_DYNAMIC_COPY);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, section_group_map_ssbo);
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(section_group_map), group_map, GL_DYNAMIC_COPY);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, section_group_update_map_ssbo);
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(section_group_update_map), &update_map, GL_DYNAMIC_COPY);
}

void octree_update() {
    glUseProgram(section_octree_pass1_compute_program);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, section_group_ssbo);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, section_group_map_ssbo);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, section_group_update_map_ssbo);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, center_octree_ssbo);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, section_octree_ssbo);
    glDispatchCompute(32, 32, 32);
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
    glUseProgram(section_octree_pass2_compute_program);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, section_group_ssbo);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, section_group_map_ssbo);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, section_group_update_map_ssbo);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, center_octree_ssbo);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, section_octree_ssbo);
    glDispatchCompute(16, 16, 16);
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
    glUseProgram(center_octree_pass1_compute_program);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, section_group_ssbo);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, section_group_map_ssbo);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, section_group_update_map_ssbo);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, center_octree_ssbo);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, section_octree_ssbo);
    glDispatchCompute(2, 2, 2);
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
    glUseProgram(center_octree_pass2_compute_program);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, section_group_ssbo);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, section_group_map_ssbo);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, section_group_update_map_ssbo);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, center_octree_ssbo);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, section_octree_ssbo);
    glDispatchCompute(1, 1, 1);
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
}

void camera_update() {
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, camera_ssbo);
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(camera), camera_get(), GL_DYNAMIC_COPY);
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
}

void shader_create() {
    block_info_list[1].attr.x = 1; //white rough block
    block_info_list[1].attr.y = 1;
    block_info_list[1].attr.z = 0;
    block_info_list[1].diffuse_color = glm::vec3(1, 1, 1);
    block_info_list[1].reflect_color = glm::vec3(0, 0, 0);
    block_info_list[2].attr.x = 1; //red rough block
    block_info_list[2].attr.y = 1;
    block_info_list[2].attr.z = 0;
    block_info_list[2].diffuse_color = glm::vec3(1, 0, 0);
    block_info_list[2].reflect_color = glm::vec3(0, 0, 0);
    block_info_list[3].attr.x = 1; //green rough block
    block_info_list[3].attr.y = 1;
    block_info_list[3].attr.z = 0;
    block_info_list[3].diffuse_color = glm::vec3(0, 1, 0);
    block_info_list[3].reflect_color = glm::vec3(0, 0, 0);
    block_info_list[4].attr.x = 0.3; //shiny copper block
    block_info_list[4].attr.y = 0;
    block_info_list[4].attr.z = 0;
    block_info_list[4].diffuse_color = glm::vec3(0.98, 0.82, 0.76);
    block_info_list[4].reflect_color = glm::vec3(0.98, 0.82, 0.76);
    block_info_list[5].attr.x = 0.1; //mirror block
    block_info_list[5].attr.y = 0;
    block_info_list[5].attr.z = 0;
    block_info_list[5].diffuse_color = glm::vec3(0, 0, 0);
    block_info_list[5].reflect_color = glm::vec3(0.31, 0.31, 0.31);
    block_info_list[6].attr.x = 1; //white light block
    block_info_list[6].attr.y = 1;
    block_info_list[6].attr.z = 1;
    block_info_list[6].diffuse_color = glm::vec3(3.14, 3.14, 3.14);
    block_info_list[6].reflect_color = glm::vec3(0, 0, 0);

    for (int u = 0; u < 16; ++u) {
        for (int v = 0; v < 16; ++v) {
            for (int w = 0; w < 16; ++w) {
                test_section_group_map.section_map[u][v][w] = u + v*16 + w*256;
                test_section_group[u + v*16 + w*256].start_pos_x = u<<4;
                test_section_group[u + v*16 + w*256].start_pos_y = v<<4;
                test_section_group[u + v*16 + w*256].start_pos_z = w<<4;
                for (int x = 0; x < 16; ++x) {
                    for (int y = 0; y < 16; ++y) {
                        for (int z = 0; z < 16; ++z) {
                            test_section_group[u + v*16 + w*256].blocks[x][y][z].block_id = 0;
                            test_section_group[u + v*16 + w*256].blocks[x][y][z].status = 0;
                        }
                    }
                }
            }
        }
    }
    for (int x = 0; x < 7; ++x) {
        for (int y = 0; y < 7; ++y) {
            test_section_group[test_section_group_map.section_map[0][0][0]].blocks[x][y][0].block_id = 1;
        }
    }
    for (int x = 0; x < 7; ++x) {
        for (int y = 0; y < 7; ++y) {
            test_section_group[test_section_group_map.section_map[0][0][0]].blocks[x][y][7].block_id = 6;
        }
    }
    for (int x = 0; x < 7; ++x) {
        for (int z = 0; z < 7; ++z) {
            test_section_group[test_section_group_map.section_map[0][0][0]].blocks[x][0][z].block_id = 3;
        }
    }
    for (int x = 0; x < 7; ++x) {
        for (int z = 0; z < 7; ++z) {
            test_section_group[test_section_group_map.section_map[0][0][0]].blocks[x][7][z].block_id = 4;
        }
    }
    for (int y = 0; y < 7; ++y) {
        for (int z = 0; z < 7; ++z) {
            test_section_group[test_section_group_map.section_map[0][0][0]].blocks[0][y][z].block_id = 1;
        }
    }
    for (int y = 0; y < 7; ++y) {
        for (int z = 0; z < 7; ++z) {
            test_section_group[test_section_group_map.section_map[0][0][0]].blocks[7][y][z].block_id = 1;
        }
    }

/*
    test_section_group[test_section_group_map.section_map[15][15][15]].blocks[15][15][15].block_id = 1;

    for (int x = 0; x < 2; ++x) {
        for (int y = 0; y < 2; ++y) {
            for (int z = 0; z < 2; ++z) {
                test_section_group[test_section_group_map.section_map[x][y][z]].blocks[0][0][0].block_id = 1;
                test_section_group[test_section_group_map.section_map[x][y][z]].blocks[2][0][0].block_id = 1;
                test_section_group[test_section_group_map.section_map[x][y][z]].blocks[0][2][0].block_id = 1;
                test_section_group[test_section_group_map.section_map[x][y][z]].blocks[2][2][0].block_id = 1;
                test_section_group[test_section_group_map.section_map[x][y][z]].blocks[1][1][1].block_id = 6;
                test_section_group[test_section_group_map.section_map[x][y][z]].blocks[0][0][2].block_id = 1;
                test_section_group[test_section_group_map.section_map[x][y][z]].blocks[2][0][2].block_id = 1;
                test_section_group[test_section_group_map.section_map[x][y][z]].blocks[0][2][2].block_id = 1;
                test_section_group[test_section_group_map.section_map[x][y][z]].blocks[2][2][2].block_id = 1;
            }
        }
    }
*/
/*
    test_section_group[test_section_group_map.section_map[0][0][0]].blocks[0][0][0].block_id = 1;
    test_section_group[test_section_group_map.section_map[0][0][0]].blocks[1][0][0].block_id = 1;
    test_section_group[test_section_group_map.section_map[0][0][0]].blocks[0][1][0].block_id = 1;
    test_section_group[test_section_group_map.section_map[0][0][0]].blocks[1][1][0].block_id = 1;
    test_section_group[test_section_group_map.section_map[0][0][0]].blocks[0][0][1].block_id = 1;
    test_section_group[test_section_group_map.section_map[0][0][0]].blocks[1][0][1].block_id = 1;
    test_section_group[test_section_group_map.section_map[0][0][0]].blocks[0][1][1].block_id = 1;
    test_section_group[test_section_group_map.section_map[0][0][0]].blocks[1][1][1].block_id = 1;
*/
    glGenBuffers(1, &camera_ssbo); //compute
    glGenBuffers(1, &section_group_ssbo);
    glGenBuffers(1, &section_group_map_ssbo);
    glGenBuffers(1, &section_group_update_map_ssbo);
    glGenBuffers(1, &center_octree_ssbo);
    glGenBuffers(1, &section_octree_ssbo);
    glGenBuffers(1, &block_info_list_ssbo);

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, center_octree_ssbo);
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(octree), NULL, GL_DYNAMIC_COPY);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, section_octree_ssbo);
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(octree)*4096, NULL, GL_DYNAMIC_COPY);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, block_info_list_ssbo);
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(block_info)*BLOCK_NUM, block_info_list, GL_DYNAMIC_COPY);
    
    glGenTextures(1, &output_image);
    glBindTexture(GL_TEXTURE_2D, output_image);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, WINDOW_WIDTH, WINDOW_HEIGHT, 0, GL_RGBA, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glGenVertexArrays(1, &vao); //display
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(display_vertices), display_vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(display_indices), display_indices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), (void*)(2*sizeof(float)));
    glEnableVertexAttribArray(1);

    section_octree_pass1_compute_shader = glCreateShader(GL_COMPUTE_SHADER); //octree
    glShaderSource(section_octree_pass1_compute_shader, 1, &section_octree_pass1_compute_shader_source, NULL);
    glCompileShader(section_octree_pass1_compute_shader);
    check_shader_compile(section_octree_pass1_compute_shader);

    section_octree_pass1_compute_program = glCreateProgram();

    glAttachShader(section_octree_pass1_compute_program, section_octree_pass1_compute_shader);
    glLinkProgram(section_octree_pass1_compute_program);
    check_program_link(section_octree_pass1_compute_program);

    glDeleteShader(section_octree_pass1_compute_shader);

    section_octree_pass2_compute_shader = glCreateShader(GL_COMPUTE_SHADER);
    glShaderSource(section_octree_pass2_compute_shader, 1, &section_octree_pass2_compute_shader_source, NULL);
    glCompileShader(section_octree_pass2_compute_shader);
    check_shader_compile(section_octree_pass2_compute_shader);

    section_octree_pass2_compute_program = glCreateProgram();

    glAttachShader(section_octree_pass2_compute_program, section_octree_pass2_compute_shader);
    glLinkProgram(section_octree_pass2_compute_program);
    check_program_link(section_octree_pass2_compute_program);

    glDeleteShader(section_octree_pass2_compute_shader);

    center_octree_pass1_compute_shader = glCreateShader(GL_COMPUTE_SHADER);
    glShaderSource(center_octree_pass1_compute_shader, 1, &center_octree_pass1_compute_shader_source, NULL);
    glCompileShader(center_octree_pass1_compute_shader);
    check_shader_compile(center_octree_pass1_compute_shader);

    center_octree_pass1_compute_program = glCreateProgram();

    glAttachShader(center_octree_pass1_compute_program, center_octree_pass1_compute_shader);
    glLinkProgram(center_octree_pass1_compute_program);
    check_program_link(center_octree_pass1_compute_program);

    glDeleteShader(center_octree_pass1_compute_shader);

    center_octree_pass2_compute_shader = glCreateShader(GL_COMPUTE_SHADER);
    glShaderSource(center_octree_pass2_compute_shader, 1, &center_octree_pass2_compute_shader_source, NULL);
    glCompileShader(center_octree_pass2_compute_shader);
    check_shader_compile(center_octree_pass2_compute_shader);

    center_octree_pass2_compute_program = glCreateProgram();

    glAttachShader(center_octree_pass2_compute_program, center_octree_pass2_compute_shader);
    glLinkProgram(center_octree_pass2_compute_program);
    check_program_link(center_octree_pass2_compute_program);

    glDeleteShader(section_octree_pass2_compute_shader);

    rt_compute_shader = glCreateShader(GL_COMPUTE_SHADER); //rt
    glShaderSource(rt_compute_shader, 1, &rt_compute_shader_source, NULL);
    glCompileShader(rt_compute_shader);
    check_shader_compile(rt_compute_shader);

    rt_compute_program = glCreateProgram();
    glAttachShader(rt_compute_program, rt_compute_shader);
    glLinkProgram(rt_compute_program);
    check_program_link(rt_compute_program);

    glDeleteShader(rt_compute_shader);

    display_vertex_shader = glCreateShader(GL_VERTEX_SHADER); //display
    glShaderSource(display_vertex_shader, 1, &display_vertex_shader_source, NULL);
    glCompileShader(display_vertex_shader);
    check_shader_compile(display_vertex_shader);

    display_fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(display_fragment_shader, 1, &display_fragment_shader_source, NULL);
    glCompileShader(display_fragment_shader);
    check_shader_compile(display_fragment_shader);

    display_program = glCreateProgram();
    glAttachShader(display_program, display_vertex_shader);
    glAttachShader(display_program, display_fragment_shader);
    glLinkProgram(display_program);
    check_program_link(display_program);

    glDeleteShader(display_vertex_shader);
    glDeleteShader(display_fragment_shader);

    section_group_udpate(test_section_group, &test_section_group_map);
    octree_update();
}

void shader_run() {
    glUseProgram(rt_compute_program);

    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, camera_ssbo);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, section_group_ssbo);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, section_group_map_ssbo);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, block_info_list_ssbo);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, center_octree_ssbo);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 5, section_octree_ssbo);
    glBindImageTexture(6, output_image, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);
    glDispatchCompute(WINDOW_WIDTH/8, WINDOW_HEIGHT/8, 1);
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

    glUseProgram(display_program);
    glBindTexture(GL_TEXTURE_2D, output_image);
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}