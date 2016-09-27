#version 430

out vec4 daColor;
in vec3 theNormal;
in vec3 thePosition;  

uniform vec3 lightPosition;
uniform vec3 ambientLight;

void main()
{
	vec4 newAmbientLight = vec4(ambientLight, 1.0f);
	vec3 newNormal = normalize(theNormal);
	vec3 lightVector = normalize(lightPosition - thePosition);
	float diffuseLight = clamp(dot(lightVector, newNormal), 0, 1);

	daColor = newAmbientLight * vec4(diffuseLight, diffuseLight, diffuseLight, 1.0f);
}
