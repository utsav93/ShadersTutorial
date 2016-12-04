#pragma once


layout(location = 0) in vec3 vertexPosModel;

uniform mat4 MVP;

void main()
{
	gl_Position = MVP * vec4(vertexPosModel, 1.0f);
}