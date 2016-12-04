#pragma once


out float fragmentDepth;

void main()
{
	fragmentDepth = gl_FragCoord.z;
}