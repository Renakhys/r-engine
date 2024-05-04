#version 300 es
precision mediump float;

layout (location = 0) in vec3 in_pos;
layout (location = 1) in vec2 in_uv;
layout (location = 2) in vec4 in_foreground;
layout (location = 3) in vec4 in_background;
layout (location = 4) in float in_tex;

uniform mat4 u_view_projection;

out vec2 uv;
out vec4 foreground;
out vec4 background;
flat out float tex;

void main()
{
  uv = in_uv;
  foreground = in_foreground;
  background = in_background;
  tex = in_tex;

  gl_Position = u_view_projection * vec4(in_pos.xyz, 1.0);
}