#version 430

in layout(location=0) vec3 vertexPositionModel;
in layout(location=1) vec3 vertexColor;
in layout(location=2) vec3 normalModel;
in layout(location=3) vec2 UVModel;


uniform mat4 modelToProjectionMatrix;
uniform mat4 modelToWorldMatrix;

out vec3 normalWorld;
out vec3 vertexPositionWorld;
out vec2 UVs;

void main()
{
	vec4 vertexPositionModelVec4 = vec4(vertexPositionModel, 1.0);
	gl_Position = modelToProjectionMatrix * vertexPositionModelVec4;
	normalWorld = normalize(vec3(modelToWorldMatrix * vec4(normalModel, 0)));
	vertexPositionWorld = vec3(modelToWorldMatrix * vertexPositionModelVec4);
	vec2 UVs = UVModel;
}