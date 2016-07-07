#version 120

varying vec4 globalColor;

uniform vec3 colA;
uniform vec3 colB;

uniform float testo;

void main()
{
    
    //if it was white, leave it white, otherwise blend the colors
    if (globalColor.r < 0.5 && globalColor.g < 0.5 && globalColor.b < 0.5){
        float prc = abs(sin( (gl_FragCoord.x + gl_FragCoord.y)/100));
        vec3 col = prc * colA + (1-prc) * colB;
        gl_FragColor = vec4 (col.x, col.y, col.z, 1.0);
    }else{
        gl_FragColor = globalColor;
    }
}