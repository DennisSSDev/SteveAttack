#version 330

in vec3 Color;

uniform vec3 SolidColor = vec3(-1,-1,-1);
uniform int InvertColor; 

vec3 endColor = vec3(0,0,0);

out vec4 Fragment;

void main()
{
	Fragment = vec4(Color,1);
	
	if(SolidColor.r != -1.0 && SolidColor.g != -1.0 && SolidColor.b != -1.0) {
		if(InvertColor > 0) {
			endColor.r = 1 - SolidColor.r;
			endColor.g = 1 - SolidColor.g;
			endColor.b = 1 - SolidColor.b;
			Fragment = vec4(endColor, 1);
			return;

		}
		Fragment = vec4(SolidColor, 1);
		return;
	}
	if(InvertColor > 0) {  
		endColor.r = 1 - Color.r;
		endColor.g = 1 - Color.g;
		endColor.b = 1 - Color.b;
		Fragment = vec4(endColor, 1);
	}
	return;
}