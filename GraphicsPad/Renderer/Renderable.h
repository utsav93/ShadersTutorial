#pragma once
#include <Renderer\Geometry.h>
#include <glm\glm.hpp>
//#include <Renderer\ShaderInfo.h>
struct ShaderProgramInfo;

struct Renderable
{
	const Geometry* geometry; //what
	glm::mat4 modelToWorld; //where
	const ShaderProgramInfo* shaderProgramInfo; //how
//	QImage myTex;
//	QImage myAlpha;
//	QImage myNormal;
//	QImage mySpec;
	bool isLight = false;
public:
	Renderable();
	~Renderable();
};

