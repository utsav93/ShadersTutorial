#version 430

in layout(location=0) vec2 position;
in layout(location=1) vec3 vertexColor;

out vec3 theColor;

uniform mat3 transform;

void main()
{
  vec3 positionVec3 = vec3(position, 1);
  vec3 transformedPosition = transform * positionVec3;
  gl_Position = vec4(transformedPosition, 1.0);
  theColor = vertexColor;
}