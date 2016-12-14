#version 430

in vec3 theColor;
in vec3 v_position;
in vec3 v_normal;
in vec2 v_uvPosition;
in vec3 v_tangent;
out vec4 daColor;

uniform vec3 ambientLight;
uniform vec3 diffuseLightPosition;
uniform mat4 modelToWorldTransform;
uniform vec3 eyePositionWorld;
uniform mat4 modelToWorldInvertTrans;

uniform sampler2D Tex1;
uniform sampler2D Normal1;
uniform sampler2D Spec1;
uniform sampler2D Alpha1;
uniform sampler2D Ao1;

void main()
{
	vec3 vertexWorld = vec3(modelToWorldTransform * vec4(v_position, 1.0));

	// normal
	vec3 normalWorld = normalize(vec3(modelToWorldInvertTrans * vec4(v_normal, 0.0f)));
	vec3 tangentWorld = normalize(vec3(modelToWorldTransform * vec4(v_tangent, 0.0f)));
	vec3 bitangentWorld = cross(normalWorld, tangentWorld);
	mat3 tbn = mat3(tangentWorld, bitangentWorld, normalWorld);

	vec3 normalMap = vec3(texture(Normal1, v_uvPosition));
	normalMap = normalize(normalMap * 2.0 - 1.0);
	normalWorld = normalize(tbn * normalMap);

	// diffuse light calculation
	vec3 lightVectorWorld = normalize(diffuseLightPosition - vertexWorld);
	float diffuseIntensity = dot(lightVectorWorld, normalWorld);
	vec3 diffuse = vec3(0.0, diffuseIntensity, 0.0);

	// specular light calculation
	vec3 reflectedLightVectorWorld = reflect(-lightVectorWorld, normalWorld);
	vec3 eyeVectorWorld = normalize(eyePositionWorld - vertexWorld);
	float specularity = dot(reflectedLightVectorWorld, eyeVectorWorld);	
	vec3 specMap = vec3(texture(Spec1, v_uvPosition));
	specularity = pow(specularity, specMap.r);
//	specularity = pow(specularity, 40);
	vec3 specularLight = vec3(0.0, 0.0, specularity);

	// attenuation
	float d = length(diffuseLightPosition - vertexWorld);
	float k1 = 1.0;
	float k2 = 0.1;
	float k3 = 0.05;

	vec3 diffAttn = clamp(diffuse, 0.0, 1.0) * (1.0/(k1 + k2*d + k3*d*d));
	vec3 lightColor = ambientLight + diffAttn + specularLight;

	// texture
	vec4 texColor = texture(Tex1, v_uvPosition);

	// ambient occlusion
	vec3 aoColor = vec3(texture(Ao1, v_uvPosition));

//	daColor = vec4(0.0f, 0.0f, 1.0f, 1.0f);	
//	daColor = vec4(aoColor, 1.0f);
//	daColor = vec4(lightColor, 1.0f);
	daColor = vec4(specularLight, 1.0f);
//	daColor = vec4(vec3(texture(Normal1, v_uvPosition)), 0.5f);
//	daColor = vec4(vec3(texColor) * lightColor * aoColor, 0.5f);
}