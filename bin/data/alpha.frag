#version 330
// outputs
out vec4 outputColor;

uniform sampler2D inTex;
uniform vec2 iResolution;
uniform int iDist;

in vec2 vTexCoord;

//in VertexAttrib {
//    vec3 normal;
//    vec2 texcoord;
//    vec4 color;
//} vertex;

void main(void){
    

    vec4 ts = texture(inTex,vTexCoord);
    if (ts.r > 0.9&&ts.g>0.9&&ts.b>0.9){
        outputColor = vec4(1.0);
    } else {
        outputColor = vec4(vec3(0.0),1.0);
    }
}
