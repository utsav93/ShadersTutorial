#version 430

in layout(location=0) vec3 position;
in layout(location=1) vec3 vertexColor;
in layout(location=2) vec3 normal;


uniform mat4 fullTransformMatrix;
uniform mat4 modelToWorldTransformMatrix;

out vec3 theNormal;
out vec3 thePosition;

void main()
{
	vec4 positionVec4 = vec4(position, 1.0);
	gl_Position = fullTransformMatrix * positionVec4;
	theNormal = normal;
	thePosition = vec3(modelToWorldTransformMatrix * positionVec4);
}