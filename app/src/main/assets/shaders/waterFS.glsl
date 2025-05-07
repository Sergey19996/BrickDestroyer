#version 300 es
precision mediump float;
out vec4 FragColor;


in vec2 TexCoord;
uniform sampler2D texture0;
uniform float time;
float radius;
vec2 circle;
void main()
{
vec4 texColor = vec4(texture(texture0,vec2(TexCoord.x + cos(time),TexCoord.y + sin(time))));
radius = 0.4f;
circle = vec2(0.5f,0.5f);
float dx = TexCoord.x - circle.x;
float dy = TexCoord.y - circle.y;
if (dx * dx + dy * dy < radius * radius){
FragColor = texColor.rgba;

}else
FragColor = vec4(0.0f,0.0f,0.0f , 0.0f);
}
