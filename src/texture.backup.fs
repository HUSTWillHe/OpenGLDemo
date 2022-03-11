#version 400 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

// texture samplers
uniform sampler2D ourTexture0;
uniform sampler2D ourTexture1;
uniform int width, height;
uniform float gaussWeight[(radius + 1) * (radius + 1)]; 

void main()
{
	float alphaSum = 0.0;
	for (int i = -radius; i < radius + 1; i++) {
		for (int j = -radius; j < radius + 1; j++) {
			alphaSum += gaussWeight[abs(i) * (radius + 1) + abs(j)] * texture(ourTexture1, vec2(TexCoord.x + i/width, TexCoord.y + j/height)).r;
		}
	}
	if (alphaSum >= 0.99) {
		FragColor = vec4(1.0, vec3(0.0));
	}
	else if (alphaSum > 0.01 && alphaSum < 0.99) { 
		FragColor = vec4(0.0, 1.0, vec2(0.0));  
	} else {
		FragColor = vec4(0.0);
	}
}
