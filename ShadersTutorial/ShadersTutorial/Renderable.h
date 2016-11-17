#pragma once
#include <glm.hpp>
#include <Geometry.h>
struct ShaderProgramInfo;

struct Renderable
{
	const Geometry* geometry; //what
	glm::mat4 modelToWorld; //where
	const ShaderProgramInfo* shaderProgramInfo; //how
};