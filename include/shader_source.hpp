#pragma once

#include "extern_includes.hpp"

const char *section_octree_pass1_compute_shader_source = 
#include "section_octree_pass1.comp"
;

const char *section_octree_pass2_compute_shader_source = 
#include "section_octree_pass2.comp"
;

const char *center_octree_pass1_compute_shader_source = 
#include "center_octree_pass1.comp"
;

const char *center_octree_pass2_compute_shader_source = 
#include "center_octree_pass2.comp"
;

const char *rt_compute_shader_source = 
#include "raytracing.comp"
;

const char *display_vertex_shader_source = 
#include "vertex.vert"
;

const char *display_fragment_shader_source = 
#include "fragment.frag"
;