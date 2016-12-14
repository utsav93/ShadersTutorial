//#pragma once
#ifndef ENGINE_DATA_TYPE_H
#define ENGINE_DATA_TYPE_H

#include <GL\glew.h>
enum DataType
{
	DT_UNDIGNED_SHORT = sizeof(GLushort),
	DT_UNDIGNED_INT = sizeof(GLuint)
};

#endif