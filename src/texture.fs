#version 400 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

// texture samplers
uniform sampler2D ourTexture0;
uniform sampler2D ourTexture1;
uniform int width, height;
uniform float gaussWeight[(9 + 1) * (9 + 1)]; 

void main()
{
	float alphaSum = 0.0;
	for (int i = -9; i < 9 + 1; i++) {
		for (int j = -9; j < 9 + 1; j++) {
			alphaSum += gaussWeight[abs(i) * (9 + 1) + abs(j)] * texture(ourTexture1, vec2(TexCoord.x + i/width, TexCoord.y + j/height)).r;
		}
	}
	if (alphaSum >= 0.99) {
		FragColor = vec4(texture(ourTexture0, TexCoord).rgb, 1.0);
	}
	else if (alphaSum > 0.01 && alphaSum < 0.99) { 
		vec4 gaussedColor = vec4(vec3(0.0), 1.0);
		for (int i = -9; i < 9 + 1; i++) {
			for (int j = -9; j < 9 + 1; j++) {
				gaussedColor += gaussWeight[abs(i) * (9 + 1) + abs(j)] * texture(ourTexture0, vec2(TexCoord.x + i/width, TexCoord.y + j/height));
			}
		}	
	} else {
		FragColor = vec4(0.0);
	}
}

