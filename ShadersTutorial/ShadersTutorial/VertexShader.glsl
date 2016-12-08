#version 430

in layout(location = 0) vec3 vertexPositionModel;
in layout(location = 1) vec3 color;
in layout(location = 2) vec3 normalWorld;
in layout(location = 3) vec2 UVs;

out vec3 daColor;

void main()
{
	vec4 vertexPositionModelVec4 = vec4(vertexPositionModel, 1.0);
	gl_Position = vertexPositionModelVec4;
	daColor = color;
}