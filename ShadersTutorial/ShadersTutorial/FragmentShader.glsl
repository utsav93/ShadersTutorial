#version 430

in vec3 daColor;

out vec4 theColor;

void main()
{	
	theColor = vec4(daColor, 1.0f);
}
