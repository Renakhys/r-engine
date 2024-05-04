#version 300 es
precision mediump float;

uniform sampler2D ourTexture;

out vec4 FragColor;

in vec2 uv;
flat in float tex_id;

void main()
{
    // FragColor = vec4(0.5f, 1.f, 0.2f, 1.f);
    FragColor = texture(ourTexture, uv);
} 