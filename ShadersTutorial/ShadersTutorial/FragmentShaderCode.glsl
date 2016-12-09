#version 430

out vec4 daColor;
in vec3 normalWorld;
in vec3 vertexPositionWorld;  
in vec2 UVs;
in vec3 tangentWorld;

uniform vec3 lightPositionWorld;
uniform vec3 ambientLight;
uniform vec3 cameraPositionWorld;
uniform sampler2D rogerTexture;
uniform sampler2D normalMap;

void main()
{

	vec3 biTangentWorld = cross(normalWorld, tangentWorld);
	mat3 tbn = mat3(tangentWorld, biTangentWorld, normalWorld);
	//normalMap
	vec3 normalMapSample = vec3(texture(normalMap, UVs));
	normalMapSample = normalize(normalMapSample * 2.0 - 1.0);

	//Diffuse Light
	vec4 newAmbientLight = vec4(ambientLight, 1.0f);
	vec3 newNormalWorld = (normalize(tbn * normalMapSample));
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
	vec3 specularLightVectorWorld = reflect(-lightVectorWorld, newNormalWorld);
	vec3 cameraVectorWorld = normalize(cameraPositionWorld - vertexPositionWorld);
	float specular = dot(specularLightVectorWorld, cameraVectorWorld);
	specular = pow(specular, 40);
	vec4 specularity = clamp(vec4(specular, specular, specular, 1), 0, 1);


	vec4 texSample = texture(rogerTexture, UVs);
	//output color
	daColor = (diffuseLight);// + newAmbientLight + specularity);
	//daColor = normalMapSample;
}
