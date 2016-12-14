#version 430

out vec4 daColor;
in vec3 v_position;
in vec3 v_vertexNormal;
in vec2 v_uvPosition;

uniform sampler2D Tex1;

void main()
{
	daColor = texture(Tex1, v_uvPosition);
}