#version 330
// outputs
out vec4 outputColor;

uniform sampler2D vor_tex;
uniform vec2 resolution;

in VertexAttrib {
    vec3 normal;
    vec2 texcoord;
    vec4 color;
} vertex;

void main (void)
{
    vec2 uv = vertex.texcoord/resolution;
    
    vec4 tex = texture(vor_tex,uv);
//    outputColor = texture(vor_tex,uv);
    outputColor = vec4(1.,tex.g,0.,1.);
}
