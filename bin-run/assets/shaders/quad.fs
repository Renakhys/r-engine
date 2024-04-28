#version 300 es
precision mediump float;

uniform sampler2D ourTexture;

in vec2 uv;

out vec4 FragColor;

void main()
{
    // FragColor = vec4(uv.x, uv.y, 0.2f, 1.0f);
    FragColor = texture(ourTexture, uv);
} 