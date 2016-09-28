#version 430

out vec4 daColor;
in vec3 normalWorld;
in vec3 vertexPositionWorld;  

uniform vec3 lightPositionWorld;
uniform vec3 ambientLight;

void main()
{
	vec4 newAmbientLight = vec4(ambientLight, 1.0f);
	vec3 newNormalWorld = normalize(normalWorld);
	vec3 lightVectorWorld = normalize(lightPositionWorld - vertexPositionWorld);
	float diffuseLight = (dot(lightVectorWorld, newNormalWorld));

	daColor = newAmbientLight + clamp(vec4(diffuseLight, diffuseLight, diffuseLight, 1.0f), 0, 1);
}
