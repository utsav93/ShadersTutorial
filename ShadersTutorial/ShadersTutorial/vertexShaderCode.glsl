#version 430

in layout(location=0) vec3 position;
in layout(location=1) vec3 vertexColor;
in layout(location=2) vec3 normal;

uniform vec3 ambientLight;
uniform vec3 lightPosition;

uniform mat4 fullTransformMatrix;

//out vec3 theColor;
out vec3 theNormal;
out vec3 thePosition;

void main()
{
	vec4 positionVec4 = vec4(position, 1.0);
	gl_Position = fullTransformMatrix * positionVec4;
	//vec3 lightVector = normalize(lightPosition - position);
	//float diffuseLight = dot(lightVector, normal);
	//theColor = vec3(diffuseLight, diffuseLight, diffuseLight);
	theNormal = normal;
	thePosition = position;
}