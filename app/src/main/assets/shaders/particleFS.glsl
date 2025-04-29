#version 300 es

in vec4  Color;
in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D texture0;
uniform vec4 color;

void main(){

vec4 texColor = vec4(texture(texture0,TexCoord));

FragColor = vec4(texColor.rgb * color.rgb, texColor.a * color.a);
//FragColor = vec4(texColor.rgb * vec3(0.99,0.76,0.45),texColor.a);
}