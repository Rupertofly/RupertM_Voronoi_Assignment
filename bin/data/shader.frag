#version 330
// outputs
out vec4 outputColor;

uniform sampler2D tex;
uniform vec2 resolution;

in vec2 vTexCoord;

//in VertexAttrib {
//    vec3 normal;
//    vec2 texcoord;
//    vec4 color;
//} vertex;

void main (void)
{
    vec2 uv = vTexCoord;
    vec2 pixel = resolution;
    
    vec4 texo = texture(tex,uv);
    vec4 col;
//    outputColor = texture(vor_tex,uv);
    float bval = 0.0;
    if (texo.rgb != vec3(1.0)){
        for (float w = -2.0; w<= 0.0;w = w + 1.0)
        {
            for (float h= -2.0; h<=0.0;h = h + 1.0)
            {
                //vec2 disp = vec2(w*pixel.x,h*pixel.y);
                vec2 pos = vec2(uv.x+(w*pixel.x),uv.y+(h*pixel.y));
                vec4 pxl = texture(tex,pos);
                if (pxl.rgb == vec3(1.0)) bval++;
            }
        }
        if (bval != 0.0){
            float shadow = bval/16;
            vec3 sh_col = vec3(
                               smoothstep(texo.r,0.0,shadow),
                               smoothstep(texo.g,0.0,shadow),
                               smoothstep(texo.b,0.0,shadow)
                               );
            outputColor = vec4(sh_col,texo.a);
        } else {
            outputColor = texo;
        }
    }
    
    
}
