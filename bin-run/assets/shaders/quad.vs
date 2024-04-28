#version 300 es
precision mediump float;

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aUv;

out vec2 uv;

void main()
{
  uv = aUv.xy;
  gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
}