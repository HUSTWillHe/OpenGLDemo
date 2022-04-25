#version 400 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec3 ourColor;
out vec2 TexCoord;
out vec2 fgTexCoord;

uniform int width, height, bgWidth, bgHeight;

void main()
{
	gl_Position = vec4(aPos.x, -aPos.y, aPos.z, 1.0);
	ourColor = aColor;
	TexCoord = aTexCoord;
	float fWidth = float(width), fHeight = float(height), fBgWidth = float(bgWidth), fBgHeight = float(bgHeight);
	fgTexCoord = vec2(aTexCoord.x*fBgWidth/fWidth + (fWidth - fBgWidth)/(2.0 * fWidth), aTexCoord.y*fBgHeight/fHeight + (fHeight -fBgHeight)/(2.0 * fHeight));
}
