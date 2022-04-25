#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

// texture sampler
uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float blending;

void main()
{
	FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), blending);
}