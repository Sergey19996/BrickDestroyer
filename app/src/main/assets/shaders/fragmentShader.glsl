#version 300 es
precision mediump float;


out vec4 FragColor;
//in vec3 fragPos;  // ������� ������� ��������� �  NDC (-1 -> 1)


in vec2 TexCoord;
uniform sampler2D texture0;
uniform vec4 color;


void main()
{


vec4 texColor = vec4(texture(texture0,TexCoord));
FragColor = vec4(texColor.rgb * color.rgb, texColor.a * color.a);
}
