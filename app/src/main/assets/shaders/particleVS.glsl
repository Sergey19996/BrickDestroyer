#version 300 es

layout (location = 0) in vec4 vertex;
out vec2 TexCoord;

out float alphaColor;

uniform mat4 projection;
uniform vec2 offset;


uniform vec2 uv;
uniform vec2 spriteScale;
uniform float scale;

void main(){
float Pixelscale = 10.0f * scale;


gl_Position = projection * vec4((vertex.xy * Pixelscale + offset ),0.0, 1.0);


TexCoord = vec2(vertex.z * spriteScale.x + uv.x,vertex.w * spriteScale.y + uv.y);




  //TexCoord = vec2(vertex.z *  spriteScale.x + uv.x,vertex.w * spriteScale.y + uv.y );

}