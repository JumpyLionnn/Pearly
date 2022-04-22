#type vertex
#version 330 core

layout(location = 0) in vec3 a_Postion;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;
			
void main()
{
	gl_Position = u_ViewProjection * u_Transform * vec4(a_Postion, 1.0);
}

#type fragment
#version 330 core

uniform vec4 u_Color;

layout(location = 0) out vec4 Color;

void main()
{
	Color = u_Color;
}