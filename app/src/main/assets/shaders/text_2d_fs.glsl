#version 300 es
precision mediump float;

in vec2 TexCoord;
out vec4 FragColor;

uniform sampler2D text;
uniform vec3 color;

void main()
{
    float alpha = texture(text, TexCoord).r;  // если GL_RED
    FragColor = vec4(color, alpha);
}