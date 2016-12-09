#version 430

in layout(location=0) vec3 vertexPositionModel;
in layout(location=1) vec3 vertexColor;
in layout(location=2) vec3 normalModel;
in layout(location=3) vec2 UVModel;
in layout(location=4) vec3 tangentModel;


uniform mat4 modelToProjectionMatrix;
uniform mat4 modelToWorldMatrix;
uniform mat4 modelToWorldInvert;

out vec3 normalWorld;
out vec3 vertexPositionWorld;
out vec2 UVs;
out vec3 tangentWorld;

void main()
{
	vec4 vertexPositionModelVec4 = vec4(vertexPositionModel, 1.0);
	gl_Position = modelToProjectionMatrix * vertexPositionModelVec4;
	normalWorld = normalize(vec3(modelToWorldMatrix * vec4(normalModel, 0.0)));
	vertexPositionWorld = vec3(modelToWorldMatrix * vertexPositionModelVec4);
	UVs = UVModel;
	tangentWorld = normalize(vec3(modelToWorldMatrix * vec4(tangentModel,1.0)));
}