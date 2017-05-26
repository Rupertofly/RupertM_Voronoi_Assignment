#version 330
//
//// OF Attributes
//layout(location = 0) in vec4  position;
//layout(location = 1) in vec4  color;
//layout(location = 2) in vec3  normal;
//layout(location = 3) in vec2  texcoord;
//
//out VertexAttrib {
//    vec4 color;
//    vec3 normal;
//    vec2 texcoord;
//} vertex;

in vec4  position;
in vec2  texcoord;

out vec2 vTexCoord;

void main()
{
//    vertex.color = color;
//    vertex.normal = normal;
    vTexCoord = texcoord;
    
    gl_Position = position;
}
