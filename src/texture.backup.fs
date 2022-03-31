#version 400 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

// texture samplers
uniform sampler2D ourTexture0;
uniform sampler2D ourTexture1;
uniform int width, height;
uniform float lightRatio;
uniform float gaussWeight[(radius + 1) * (radius + 1)]; 

void main()
{
	float alphaSum = 0.0;
	for (int i = -radius; i < radius + 1; i++) {
		for (int j = -radius; j < radius + 1; j++) {
			float i_float = float(i);
			float j_float = float(j);
			alphaSum += gaussWeight[abs(i) * (radius + 1) + abs(j)] * texture(ourTexture1, vec2(TexCoord.x + i_float/width, TexCoord.y + j_float/height)).r;
		}
	}
	vec4 gaussedColor = vec4(vec3(0.0), 1.0);
	for (int i = -radius; i < radius + 1; i++) {  
		for (int j = -radius; j < radius + 1; j++) {
			float i_float = float(i);
			float j_float = float(j);
			gaussedColor += gaussWeight[abs(i) * (radius + 1) + abs(j)] * texture(ourTexture0, vec2(TexCoord.x + i_float/width, TexCoord.y + j_float/height));
		}
	}
	float gaussRatio = 2.0 * abs(alphaSum - 0.5);
	FragColor = mix(gaussedColor * lightRatio, texture(ourTexture0, TexCoord), pow(gaussRatio, radius));
}
