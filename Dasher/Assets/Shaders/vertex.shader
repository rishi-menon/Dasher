#shader vertex

#version 330 core
layout(location = 0) in vec4 a_pos;
layout(location = 1) in vec4 a_col;
layout(location = 2) in vec2 a_texCord;
layout(location = 3) in float a_texIndex;

out vec4 v_col;
out vec2 v_texCord;
out float v_texIndex;

void main()
{
	gl_Position = a_pos;
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

uniform sampler2D u_textureSlots[32];

void main()
{
	int texId = int(v_texIndex);
	
	col = v_col * texture(u_textureSlots[texId], v_texCord);
	//col = vec4 (v_texCord.xy, 0, 1.0);
	//col = vec4 (v_texIndex, v_texIndex, v_texIndex, 1.0);
}

