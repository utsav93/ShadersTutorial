#version 430

in layout(location=0) vec3 position;
in layout(location=1) vec3 color;
in layout(location=2) vec3 normal;
in layout(location=3) vec2 uvPosition;

out vec3 theColor;
out vec3 v_position;
out vec3 v_normal;

uniform mat4 mvp;	// model to view matrix

void main()
{
	vec4 v = vec4(position, 1.0);
 	gl_Position = mvp * v;
	theColor = color;

	v_position = position;
	v_normal = normal;
}
