#version 430

in layout(location=0) vec3 position;
in layout(location=1) vec3 vertexColor;
in layout(location=2) vec3 vertexNormal;
in layout(location=3) vec2 uvPosition;


uniform mat4 mvp;

out vec3 v_position;
out vec3 v_vertexNormal;
out vec2 v_uvPosition;

void main()
{
	vec4 v = vec4(position, 1.0);
	gl_Position = mvp * v;

	v_position = position;
	v_vertexNormal = vertexNormal;
	v_uvPosition = uvPosition;

}