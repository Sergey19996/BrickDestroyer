#version 300 es
precision mediump float;


out vec4 FragColor;
//in vec3 fragPos;  // ������� ������� ��������� �  NDC (-1 -> 1)


in vec2 TexCoord;
uniform sampler2D texture0;
uniform vec4 color;
uniform bool isWhite;

void main()
{


vec4 texColor = vec4(texture(texture0,TexCoord));

if(isWhite == false){
FragColor = vec4(texColor.rgb * color.rgb, texColor.a * color.a);
}else{
FragColor = vec4(1.0f,1.0f,1.0f, texColor.a  );
}
}
