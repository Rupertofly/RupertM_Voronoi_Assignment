#version 330
// outputs
out vec4 outputColor;

uniform sampler2D inTex;
uniform sampler2D alpha;
uniform vec2 iResolution;
uniform int iDist;

in vec2 vTexCoord;

//in VertexAttrib {
//    vec3 normal;
//    vec2 texcoord;
//    vec4 color;
//} vertex;

void main(void){
    bool shade = false;
    vec2 uv = gl_FragCoord.xy/iResolution;
    vec4 this_sam = texture(alpha,uv);
    if (this_sam.r < 0.5){
        vec2 pos;
        vec2 puv;
        vec4 sam;
        pos = vec2(gl_FragCoord.x-1.0,gl_FragCoord.y);
        puv = pos/iResolution;
        sam = texture(alpha,puv);
        if (sam.r > 0.5) shade = true;
        pos = vec2(gl_FragCoord.x-1.0,gl_FragCoord.y-1.0);
        puv = pos/iResolution;
        sam = texture(alpha,puv);
        if (sam.r > 0.5) shade = true;
        pos = vec2(gl_FragCoord.x,gl_FragCoord.y-1.0);
        puv = pos/iResolution;
        sam = texture(alpha,puv);
        if (sam.r > 0.5) shade = true;
        if (shade){
            outputColor = vec4(0.0,0.0,0.0,1.0);
        } else {
            outputColor = this_sam;
        }
        } else {
            outputColor = vec4(vec3(1.0),1.0);
        }
    
}
