#version 330

// outputs
out vec4 outputColor;

in VertexAttrib {
    vec3 normal;
    vec2 texcoord;
    vec4 color;
} vertex;

void main (void)  
{  
    outputColor = vec4(1.);//vertex.color;
}