#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

// texture samplers
uniform sampler2D ourTexture0;
uniform sampler2D ourTexture1;
uniform int width, height;
uniform mat3 gaussWeight;

void main()
{
	// linearly interpolate between both textures (80% container, 20% awesomeface)
	// 根据core大小，生成高斯系数
	int core = 3;
	float alpha = texture(ourTexture1, TexCoord).r;
	float bottom = alpha, top = alpha;
	for (float i = -core; i < core + 0.1; i += 1.0) {
		for (float j = -core; j < core + 0.1; j += 1.0) {
			float a = texture(ourTexture1, vec2(TexCoord.x + i / width, TexCoord.y + j / height)).r; // 1. 在外面传入宽高， 2. 使用gauss替代平均值，3。在外面计算出高斯系数，将合理的值放在一个mat中
			if (a > top)
				top = a;
			if (a < bottom)
				bottom  = a;
		}
	}
	if (alpha < 0.001 && top - bottom < 0.0001) {
		FragColor = vec4(0.0);
	} else if (alpha > 0.999 && top - bottom < 0.0001) {
		FragColor = texture(ourTexture0, TexCoord);
	} else {
		for (float i = -core; i < core + 0.1; i += 1.0) {
			for (float j = -core; j < core + 0.1; j += 1.0) {
				FragColor += texture(ourTexture0, vec2(TexCoord.x + i / width, TexCoord.y + j / height));
			}
		}
		FragColor /= (core + 1.0)*(core + 1.0);
		// FragColor = vec4(1.0, 0.0, 0.0, 0.0);
	}
}
