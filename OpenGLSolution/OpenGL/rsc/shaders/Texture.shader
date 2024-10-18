#shader vertex

#version 460 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 color;
layout(location = 2) in vec2 texCoord;
layout(location = 3) in float texIndex;

uniform mat4 u_MVP;

out vec4 v_color;
out vec2 v_texCoord;
out float v_texIndex;

void main()
{
    v_color = color;
    v_texCoord = texCoord;
    v_texIndex = texIndex;
    gl_Position = u_MVP * position;
}

#shader fragment

#version 460 core

layout(location = 0) out vec4 color;

in vec4 v_color;
in vec2 v_texCoord;
in float v_texIndex;

uniform sampler2D u_texture[32];

void main()
{
    int index = int(v_texIndex);
    color = texture(u_texture[index], v_texCoord) * v_color;
}
