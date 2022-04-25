#version 400 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;
in vec2 fgTexCoord;

// texture samplers
uniform sampler2D ourTexture0;
uniform sampler2D ourTexture1;
uniform sampler2D backgroundTexture;
uniform int width, height, bgWidth, bgHeight;
uniform float lightRatio;
uniform float gaussWeight[(radius + 1) * (radius + 1)]; 

void main()
{
	float alphaSum = 0.0;
	for (int i = -radius; i < radius + 1; i++) {
		for (int j = -radius; j < radius + 1; j++) {
			float i_float = float(i);
			float j_float = float(j);
			alphaSum += gaussWeight[abs(i) * (radius + 1) + abs(j)] * texture(ourTexture1, vec2(fgTexCoord.x + i_float/width, fgTexCoord.y + j_float/height)).r;
		}
	}

	// FragColor = mix(texture(backgroundTexture, TexCoord), texture(ourTexture0, fgTexCoord), alphaSum);
	vec4 originBlend = mix(texture(backgroundTexture, TexCoord), texture(ourTexture0, fgTexCoord), texture(ourTexture1, fgTexCoord).r);

	vec4 gaussedColor = vec4(vec3(0.0), 1.0);
	for (int i = -radius; i < radius + 1; i++) {  
		for (int j = -radius; j < radius + 1; j++) {
			float i_float = float(i);
			float j_float = float(j);
			// gaussedColor += gaussWeight[abs(i) * (radius + 1) + abs(j)] * texture(ourTexture0, vec2(TexCoord.x + i_float/width, TexCoord.y + j_float/height));
			gaussedColor += gaussWeight[abs(i) * (radius + 1) + abs(j)] * mix(texture(backgroundTexture, vec2(TexCoord.x + i_float/bgWidth, TexCoord.y + j_float/bgHeight)), texture(ourTexture0, vec2(fgTexCoord.x + i_float/width, fgTexCoord.y + j_float/height)), texture(ourTexture1, vec2(fgTexCoord.x + i_float/width, fgTexCoord.y + j_float/height)).r);
		}
	}
	float gaussRatio = 2.0 * abs(alphaSum - 0.5);
	FragColor = mix(gaussedColor * lightRatio, originBlend, pow(gaussRatio, radius));
}
