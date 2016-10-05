#version 430

out vec4 daColor;
in vec3 normalWorld;
in vec3 vertexPositionWorld;  

uniform vec3 lightPositionWorld;
uniform vec3 ambientLight;
uniform vec3 cameraPositionWorld;

void main()
{
	//Diffuse Light
	vec4 newAmbientLight = vec4(ambientLight, 1.0f);
	vec3 newNormalWorld = normalize(normalWorld);
	vec3 lightVectorWorld = normalize(lightPositionWorld - vertexPositionWorld);
	float brightness = (dot(lightVectorWorld, newNormalWorld));
	vec4 diffuseLight = clamp(vec4(brightness, brightness, brightness, 1.0f), 0, 1);

	//attenuation
	float lightDistance = length(lightVectorWorld);
	float attenuationConstant = 0.1f;
	float attenuationConstantLinear = 0.7;
	float attenuationConstantQuadratic = 0.9;
	float lightAttenuation = 1/(attenuationConstant + (attenuationConstantLinear * lightDistance) + (attenuationConstantQuadratic * pow(lightDistance, 2)));
	diffuseLight = lightAttenuation * diffuseLight;

	//Specular
	vec3 specularLightVectorWorld = reflect(-lightVectorWorld, normalWorld);
	vec3 cameraVectorWorld = normalize(cameraPositionWorld - vertexPositionWorld);
	float specular = dot(specularLightVectorWorld, cameraVectorWorld);
	specular = pow(specular, 40);
	vec4 specularity = clamp(vec4(specular, 0, 0, 1), 0, 1);


	daColor = diffuseLight + newAmbientLight + specularity;
}
