#version 330

// OF Attributes
layout(location = 0) in vec4  position;
layout(location = 1) in vec4  color;
layout(location = 2) in vec3  normal;
layout(location = 3) in vec2  texcoord;

out VertexAttrib {
    vec4 color;
    vec3 normal;
    vec2 texcoord;
} vertex;

void main()
{
    vertex.color = color;
    vertex.normal = normal;
    vertex.texcoord = texcoord;
    
    gl_Position = position;
}
