#version 120

varying vec4 globalColor;

uniform vec3 colA;
uniform vec3 colB;

uniform float testo;

void main()
{
    // globalColor is being set inside the vertex shader.
    // the fragment color is then set to the globalColor.
    gl_FragColor = globalColor;
    float prc = abs(sin( (gl_FragCoord.x + gl_FragCoord.y)/100));
    vec3 col = prc * colA + (1-prc) * colB;
    
    //float r = abs(sin(gl_FragCoord.x/100));
    //float g = abs(sin(gl_FragCoord.y/100));
    gl_FragColor = vec4 (col.x, col.y, col.z, 1.0);
    //gl_FragColor = vec4 (1.0, 0.0 , 0.0, 1.0);
}