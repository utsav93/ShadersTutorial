#version 430

in layout(location=0) vec3 position;
in layout(location=1) vec3 vertexColor;

uniform mat4 modelTransformMatrix;
uniform mat4 projectionMatrix;

out vec3 theColor;

void main()
{
  vec4 positionVec4 = vec4(position, 1.0);
  vec4 newPositionVec4 = modelTransformMatrix * positionVec4;
  vec4 projectedPosition = projectionMatrix * newPositionVec4;
  gl_Position = projectedPosition;
  theColor = vertexColor;
}