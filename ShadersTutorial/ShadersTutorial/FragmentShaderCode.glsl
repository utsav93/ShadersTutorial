#version 430

out vec4 daColor;
in vec3 theNormal;
in vec3 thePosition;

uniform vec3 lightPosition;

void main()
{
	vec3 lightVector = normalize(lightPosition - thePosition);
	float diffuseLight = dot(lightVector, theNormal);
	daColor = vec4(diffuseLight, diffuseLight, diffuseLight, 1.0f);
	//daColor = vec4(theColor, 1.0);
}
