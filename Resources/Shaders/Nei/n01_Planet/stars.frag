#version 440

out vec4 fragColor;
void main(void){
    float l = length(gl_PointCoord-vec2(0.5,0.5));
    if(l>0.5)discard;

    l*=2;
    l=1-l;
    float c = (l*l)*2;
    fragColor = vec4(c,c,c,c);
}
