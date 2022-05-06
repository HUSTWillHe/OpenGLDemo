#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
uniform vec3 color;

// texture samplers
// uniform sampler2D texture1;
//uniform sampler2D texture2;

void main()
{
	// linearly interpolate between both textures (80% container, 20% awesomeface)
	//FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2);
	// FragColor = texture(texture1, TexCoord);
	FragColor = vec4(color, 1.0);
}
