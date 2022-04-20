#version 400 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

// texture samplers
uniform sampler2D ourTexture0;  //背景图
uniform sampler2D ourTexture1;	//商品图
uniform int width, height, goods_x, goods_y, goods_width, goods_height;
uniform float lightRatio;
uniform float gaussWeight[(radius + 1) * (radius + 1)]; 

void main()
{
	float alphaSum = 0.0;
	vec2 GoodsTexCoord = vec2((TexCoord.x * width - goods_x + goods_width / 2.0)/goods_width, (TexCoord.y * height - goods_y + goods_height / 2.0)/goods_height);
	for (int i = -radius; i < radius + 1; i++) {
		for (int j = -radius; j < radius + 1; j++) {
			float i_float = float(i);
			float j_float = float(j);
			alphaSum += gaussWeight[abs(i) * (radius + 1) + abs(j)] * texture(ourTexture1, vec2(GoodsTexCoord.x + i_float/goods_width, GoodsTexCoord.y + j_float/goods_height)).a;
		}
	}
	vec3 gaussedColor = vec3(0.0);
	for (int i = -radius; i < radius + 1; i++) {
		for (int j = -radius; j < radius + 1; j++) {
			float i_float = float(i);
			float j_float = float(j);
			vec2 inTexCoord = vec2(TexCoord.x + i_float/width, TexCoord.y + j_float/height);
			vec2 inGoodsTexCoord = vec2(GoodsTexCoord.x + i_float/goods_width, GoodsTexCoord.y + j_float/goods_height);
			vec3 color = mix(texture(ourTexture0, inTexCoord).rgb, texture(ourTexture1, inGoodsTexCoord).rgb, texture(ourTexture1, inGoodsTexCoord).a);
			gaussedColor += gaussWeight[abs(i) * (radius + 1) + abs(j)] * color;
		}
	}

	float gaussRatio = 2.0 * abs(alphaSum - 0.5);
	vec3 originColor = mix(texture(ourTexture0, TexCoord).rgb, texture(ourTexture1, GoodsTexCoord).rgb, texture(ourTexture1, GoodsTexCoord).a);
	FragColor = vec4(mix(gaussedColor * lightRatio, originColor, pow(gaussRatio, radius)), 1.0);
}
