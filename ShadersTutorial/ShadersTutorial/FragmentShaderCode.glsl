#version 430

out vec4 daColor;
in vec3 normalWorld;
in vec3 vertexPositionWorld;  
in vec2 UVs;

uniform vec3 lightPositionWorld;
uniform vec3 ambientLight;
uniform vec3 cameraPositionWorld;
//uniform sampler2D rogerTexture;
uniform sampler2D normalMap;

void main()
{

	//normalMap
	mat3 normalMapTransformation;
	normalMapTransformation[0] = vec3(1.0, 0.0, 0.0);
	normalMapTransformation[1] = vec3(0.0, 1.0, 0.0);
	float normalScale = 1.0f/normalWorld[2];
	//if (normalWorld[2] < 0)
	//{
	//normalScale *= -1.0f;
	//}
	normalMapTransformation[2] = normalize(normalWorld * vec3(normalScale, normalScale, normalScale));
	vec4 normalMapSample = texture(normalMap, UVs);
	//vec3 tangent = normalMapTransformation * vec3(normalMapSample);
	vec3 normalTangent = normalMapTransformation * vec3(normalMapSample);
	normalTangent = normalTangent * 2.0 - 1.0;

	//Diffuse Light
	vec4 newAmbientLight = vec4(ambientLight, 1.0f);
	vec3 newNormalWorld = normalize(normalTangent);
	vec3 lightVectorWorld = normalize(lightPositionWorld - vertexPositionWorld);
	float brightness = (dot(lightVectorWorld, newNormalWorld));
	vec4 diffuseLight = clamp(vec4(brightness, brightness, brightness, 1.0f), 0, 1);

	//attenuation
	float lightDistance = distance(lightPositionWorld, vertexPositionWorld);
	float attenuationConstant = 1.2f;
	float attenuationConstantLinear = 0.2;
	float attenuationConstantQuadratic = 0.1;
	float lightAttenuation = 1/(attenuationConstant + (attenuationConstantLinear * lightDistance) + (attenuationConstantQuadratic * pow(lightDistance, 2)));
	diffuseLight = lightAttenuation * diffuseLight;

	//Specular
	vec3 specularLightVectorWorld = reflect(-lightVectorWorld, normalTangent);
	vec3 cameraVectorWorld = normalize(cameraPositionWorld - vertexPositionWorld);
	float specular = dot(specularLightVectorWorld, cameraVectorWorld);
	specular = pow(specular, 40);
	vec4 specularity = clamp(vec4(specular, specular, specular, 1), 0, 1);


	//vec4 texSample = texture(rogerTexture, UVs);
	//output color
	//daColor = texSample * (diffuseLight + newAmbientLight + specularity);
	//daColor = normalMapSample;
	daColor = normalMapSample * (diffuseLight + newAmbientLight + specularity);
}
