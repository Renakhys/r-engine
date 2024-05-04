#version 300 es
precision mediump float;

layout (location = 0) in vec3 in_pos;
layout (location = 1) in vec2 in_uv;
// layout (location = 2) in float in_tex;

out vec2 uv;
flat out float tex_id;

void main()
{
  uv = in_uv;
  // tex_id = in_tex;
  gl_Position = vec4(in_pos.xyz, 1.0);
}