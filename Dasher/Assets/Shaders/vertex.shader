#shader vertex

layout(location = 0) in vec4 a_pos;
layout(location = 0) in vec4 a_col;

void main()
{
	gl_Position = a_pos;
}

#shader fragment

layout(location = 0) out vec4 col;


void main()
{
	col = vec4(0.8, 0.2, 0.3, 1.0);
}

