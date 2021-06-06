#shader vertex

#version 330 core
layout(location = 0) in vec4 a_pos;
layout(location = 1) in vec4 a_col;
layout(location = 2) in vec2 a_texCord;
layout(location = 3) in float a_texIndex;

out vec4 v_col;
out vec2 v_texCord;
out float v_texIndex;

uniform mat4 u_mvp;
void main()
{
	gl_Position = u_mvp * a_pos;
	v_col = a_col;
	v_texCord = a_texCord;
	v_texIndex = a_texIndex;
}

#shader fragment

#version 330 core
layout(location = 0) out vec4 col;

in vec4 v_col;
in vec2 v_texCord;
in float v_texIndex;

uniform sampler2D u_textureSlots[16];

void main()
{
	vec4 texColor = v_col;
	switch (int(v_texIndex)) {
	case  0: texColor *= texture(u_textureSlots[0], v_texCord); break;
	case  1: texColor *= texture(u_textureSlots[1], v_texCord); break;
	case  2: texColor *= texture(u_textureSlots[2], v_texCord); break;
	case  3: texColor *= texture(u_textureSlots[3], v_texCord); break;
	case  4: texColor *= texture(u_textureSlots[4], v_texCord); break;
	case  5: texColor *= texture(u_textureSlots[5], v_texCord); break;
	case  6: texColor *= texture(u_textureSlots[6], v_texCord); break;
	case  7: texColor *= texture(u_textureSlots[7], v_texCord); break;
	case  8: texColor *= texture(u_textureSlots[8], v_texCord); break;
	case  9: texColor *= texture(u_textureSlots[9], v_texCord); break;
	case 10: texColor *= texture(u_textureSlots[10], v_texCord); break;
	case 11: texColor *= texture(u_textureSlots[11], v_texCord); break;
	case 12: texColor *= texture(u_textureSlots[12], v_texCord); break;
	case 13: texColor *= texture(u_textureSlots[13], v_texCord); break;
	case 14: texColor *= texture(u_textureSlots[14], v_texCord); break;
	case 15: texColor *= texture(u_textureSlots[15], v_texCord); break;
	}
	col = texColor;
}

